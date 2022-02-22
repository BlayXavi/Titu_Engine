#include "tepch.h"

#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"

#include "Camera.h"

#include "Renderer.h"
#include "Renderer2D.h"
#include "Renderer3D.h"

#include "Framebuffer.h"

#include "TituEngine/Platform/OpenGL/OpenGlRendererAPI.h"

namespace TituEngine
{
	//---------------------------------RENDERER API---------------------------------
	RendererAPI::API RendererAPI::s_RendererAPIID = API::OpenGL;
	
	RendererAPI* RendererAPI::Create()
	{
		TE_PROFILE_PROFILE_FUNC();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return new OpenGLRendererAPI();
		}

		TE_ASSERT(false, "RendererAPI not supported. Context: [Renderer API]"); return nullptr;
	}

	//---------------------------------  RENDERER  ---------------------------------
	Framebuffer* Renderer::m_MainFramebuffer = nullptr;
	FramebufferSpecs Renderer::m_MainFramebufferSpecs = FramebufferSpecs();
	
	void Renderer::Init()
	{
		TE_PROFILE_PROFILE_FUNC();

		TextureUtilities::Init();
		ShaderUtilities::Init();

		RenderCommand::Init();
		Renderer2D::Init();
		Renderer3D::Init();

		m_MainFramebufferSpecs.Width = 1280;
		m_MainFramebufferSpecs.Height = 720;
		m_MainFramebufferSpecs.Samples = 1;
		m_MainFramebufferSpecs.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER_32, FramebufferTextureFormat::DEPTH };
		m_MainFramebuffer = Framebuffer::Create(m_MainFramebufferSpecs);

	}

	void Renderer::Shutdown()
	{
		TE_PROFILE_PROFILE_FUNC();

		TextureUtilities::Shutdown();
		ShaderUtilities::Shutdown();

		RenderCommand::Shutdown();
		Renderer2D::Shutdown();
		Renderer3D::Shutdown();
	}

	void Renderer::OnWindowResized(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginFrame()
	{
		m_MainFramebuffer->ProcessDirty();

		m_MainFramebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		m_MainFramebuffer->ClearAttachment(1, -1);
	}

	void Renderer::BeginScene(const Camera* camera, const glm::mat4 viewProjectionMatrix)
	{
		Renderer2D::BeginScene(camera, viewProjectionMatrix);
		Renderer3D::BeginScene(camera, viewProjectionMatrix);
	}

	void Renderer::EndScene()
	{
		Renderer2D::EndScene();
		Renderer3D::EndScene();
	}

	void Renderer::EndFrame()
	{
		m_MainFramebuffer->UnBind();
	}

	FramebufferSpecs Renderer::GetMainFramebufferSpecs()
	{
		return m_MainFramebufferSpecs;
	}

	void Renderer::ResizeMainFramebuffer(const uint32_t& width, const uint32_t& height)
	{
		m_MainFramebuffer->SetDirty(width, height);
	}

	uint32_t Renderer::GetMainFramebufferColorAttachment(const uint32_t& index)
	{
		return m_MainFramebuffer->GetColorAttachment(index);
	}

	uint32_t Renderer::GetMainFramebufferPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y)
	{
		return m_MainFramebuffer->GetPixel(attachmentIndex, x, y);
	}

	Framebuffer* Renderer::GetMainFramebuffer()
	{
		return m_MainFramebuffer;
	}

	//-----------------------------------RENDER COMMAND------------------
	RendererAPI* RenderCommand::s_RendererAPI = RendererAPI::Create();

	
}
