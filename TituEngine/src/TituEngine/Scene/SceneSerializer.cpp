#include "tepch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"
#include "ComponentSerializer.h"

#include <yaml-cpp/yaml.h>

#include <sstream>
#include <fstream>

namespace TituEngine
{
	void SerializeEntity(YAML::Emitter& out, const Entity& e)
	{
		out << YAML::BeginMap;

		out << YAML::Key << "Entity" << YAML::Value << "123";

		if (e.HasComponent<TagComponent>())
		{
			TagComponent& tag = e.GetComponent<TagComponent>();
			out << tag;
		}
		if (e.HasComponent<TransformComponent>())
		{
			TransformComponent transform = e.GetComponent<TransformComponent>();
			out << transform;
		}
		if (e.HasComponent<CameraComponent>())
		{
			CameraComponent camera = e.GetComponent<CameraComponent>();
			out << camera;
		}
		if (e.HasComponent<SpriteRendererComponent>())
		{
			SpriteRendererComponent spriteRenderer = e.GetComponent<SpriteRendererComponent>();
			out << spriteRenderer;
		}
		out << YAML::EndMap;
	}

	void SceneSerializer::SerializeScene(Scene* const scene, const std::string path)
	{
		TE_CORE_TRACE("Serializing scene {0}", path);

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Scene Name";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		scene->m_Registry.each([&](auto entityId)
			{
				Entity e{ entityId, scene };
				SerializeEntity(out, e);
			});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout;
		fout.open(path);
		fout << out.c_str();
		fout.close();
	}

	bool SceneSerializer::DeserializeScene(Scene* const scene, const std::string path)
	{
		std::ifstream stream(path);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream);
		if (!data["Scene"])
		{
			TE_ASSERT(false, "Scene cannot be deserialized. {0}", path);
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();
		TE_CORE_TRACE("Deserializing scene {0}", sceneName);

		YAML::Node entities = data["Entities"];
		if (entities)
		{
			for each (YAML::Node entity in entities)
			{
				uint64_t enttId = entity["Entity"].as<uint64_t>();

				Entity deserializedEntt = scene->CreateEntity();

				std::string tag = "";
				YAML::Node tagCNode = entity["TagComponent"];
				if (tagCNode)
					TagComponent& tagComponent = deserializedEntt.AddOrGetComponent<TagComponent>(tagCNode.as<TagComponent>());

				TE_CORE_TRACE("Deserializing entity with ID {0}, name = {1}", enttId, tag);

				YAML::Node tcNode = entity["TransformComponent"];
				if (tcNode)
					TransformComponent& transformComponent = deserializedEntt.AddOrGetComponent<TransformComponent>(tcNode.as<TransformComponent>());

				YAML::Node ccNode = entity["CameraComponent"];
				if (ccNode)
					CameraComponent& cameraComponent = deserializedEntt.AddOrGetComponent<CameraComponent>(ccNode.as<CameraComponent>());

				YAML::Node srCNode = entity["SpriteRendererComponent"];
				if (srCNode)
					SpriteRendererComponent& spriteRendererComponent = deserializedEntt.AddOrGetComponent<SpriteRendererComponent>(srCNode.as<SpriteRendererComponent>());
			}

			return true;
		}

		return false;
	}
}
