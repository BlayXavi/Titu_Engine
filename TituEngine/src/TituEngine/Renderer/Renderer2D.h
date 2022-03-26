#pragma once

#include "Camera.h"
#include "Renderer.h"

#include "TituEngine/Scene/Components.h"


namespace TituEngine
{
	class Texture2D;

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void ResetBatchingData();
		static void BeginScene();
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::mat4& model, const glm::vec4& color, const uint32_t& entityID = -1);
		static void DrawQuad(const glm::mat4& model, const glm::vec4& color, SubTexture2D* texture, const glm::vec2& tileSize = { 1.0f, 1.0f }, const uint32_t& entityID = -1);
		static void DrawQuad(const glm::mat4& model, const SpriteRendererComponent& spriteRendererC, const uint32_t& entityID = -1);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const uint32_t& entityID = -1);
		static void DrawQuad(const glm::vec2& position, const float& angle, const glm::vec2& size, const glm::vec4& color, const uint32_t& entityID = -1);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const uint32_t& entityID = 0);
		static void DrawQuad(const glm::vec3& position, const float& angle, const glm::vec2& size, const glm::vec4& color, const uint32_t& entityID = -1);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, SubTexture2D* texture, const glm::vec2& tileSize = { 1.0f, 1.0f }, const uint32_t& entityID = -1);
		static void DrawQuad(const glm::vec2& position, const float& angle, const glm::vec2& size, const glm::vec4& color, SubTexture2D* texture, const glm::vec2& tileSize = { 1.0f, 1.0f }, const uint32_t& entityID = -1);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, SubTexture2D* texture, const glm::vec2& tileSize = { 1.0f, 1.0f }, const uint32_t& entityID = -1);
		static void DrawQuad(const glm::vec3& position, const float& angle, const glm::vec2& size, const glm::vec4& color, SubTexture2D* texture, const glm::vec2& tileSize = { 1.0f, 1.0f }, const uint32_t& entityID = -1);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const SpriteRendererComponent& spriteRendererC, const uint32_t& entityID = -1);
		static void DrawQuad(const glm::vec2& position, const float& angle, const glm::vec2& size, const SpriteRendererComponent& spriteRendererC, const uint32_t& entityID = -1);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const SpriteRendererComponent& spriteRendererC, const uint32_t& entityID = -1);
		static void DrawQuad(const glm::vec3& position, const float& angle, const glm::vec2& size, const SpriteRendererComponent& spriteRendererC, const uint32_t& entityID = -1);

	private:
		static void AddVertices(const glm::vec3& position, const float& rotation, const glm::vec2& size, const glm::vec4& color, SubTexture2D* const tex, const glm::vec2& tiling, const int32_t& entityID = 0);
		static void AddVertices(const glm::mat4 transform, const glm::vec4& color, SubTexture2D* const tex, const glm::vec2& tiling, const int32_t& entityID = 0);
	};
}