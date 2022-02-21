#include "tepch.h"
#include "Mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace TituEngine
{
	Mesh* Mesh::Create(const std::string& path)
	{
		/*
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		*/

		return nullptr;
	}

	Mesh::Mesh()
	{
	}
}
