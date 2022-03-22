#pragma once

#include "Camera.h"
#include "TituEngine/Scene/Components.h"
#include "LightingData.h"

namespace TituEngine
{
	class Material;
	class Mesh;
	class Model;
		
	class Renderer3D
	{
	public:
		static void Shutdown();
		static void Init();

		static void BeginScene();
		static void EndScene();

		static void DrawMesh(const glm::mat4& modelMatrix, const Mesh* mesh, const Material* material, const int32_t& entityID = -1, const Shader* overrideShader = nullptr);
		static void DrawModel(const glm::mat4& modelMatrix, const Model* model, std::vector<Material*>& materials, const int32_t& entityID = -1, const Shader* overrideShader = nullptr);
		static void DrawModel(const glm::mat4& modelMatrix, ModelRendererComponent& modelRendererC, const int32_t& entityID = -1, const Shader* overrideShader = nullptr);

		static void UploadLightDataToGPU();
		static void UploadModelContext(const glm::mat4& modelMatrix, const int32_t& entityID = -1);
		static void UploadLight(const LightComponent& light, const TransformComponent& transform);

	private:
		static uint32_t s_PointLightCount;
		static uint32_t s_DirectionalLightCount;
	};
}