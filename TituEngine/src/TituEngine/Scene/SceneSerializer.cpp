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
		if (e.HasComponent<ModelRendererComponent>())
		{
			ModelRendererComponent modelRenderer = e.GetComponent<ModelRendererComponent>();
			out << modelRenderer;
		}
		if (e.HasComponent<LightComponent>())
		{
			LightComponent modelRenderer = e.GetComponent<LightComponent>();
			out << modelRenderer;
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

				YAML::Node tagCNode = entity["TagComponent"];


				if (tagCNode)
				{
					TagComponent& tagComponent = deserializedEntt.GetComponent<TagComponent>();
					tagComponent = tagCNode.as<TagComponent>();

					TE_CORE_TRACE("Deserializing entity with ID {0}, name = {1}", enttId, tagComponent.Tag);
				}

				YAML::Node tcNode = entity["TransformComponent"];
				if (tcNode)
					TransformComponent& transformComponent = deserializedEntt.AddComponent<TransformComponent>(tcNode.as<TransformComponent>());

				YAML::Node ccNode = entity["CameraComponent"];
				if (ccNode)
					CameraComponent& cameraComponent = deserializedEntt.AddComponent<CameraComponent>(ccNode.as<CameraComponent>());

				YAML::Node srCNode = entity["SpriteRendererComponent"];
				if (srCNode)
					SpriteRendererComponent& spriteRendererComponent = deserializedEntt.AddComponent<SpriteRendererComponent>(srCNode.as<SpriteRendererComponent>());

				YAML::Node mrCNode = entity["ModelRendererComponent"];
				if (mrCNode)
					ModelRendererComponent& modelRendererComponent = deserializedEntt.AddComponent<ModelRendererComponent>(mrCNode.as<ModelRendererComponent>());

				YAML::Node lCNode = entity["LightComponent"];
				if (lCNode)
					LightComponent& lightComponent = deserializedEntt.AddComponent<LightComponent>(lCNode.as<LightComponent>());

			}

			return true;
		}

		return false;
	}
}
