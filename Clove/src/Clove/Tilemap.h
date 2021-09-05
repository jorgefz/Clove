#pragma once

#include "Core.h"

#include "VertexArray.h"
#include "Texture.h"

#ifdef CLOVE_DEBUG
#endif
namespace Clove {

	class Tileset : public Clove::Texture {
	private:
		GLuint m_tiles_x, m_tiles_y; // sides of tileset in units of tiles
		GLuint m_tile_sz; // Side of each tile in pixels
		std::vector<std::array<float,8>> m_coords;
	public:
		Tileset();
		Tileset(std::string filename, GLuint tile_sz = 16);
		~Tileset();
		GLuint TileNum();
		std::array<float,8> GetTileTextureCoords(GLuint tile);
		// Reading tilesets: tiles from left to right, top to bottom.
		// Reading tiles: top left, top right, bottom right, bottom left
	};

	class Tilemap {
	private:
		GLuint m_side_x, m_side_y;
		std::vector<GLuint> m_tile_grid, m_logic_grid;
		Clove::Tileset* m_tileset;
		Clove::IndexBuffer* m_ib;
		Clove::VertexBuffer* m_vb;
		Clove::VertexArray* m_vertices;
		std::vector<GLuint> m_indices;
	public:
		Tilemap();
		Tilemap(GLuint sx, GLuint sy, std::vector<GLuint>& tmap, std::vector<GLuint>& lmap, Clove::Tileset& tset);
		~Tilemap();
		Clove::VertexArray* GetVertexArray();
		Clove::IndexBuffer* GetIndexBuffer();
		void Update();
		void Debug();
		void ReadFromFile(std::string& filename);
		void WriteToFile(std::string& filename);
	};
}

