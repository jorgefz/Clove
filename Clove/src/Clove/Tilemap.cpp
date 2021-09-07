#include "clovepch.h"

#include "VertexArray.h"
#include "Texture.h"
#include "Tilemap.h"

namespace Clove {

	Tileset::Tileset()
		: Clove::Texture(),
		m_tiles_x(0), m_tiles_y(0), m_tile_sz(0) { }

	Tileset::Tileset(std::string filename, GLuint tile_sz)
		: Clove::Texture(filename),
		m_tiles_x(0), m_tiles_y(0), m_tile_sz(tile_sz) {

		if (Tileset::height() % tile_sz != 0 || Tileset::width() % tile_sz != 0) {
			throw std::runtime_error("Clove::Tileset: texture dimensions not divisible by tile pixel size\n");
		}

		m_tiles_x = Tileset::width() / m_tile_sz;
		m_tiles_y = Tileset::height() / m_tile_sz;
		float tx = 1.0f / static_cast<float>(m_tiles_x);
		float ty = 1.0f / static_cast<float>(m_tiles_y);

		#ifdef _DEBUG
		std::cout << "Clove::Tileset: new tileset " << Tileset::width() << 'x' << Tileset::height();
		std::cout << ", tile size " << m_tile_sz << ", " << m_tiles_x << 'x' << m_tiles_y << " tiles" << std::endl;
		#endif

		// Generate texture coordinates
		for (GLuint j = 0; j != m_tiles_y; ++j) {
			float fj = static_cast<float>(m_tiles_y - j - 1);
			for (GLuint i = 0; i != m_tiles_x; ++i) {
				float fi = static_cast<float>(i);
				std::array<float,8> tex_coords = {
					tx * fi,          ty * (fj + 1.0f),
					tx * (fi + 1.0f), ty * (fj + 1.0f),
					tx * (fi + 1.0f), ty * fj,
					tx * fi,          ty * fj
				};
				m_coords.push_back( tex_coords );
			}
		}

	}

	Tileset::~Tileset() { }
	
	GLuint Tileset::TileNum() {
		return m_tiles_x * m_tiles_y;
	}

	std::array<float,8> Tileset::GetTileTextureCoords(GLuint tile) {
		if (tile > m_tiles_x * m_tiles_y) throw std::runtime_error("Clove::Tileset: requested tile out of bounds");
		return m_coords.at(tile);
	}

	// ================== Tilemap ======================

	Tilemap::Tilemap() :
		m_side_x(0), m_side_y(0),
		m_tile_grid(), m_logic_grid(),
		m_tileset(), m_indices(), m_vertices(nullptr), m_vb(nullptr), m_ib(nullptr)
	{ }

	Tilemap::Tilemap(GLuint sx, GLuint sy, std::vector<GLuint>& tmap, std::vector<GLuint>& lmap, Clove::Tileset& tset) :
		m_side_x(sx), m_side_y(sy),
		m_tile_grid(tmap), m_logic_grid(lmap),
		m_tileset(&tset), m_indices(), m_vertices(nullptr), m_vb(nullptr), m_ib(nullptr) {
		
		m_vertices = new Clove::VertexArray();

		#ifdef _DEBUG
		std::cout << "Clove::Tilemap: new tilemap " << m_side_x <<'x'<< m_side_y << " tiles";
		std::cout << ", tileset of " << tset.TileNum() << " tiles" << std::endl;
		#endif

		// Generate vertices and indices
		for (unsigned int j = 0; j != sy; ++j) {
			for (unsigned int i = 0; i != sx; ++i) {
				GLuint ntile = j * sx + i;
				float fi = static_cast<float>(i);
				float fj = -static_cast<float>(j); //-ve sign generates map top to bottom
				std::array<float, 8> txcoords = tset.GetTileTextureCoords(m_tile_grid[ntile]);
				// generate vertices
				m_vertices->AddVertex(fi,      fj,      txcoords[0], txcoords[1]);
				m_vertices->AddVertex(fi+1.0f, fj,      txcoords[2], txcoords[3]);
				m_vertices->AddVertex(fi+1.0f, fj+1.0f, txcoords[4], txcoords[5]);
				m_vertices->AddVertex(fi,      fj+1.0f, txcoords[6], txcoords[7]);
				// generate indices
				GLuint ind = ntile * 4; // 0 1 2 2 3 0 -> 4 5 6 6 7 4 ->  etc
				GLuint tile_indices[] = {
					0 + ind, 1 + ind, 2 + ind,
					2 + ind, 3 + ind, 0 + ind
				};
				m_indices.insert(m_indices.end(), tile_indices, tile_indices + 6);
			}
		}
		m_ib = new Clove::IndexBuffer(m_indices.data(), m_indices.size());
		m_vb = new Clove::VertexBuffer(m_vertices->data(), m_vertices->size() * sizeof(float));
		//Tilemap::Update();
		m_vertices->UpdateAttributes(*m_vb);
		m_vb->Unbind();
	}

	Tilemap::~Tilemap() {
		if (m_vertices) delete m_vertices;
		if (m_vb) delete m_vb;
		if (m_ib) delete m_ib;
	}

	Clove::VertexArray* Tilemap::GetVertexArray() {
		return m_vertices;
	}

	Clove::IndexBuffer* Tilemap::GetIndexBuffer() {
		return m_ib;
	}

	void Tilemap::Update() {
		m_vb->Bind();
		m_vertices->Bind();
		m_ib->Bind();
		m_vertices->UpdateAttributes(*m_vb);
		m_vb->Unbind();
	}

	void Tilemap::Debug() {
		// Debugging - printing each tile
		GLuint icount = 0;
		GLuint vcount = 0;
		for (GLuint i = 0; i != m_side_x * m_side_y; ++i) {
			std::cout << "Tile " << i << std::endl;
			// print vertices
			for (GLuint j = 0; j != 4; ++j) {
				std::cout << "v" << j << ") ";
				std::cout << m_vertices->x(i * 4 + j) << ", " << m_vertices->y(i * 4 + j) << ", ";
				std::cout << m_vertices->s(i * 4 + j) << ", " << m_vertices->t(i * 4 + j) << std::endl;
			}
			// print indices
			std::cout << "[";
			for (GLuint j = icount; j != icount + 6; ++j) {
				std::cout << m_indices[j] << ", ";
			}
			std::cout << " ]" << std::endl;
			icount += 6;
		}
	}
	
	void Tilemap::ReadFromFile(std::string& filename) {
		/*
		* STANDARD for tilemap binary files
		* basic info:
		*	width(uchar),  height(uchar)
		* then each tile:
		*	texture(uchar), logic(uchar), type(uchar), type data
		* 
		* type data:
		*	idle tiles: none
		*	wall: none
		*	door: open/closed state(uchar)
		*	portal: filename(50 uchar), x(uchar), y(uchar)
		*	
		* objects on the tilemap:
		*	probably use JSON for this
		* 
		*/
	}

	void Tilemap::WriteToFile(std::string& filename) {

	}

}
