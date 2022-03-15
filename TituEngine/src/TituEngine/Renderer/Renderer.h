#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"
#include "TituEngine/Core/Core.h"

#define VERTEX_PER_QUAD 4
#define INDICES_PER_QUAD 6

namespace TituEngine
{
	class Camera;
	class Framebuffer;
	struct FramebufferSpecs;
	class UniformBuffer;

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};

		virtual void Init() = 0;
		virtual ~RendererAPI() = default;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() const = 0;

		virtual void DrawIndexed(const VertexArray* vertexArray, const uint32_t count = -1) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void EnableDepthTest(bool enable) = 0;

		inline static API GetAPIID() { return s_RendererAPIID; }
		static RendererAPI* Create();

	private:
		static API s_RendererAPIID;
	};

	class Renderer
	{
	public:

		static void Init();
		static void Shutdown();
		static void OnWindowResized(uint32_t width, uint32_t height);

		static void BeginFrame();
		static void BeginFrameGBuffer();
		static void UploadCameraDataToGPU();
		static void EndFrame();
		static void EndFrameGBuffer();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPIID(); }

		static FramebufferSpecs GetMainFramebufferSpecs();
		static void ResizeMainFramebuffer(const uint32_t& width, const uint32_t& height);
		static uint32_t GetMainFramebufferColorAttachment(const uint32_t& index);
		static uint32_t GetMainFramebufferDepthAttachment();
		static uint32_t GetMainFramebufferPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y);
		static uint32_t GetGBufferDepthAttachment();
		static Framebuffer* GetMainFramebuffer();
		static Framebuffer* GetGBuffer();

	private:
		static Framebuffer* m_MainFramebuffer;
		static Framebuffer* m_GBuffer;
		static FramebufferSpecs m_MainFramebufferSpecs;
		static FramebufferSpecs m_GBufferSpecs;

		static UniformBuffer* m_CameraDataUnifformBuffer;
	};

	class RenderCommand
	{
	public:

		inline static void Init() { s_RendererAPI->Init(); }
		inline static void Shutdown() { delete s_RendererAPI; }
		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		inline static void Clear() { s_RendererAPI->Clear(); }
		inline static void EnableDepthTest(bool enable) { s_RendererAPI->EnableDepthTest(enable); }

		void static DrawIndexed(const VertexArray* vertexArray, const uint32_t count = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, count);
		}

		void static SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};


}
