

#include <iostream>
#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Clove.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void checkGLError() {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << err;
	}
}


void printMatrix4x4(glm::mat4 mat) {
	for (GLuint i = 0; i != 4; ++i) {
		for (GLuint j = 0; j != 4; ++j) {
			std::cout << mat[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}


double TEST_mouse_scroll = 0.0;

void TEST_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	TEST_mouse_scroll = yoffset;
}

int main() {

	std::string vshader = "../Resources/shaders/multicolor.vs";
	std::string fshader = "../Resources/shaders/multicolor.fs";
	//std::string ftexture = "../Resources/player.jpg";
	//std::string ftexture = "../Resources/cursor.png";
	std::string ftexture = "../Resources/tilesets/disco.png";
	//std::string ftexture = "../Resources/tilesets/minecraft.png";

	Clove::Window& w = *new Clove::Window(1280, 720);
	w.Create();

	// Adds up alpha channels
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::ortho(0.0f, 1280.0f / 720.0f, 0.0f, 1.0f, -1.0f, 1.0f);

	glm::mat4 mvp = proj * view * model;

	std::cout << "Projection matrix" << std::endl;
	printMatrix4x4(proj);

	std::cout << "View matrix" << std::endl;
	printMatrix4x4(view);

	std::cout << "Model matrix" << std::endl;
	printMatrix4x4(model);

	std::cout << "MVP" << std::endl;
	printMatrix4x4(mvp);
	

	Clove::Shader* shader = new Clove::Shader(vshader, fshader);
	shader->Bind();
	//shader->SetUniform4f("u_color", 0.2f, 0.3f, 0.8f, 1.0f);
	shader->SetUniformMat4f("u_mvp", mvp);

	// Main texture to use
	Clove::Tileset tset = Clove::Tileset(ftexture, 8);
	std::array<float, 8> tile_texc = tset.GetTileTextureCoords(7);
	tset.Bind();

	// ======= Test quad
	Clove::VertexArray* va = new Clove::VertexArray();
	va->AddVertex(0.0f, 0.0f, tile_texc[0], tile_texc[1]);
	va->AddVertex(-1.0f, 0.0f, tile_texc[2], tile_texc[3]);
	va->AddVertex(-1.0f, 1.0f, tile_texc[4], tile_texc[5]);
	va->AddVertex(0.0f, 1.0f, tile_texc[6], tile_texc[7]);
	unsigned int indices[] = { 0,1,2,  2,3,0  };
	Clove::IndexBuffer* ib = new Clove::IndexBuffer(indices, 6);
	Clove::VertexBuffer* vb = new Clove::VertexBuffer(va->data(), va->size() * sizeof(float));
	va->UpdateAttributes(*vb);
	vb->Unbind(); //Vertex Buffer no longer necessary once VertexArray attributes have been set.
	
	// ======= Tilemap
	std::vector<GLuint> texmap = {
		0,1,2,3,4,
		1,1,1,1,1,
		2,2,2,2,2,
		3,3,3,3,3,
		4,4,4,4,4
	};
	std::vector<GLuint> logicmap = {
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0
	};
	Clove::Tilemap tmap = Clove::Tilemap(5, 5, texmap, logicmap, tset);
	
	shader->SetUniform1i("u_texture", 0); //texture sampler in shader (from which texture slot to take texture).
	Clove::Renderer& renderer = *new Clove::Renderer;

	float shift[2] = { 0.0f, 0.0f }, scale = 1.0f;
	float speed_x = 0.0f, speed_y = 0.0f, zoom = 1.0f, delta_zoom = 1.05f;

	// for testing purposes, remove when input manager is set-up
	glfwSetScrollCallback(w.GetWindow(), TEST_scroll_callback);

	while (!w.ShouldClose()) {
		renderer.Clear();
		checkGLError();

		renderer.Draw(*tmap.GetVertexArray(), *tmap.GetIndexBuffer(), *shader);

		renderer.Draw(*va, *ib, *shader);

		// Displacement x-y
		if      (glfwGetKey(w.GetWindow(), GLFW_KEY_D) == GLFW_PRESS) speed_x = 0.01f;
		else if (glfwGetKey(w.GetWindow(), GLFW_KEY_A) == GLFW_PRESS) speed_x = -0.01f;
		else speed_x = 0;

		if      (glfwGetKey(w.GetWindow(), GLFW_KEY_W) == GLFW_PRESS) speed_y = 0.01f;
		else if (glfwGetKey(w.GetWindow(), GLFW_KEY_S) == GLFW_PRESS) speed_y = -0.01f;
		else speed_y = 0;

		
		// zooming with R & F keys
		/*
		if      (glfwGetKey(w.GetWindow(), GLFW_KEY_R) == GLFW_PRESS) zoom = 0.99f;
		else if (glfwGetKey(w.GetWindow(), GLFW_KEY_F) == GLFW_PRESS) zoom = 1.01f;
		else zoom = 1.0f;
		*/

		// Zooming with mouse callback
		if (TEST_mouse_scroll >= 1.0f) zoom = 1.0f/delta_zoom;
		else if (TEST_mouse_scroll <= -1.0f) zoom = delta_zoom;
		else zoom = 1.0f;

		TEST_mouse_scroll = 0.0; // reset mouse scroll notify

		shift[0] += speed_x / scale;
		shift[1] += speed_y / scale;
		scale *= zoom;

		// Rebuild MVP
		glm::vec3 center(0.5f * 1280.0f / 720.0f, 0.5f, 1.0f);
		view = glm::translate(glm::mat4(1.0f), center); // to center in screen coordinates ?
		view = glm::scale(view, glm::vec3(scale, scale, 1.0f)); // zoom
		view = glm::translate(view, glm::vec3(-shift[0], -shift[1], 0.0f) ); // displacement
		mvp = proj * view * model;
		shader->SetUniformMat4f("u_mvp", mvp); // pass to shader

		w.Update();
	}

	delete va, vb, ib, shader, renderer;
	w.Destroy();
	delete &w;

	std::cout << "End!" << std::endl;
	return 0;
}