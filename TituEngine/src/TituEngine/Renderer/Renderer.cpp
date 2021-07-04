#include "tepch.h"
#include "Renderer.h"

#include "TituEngine/Platform/OpenGL/OpenGlRendererAPI.h"

namespace TituEngine
{
	RendererAPI::API RendererAPI::s_RendererAPI = API::OpenGL;
	Renderer::SceneData Renderer::s_SceneData = SceneData();

	void Renderer::BeginScene(const Camera* camera)
	{
		s_SceneData.ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const VertexArray* vertexArray, const Shader* shader)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjectionMatrix", s_SceneData.ViewProjectionMatrix);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}


	//-----------------------------------RENDER COMMAND------------------
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}
