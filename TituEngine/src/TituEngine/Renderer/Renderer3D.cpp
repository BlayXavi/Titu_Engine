#include "tepch.h"
#include "Renderer3D.h"

#include "Mesh.h"
#include "Shader.h"
#include "UniformBuffer.h"

namespace TituEngine
{

	uint32_t Renderer3D::s_PointLightCount = 0;
	uint32_t Renderer3D::s_DirectionalLightCount = 0;

	UniformBuffer* ModelMatrixBuffer;

	UniformBuffer* PointLightBuffer;

	DirectionalLightData DirectionalLightsArray[DIRECTIONAL_LIGHTS_SIZE];
	PointLightData PointLightsArray[POINT_LIGHTS_SIZE];

	void Renderer3D::Shutdown()
	{
	}

	void Renderer3D::Init()
	{
		ModelMatrixBuffer = UniformBuffer::Create(sizeof(glm::mat4) + 16, 1);

		PointLightBuffer = UniformBuffer::Create(4 + ((POINT_LIGHTS_SIZE) * (16 + 16)), 2);
	}

	void Renderer3D::BeginScene()
	{

	}

	void Renderer3D::DrawMesh(const glm::mat4& modelMatrix, const Mesh* mesh, const Material* material, const uint32_t& entityID, const Shader* overrideShader)
	{
		UpdateModelMatrix(modelMatrix);
		mesh->Render(modelMatrix, material, overrideShader);
	}

	void Renderer3D::DrawModel(const glm::mat4& modelMatrix, const Model* model, std::vector<Material*>& materials, const uint32_t& entityID, const Shader* overrideShader)
	{
		UpdateModelMatrix(modelMatrix);
		model->Render(modelMatrix, materials, overrideShader);
	}

	void Renderer3D::DrawModel(const glm::mat4& modelMatrix, ModelRendererComponent& modelRendererC, const uint32_t& entityID, const Shader* overrideShader)
	{
		if (modelRendererC.GetModel() != nullptr)
			DrawModel(modelMatrix, modelRendererC.GetModel(), modelRendererC.GetMaterials(), entityID, overrideShader);
	}

	void Renderer3D::UploadLightDataToGPU()
	{
		for (uint32_t i = 0; i < s_PointLightCount; i++) 
		{
			PointLightBuffer->SetData(&PointLightsArray[i].Position, 16, (32 * i));
			PointLightBuffer->SetData(&PointLightsArray[i].Color, 16, (32 * i) + 16);
		}
		PointLightBuffer->SetData(&s_PointLightCount, 4, 32 * POINT_LIGHTS_SIZE);
	}

	void Renderer3D::UpdateModelMatrix(const glm::mat4& modelMatrix)
	{
		ModelMatrixBuffer->SetData(&modelMatrix, sizeof(glm::mat4));
	}

	void Renderer3D::UploadLight(const LightComponent& light, const TransformComponent& transform)
	{
		switch (light.LightType)
		{
		case LightType::DIRECTIONAL_LIGHT:
		{
			DirectionalLightData* dLightData = &DirectionalLightsArray[s_DirectionalLightCount];
			dLightData->Color = light.Color;
			dLightData->Direction = transform.GetForward();
			dLightData->Intensity = light.Intensity;
			s_DirectionalLightCount++;
			break;

		}
		case LightType::POINT_LIGHT:
		{
			PointLightData* pLightData = &PointLightsArray[s_PointLightCount];
			pLightData->Color = light.Color;
			glm::vec3 pos = transform.GetTranslation();
			pLightData->Position = glm::vec4(pos.x, pos.y, pos.z, 1.0f);
			s_PointLightCount++;
			break;
		}
		default:
		{
			break;
		}
		}
	}

	void Renderer3D::EndScene()
	{
		s_PointLightCount = 0;
		s_DirectionalLightCount = 0;
	}
}
