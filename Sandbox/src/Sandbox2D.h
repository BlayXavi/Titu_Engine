#pragma once

#include <TituEngine.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

#include "imgui/imgui.h"
#include "TituEngine/Platform/OpenGL/OpenGLShader.h"

#include "Controller/OrthographicCameraController.h"

class Sandbox2DLayer : public Layer
{
public:
	Sandbox2DLayer();
	~Sandbox2DLayer();

	void OnImGuiRender() override;

	void OnUpdate(Timestep ts) override;
	void OnEvent(Event& e) override;

private:

	Shader* m_Shader = nullptr;
	VertexArray* m_VertexArray = nullptr;
	OrthographicCamera* m_Camera = nullptr;
	OrthographicCameraController m_OrthographicCameraController;
	Texture* texture = nullptr;
	Texture* m_BlendTexture = nullptr;

	glm::mat4 m_TriangleTransform;
	float m_TriangleSpeed;
	float m_TriangleAngularSpeed;

	glm::vec3 m_CameraPosition;
	float m_CameraSpeed;

	float m_CameraAngularSpeed;
	float m_CameraRotation;

	Timestep currentTimeStep;

};