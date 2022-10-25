#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <vector>
#include <math.h>

#include "../headers/renderer.hpp"
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

	glfwMakeContextCurrent(win);

	if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ) {
		printf("Failed to init GLAD.\n");
		return 1;
	}

	float verts[] = {
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 
		 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f
	};
	// Vert struc: x y z  r g b  tx ty

	unsigned int indices[] = {  
		0, 1, 3, 
		1, 2, 3	
	};  

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	Renderer::Obj2D ro(indices, sizeof(indices), verts, sizeof(verts));

	while (!glfwWindowShouldClose(win)) {
		// Handle input
		processInput(win);

		// rendering
		renderCallback();

		/* OBJECT RENDERING */
		float time = glfwGetTime();
		float gVal = (sin(time) / 1.5f) + 0.5f;

		// Transformation
		float rotang = time;

		glm::mat4 T = glm::mat4(1.0f);
		T = glm::rotate(T, rotang, glm::vec3(0.0, 0.707, 0.707));
		T = glm::scale(T, glm::vec3(0.5, 0.5, 0.5));
		ro.transform(T);
		ro.render();

		glm::mat4 T2 = glm::mat4(1.0f);
		T2 = glm::rotate(T2, rotang, glm::vec3(0.707, 0.707, 0.0));
		T2 = glm::translate(T2, glm::vec3(-0.5, 0.4, 0.0));
		T2 = glm::scale(T2, glm::vec3(0.5, 0.5, 0.2));
		ro.transform(T2);
		ro.render();

		// glfw
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
