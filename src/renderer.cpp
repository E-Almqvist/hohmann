#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdio.h>

#include "GLFW/glfw3.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/fwd.hpp"
#include "renderer.hpp"
#include "shaders.hpp"

namespace Renderer {
	// Entity
	Entity::Entity() {}

	Entity::Entity(glm::vec3 pos) : Entity() {
		setPosition(pos);
	}

	Entity::Entity(glm::vec3 pos, glm::vec3 angle) : Entity(pos) {
		setRotation(angle);
	}

	void Entity::scale(glm::vec3 vscale) {
		glm::mat4 T = glm::mat4(1.0f);
		T = glm::scale(T, vscale);
		modelTransform = T;
	}

	void Entity::transform(glm::mat4 T) {
		modelTransform = T;
	}

	void Entity::updatePositionTransform() {
		glm::mat4 T = glm::mat4(1.0f);
		positionTransform = glm::translate(T, position);
	}

	void Entity::setPosition(glm::vec3 pos) {
		position = pos;
		updatePositionTransform();
	}

	void Entity::translate(glm::vec3 dpos) {
		setPosition(position + dpos);
	}

	void Entity::updateRotationTransform() {
		float x_Ang, y_Ang, z_Ang;
		x_Ang = angle[0];
		y_Ang = angle[1];
		z_Ang = angle[2];

		// TODO: make better
		glm::mat4 T = glm::mat4(1.0f);
		T = glm::rotate(T, glm::radians(x_Ang), glm::vec3(1.0f, 0.0f, 0.0f));
		T = glm::rotate(T, glm::radians(y_Ang), glm::vec3(0.0f, 1.0f, 0.0f));
		T = glm::rotate(T, glm::radians(z_Ang), glm::vec3(0.0f, 0.0f, 1.0f));

		rotationTransform = T;
	}

	void Entity::setRotation(glm::vec3 ang) {
		angle = ang;
		updateRotationTransform();
	}

	void Entity::rotate(glm::vec3 dangle) {
		setRotation(angle + dangle);
	}

	// Light data
	LightningData DEFAULT_LIGHT_DATA;

	// Scene
	Scene::Scene(Window* win) {
		window = win;
		lightData = &DEFAULT_LIGHT_DATA; // default lightning data
		camera = new Camera(win); // default camera
	}

	Scene::Scene(Window* win, LightningData* lightData) : Scene(win) {
		this->lightData = lightData;
	}

	Scene::Scene(Window* win, std::vector<RenderEntity*> ROs) : Scene(win) {
		renderEntitys = ROs;
	}

	void Scene::spawnEntity(RenderEntity *ro) {
		renderEntitys.push_back(ro);
	}

	void Scene::setCamera(Camera *cam) {
		camera = cam;
	}

	void Scene::render() {
		// Record deltaTime
		float curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;

		// TODO: rm
		printf("\rdeltaTime=%f FPS=%f", deltaTime, 1/(deltaTime+0.0001f));

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for ( RenderEntity *ro: renderEntitys )
			ro->render(*camera, lightData);

		// Record the last frame
		lastFrame = curFrame;
	}

	// Camera
	Camera::Camera(Window* win) {
		window = win;
		FOV = DEFAULT_FOV;
	}

	Camera::Camera(Window* win, glm::vec3 pos) : Camera(win) {
		setPosition(pos);
	}

	Camera::Camera(Window* win, glm::vec3 pos, glm::vec3 angle) : Camera(win, pos) {
		setRotation(angle);
	}

	glm::mat4 Camera::getProjection() {
		projection = glm::perspective(glm::radians(FOV),
									  (float)window->getWidth() / (float)window->getHeight(), NEAR_PLANE, FAR_PLANE);
		return projection;
	}

	void Camera::updateCameraTransforms() {
		view = glm::lookAt(position, position + front, up);
	}

	// RenderEntity
	RenderEntity::RenderEntity(std::vector<float> verts, std::vector<unsigned int> indices)
		: shader(VERT_SHADER_SRC_FILE, FRAG_SHADER_SRC_FILE) {
		vertsVec = verts;
		indicesVec = indices;

		float vertsArray[vertsVec.size()];
		std::copy(vertsVec.begin(), vertsVec.end(), vertsArray);

		unsigned int indicesArray[indicesVec.size()];
		std::copy(indicesVec.begin(), indicesVec.end(), indicesArray);

		// Vertex Array Entity
		glGenVertexArrays(1, &VAO); // gen the VAO
		glBindVertexArray(VAO); // bind it

		// Copy the verts into the buffer
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertsVec.size() * sizeof(vertsVec[0]), vertsArray, GL_DYNAMIC_DRAW); // for moving stuff

		// Copy the indices for the verts into another buffer
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesVec.size() * sizeof(indicesVec[0]), indicesArray, GL_STATIC_DRAW);

		// Shader stuff
		// Pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_ATTRIB_PTR_SIZE, (void*)0);
		glEnableVertexAttribArray(0);

		// Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_ATTRIB_PTR_SIZE, (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(1);

		// Texture
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_ATTRIB_PTR_SIZE, (void*)(6*sizeof(float)));
		glEnableVertexAttribArray(2);

		// Normal
		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)0);
		// glEnableVertexAttribArray(3);
	}

	void RenderEntity::preRenderHook() {} // TODO: rm

	void RenderEntity::render(Camera cam, LightningData* lightData) {
		// Model viewspace rotation etc
		shader.setMat4("modelPosition", positionTransform);
		shader.setMat4("modelRotation", rotationTransform);
		shader.setMat4("model", modelTransform);

		// Camera angles & such
		shader.setMat4("view", cam.view);
		shader.setMat4("projection", cam.getProjection());

		// Lightning
		shader.setVec3("lightColor", lightData->lightColor);
		shader.setFloat("ambientStrength", lightData->ambientStrength);

		// Use the shader
		shader.use();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawArrays(GL_TRIANGLES, 0, indicesVec.size());
	}

	// TexturedEntity
	void TexturedEntity::setTexture(const char* t_src) {
		texture.texture_src = t_src;
		texture.load();
	}

	void TexturedEntity::preRenderHook() {
		if (texture.loaded)
			texture.bind();
	}

	// Private stuff
	void TexturedEntity::bind_texture(Textures::Texture2D new_texture) {
		texture = new_texture;
	}
}
