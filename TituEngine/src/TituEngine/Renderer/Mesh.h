#pragma once

#include "TituEngine/Core/FilesystemUtilities.h"

#include "Buffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Material.h"

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

		void Initialize(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

	private:
		VertexBuffer* m_VertexBuffer = nullptr;
		VertexArray* m_VertexArray = nullptr;
		IndexBuffer* m_IndexBuffer = nullptr;

	public:
		static Mesh* Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		void Render(const glm::mat4& modelMatrix, const Material* shader) const;
	};

	class Model
	{
	public:
		Model() = default;
		static Model* Create(const std::string& modelName);
		void Render(const glm::mat4& modelMatrix, const std::vector<Material*>& mats) const;
		uint32_t GetMeshesCount() const { return (uint32_t) m_Meshes.size(); }
	private:
		Model(const std::string& path);

		std::vector<Mesh*> m_Meshes;

		std::filesystem::path m_Path;
		std::string m_Extension;

		void LoadModel();
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	};
}