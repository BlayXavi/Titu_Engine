#include "tepch.h"
#include "Renderer3D.h"

#include "Mesh.h"
#include "Shader.h"
#include "UniformBuffer.h"

namespace TituEngine
{

	//struct Renderer3DData
	//{
	//	struct CameraData
	//	{
	//		glm::mat4 ViewProjection;
	//	};
	//	CameraData CameraBufferData;
	//	UniformBuffer* CameraUniformBuffer;
	//};

	//static Renderer3DData s_Data;

	UniformBuffer* ModelMatrix;

	void Renderer3D::Shutdown()
	{
	}

	void Renderer3D::Init()
	{
		//s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);
		ModelMatrix = UniformBuffer::Create(sizeof(glm::mat4), 1);
	}

	void Renderer3D::BeginScene(const Camera* cam, const glm::mat4& view_Projection_matrix)
	{
		/*s_Data.CameraBufferData.ViewProjection = view_Projection_matrix;
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBufferData.ViewProjection, sizeof(Renderer3DData::CameraBufferData));*/
	}

	void Renderer3D::DrawMesh(const glm::mat4& modelMatrix, const Mesh* mesh, const Material* material)
	{
		UpdateModelMatrix(modelMatrix);
		mesh->Render(modelMatrix, material);
	}

	void Renderer3D::DrawModel(const glm::mat4& modelMatrix, const Model* model, std::vector<Material*>& materials)
	{
		UpdateModelMatrix(modelMatrix);
		model->Render(modelMatrix, materials);
	}

	void Renderer3D::DrawModel(const glm::mat4& modelMatrix, ModelRendererComponent& modelRendererC)
	{
		if (modelRendererC.GetModel() != nullptr)
			DrawModel(modelMatrix, modelRendererC.GetModel(), modelRendererC.GetMaterials());
	}

	void Renderer3D::UpdateModelMatrix(const glm::mat4& modelMatrix)
	{
		ModelMatrix->SetData(&modelMatrix, sizeof(glm::mat4));
	}

	void Renderer3D::EndScene()
	{
	}
}
