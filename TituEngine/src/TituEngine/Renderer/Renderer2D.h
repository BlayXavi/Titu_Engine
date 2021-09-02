#pragma once

#include "Camera.h"
#include "Texture.h"
#include "Renderer.h"

namespace TituEngine
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void ResetBatchingData();
		static void BeginScene(const Camera* camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::mat4& model, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& model, const glm::vec4& color, SubTexture2D* texture, const glm::vec2& tileSize = { 1.0f, 1.0f });

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const float& angle, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const float& angle, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, SubTexture2D* texture, const glm::vec2& tileSize = { 1.0f, 1.0f });
		static void DrawQuad(const glm::vec2& position, const float& angle, const glm::vec2& size, const glm::vec4& color, SubTexture2D* texture, const glm::vec2& tileSize = { 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, SubTexture2D* texture, const glm::vec2& tileSize = { 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const float& angle, const glm::vec2& size, const glm::vec4& color, SubTexture2D* texture, const glm::vec2& tileSize = { 1.0f, 1.0f });


		struct RenderStats
		{
		public:
			static int inline GetDrawCalls() { return DrawCalls; }
			static int inline GetQuads() { return Quads; }
			static int inline GetVertices() { return Quads * VERTEX_PER_QUAD; }
			static void IncreaseDrawCalls() { DrawCalls++; }
			static void IncreaseQuads() { Quads++; }

			static void Reset()
			{
				DrawCalls = 0;
				Quads = 0;
			}

		private:
			static int DrawCalls;
			static int Quads;
		};

	private:
		static void AddVertices(const glm::vec3& position, const float& rotation, const glm::vec2& size, const glm::vec4& color, SubTexture2D* const tex, const glm::vec2& tiling);
		static void AddVertices(const glm::mat4 transform, const glm::vec4& color, SubTexture2D* const tex, const glm::vec2& tiling);
	};
}