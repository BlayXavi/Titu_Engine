#include "tepch.h"
#include "Renderer3D.h"

#include "Mesh.h"
#include "Shader.h"
#include "UniformBuffer.h"

namespace TituEngine
{

	uint32_t Renderer3D::s_PointLightCount = 0;
	uint32_t Renderer3D::s_DirectionalLightCount = 0;

	UniformBuffer* ModelContextBuffer;

	UniformBuffer* PointLightBuffer;

	DirectionalLightData DirectionalLightsArray[DIRECTIONAL_LIGHTS_SIZE];
	PointLightData PointLightsArray[POINT_LIGHTS_SIZE];

	void Renderer3D::Shutdown()
	{
	}

	void Renderer3D::Init()
	{
		ModelContextBuffer = UniformBuffer::Create(/*EntityID*/ + sizeof(glm::mat4) /*ModelMatrix*/ + 4, 1);
		PointLightBuffer = UniformBuffer::Create(4/*int->numOfLights*/ + ((POINT_LIGHTS_SIZE) * (16/*Position*/ + 16/*Color*/)), 2);
	}

	void Renderer3D::BeginScene()
	{

	}

	void Renderer3D::DrawMesh(const glm::mat4& modelMatrix, const Mesh* mesh, const Material* material, const int32_t& entityID, const Shader* overrideShader)
	{
		UploadModelContext(modelMatrix, entityID);
		mesh->Render(modelMatrix, material, overrideShader);
	}

	void Renderer3D::DrawModel(const glm::mat4& modelMatrix, const Model* model, std::vector<Material*>& materials, const int32_t& entityID, const Shader* overrideShader)
	{
		UploadModelContext(modelMatrix, entityID);
		model->Render(modelMatrix, materials, overrideShader);
	}

	void Renderer3D::DrawModel(const glm::mat4& modelMatrix, ModelRendererComponent& modelRendererC, const int32_t& entityID, const Shader* overrideShader)
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

	void Renderer3D::UploadModelContext(const glm::mat4& modelMatrix, const int32_t& entityID)
	{
		ModelContextBuffer->SetData(&modelMatrix, sizeof(glm::mat4));
		ModelContextBuffer->SetData(&entityID, 4, sizeof(glm::mat4));
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
