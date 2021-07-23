#include "tepch.h"
#include "Renderer.h"

#include "TituEngine/Platform/OpenGL/OpenGlRendererAPI.h"
#include "TituEngine/Platform/OpenGL/OpenGLShader.h"

namespace TituEngine
{
	RendererAPI::API RendererAPI::s_RendererAPI = API::OpenGL;
	Renderer::SceneData Renderer::s_SceneData = SceneData();

	int Renderer::Init()
	{
		return RenderCommand::Init();
	}

	void Renderer::BeginScene(const Camera* camera)
	{
		s_SceneData.ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::OnWindowResized(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::Submit(const VertexArray* vertexArray, Shader* shader, const glm::mat4 transform)
	{
		shader->Bind();
		if (GetAPI() == RendererAPI::API::OpenGL)
			static_cast<OpenGLShader*>(shader)->UploadUniformMat4("u_ModelViewProjectionMatrix", s_SceneData.ViewProjectionMatrix * transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}


	//-----------------------------------RENDER COMMAND------------------
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}
