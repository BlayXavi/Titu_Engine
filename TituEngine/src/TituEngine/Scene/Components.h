#pragma once

#include <glm/vec3.hpp> // glm::mat4
#include <glm/vec4.hpp> // glm::mat4
#include <glm/mat4x4.hpp> // glm::mat4

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "TituEngine/Renderer/Camera.h"
#include "Scene.h"
#include "Entity.h"

namespace TituEngine
{
	struct TagComponent
	{
		const char* PrettyName = "Tag Component";

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
		const char* PrettyName = "Transform Component";

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(glm::mat4& transform)
			: Transform(transform) {}
		TransformComponent(glm::vec3& translation)
			: Translation(translation)
		{
			UpdateTransform();
		}

		void SetTranslation(glm::vec3& translation) { Translation = translation; UpdateTransform(); }
		void SetRotation(glm::vec3& rotation) { Rotation = rotation; UpdateTransform(); }
		void SetScale(glm::vec3& scale) { Scale = scale; UpdateTransform(); }

		glm::vec3 GetTranslation() const { return Translation; }
		glm::vec3 GetRotation() const { return Rotation; }
		glm::vec3 GetScale() const { return Scale; }

		glm::mat4 GetTransform() const { return Transform; }

		void SetTranslationAndRotation(glm::vec3& translation, glm::vec3& rotation) { Translation = translation; Rotation = rotation; UpdateTransform(); }
		void SetTranslationAndRotationAndScale(glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale) { Translation = translation; Rotation = rotation; Scale = scale; UpdateTransform(); }

		operator glm::mat4& () { return Transform; }
		operator glm::mat4* () { return &Transform; }
		operator const glm::mat4& () const { return Transform; }
		operator const glm::mat4* () const { return &Transform; }

	private:

		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		void UpdateTransform()
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			Transform = glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		glm::mat4 Transform{ 1.0f };
	};

	struct SpriteRendererComponent
	{
		const char* PrettyName = "Sprite Renderer Component";

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
		const char* PrettyName = "Camera Component Component";

		TituEngine::Camera Camera;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(TituEngine::Camera& camera) : Camera(camera) {};

		void SetAsActiveCamera()
		{
			TituEngine::Camera::SetActiveCamera(&Camera, m_ViewMatrix);
		}

	private:
		glm::mat4* m_ViewMatrix = nullptr;
	};

	class TituEditorLayer; //temp
	class NativeScript
	{
	public:
		char* PrettyName = "Native Script";

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

