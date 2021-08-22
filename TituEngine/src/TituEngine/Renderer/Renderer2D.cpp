#include "tepch.h"
#include "Renderer2D.h"

#include "Shader.h"
#include "VertexArray.h"

#include <glm/gtx/matrix_decompose.hpp>

namespace TituEngine
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		uint32_t TexIndex;
		glm::vec2 Tiling;
	};

	struct Renderer2DData
	{
		const glm::mat4* m_CameraViewProjectionMatrix;
		VertexArray* QuadVertexArray = nullptr;

		Shader* TextureColorShader = nullptr;
		Shader* BatchRenderingShader = nullptr;
		Texture* WhiteTexture = nullptr;

		//Static Batching Stuff
		static const uint32_t MaxQuadsPerBatch = 10000;
		static const uint32_t MaxVerticesPerBatch = MaxQuadsPerBatch * VERTEX_PER_QUAD;
		static const uint32_t MaxIndicesPerBatch = MaxQuadsPerBatch * INDICES_PER_QUAD;
		static const uint32_t MaxTexSlotsPerBatch = 32; //TODO: Check GraphicsCard Textures Slots

		QuadVertex* QuadVertexBase = nullptr;
		QuadVertex* QuadVertexPtr = nullptr;
		uint32_t FrameQuadCount = 0;

		VertexBuffer* QuadVertexBuffer = nullptr;
		IndexBuffer* QuadIndexBuffer = nullptr;

		int32_t currentTexSlots = -1;

		Texture* texSlots[MaxTexSlotsPerBatch] = { nullptr };
	};

	static Renderer2DData s_Data;


	int Renderer2D::RenderStats::DrawCalls = 0;
	int Renderer2D::RenderStats::Quads = 0;

	void Renderer2D::Init()
	{
		TE_PROFILE_PROFILE_FUNC();

		//float squareVertices[4 * 3/*vertices*/ + 2 * 4/*textCoords*/] = {
		//		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,// top left 
		//		0.5f,   0.5f, 0.0f,  1.0f, 1.0f,// top right
		//		0.5f,  -0.5f, 0.0f,  1.0f, 0.0f,// bottom right
		//		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,// bottom left
		//};

		//uint indices[3 * 2] = { 0, 1, 3,   // first triangle
		//						1, 2, 3    // second triangle
		//};

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(nullptr, s_Data.MaxVerticesPerBatch * sizeof(QuadVertex), false);
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3,	false, "a_Position" },
			{ ShaderDataType::Float4,	false, "a_Color" },
			{ ShaderDataType::Float2,	false, "a_TexCoord" },
			{ ShaderDataType::Int,		false, "a_TexIndex"},
			{ ShaderDataType::Float2,	false, "a_Tiling"},
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBase = new QuadVertex[s_Data.MaxVerticesPerBatch];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndicesPerBatch];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndicesPerBatch; i += INDICES_PER_QUAD)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		IndexBuffer* quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndicesPerBatch);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		int* texSlots = new int[s_Data.MaxTexSlotsPerBatch];
		for (uint32_t i = 0; i < s_Data.MaxTexSlotsPerBatch; i++)
			texSlots[i] = i;

		s_Data.BatchRenderingShader = Shader::Create("assets/shaders/testing/BatchRendering.glsl");
		s_Data.BatchRenderingShader->Bind();
		s_Data.BatchRenderingShader->SetIntArray("u_Textures", texSlots, s_Data.MaxTexSlotsPerBatch);
		delete[] texSlots;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint data = 0xffffffff;
		s_Data.WhiteTexture->SetData(&data, sizeof(uint));


		/*
		s_Renderer2DData.QuadVertexArray = VertexArray::Create();
		s_Renderer2DData.QuadVertexArray->AddVertexBuffer(squareVB);
		s_Renderer2DData.QuadVertexArray->SetIndexBuffer(indexBuffer);

		s_Renderer2DData.TextureColorShader = Shader::Create("assets/shaders/testing/Blending.glsl");
		s_Renderer2DData.TextureColorShader->Bind();
		s_Renderer2DData.TextureColorShader->SetInt("u_Texture", 0);
		*/
	}

	void Renderer2D::Shutdown()
	{
		TE_PROFILE_PROFILE_FUNC();

		delete s_Data.TextureColorShader;
		delete s_Data.BatchRenderingShader;
		delete s_Data.WhiteTexture;

		delete s_Data.QuadVertexArray;
		s_Data.QuadVertexArray = nullptr;

		delete s_Data.QuadIndexBuffer;
		s_Data.QuadIndexBuffer = nullptr;

		delete s_Data.QuadVertexBase;
		s_Data.QuadVertexBase = nullptr;
	}

	void Renderer2D::ResetBatchingData()
	{
		s_Data.QuadVertexPtr = s_Data.QuadVertexBase;
		s_Data.FrameQuadCount = 0;
		s_Data.currentTexSlots = -1;
	}

	void Renderer2D::BeginScene(const Camera* camera)
	{
		TE_PROFILE_PROFILE_FUNC();

		camera = camera;
		s_Data.m_CameraViewProjectionMatrix = &camera->GetViewProjectionMatrix();

		RenderStats::Reset();
		ResetBatchingData();
	}

	void Renderer2D::EndScene()
	{
		TE_PROFILE_PROFILE_FUNC();

		Flush();
	}

	void Renderer2D::Flush()
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexPtr - (uint8_t*)s_Data.QuadVertexBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBase, dataSize);
		s_Data.QuadVertexArray->Bind();

		s_Data.BatchRenderingShader->Bind();
		s_Data.BatchRenderingShader->SetMat4("u_ModelViewProjectionMatrix", *(s_Data.m_CameraViewProjectionMatrix));

		// Bind textures
		for (int32_t i = 0; i <= s_Data.currentTexSlots; i++)
			s_Data.texSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.FrameQuadCount * INDICES_PER_QUAD);
		RenderStats::IncreaseDrawCalls();
		ResetBatchingData();
	}

	void Renderer2D::AddVertices(const glm::mat4 transform, const glm::vec4& color, Texture* const tex, const glm::vec2& tiling)
	{
		if (s_Data.FrameQuadCount >= s_Data.MaxQuadsPerBatch)
		{
			Flush();
			ResetBatchingData();
		}

		int32_t texIndex = -1;

		for (int32_t i = 0; i < s_Data.currentTexSlots; i++)
		{
			if (*tex == *s_Data.texSlots[i])
				texIndex = i;
		}

		if (texIndex == -1)
		{
			s_Data.currentTexSlots++;
			texIndex = s_Data.currentTexSlots;
			s_Data.texSlots[s_Data.currentTexSlots] = tex;
		}

		static const glm::vec4 vecPositions[VERTEX_PER_QUAD] =
		{ {-0.5f,  -0.5f, 0.0f, 1.0f},
		 { 0.5f,  -0.5f, 0.0f, 1.0f},
		 { 0.5f,   0.5f, 0.0f, 1.0f},
		 {-0.5f,   0.5f, 0.0f, 1.0f} };

		static const glm::vec2 uvPositions[VERTEX_PER_QUAD] =
		{ {0.0f, 0.0f},
		{1.0f, 0.0f} ,
		{1.0f, 1.0f} ,
		{0.0f, 1.0f} };

		for (size_t i = 0; i < VERTEX_PER_QUAD; i++)
		{
			s_Data.QuadVertexPtr->Position = transform * vecPositions[i];
			s_Data.QuadVertexPtr->Color = color;
			s_Data.QuadVertexPtr->TexCoord = uvPositions[i];
			s_Data.QuadVertexPtr->TexIndex = texIndex;
			s_Data.QuadVertexPtr->Tiling = tiling;
			s_Data.QuadVertexPtr++;
		}

		s_Data.FrameQuadCount++;
		RenderStats::IncreaseQuads();
	}

	void Renderer2D::AddVertices(const glm::vec3& position, const float& rotation, const glm::vec2& size, const glm::vec4& color, Texture* const tex, const glm::vec2& tiling)
	{
		TE_PROFILE_PROFILE_FUNC();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		AddVertices(transform, color, tex, tiling);
	}

	void Renderer2D::DrawQuad(const glm::mat4& model, const glm::vec4& color)
	{
		AddVertices(model, color, s_Data.WhiteTexture, { 1.0f, 1.0f });

		//TE_PROFILE_PROFILE_FUNC();

		//s_Renderer2DData.WhiteTexture->Bind();
		//s_Renderer2DData.TextureColorShader->Bind();
		//s_Renderer2DData.TextureColorShader->SetFloat4("u_Color", color);
		//s_Renderer2DData.TextureColorShader->SetMat4("u_ModelViewProjectionMatrix", *(s_SceneData->m_CameraViewProjectionMatrix) * model);

		/*s_Renderer2DData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Renderer2DData.QuadVertexArray);*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		TE_PROFILE_PROFILE_FUNC();

		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float& angle, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, angle, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		TE_PROFILE_PROFILE_FUNC();

		AddVertices(position, 0.0f, size, color, s_Data.WhiteTexture, glm::vec2(1.0f));

		/*glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
		DrawQuad(modelMatrix, color);
		*/
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float& angle, const glm::vec2& size, const glm::vec4& color)
	{
		TE_PROFILE_PROFILE_FUNC();

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));

		DrawQuad(modelMatrix, color);
	}

	void Renderer2D::DrawQuad(const glm::mat4& model, const glm::vec4& color, Texture* texture, const glm::vec2& tileSize)
	{
		AddVertices(model, color, texture, tileSize);

		/*texture.Bind(0);
		s_Data.TextureColorShader->Bind();
		s_Data.TextureColorShader->SetFloat4("u_Color", color);
		s_Data.TextureColorShader->SetMat4("u_ModelViewProjectionMatrix", *(s_SceneData->m_CameraViewProjectionMatrix) * model);
		s_Data.TextureColorShader->SetFloat2("u_TileSize", tileSize);

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, Texture* texture, const  glm::vec2& tileSize)
	{
		TE_PROFILE_PROFILE_FUNC();

		DrawQuad({ position.x, position.y, 0.0f }, size, color, texture, tileSize);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float& angle, const glm::vec2& size, const glm::vec4& color, Texture* texture, const glm::vec2& tileSize)
	{
		DrawQuad({ position.x, position.y, 0.0f }, angle, size, color, texture, tileSize);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Texture* texture, const glm::vec2& tileSize)
	{
		TE_PROFILE_PROFILE_FUNC();

		AddVertices(position, 0.0f, size, color, texture, tileSize);

		/*glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
		DrawQuad(modelMatrix, color, texture, tileSize);*/
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float& angle, const glm::vec2& size, const glm::vec4& color, Texture* texture, const glm::vec2& tileSize)
	{
		TE_PROFILE_PROFILE_FUNC();

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));

		DrawQuad(modelMatrix, color, texture, tileSize);
	}
}
