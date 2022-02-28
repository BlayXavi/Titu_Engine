#include "tepch.h"
#include "Renderer3D.h"

#include "Mesh.h"
#include "Shader.h"
#include "UniformBuffer.h"

namespace TituEngine
{

	struct LightingData
	{
		glm::vec4 AmbientLightColor = glm::vec4(0.9f, 0.2f, 0.3f, 1.0f);
		float AmbientLightIntensity = 1.0f;
	};

	static LightingData s_LightingData;

	UniformBuffer* ModelMatrixBuffer;
	UniformBuffer* LightingDataBuffer;

	void Renderer3D::Shutdown()
	{
	}

	void Renderer3D::Init()
	{
		ModelMatrixBuffer = UniformBuffer::Create(sizeof(glm::mat4), 1);
		LightingDataBuffer = UniformBuffer::Create(sizeof(s_LightingData), 2);
	}

	void Renderer3D::BeginScene(const Camera* cam, const glm::mat4& view_Projection_matrix)
	{
		LightingDataBuffer->SetData(&s_LightingData, sizeof(s_LightingData));
	}

	void Renderer3D::DrawMesh(const glm::mat4& modelMatrix, const Mesh* mesh, const Material* material, const uint32_t& entityID)
	{
		UpdateModelMatrix(modelMatrix);
		mesh->Render(modelMatrix, material);
	}

	void Renderer3D::DrawModel(const glm::mat4& modelMatrix, const Model* model, std::vector<Material*>& materials, const uint32_t& entityID)
	{
		UpdateModelMatrix(modelMatrix);
		model->Render(modelMatrix, materials);
	}

	void Renderer3D::DrawModel(const glm::mat4& modelMatrix, ModelRendererComponent& modelRendererC, const uint32_t& entityID)
	{
		if (modelRendererC.GetModel() != nullptr)
			DrawModel(modelMatrix, modelRendererC.GetModel(), modelRendererC.GetMaterials(), entityID);
	}

	void Renderer3D::UpdateModelMatrix(const glm::mat4& modelMatrix)
	{
		ModelMatrixBuffer->SetData(&modelMatrix, sizeof(glm::mat4));
	}

	void Renderer3D::EndScene()
	{
	}
}
