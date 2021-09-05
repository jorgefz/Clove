
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Engine.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


/*

============== Tilemap Editor ==============



VERSION

v0.1
- Loads tilemap and its tileset - DONE
- You can move around - DONE
- Key to switch selected tile - DONE

v0.2
- Change tiles via mouse position and clicking - DONE
- Change target tile texture with key - DONE
- Creating new tilemap - DONE
- Saving tilemap with key G, only if it has a spawn tile - DONE

v0.3
- UI Element shows current tile to draw - DONE

FUTURE
- Proper implementation of world coordinates
- Key to switch between editing logic and tile grids
- Scrolling/Zooming




*/

//using namespace std;

GLuint cooldown = 15;
GLuint chosen_tile = 0;


std::string* TilemapWrite(std::string& ftmap, std::vector<GLuint>& logic_grid, std::vector<GLuint>& tile_grid, GLuint width, GLuint height) {

	std::fstream file(ftmap.c_str(), std::ios::binary|std::ios::out|std::ios::trunc);
	if(!file.is_open()){
		std::cout << "Error opening file " << ftmap << std::endl;
		return nullptr;
	}
	// Writing dimensions of tilemap: 1 byte width, 1 byte height
	file.put((uchar)width);
	file.put((uchar)height);
	
	// Write tile grid
	for(int i=0; i!=width*height; ++i){
		file.put((uchar)tile_grid[i]);
	}
	// Write logic grid
	for(int i=0; i!=width*height; ++i){
		file.put((uchar)logic_grid[i]);
	}

	file.close();
	return &ftmap;
}

void TilemapCreate(std::string &ftmap, std::string &ftset, GLuint width, GLuint height){
	//Create tilemap data and fill it with zeroes
	std::vector<GLuint> logic_grid(width*height, 0);
	std::vector<GLuint> tile_grid(width*height, 0);

	//Add spawns
	logic_grid[width*height/2] = Engine::L_SPAWN; //FIX TILE_SPAWN
	tile_grid[width*height/2] = Engine::L_SPAWN;

	// Write to file
	TilemapWrite(ftmap, logic_grid, tile_grid, width, height);
}


bool ProcessInput(GLFWwindow* window, Engine::Shape &cursor, Engine::Tilemap &tmap, float &dx, float &dy){
	
	// Movement	
	if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS or glfwGetKey(window, GLFW_KEY_UP)==GLFW_PRESS) dy=0.01f;
	else if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS or glfwGetKey(window, GLFW_KEY_DOWN)==GLFW_PRESS) dy=-0.01f;
	else dy = 0;

	if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS or glfwGetKey(window, GLFW_KEY_RIGHT)==GLFW_PRESS) dx=0.01f;
	else if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS or glfwGetKey(window, GLFW_KEY_LEFT)==GLFW_PRESS) dx=-0.01f;
	else dx = 0;

	// Correct for aspect ratio deformations
	dx *= 0.77;
	// Diagonal movement
	if(dx != 0 && dy != 0){
		dx *= sin(45);
		dy *= sin(45);
	}

	// Exit option
	if(glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
	
	// Change tile to draw
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS and Engine::KEYSTATES[GLFW_KEY_SPACE] != GLFW_PRESS){
		if(chosen_tile == tmap.tset_tilenum-1) chosen_tile = 0;
		else chosen_tile++;
		cursor.SetTile(chosen_tile);
	}

	// Change tile option, mouse cursor
	if( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ){
		double mouse_x, mouse_y;
		int sz_x, sz_y; // from upper left corner
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		glfwGetFramebufferSize(window, &sz_x, &sz_y);
		
		// Mouse on screen
		if(mouse_x>=0 and mouse_x<sz_x and mouse_y>=0 and mouse_y<sz_y){
			//Get fractional positions
			float cx = 2*(float(mouse_x)/float(sz_x) - 0.5f);
			float cy = 2*(float(sz_y - mouse_y)/float(sz_y) - 0.5f);
			int tile_id = tmap.GetTile(cx, cy);
			if(tile_id != tmap.width*tmap.height and tmap.logic_grid[tile_id] != chosen_tile){
				tmap.logic_grid[tile_id] = chosen_tile;
				tmap.GenTileTextureCoords(tile_id);
			}
		}
		
	}

	// Save Tilemap
	if( glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS and Engine::KEYSTATES[GLFW_KEY_G] != GLFW_PRESS ){
		//Check for at least one spawn tile
		bool spawn_found = false;
		for(int i=0; i!=tmap.width*tmap.height; ++i){
			if(tmap.logic_grid[i] == Engine::L_SPAWN) spawn_found = true; //FIX SPAWN FILE
		}
		if(spawn_found == false) std::cout <<"You must have a spawn tile before saving!"<<std::endl;
		else{// Write to file
			std::cout << " Saving tilemap..." << std::endl;
			std::vector<GLuint> logic(tmap.logic_grid, tmap.logic_grid+tmap.width*tmap.height);
			std::vector<GLuint> tiles(tmap.tile_grid, tmap.tile_grid+tmap.width*tmap.height);
			if(!TilemapWrite(tmap.ftmap, logic, tiles, tmap.width, tmap.height)){
				std::cout << "Failed to write to file!" << std::endl;
			}
		}
	}

	// Debug info
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS and Engine::KEYSTATES[GLFW_KEY_H] != GLFW_PRESS) {
		
		std::cout << "DEBUG" << std::endl;
		
		/* Get cursor position and convert to screen coordinates*/
		double mx, my;
		glfwGetCursorPos(window, &mx, &my); //in pixels
		float mouse_x = float(mx), mouse_y = float(my);
		mouse_x = 2.0f * mouse_x / float(Engine::SCR_WIDTH) - 1;
		mouse_y = -2.0f * mouse_y / float(Engine::SCR_HEIGHT) + 1;
		std::cout << "Screen: " << mouse_x << ", " << mouse_y << std::endl;

		/* Try converting to world coordinates */
		Engine::ScreenToWorld(mouse_x, mouse_y, mouse_x, mouse_y);
		std::cout << "World: " << (int)mouse_x << ", " << (int)mouse_y << std::endl;
	}

	if(cooldown > 0) cooldown--;
	else cooldown = 15;

	return true;
}


