#pragma once

#include <glm/vec3.hpp> // glm::mat4
#include <glm/vec4.hpp> // glm::mat4
#include <glm/mat4x4.hpp> // glm::mat4
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Scene.h"
#include "Entity.h"
#include "TituEngine/Math/Math.h"

#include "TituEngine/Renderer/Camera.h"
#include "TituEngine/Renderer/Texture.h"
#include "TituEngine/Renderer/Mesh.h"

namespace TituEngine
{
	struct Component
	{
		Component() = default;
		Component(const Entity& e) : Owner(e) { }
	protected:
		Entity Owner;
	};

	struct TagComponent : public Component
	{
		const char* PrettyName = "Tag Component";

		std::string Tag;

		TagComponent() = default;
		TagComponent(const Entity& e) : Component(e) { };
		TagComponent(const Entity& e, const std::string& tag) : Component(e), Tag(tag) {}

		operator std::string& () { return Tag; }
		operator const char* () { return Tag.c_str(); }
		bool operator ==(TagComponent other) { return  Tag.compare(other.Tag) == 0; }
		bool operator !=(TagComponent other) { return  !(other == *this); }
	};

	struct TransformComponent : public Component
	{
		const char* PrettyName = "Transform Component";

		TransformComponent() = default;
		TransformComponent(const Entity& e) : Component(e) { };
		TransformComponent(const Entity& e, glm::mat4& transform) : Component(e)
		{
			UpdateMatrix(transform);
		}
		TransformComponent(const Entity& e, glm::vec3& translation) : Component(e), Translation(translation) { UpdateTransform(); }

		glm::mat4& GetTransform() { return Transform; }

		void SetTranslationAndRotation(glm::vec3& translation, glm::vec3& rotation) { Translation = translation; Rotation = rotation; UpdateTransform(); }
		void SetTranslationAndRotationAndScale(glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
		{
			Translation = translation;
			Rotation = rotation;
			Scale = scale;
			UpdateTransform();
		}

		void UpdateMatrix(const glm::mat4& transform)
		{
			Math::DecomposeTransform(transform, Translation, Rotation, Scale);
			UpdateTransform();
		}

		glm::vec3 GetTranslation() const { return Translation; }
		glm::vec3 GetRotation() const { return Rotation; }
		glm::vec3 GetScale() const { return Scale; }

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

	struct SpriteRendererComponent : public Component
	{
		const char* PrettyName = "Sprite Renderer Component";

		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f }; //white color
		SubTexture2D* Texture = nullptr;
		glm::vec2 TileSize{ 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const Entity& e) : Component(e) { };
		SpriteRendererComponent(const Entity& e, glm::vec4& color) : Component(e), Color(color) {}

		operator glm::vec4& () { return Color; }
		operator const glm::vec4& () const { return Color; }
	};

	struct CameraComponent : public Component
	{
		const char* PrettyName = "Camera Component Component";

		TituEngine::Camera Camera;

		CameraComponent() = default;
		CameraComponent(const Entity& e) : Component(e) { };
		CameraComponent(const Entity& e, const TituEngine::Camera& c) : Component(e), Camera(c) { };

		void SetAsActiveCamera()
		{
			TransformComponent& T = Owner.GetComponent<TransformComponent>();
			glm::mat4& viewMat = T.GetTransform();
			TituEngine::Camera::SetActiveCamera(&Camera, &viewMat, true);
		}

		operator TituEngine::Camera& () { return Camera; }
		operator const TituEngine::Camera& () { return Camera; }

	private:
	};

	struct ModelRendererComponent : public Component 
	{
		const char* PrettyName = "m_Model Renderer Components";

		ModelRendererComponent() = default;
		ModelRendererComponent(const Entity& e) : Component(e), m_Model(nullptr) { }
		ModelRendererComponent(const Entity& e, TituEngine::Model* model) : Component(e) { SetModel(model); };

		void SetModel(Model* model)
		{
			m_Materials.clear();

			m_Model = model;
			m_Materials.clear();
			m_Materials.resize(m_Model->GetMeshesCount());
			for (size_t i = 0; i < m_Materials.size(); i++)
				m_Materials[i] = MaterialUtilities::s_DefaultMaterial;
		}

		Model* GetModel() const { return m_Model; }
		std::vector<Material*> GetMaterials() const { return m_Materials; }

	private:

		TituEngine::Model* m_Model;
		std::vector<TituEngine::Material*> m_Materials;
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
	struct NativeScriptComponent : public Component
	{

		NativeScriptComponent(const Entity& e) : Component(e), Instance(nullptr), InstantiateScript(nullptr), DestroyScript(nullptr) { }

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

