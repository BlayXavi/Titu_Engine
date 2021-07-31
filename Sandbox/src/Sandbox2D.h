#pragma once

#include <TituEngine.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

#include "imgui/imgui.h"

#include "Controller/OrthographicCameraController.h"
#include "TituEngine/Instrumentation/Profiler.h"

class Sandbox2DLayer : public Layer
{
public:
	Sandbox2DLayer();
	~Sandbox2DLayer();

	void OnImGuiRender() override;

	void OnUpdate(Timestep ts) override;
	void OnEvent(Event& e) override;

private:

	OrthographicCamera* m_Camera = nullptr;
	OrthographicCameraController m_OrthographicCameraController;
	glm::vec4 m_QuadColor;
	glm::vec4 m_QuadTextureColor;
	Texture2D* m_QuadTexture;

	glm::mat4 m_TriangleTransform;
	float m_TriangleSpeed;
	float m_TriangleAngularSpeed;
	float m_zSquare;

	glm::vec3 m_CameraPosition;
	float m_CameraSpeed;

	float m_CameraAngularSpeed;
	float m_CameraRotation;

	Timestep currentTimeStep;
};