#pragma once
#include "clovepch.h"

#include "VertexArray.h"
#include "Texture.h"

#ifdef CLOVE_DEBUG
#endif
namespace Clove {

	class Tileset : public Clove::Texture {
	private:
		unsigned int m_tiles_x, m_tiles_y; // sides of tileset in units of tiles
		unsigned int m_tile_sz; // Side of each tile in pixels
		std::vector<std::array<float,8>> m_coords;
	public:
		Tileset();
		Tileset(std::string filename, unsigned int tile_sz = 16);
		~Tileset();
		unsigned int TileNum();
		std::array<float,8> GetTileTextureCoords(unsigned int tile);
		// Reading tilesets: tiles from left to right, top to bottom.
		// Reading tiles: top left, top right, bottom right, bottom left
	};

	class Tilemap {
	private:
		unsigned int m_side_x, m_side_y;
		std::vector<unsigned int> m_tile_grid, m_logic_grid;
		Clove::Tileset* m_tileset;
		Clove::IndexBuffer* m_ib;
		Clove::VertexBuffer* m_vb;
		Clove::VertexArray* m_vertices;
		std::vector<unsigned int> m_indices;
	public:
		Tilemap();
		Tilemap(unsigned int sx, unsigned int sy, std::vector<unsigned int>& tmap, std::vector<unsigned int>& lmap, Clove::Tileset& tset);
		~Tilemap();
		Clove::VertexArray* GetVertexArray();
		Clove::IndexBuffer* GetIndexBuffer();
		void Update();
		void Debug();
		void ReadFromFile(std::string& filename);
		void WriteToFile(std::string& filename);
	};
}

