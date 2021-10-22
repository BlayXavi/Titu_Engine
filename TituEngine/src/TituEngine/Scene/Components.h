#pragma once

#include <glm/vec4.hpp> // glm::mat4
#include <glm/mat4x4.hpp> // glm::mat4

#include "TituEngine/Renderer/Camera.h"
#include "Scene.h"
#include "Entity.h"

namespace TituEngine
{
	struct TagComponent
	{
		std::string Tag;
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) :
			Tag(tag) {}

		operator std::string& () { return Tag; }
		operator const char* () { return Tag.c_str(); }
		bool operator ==(TagComponent other) { return  Tag.compare(other.Tag) == 0; }
		bool operator !=(TagComponent other) { return  !(other == *this); }
	};

	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(glm::mat4& transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f }; //white color

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(glm::vec4& color)
			: Color(color) {}

		operator glm::vec4& () { return Color; }
		operator const glm::vec4& () const { return Color; }
	};

	struct CameraComponent
	{
		TituEngine::Camera Camera;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(TituEngine::Camera& camera) : Camera(camera) {};
	};

	class TituEditorLayer; //temp
	class NativeScript
	{
	public:
		virtual ~NativeScript() = default;

		template<class T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		friend Scene;

		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}

	private:
		friend TituEditorLayer;
		Entity m_Entity;
	};

	//References and Call the binded NativeScript
	struct NativeScriptComponent
	{
		NativeScript* Instance;

		NativeScript* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<class T>
		void Bind()
		{
			InstantiateScript = []() {return static_cast<NativeScript*>(new T()); };
			DestroyScript = [](NativeScriptComponent* ncs) {delete ncs->Instance; ncs->Instance = nullptr; };
		}

	};
}

