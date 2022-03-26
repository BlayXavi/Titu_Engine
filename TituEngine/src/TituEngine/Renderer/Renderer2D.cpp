#include "tepch.h"
#include "Renderer2D.h"

#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"

#include "TituEngine/Renderer/UniformBuffer.h"

namespace TituEngine
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		uint32_t TexIndex;
		glm::vec2 Tiling;

		//Editor stuff
		uint32_t EntityID;
	};

	struct Renderer2DData
	{
		VertexArray* QuadVertexArray = nullptr;

		Shader* BatchRenderingShader = nullptr;
		SubTexture2D* whiteSubTexture2D = nullptr;

		//Static Batching Stuff
		static const uint32_t MaxQuadsPerBatch = 1000;
		static const uint32_t MaxVerticesPerBatch = MaxQuadsPerBatch * VERTEX_PER_QUAD;
		static const uint32_t MaxIndicesPerBatch = MaxQuadsPerBatch * INDICES_PER_QUAD;
		static const int32_t MaxTexSlotsPerBatch = 32; //TODO: Check GraphicsCard Textures Slots

		QuadVertex* QuadVertexBase = nullptr;
		QuadVertex* QuadVertexPtr = nullptr;
		uint32_t FrameQuadCount = 0;

		VertexBuffer* QuadVertexBuffer = nullptr;
		IndexBuffer* QuadIndexBuffer = nullptr;

		int32_t currentTexSlots = -1;

		Texture2D* texSlots[MaxTexSlotsPerBatch] = { nullptr };
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		TE_PROFILE_PROFILE_FUNC();

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(nullptr, s_Data.MaxVerticesPerBatch * sizeof(QuadVertex), false);
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3,	false, "a_Position" },
			{ ShaderDataType::Float4,	false, "a_Color" },
			{ ShaderDataType::Float2,	false, "a_TexCoord" },
			{ ShaderDataType::Int,		false, "a_TexIndex"},
			{ ShaderDataType::Float2,	false, "a_Tiling"},
			{ ShaderDataType::Int,		false, "a_EntityID"},
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

		s_Data.BatchRenderingShader = Shader::Create("assets\\shaders\\testing\\BatchRendering.glsl");

		s_Data.whiteSubTexture2D = TextureUtilities::s_WhiteTexture;
	}

	void Renderer2D::Shutdown()
	{
		TE_PROFILE_PROFILE_FUNC();

		delete s_Data.BatchRenderingShader;

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

	void Renderer2D::BeginScene()
	{
		TE_PROFILE_PROFILE_FUNC();

		ResetBatchingData();
	}

	void Renderer2D::EndScene()
	{
		TE_PROFILE_PROFILE_FUNC();

		Flush();
	}

	void Renderer2D::Flush()
	{
		if ((uint8_t*)s_Data.QuadVertexPtr != (uint8_t*)s_Data.QuadVertexBase)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexPtr - (uint8_t*)s_Data.QuadVertexBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBase, dataSize);
			s_Data.QuadVertexArray->Bind();

			// Bind textures
			for (int32_t i = 0; i <= s_Data.currentTexSlots; i++)
				s_Data.texSlots[i]->Bind(i);

			s_Data.BatchRenderingShader->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.FrameQuadCount * INDICES_PER_QUAD);
			ResetBatchingData();
		}
	}

	void Renderer2D::AddVertices(const glm::mat4 transform, const glm::vec4& color, SubTexture2D* tex, const glm::vec2& tiling, const int32_t& entityID)
	{
		if (s_Data.FrameQuadCount >= s_Data.MaxQuadsPerBatch)
			Flush();

		int32_t texIndex = -1;

		for (int32_t i = 0; i <= s_Data.currentTexSlots; i++) //find texture index if it is already used 
		{
			if (*tex->GetTexture() == *s_Data.texSlots[i])
				texIndex = i;
		}

		if (texIndex == -1) //if texture index was not found
		{
			if (s_Data.currentTexSlots > s_Data.MaxTexSlotsPerBatch) //Flush! limit textures exceeded! need to Flush, release current textures used and restart the process
				Flush();

			s_Data.currentTexSlots++;
			texIndex = s_Data.currentTexSlots;
			s_Data.texSlots[s_Data.currentTexSlots] = tex->GetTexture(); //assign index texture
		}

		static const glm::vec4 vecPositions[VERTEX_PER_QUAD] =
		{ {-0.5f,  -0.5f, 0.0f, 1.0f},
		 { 0.5f,  -0.5f, 0.0f, 1.0f},
		 { 0.5f,   0.5f, 0.0f, 1.0f},
		 {-0.5f,   0.5f, 0.0f, 1.0f} };

		glm::vec2* texCoords = tex->GetTexCoords();

		for (size_t i = 0; i < VERTEX_PER_QUAD; i++)
		{
			s_Data.QuadVertexPtr->Position = transform * vecPositions[i];
			s_Data.QuadVertexPtr->Color = color;
			s_Data.QuadVertexPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexPtr->TexIndex = texIndex;
			s_Data.QuadVertexPtr->Tiling = tiling;
			s_Data.QuadVertexPtr->EntityID = entityID;
			s_Data.QuadVertexPtr++;
		}

		s_Data.FrameQuadCount++;
	}

	void Renderer2D::AddVertices(const glm::vec3& position, const float& rotation, const glm::vec2& size, const glm::vec4& color, SubTexture2D* tex, const glm::vec2& tiling, const int32_t& entityID)
	{
		TE_PROFILE_PROFILE_FUNC();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		AddVertices(transform, color, tex, tiling, entityID);
	}

	void Renderer2D::DrawQuad(const glm::mat4& model, const glm::vec4& color, const uint32_t& entityID)
	{
		AddVertices(model, color, s_Data.whiteSubTexture2D, { 1.0f, 1.0f }, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const uint32_t& entityID)
	{
		TE_PROFILE_PROFILE_FUNC();

		DrawQuad({ position.x, position.y, 0.0f }, size, color, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float& angle, const glm::vec2& size, const glm::vec4& color, const uint32_t& entityID)
	{
		DrawQuad({ position.x, position.y, 0.0f }, angle, size, color, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const uint32_t& entityID)
	{
		TE_PROFILE_PROFILE_FUNC();

		AddVertices(position, 0.0f, size, color, s_Data.whiteSubTexture2D, glm::vec2(1.0f), entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float& angle, const glm::vec2& size, const glm::vec4& color, const uint32_t& entityID)
	{
		TE_PROFILE_PROFILE_FUNC();

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));

		DrawQuad(modelMatrix, color, entityID);
	}

	void Renderer2D::DrawQuad(const glm::mat4& model, const glm::vec4& color, SubTexture2D* texture, const glm::vec2& tileSize, const uint32_t& entityID)
	{
		AddVertices(model, color, texture, tileSize, entityID);
	}

	void Renderer2D::DrawQuad(const glm::mat4& model, const SpriteRendererComponent& spriteRendererC, const uint32_t& entityID)
	{
		if (spriteRendererC.Texture != nullptr)
			AddVertices(model, spriteRendererC.Color, spriteRendererC.Texture, spriteRendererC.TileSize, entityID);
		else
			AddVertices(model, spriteRendererC.Color, s_Data.whiteSubTexture2D, spriteRendererC.TileSize, entityID);

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, SubTexture2D* texture, const  glm::vec2& tileSize, const uint32_t& entityID)
	{
		TE_PROFILE_PROFILE_FUNC();

		DrawQuad({ position.x, position.y, 0.0f }, size, color, texture, tileSize, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float& angle, const glm::vec2& size, const glm::vec4& color, SubTexture2D* texture, const glm::vec2& tileSize, const uint32_t& entityID)
	{
		DrawQuad({ position.x, position.y, 0.0f }, angle, size, color, texture, tileSize, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, SubTexture2D* texture, const glm::vec2& tileSize, const uint32_t& entityID)
	{
		TE_PROFILE_PROFILE_FUNC();

		AddVertices(position, 0.0f, size, color, texture, tileSize, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float& angle, const glm::vec2& size, const glm::vec4& color, SubTexture2D* texture, const glm::vec2& tileSize, const uint32_t& entityID)
	{
		TE_PROFILE_PROFILE_FUNC();

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));

		DrawQuad(modelMatrix, color, texture, tileSize, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const SpriteRendererComponent& spriteRendererC, const uint32_t& entityID)
	{
		TE_PROFILE_PROFILE_FUNC();
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, spriteRendererC.Color, spriteRendererC.Texture, spriteRendererC.TileSize, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float& angle, const glm::vec2& size, const SpriteRendererComponent& spriteRendererC, const uint32_t& entityID)
	{
		TE_PROFILE_PROFILE_FUNC();
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), angle, size, spriteRendererC.Color, spriteRendererC.Texture, spriteRendererC.TileSize, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const SpriteRendererComponent& spriteRendererC, const uint32_t& entityID)
	{
		TE_PROFILE_PROFILE_FUNC();
		DrawQuad(position, size, spriteRendererC.Color, spriteRendererC.Texture, spriteRendererC.TileSize, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float& angle, const glm::vec2& size, const SpriteRendererComponent& spriteRendererC, const uint32_t& entityID)
	{
		TE_PROFILE_PROFILE_FUNC();
		DrawQuad(position, size, spriteRendererC.Color, spriteRendererC.Texture, spriteRendererC.TileSize, entityID);
	}
}
