#pragma once

#include "Camera.h"
#include "TituEngine/Scene/Components.h"

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

		static void BeginScene(const Camera* cam, const glm::mat4& view_Projection_matrix);

		static void DrawMesh(const glm::mat4& modelMatrix, const Mesh* mesh, const Material* material);
		static void DrawModel(const glm::mat4& modelMatrix, const Model* model, const std::vector<Material*>& materials);

		static void DrawModel(const glm::mat4& modelMatrix, const ModelRendererComponent& modelRendererC);

		static void UpdateModelMatrix(const glm::mat4& modelMatrix);

		static void EndScene();
	};
}