#include "tepch.h"
#include "Renderer2D.h"
#include "Renderer.h"

#include "Shader.h"
#include "VertexArray.h"

namespace TituEngine
{
	struct Renderer2DStorage
	{
		VertexArray* QuadVertexArray;
		Shader* FlatColorShader;
		Shader* TextureColorShader;
	};

	static Renderer2DStorage* s_StorageData;
	struct SceneData
	{
		const glm::mat4* m_CameraViewProjectionMatrix;
	};

	static SceneData* s_SceneData;

	void Renderer2D::Init()
	{
		s_StorageData = new Renderer2DStorage();
		s_SceneData = new SceneData();

		float squareVertices[4 * 3/*vertices*/ + 2 * 4/*textCoords*/] = {
				-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,// top left 
				0.5f,   0.5f, 0.0f,  1.0f, 1.0f,// top right
				0.5f,  -0.5f, 0.0f,  1.0f, 0.0f,// bottom right
				-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,// bottom left
		};

		uint indices[3 * 2] = { 0, 1, 3,   // first triangle
								1, 2, 3    // second triangle
		};

		VertexBuffer* squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		IndexBuffer* indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint));

		BufferLayout layout = { {ShaderDataType::Float3, false, "a_Position"},
								{ShaderDataType::Float2, false, "a_TextCoord"} };

		squareVB->SetLayout(layout);

		s_StorageData->QuadVertexArray = VertexArray::Create();
		s_StorageData->QuadVertexArray->AddVertexBuffer(squareVB);
		s_StorageData->QuadVertexArray->SetIndexBuffer(indexBuffer);

		s_StorageData->FlatColorShader = Shader::Create("assets/shaders/testing/FlatColor.glsl");
		s_StorageData->TextureColorShader = Shader::Create("assets/shaders/testing/Blending.glsl");
		s_StorageData->TextureColorShader->Bind();
			s_StorageData->TextureColorShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_StorageData->QuadVertexArray;
		delete s_StorageData->FlatColorShader;
		delete s_StorageData;
		delete s_SceneData;
	}

	void Renderer2D::BeginScene(const Camera* camera)
	{
		(s_StorageData->FlatColorShader)->Bind();
		camera = camera;
		s_SceneData->m_CameraViewProjectionMatrix = &camera->GetViewProjectionMatrix();
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::mat4& model, const glm::vec4& color)
	{
		s_StorageData->FlatColorShader->Bind();
		s_StorageData->FlatColorShader->SetFloat4("u_Color", color);
		s_StorageData->FlatColorShader->SetMat4("u_ModelViewProjectionMatrix", *(s_SceneData->m_CameraViewProjectionMatrix) * model);

		s_StorageData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_StorageData->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
		DrawQuad(modelMatrix, color);
	}

	void Renderer2D::DrawQuad(const glm::mat4& model, const glm::vec4& color, const Texture& texture)
	{
		texture.Bind(0);
		s_StorageData->TextureColorShader->Bind();
		s_StorageData->TextureColorShader->SetFloat4("u_Color", color);
		s_StorageData->TextureColorShader->SetMat4("u_ModelViewProjectionMatrix", *(s_SceneData->m_CameraViewProjectionMatrix) * model);

		s_StorageData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_StorageData->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const Texture& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const Texture& texture)
	{
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
		DrawQuad(modelMatrix, color, texture);
	}
}
