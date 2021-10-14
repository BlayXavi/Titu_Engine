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
#include "Utilities/ParticleSystem.h"

#define FPS_DEBUG_COUNT 120

class Sandbox2DLayer : public Layer
{
public:
	Sandbox2DLayer();
	~Sandbox2DLayer();

	void OnImGuiRender() override;

	void OnUpdate(Timestep ts) override;
	void OnEvent(Event& e) override;

private:

	EditorOrthographicCamera* m_EditorCamera = nullptr;
	EditorOrthographicCameraController m_CameraController;
	glm::vec4 m_QuadColor;
	glm::vec4 m_QuadTextureColor;
	glm::ivec2 m_BackgroundTileSize;
	Texture2D* m_QuadTexture = nullptr;
	Texture2D* m_QuadTexture2 = nullptr;
	Texture2D* m_SpriteSheet = nullptr;
	SubTexture2D* m_SubTexture2D = nullptr;
	SubTexture2D* m_SubTextures2D[10];
	Framebuffer* m_Framebuffer = nullptr;

	glm::mat4* randomPositions = nullptr;
	glm::vec4* randomColors = nullptr;

	glm::mat4 m_TriangleTransform;
	float m_TriangleSpeed;
	float m_TriangleAngularSpeed;
	float m_zSquare;
	float m_squareRotationAccumulated;

	glm::vec3 m_CameraPosition;
	float m_CameraSpeed;

	float m_CameraAngularSpeed;
	float m_CameraRotation;

	Timestep currentTimeStep;

	int currentFPSDebugIndex = 0;
	float debugFPS[FPS_DEBUG_COUNT];
	float debugMS[FPS_DEBUG_COUNT];
	bool m_UpdateFPS = true;
	float m_AverageFPS = 0;
	float m_AverageMS = 0;

	ParticleSystem m_ParticleSystem;
	glm::vec4 m_particleVel;
	float	  m_particleSizeStart;
	float	  m_particleSizeEnd;
	glm::vec4 m_particleColorStart;
	glm::vec4 m_particleColorEnd;
	float	  m_particleAngularVel;
	float	  m_particleLifeTime;
	int m_particlesPerFrame;
	bool m_VSync;
};