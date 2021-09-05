

#include <iostream>
#include <array>

#include "Clove.h"

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

	std::string vshader = "../resources/shaders/multicolor.vs";
	std::string fshader = "../resources/shaders/multicolor.fs";
	//std::string ftexture = "../resources/player.jpg";
	//std::string ftexture = "../resources/cursor.png";
	std::string ftexture = "../resources/tilesets/disco.png";

	Clove::Window& window = *new Clove::Window(1280, 720);
	window.Create();

	Clove::Camera camera(-window.AspectRatio(), window.AspectRatio(), -1.0f, 1.0f);
	glm::mat4 model = glm::mat4(1.0f), view, proj;

	Clove::Renderer& renderer = *new Clove::Renderer;

	Clove::Shader* shader = new Clove::Shader(vshader, fshader);
	shader->Bind();

	// Main texture to use
	Clove::Tileset tset = Clove::Tileset(ftexture, 8);
	tset.Bind();
	//texture sampler in shader (from which texture slot to take texture).
	shader->SetUniform1i("u_texture", 0);

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
	
	// Loop variables
	float pos_x = 0.0f, pos_y = 0.0f, speed_x = 0.0f, speed_y = 0.0f;
	float scale = 2.0f, zoom = 1.0f, delta_zoom = 1.05f;

	// for testing purposes, remove when input manager is set-up
	glfwSetScrollCallback(window.GetWindow(), TEST_scroll_callback);

	while (!window.ShouldClose()) {
		renderer.Clear();
		checkGLError();

		renderer.Draw(*tmap.GetVertexArray(), *tmap.GetIndexBuffer(), *shader);

		// Displacement x-y
		if (glfwGetKey(window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS) speed_x = 0.02f;
		else if (glfwGetKey(window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS) speed_x = -0.02f;
		else speed_x = 0;

		if (glfwGetKey(window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS) speed_y = 0.02f;
		else if (glfwGetKey(window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS) speed_y = -0.02f;
		else speed_y = 0;

		// Zooming with mouse callback
		if (TEST_mouse_scroll >= 1.0f) zoom = 1.0f / delta_zoom;
		else if (TEST_mouse_scroll <= -1.0f) zoom = delta_zoom;
		else zoom = 1.0f;

		TEST_mouse_scroll = 0.0; // reset mouse scroll notify

		pos_x += speed_x * scale;
		pos_y += speed_y * scale;
		scale *= zoom;

		// Rebuild MVP
		camera.SetScale(scale);
		glm::vec3 pos(pos_x, pos_y, 1.0f);
		camera.SetPosition(pos);
		camera.UpdateMatrices(window);
		proj = camera.GetProjectionMatrix();
		view = camera.GetViewMatrix();
		
		shader->SetUniformMat4f("u_mvp", proj * view * model); // pass to shader

		window.Update();
	}

	window.Destroy();
	delete& window;
	delete shader, renderer;

	std::cout << "End!" << std::endl;
	return 0;
}