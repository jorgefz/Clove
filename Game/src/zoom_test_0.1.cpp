

#include <iostream>
#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Clove.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


int main() {

	std::string vshader = "../Resources/shaders/multicolor.vs";
	std::string fshader = "../Resources/shaders/multicolor.fs";
	std::string ftexture = "../Resources/tilesets/disco.png";

	Clove::Window w = Clove::Window(1280, 720);
	w.Create();

	// Adds up alpha channels
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 proj = glm::ortho(0.0f, float(1280)/float(720), 0.0f, 1.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 mvp = proj * view * model;

	Clove::Shader* shader = new Clove::Shader(vshader, fshader);
	shader->Bind();
	//shader->SetUniform4f("u_color", 0.2f, 0.3f, 0.8f, 1.0f);
	shader->SetUniformMat4f("u_mvp", mvp);

	Clove::Tileset tset = Clove::Tileset("../Resources/tilesets/disco.png", 8);
	std::array<float, 8> tile_texc = tset.GetTileTextureCoords(7); // choose tile 7
	tset.Bind();

	Clove::VertexArray* va = new Clove::VertexArray();
	va->AddVertex(0.0f, 0.0f, tile_texc[0], tile_texc[1]);
	va->AddVertex(1.0f, 0.0f, tile_texc[2], tile_texc[3]);
	va->AddVertex(1.0f, 1.0f, tile_texc[4], tile_texc[5]);
	va->AddVertex(0.0f, 1.0f, tile_texc[6], tile_texc[7]);

	unsigned int indices[] = { 0,1,2,  2,3,0 };
	Clove::IndexBuffer* ib = new Clove::IndexBuffer(indices, 6);
	Clove::VertexBuffer* vb = new Clove::VertexBuffer(va->data(), va->size());
	va->UpdateAttributes(*vb);
	vb->Unbind(); //Vertex Buffer no longer necessary once VertexArray attributes have been set.

	shader->SetUniform1i("u_texture", 0); //texture sampler in shader (from which texture slot to take texture).

	Clove::Renderer &renderer = *new Clove::Renderer;

	float shift[2] = { 0, 0 }, scale = 1.0f;
	float speed_x = 0, speed_y = 0, zoom = 1;

	while (!w.ShouldClose()) {
		renderer.Clear();

		tset.Bind();
		renderer.Draw(*va, *ib, *shader);

		// Displacement x-y
		if (glfwGetKey(w.GetWindow(), GLFW_KEY_D) == GLFW_PRESS) speed_x = 0.01f;
		else if (glfwGetKey(w.GetWindow(), GLFW_KEY_A) == GLFW_PRESS) speed_x = -0.01f;
		else speed_x = 0;

		if (glfwGetKey(w.GetWindow(), GLFW_KEY_W) == GLFW_PRESS) speed_y = 0.01f;
		else if (glfwGetKey(w.GetWindow(), GLFW_KEY_S) == GLFW_PRESS) speed_y = -0.01f;
		else speed_y = 0;

		// Zooming
		if (glfwGetKey(w.GetWindow(), GLFW_KEY_R) == GLFW_PRESS) zoom = 0.99f;
		else if (glfwGetKey(w.GetWindow(), GLFW_KEY_F) == GLFW_PRESS) zoom = 1.01f;
		else zoom = 1.0f;

		shift[0] += speed_x;
		shift[1] += speed_y;
		scale *= zoom;
		
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-shift[0]*scale, -shift[1]*scale, 0.0f));
		glm::vec3 center(float(1280)/float(720)/2.0f, 1.0f/2.0f, 1.0f);
		view = glm::translate(glm::mat4(1.0f), center) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f / scale, 0.5f / scale, 1.0f)) * glm::translate(glm::mat4(1.0f), -center);
		glm::mat4 mvp = proj * view * model;
		//mvp = glm::scale(mvp, glm::vec3(0.5f / scale, 0.5f / scale, 1.0f));
		shader->SetUniformMat4f("u_mvp", mvp);

		w.Update();
	}

	delete va, vb, ib, shader, renderer;

	w.Destroy();
	std::cout << "End!" << std::endl;
	return 0;
}