#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <vector>
#include <math.h>

#include "../headers/renderer.hpp"
#include "glm/trigonometric.hpp"
// #include "../headers/shaders.hpp"
// #include "../headers/textures.hpp"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define VERT_SHADER_SRC_FILE "shaders/vertex.glsl"
#define FRAG_SHADER_SRC_FILE "shaders/fragment.glsl"

#define RUSTY_METAL_TEXTURE "assets/textures/rusty_metal.jpg"

void framebuffer_size_callback(GLFWwindow* win, int w, int h) {
	glViewport(0, 0, w, h);
}

void processInput(GLFWwindow *win) {
	int action = glfwGetKey(win, GLFW_KEY_ESCAPE);
	if (action == GLFW_PRESS) {
		glfwSetWindowShouldClose(win, true);
	}
}

void renderCallback() {
	// Make background
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

int main() {
	glfwInit(); glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hohmann", NULL, NULL);
	if (win == NULL) {
		printf("Failed to create a window.\n");
		glfwTerminate();
		return 1;
	}

	Renderer::Renderer3D renderer(win);

	glfwMakeContextCurrent(win);

	if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ) {
		printf("Failed to init GLAD.\n");
		return 1;
	}

	// std::vector<float> verts({
	// 	 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
	// 	 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	// 	-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	// 	-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	// });

	std::vector<float> verts({
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
	});
	// Vert struc: x y z  r g b  tx ty

	std::vector<unsigned int> indices({  
		0, 1, 3, 
		3, 4, 5, 
		5, 6, 7, 
		7, 8, 9, 
		9, 10, 11, 
		11, 12, 13,
		0, 1, 3, 
		3, 4, 5, 
		5, 6, 7, 
		7, 8, 9, 
		9, 10, 11, 
		11, 12, 13,
		0, 1, 3, 
		3, 4, 5, 
		5, 6, 7, 
		7, 8, 9, 
		9, 10, 11, 
		11, 12, 13,
		0, 1, 3, 
		3, 4, 5, 
		5, 6, 7, 
		7, 8, 9, 
		9, 10, 11, 
		11, 12, 13,
		0, 1, 3, 
		3, 4, 5, 
		5, 6, 7, 
		7, 8, 9, 
		9, 10, 11, 
		11, 12, 13,
	});  

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	Renderer::Obj2D ro(verts, indices);
	Renderer::Obj2D ro2(verts, indices);

	ro2.setTexture("assets/textures/meep.jpg");
	ro.setTexture(RUSTY_METAL_TEXTURE);

	renderer.spawnObject(ro);
	renderer.spawnObject(ro2);
	renderer.setCamera(glm::vec3(0.0f, 0.0f, -4.0f));

	// Window width & height
	while (!glfwWindowShouldClose(win)) {
		// Handle input
		processInput(win);

		// rendering
		renderCallback();

		/* OBJECT RENDERING */
		float time = glfwGetTime();
		float gVal = sin(time) / 10.5f;
		renderer.setCamera(glm::vec3(0.0f, 0.0f, gVal/10.0f));

		// Transformation
		float rotang = time;

		glm::mat4 T = glm::mat4(1.0f);
		T = glm::rotate(T, rotang, glm::vec3(1.0, 0.0, 1.0));
		T = glm::scale(T, glm::vec3(0.5, 0.5, 0.5));
		ro.transform(T);

		renderer.render();

		// glfw
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
