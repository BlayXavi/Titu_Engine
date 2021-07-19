#include "tepch.h"
#include "Renderer.h"

#include "TituEngine/Platform/OpenGL/OpenGlRendererAPI.h"

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

	void Renderer::Submit(const VertexArray* vertexArray, Shader* shader, const glm::mat4 transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ModelViewProjectionMatrix", s_SceneData.ViewProjectionMatrix * transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}


	//-----------------------------------RENDER COMMAND------------------
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}