void ParseArgs(int argc, char** argv, std::string& ftilemap, std::string& ftileset){

	if( argc < 4 ){
		std::cout << " [EDITOR] Not enough arguments" << std::endl;
		std::cout << " Arguments: [mode] [tilemap] [tileset]" << std::endl;
		std::cout << " 1 - Mode, string (\"Load\" or \"New\") " << std::endl;
		std::cout << " 2 - Path to Tilemap file, string " << std::endl;
		std::cout << " 3 - Path to Tileset file, string " << std::endl;
		exit(-1);
	}
	std::string mode = argv[1];
	ftilemap  = argv[2];
	ftileset = argv[3];

	if(mode == "new"){
		if(argc < 6){
			std::cout << "Missing new tilemap dimensions" << std::endl;
			exit(-1);
		} else{
			std::string swidth = argv[4];
			std::string sheight = argv[5];
			int width = std::stoi(swidth);
			int height = std::stoi(sheight);
			std::cout <<"Creating tilemap with dimensions: "<<width<<" "<<height<<std::endl;
			TilemapCreate(ftilemap, ftileset, GLuint(width), GLuint(height));
		}
	} else if(mode != "load"){
		std::cout << " Unknown mode " << mode << std::endl;
		exit(-1);
	}	
	std::cout << "Loading tilemap " << ftilemap << " and tileset " << ftileset <<"..."<< std::endl;
}



int main(int argc, char** argv)
{
	std::string ftilemap("../Resources/test3.tm"), ftileset("../Resources/tileset.png");

	// Uncomment to create new tilemap
	//TilemapCreate(ftilemap, ftileset, 20, 20);

	//ParseArgs(argc, argv, ftilemap, ftileset);

    float dx=0, dy=0;
	int tileside = 50;
	std::string vshader = "../Resources/vertex.shader";
	std::string fshader = "../Resources/fragment.shader";
	std::string texcursor_path = "../Resources/cursor.png";

	GLFWwindow *window;
	window = Engine::GLBegin(1280, 720);

	{ // Scope to delete opengl buffers before glfwTerminate gets called.

		// Shader
		Engine::Shader shader;
		shader.Init(vshader, fshader);
		shader.Bind();

		// Tilemap
		Engine::Tilemap tmap;
		tmap.Init(ftilemap, ftileset, tileside);

		// UI element for selected tile to draw
		Engine::Shape shape;
		shape.Init(ftileset, 100, 100);
		shape.SetPosition(110, Engine::SCR_HEIGHT - 110);
		shape.SetTile(0);

		std::cout << "WIDTH: " << Engine::SCR_WIDTH << std::endl;
		std::cout << "HEIGHT: " << Engine::SCR_HEIGHT << std::endl;

		int old_width = Engine::SCR_WIDTH;
		int old_height = Engine::SCR_HEIGHT;

		Engine::SetScreenWorldPosition(0.5, 0.5);

		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT);
			ProcessInput(window, shape, tmap, dx, dy);

			//Update
			tmap.Move(-dx, -dy);
			tmap.Draw();
			shape.Draw();

			//Render
			Engine::UpdateKeyStates(window);
			Engine::OnWindowResize(window);
			Engine::UpdateScreenWorldCoords(dx, dy);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

	}

	glfwTerminate();
	return 0;
}
