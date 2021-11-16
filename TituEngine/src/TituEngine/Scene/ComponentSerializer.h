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
			component.Camera.SetFOV(node["AspectRatio"].as<float>());
			component.Camera.SetFOV(node["NearPlane"].as<float>());
			component.Camera.SetFOV(node["FarPlane"].as<float>());
			component.Camera.SetFOV(node["Fov"].as<float>());
			component.Camera.SetFOV(node["OrthographicSize"].as<float>());
			component.Camera.SetFOV(node["ProjectionType"].as<float>());
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
		out << YAML::Key << "Translation" << YAML::Value << c.Translation;
		out << YAML::Key << "Rotation" << YAML::Value << c.Rotation;
		out << YAML::Key << "Scale" << YAML::Value << c.Scale;
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
}