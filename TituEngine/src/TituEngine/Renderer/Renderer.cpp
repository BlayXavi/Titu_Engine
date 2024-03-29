#include "tepch.h"

#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"

#include "UniformBuffer.h"

#include "Camera.h"

#include "Renderer.h"
#include "Renderer2D.h"
#include "Renderer3D.h"

#include "Framebuffer.h"

#include "TituEngine/Platform/OpenGL/OpenGlRendererAPI.h"

namespace TituEngine
{
	//---------------------------------RenderStats API---------------------------------

	uint32_t RenderStats::s_DrawCalls = 0;
	uint32_t RenderStats::s_Vertices = 0;

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
	std::unordered_map<Renderer::FramebufferType, Framebuffer*> Renderer::s_FramebufferMap;

	FramebufferSpecs Renderer::m_MainFramebufferSpecs = FramebufferSpecs();
	FramebufferSpecs Renderer::m_GBufferSpecs = FramebufferSpecs();

	UniformBuffer* Renderer::m_CameraDataUnifformBuffer = nullptr;
	
	Mesh* Renderer::Quad = nullptr;

	void Renderer::Init()
	{
		TE_PROFILE_PROFILE_FUNC();

		TextureUtilities::Init();
		ShaderUtilities::Init();
		MaterialUtilities::Init();

		RenderCommand::Init();
		Renderer2D::Init();
		Renderer3D::Init();

		m_MainFramebufferSpecs.Width = 1280;
		m_MainFramebufferSpecs.Height = 720;
		m_MainFramebufferSpecs.Samples = 1;
		m_MainFramebufferSpecs.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER_32, FramebufferTextureFormat::DEPTH };
		Framebuffer* m_MainFramebuffer = Framebuffer::Create(m_MainFramebufferSpecs);
		s_FramebufferMap[FramebufferType::ColorFramebuffer] = m_MainFramebuffer;

		m_GBufferSpecs.Width = 1280;
		m_GBufferSpecs.Height = 720;
		m_GBufferSpecs.Samples = 1;
		m_GBufferSpecs.Attachments = 
		{ FramebufferTextureFormat::RGBA16, FramebufferTextureFormat::RGBA16, FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER_32, FramebufferTextureFormat::DEPTH };
		Framebuffer* m_GBuffer = Framebuffer::Create(m_GBufferSpecs);
		s_FramebufferMap[FramebufferType::GBuffer] = m_GBuffer;

		m_CameraDataUnifformBuffer = UniformBuffer::Create(16 + 16 * 4, 0);

		std::vector<Vertex> vertices;
		Vertex v;
		v.Position = glm::vec3(-1.0f, -1.0f, 0.0f); 
		v.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.emplace_back(v);
		v.Position = glm::vec3(1.0f, -1.0f, 0.0f); 
		v.TexCoords = glm::vec2(1.0f, 0.0f);
		vertices.emplace_back(v);
		v.Position = glm::vec3(1.0f, 1.0f, 0.0f); 
		v.TexCoords = glm::vec2(1.0f, 1.0f);
		vertices.emplace_back(v);
		v.Position = glm::vec3(-1.0f, 1.0f, 0.0f); 
		v.TexCoords = glm::vec2(0.0f, 1.0f);
		vertices.emplace_back(v);

		
		std::vector<uint32_t> indices({ 0, 1, 2, 2, 3, 0 });

		//Model* model = Model::Create("assets\\meshes\\backpack\\backpack.obj");
		//Quad = model->GetMesh();
		Quad = Mesh::Create(vertices, indices);
	}

	void Renderer::Shutdown()
	{
		TE_PROFILE_PROFILE_FUNC();

		TextureUtilities::Shutdown();
		ShaderUtilities::Shutdown();

		RenderCommand::Shutdown();
		Renderer2D::Shutdown();
		Renderer3D::Shutdown();

		for(auto& fb : s_FramebufferMap)
		{
			delete fb.second;
			fb.second = nullptr;
		}

	}

	void Renderer::PrepareColorBuffer()
	{
		Framebuffer* ColorBuffer = Renderer::GetFramebuffer(Renderer::FramebufferType::ColorFramebuffer);
		ColorBuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		ColorBuffer->ClearAttachment(1, -1);
	}

	void Renderer::UnbindColorBuffer()
	{
		Framebuffer* ColorBuffer = Renderer::GetFramebuffer(Renderer::FramebufferType::ColorFramebuffer);
		ColorBuffer->UnBind();
	}

	void Renderer::PrepareGBuffer()
	{
		Framebuffer* GBuffer = Renderer::GetFramebuffer(Renderer::FramebufferType::GBuffer);
		GBuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		GBuffer->ClearAttachment(3, -1);
	}

	void Renderer::UnbindGBuffer()
	{
		Framebuffer* GBuffer = Renderer::GetFramebuffer(Renderer::FramebufferType::GBuffer);
		GBuffer->UnBind();
	}

	void Renderer::OnWindowResized(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginFrame()
	{
		RenderStats::Reset();

		Renderer::UploadCameraDataToGPU();

		for (auto& fb : s_FramebufferMap)
			fb.second->ProcessDirty();
	}

	void Renderer::UploadCameraDataToGPU()
	{
		Camera::ActiveCameraData activeCamData = Camera::GetActiveCamera();
		glm::vec3 camPos = activeCamData.GetPosition();
		m_CameraDataUnifformBuffer->SetData(&camPos, 16, 0);
		glm::mat4 viewProjectionMatrix = activeCamData.GetViewProjectionMatrix();
		m_CameraDataUnifformBuffer->SetData(&viewProjectionMatrix, 16 * 4, 16);
	}

	void Renderer::EndFrame()
	{
	}

	FramebufferSpecs Renderer::GetMainFramebufferSpecs()
	{
		return m_MainFramebufferSpecs;
	}

	void Renderer::ResizeFramebuffer(const FramebufferType& fbType, const uint32_t& width, const uint32_t& height)
	{
		s_FramebufferMap[fbType]->SetDirty(width, height);
	}

	uint32_t Renderer::GetFramebufferColorAttachment(const FramebufferType& fbType, const uint32_t& index)
	{
		return s_FramebufferMap[fbType]->GetColorAttachment(index);
	}

	uint32_t Renderer::GetFramebufferDepthAttachment(const FramebufferType& fbType)
	{
		return s_FramebufferMap[fbType]->GetDepthAttachment();
	}

	uint32_t Renderer::GetFramebufferPixel(const FramebufferType& fbType, uint32_t attachmentIndex, uint32_t x, uint32_t y)
	{
		return s_FramebufferMap[fbType]->GetPixel(attachmentIndex, x, y);
	}

	Mesh* Renderer::GetQuad()
	{
		return Quad;
	}

	//-----------------------------------RENDER COMMAND------------------
	RendererAPI* RenderCommand::s_RendererAPI = RendererAPI::Create();
	
}
