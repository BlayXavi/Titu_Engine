#pragma once

#include "Buffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace TituEngine
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;

		//Editor
		uint32_t EntityID;
	};


	class Mesh
	{
	private:

		Mesh();
		virtual ~Mesh();

		void Initialize(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Texture2D*>& textures);

	private:
		VertexBuffer* m_VertexBuffer = nullptr;
		VertexArray* m_VertexArray = nullptr;
		IndexBuffer* m_IndexBuffer = nullptr;

		std::vector<Texture2D*> m_Textures2D;

	public:
		static Mesh* Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Texture2D*>& textures);
		void Render(const Shader* shader) const;
	};

	class Model
	{
	public:
		Model(const std::string& modelName);
		void Render(const Shader* shader);

	private:
		std::vector<Mesh*> m_Meshes;
		std::string m_ModelName;
		std::string m_ModelNameNoExtension;


		void LoadModel();
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Texture2D*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}