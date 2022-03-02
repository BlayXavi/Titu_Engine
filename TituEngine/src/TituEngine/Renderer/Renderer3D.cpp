#include "tepch.h"
#include "Renderer3D.h"

#include "Mesh.h"
#include "Shader.h"
#include "UniformBuffer.h"

namespace TituEngine
{

	struct LightingData
	{
		float AmbientLightIntensity = 0.2f;
		glm::vec4 AmbientLightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec3 LightPos = glm::vec3(0.0f, 0.0f, 10.0f);
		glm::vec4 LightColor = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
	};

	static LightingData s_LightingData;

	UniformBuffer* ModelMatrixBuffer;
	UniformBuffer* LightingDataBuffer;

	void Renderer3D::Shutdown()
	{
	}

	void Renderer3D::Init()
	{
		ModelMatrixBuffer = UniformBuffer::Create(sizeof(glm::mat4) + 16, 1);
		LightingDataBuffer = UniformBuffer::Create(16 * 4, 2);
	}

	void Renderer3D::BeginScene()
	{
		//LightingDataBuffer->SetData(&s_LightingData.AmbientLightColor, sizeof(LightingData)); -> Oh god. 2 Hours to see the f****g alingment
		LightingDataBuffer->SetData(&s_LightingData.AmbientLightIntensity, 4, 0);
		LightingDataBuffer->SetData(&s_LightingData.AmbientLightColor, 16, 16 * 1);
		LightingDataBuffer->SetData(&s_LightingData.LightPos, 16, 16 * 2);
		LightingDataBuffer->SetData(&s_LightingData.LightColor, 16, 16 * 3);
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

	void Renderer3D::SetLightPosition(const glm::vec3& pos)
	{
		s_LightingData.LightPos = pos;
	}

	void Renderer3D::EndScene()
	{
	}
}
