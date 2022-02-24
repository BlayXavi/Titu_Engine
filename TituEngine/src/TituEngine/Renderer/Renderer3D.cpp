#include "tepch.h"
#include "Renderer3D.h"

#include "Mesh.h"
#include "Shader.h"
#include "UniformBuffer.h"

namespace TituEngine
{

	struct Renderer3DData
	{
		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBufferData;
		UniformBuffer* CameraUniformBuffer;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Shutdown()
	{
	}

	void Renderer3D::Init()
	{
		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);
	}

	void Renderer3D::BeginScene(const Camera* cam, const glm::mat4& view_Projection_matrix)
	{
		s_Data.CameraBufferData.ViewProjection = view_Projection_matrix;
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBufferData.ViewProjection, sizeof(Renderer3DData::CameraBufferData));
	}

	void Renderer3D::DrawMesh(const Mesh* mesh, const glm::mat4& modelMatrix, const Shader* shader)
	{
		mesh->Render(shader);
	}

	void Renderer3D::DrawModel(const Model* model, const glm::mat4& modelMatrix, const Shader* shader)
	{
		model->Render(shader);
	}

	void Renderer3D::EndScene()
	{
	}
}
