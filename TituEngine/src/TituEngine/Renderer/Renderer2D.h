#pragma once

#include "Camera.h"
#include "Texture.h"
namespace TituEngine
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera* camera);
		static void EndScene();

		static void DrawQuad(const glm::mat4& model, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& model, const glm::vec4& color, const Texture& texture, const glm::vec2& tileSize = { 1.0f, 1.0f });

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const float& angle, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const float& angle, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const Texture& texture, const glm::vec2& tileSize = { 1.0f, 1.0f });
		static void DrawQuad(const glm::vec2& position, const float& angle, const glm::vec2& size, const glm::vec4& color, const Texture& texture, const glm::vec2& tileSize = { 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const Texture& texture, const glm::vec2& tileSize = { 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const float& angle, const glm::vec2& size, const glm::vec4& color, const Texture& texture, const glm::vec2& tileSize = { 1.0f, 1.0f });
	};
}