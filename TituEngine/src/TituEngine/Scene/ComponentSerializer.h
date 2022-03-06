#pragma once

#include <iostream>
#include "Components.h"
#include "yaml-cpp/yaml.h"
#include "glm/vec3.hpp"

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static bool decode(const Node& node, glm::vec3& v)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			v.z = node[2].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static bool decode(const Node& node, glm::vec4& v)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			v.z = node[2].as<float>();
			v.w = node[3].as<float>();

			return true;
		}
	};

	template<>
	struct convert<TituEngine::TagComponent>
	{
		static bool decode(const Node& node, TituEngine::TagComponent& component)
		{
			component.Tag = node["Tag"].as<std::string>();
			return true;
		}
	};

	template<>
	struct convert<TituEngine::TransformComponent>
	{
		static bool decode(const Node& node, TituEngine::TransformComponent& component)
		{
			component.SetTranslationAndRotationAndScale(
				node["Translation"].as<glm::vec3>(),
				node["Rotation"].as<glm::vec3>(),
				node["Scale"].as<glm::vec3>());
			return true;
		}
	};

	template<>
	struct convert<TituEngine::SpriteRendererComponent>
	{
		static bool decode(const Node& node, TituEngine::SpriteRendererComponent& component)
		{
			component.Color = node["Color"].as<glm::vec4>();
			return true;
		}
	};

	template<>
	struct convert<TituEngine::CameraComponent>
	{
		static bool decode(const Node& node, TituEngine::CameraComponent& component)
		{
			component.Camera.SetAspectRatio(node["AspectRatio"].as<float>());
			component.Camera.SetNearPlane(node["NearPlane"].as<float>());
			component.Camera.SetFarPlane(node["FarPlane"].as<float>());
			component.Camera.SetFOV(node["Fov"].as<float>());
			component.Camera.SetOrthographicSize(node["OrthographicSize"].as<float>());
			component.Camera.SetProjectionType((TituEngine::Camera::Projection)node["ProjectionType"].as<int>());
			return true;
		}
	};

	template<>
	struct convert<TituEngine::ModelRendererComponent>
	{
		static bool decode(const Node& node, TituEngine::ModelRendererComponent& component)
		{
			component.SetModel(node["AssetPath"].as<std::string>());
			std::vector<TituEngine::Material*>& materials = component.GetMaterials();

			for (size_t i = 0; i < materials.size(); i++)
			{
				TituEngine::Material** mat = &materials[i];
				const Node& matNode = node["Materials"][i];
				std::string shaderPath = matNode["ShaderPath"].as<std::string>();
				TituEngine::Shader* shader = TituEngine::Shader::Create(shaderPath);
				(*mat) = TituEngine::Material::Create(shader);
				std::vector<TituEngine::Texture2D*>& textures = const_cast<std::vector<TituEngine::Texture2D*>&>((*mat)->GetTextures());
				for (size_t j = 0; j < textures.size(); j++)
				{
					const Node& textsNode = matNode["Textures"][j];
					if (textsNode.IsDefined() && textsNode.IsNull() == false)
					{
						std::string texPath = textsNode.as<std::string>();
						if (texPath.empty() == false)
							textures[j] = TituEngine::Texture2D::Create(texPath);
					}
				}
			}
			return true;
		}
	};

	template<>
	struct convert<TituEngine::LightComponent>
	{
		static bool decode(const Node& node, TituEngine::LightComponent& component)
		{
			component.Color = node["Color"].as<glm::vec4>();
			component.LightType = (TituEngine::LightType)node["Type"].as<int>();
			return true;
		}
	};
}

namespace TituEngine
{
	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;

		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;

		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter& out, const TagComponent& c)
	{
		out << YAML::Key << "TagComponent";
		out << YAML::BeginMap;

		out << YAML::Key << "Tag" << YAML::Value << c.Tag;

		out << YAML::EndMap;

		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter& out, const TransformComponent& c)
	{
		out << YAML::Key << "TransformComponent";

		out << YAML::BeginMap;
		out << YAML::Key << "Translation" << YAML::Value << c.GetTranslation();
		out << YAML::Key << "Rotation" << YAML::Value << c.GetRotation();
		out << YAML::Key << "Scale" << YAML::Value << c.GetScale();
		out << YAML::EndMap;

		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter& out, const SpriteRendererComponent& c)
	{
		out << YAML::Key << "SpriteRendererComponent";

		out << YAML::BeginMap;
		out << YAML::Key << "Color" << YAML::Value << c.Color;
		out << YAML::EndMap;

		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter& out, const CameraComponent& c)
	{
		out << YAML::Key << "CameraComponent";

		out << YAML::BeginMap;

		Camera cam = c.Camera;

		out << YAML::Key << "AspectRatio" << YAML::Value << cam.GetAspectRatio();
		out << YAML::Key << "NearPlane" << YAML::Value << cam.GetNearPlane();
		out << YAML::Key << "FarPlane" << YAML::Value << cam.GetFarPlane();
		out << YAML::Key << "Fov" << YAML::Value << cam.GetFOV();
		out << YAML::Key << "OrthographicSize" << YAML::Value << cam.GetOrthographicSize();
		out << YAML::Key << "ProjectionType" << YAML::Value << (int)cam.GetProjectionType();

		out << YAML::EndMap;

		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter& out, const Texture2D* texture)
	{
		out << texture->GetPath();
		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter& out, const std::vector<Texture2D*>& textures)
	{
		out << YAML::BeginSeq;

		for (size_t i = 0; i < textures.size(); i++)
			out << textures[i];

		out << YAML::EndSeq;

		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter& out, const TituEngine::Material* material)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "ShaderPath" << YAML::Value << material->GetShader()->GetPath();
		const std::vector<Texture2D*>& textures = material->GetTextures();
		out << YAML::Key << "Textures" << YAML::Value << textures;
		out << YAML::EndMap;
		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter& out, const std::vector<Material*>& materials)
	{
		out << YAML::BeginSeq;
		for (size_t i = 0; i < materials.size(); i++)
			out << materials[i];
		out << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter& out, const ModelRendererComponent& mdr)
	{
		out << YAML::Key << "ModelRendererComponent";

		out << YAML::BeginMap;

		Model* model = mdr.GetModel();
		if (model != nullptr)
			out << YAML::Key << "AssetPath" << YAML::Value << model->GetAssetPath().string().c_str();

		std::vector<Material*>& materials = const_cast<ModelRendererComponent&>(mdr).GetMaterials();
		if (materials.size() > 0)
			out << YAML::Key << "Materials" << YAML::Value << materials;

		out << YAML::EndMap;
		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter& out, const LightComponent& lc)
	{
		out << YAML::Key << "LightComponent";

		out << YAML::BeginMap;

		out << YAML::Key << "Color" << YAML::Value << lc.Color;
		out << YAML::Key << "Type" << YAML::Value << (int)lc.LightType;

		out << YAML::EndMap;

		return out;
	}

}