#pragma once

#include "Camera.h"

namespace TituEngine
{
	class Shader;
	class Mesh;
	class Model;
		
	class Renderer3D
	{
	public:
		static void Shutdown();
		static void Init();

		static void BeginScene(const Camera* cam, const glm::mat4& view_Projection_matrix);

		static void DrawMesh(const Mesh* mesh, const glm::mat4& modelMatrix, const Shader* shader);
		static void DrawModel(const Model* model, const glm::mat4& modelMatrix, const Shader* shader);

		static void EndScene();
	};
}