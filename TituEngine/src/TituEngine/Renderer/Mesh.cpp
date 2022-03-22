#include "tepch.h"
#include "Mesh.h"

#include "TituEngine/Core/FilesystemUtilities.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <stb_image.h>

#include "glm/glm.hpp"

#include "Renderer.h"

namespace TituEngine
{

	Mesh* Mesh::Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		Mesh* mesh = new Mesh();
		mesh->Initialize(vertices, indices);
		return mesh;
	}

	Mesh::Mesh()
	{
	}

	Mesh::~Mesh()
	{
		delete m_VertexBuffer;
		delete m_VertexArray;
		delete m_IndexBuffer;
	}

	void Mesh::Render(const glm::mat4& modelMatrix, const Material* material, const Shader* overrideShader) const
	{
		m_VertexArray->Bind();
		material->Bind(overrideShader);
		RenderCommand::DrawIndexed(m_VertexArray);
		material->UnBind();
		if (overrideShader != nullptr)
			overrideShader->Unbind();
	}

	void Mesh::Render(const Shader* shader)
	{
		m_VertexArray->Bind();
		shader->Bind();
		RenderCommand::DrawIndexed(m_VertexArray);
		shader->Unbind();
	}

	void Mesh::Initialize(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		m_VertexArray = VertexArray::Create();
		m_VertexBuffer = VertexBuffer::Create(nullptr, sizeof(Vertex) * (uint32_t)vertices.size(), true);
		m_VertexBuffer->SetLayout(
			{
				{ShaderDataType::Float3,	false, "a_Position"},
				{ShaderDataType::Float3,	false, "a_Normal"},
				{ShaderDataType::Float3,	false, "a_Tangent"},
				{ShaderDataType::Float2,	false, "a_TextCoord" }

			});

		m_VertexBuffer->SetData(vertices.data(), sizeof(Vertex) * vertices.size());
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		m_IndexBuffer = IndexBuffer::Create(indices.data(), (uint32_t)indices.size(), true);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

	//---------------- MODEL ----------------

	std::unordered_map<std::string, Model*> Model::s_LoadedModels;

	Model::Model(const std::string& path)
	{
		m_Path = std::filesystem::path(path);
		std::cout << "Loading: " << path << std::endl;
		LoadModel();
	}

	Model* Model::Create(const std::string& modelName)
	{
		auto loadedModel = s_LoadedModels.find(modelName);
		if (loadedModel != s_LoadedModels.end())
			return loadedModel->second;

		Model* newModel = new Model(modelName);
		if (newModel != nullptr)
			s_LoadedModels.insert(std::make_pair(modelName, newModel));

		return newModel;
	}

	void Model::Render(const glm::mat4& modelMatrix, const std::vector<Material*>& mats, const Shader* overrideShader) const
	{
		TE_ASSERT(m_Meshes.size() == mats.size(), "[Model.Render] Meshes vector has not the same size as Materials vector.");

		for (size_t i = 0; i < m_Meshes.size(); i++)
			m_Meshes[i]->Render(modelMatrix, mats[i], overrideShader);
	}

	void Model::LoadModel()
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(m_Path.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_CalcTangentSpace);

		//Read  Load  Initialize
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::string error = "Error loading Mesh::ASSIMP::" + std::string(importer.GetErrorString());
			TE_ASSERT(false, error);
			return;
		}

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture2D*> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			// process vertex positions, normals and texture coordinates
			//[...]

			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

		// process indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		return Mesh::Create(vertices, indices);
	}
}
