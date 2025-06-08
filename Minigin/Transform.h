#pragma once
#include <glm.hpp>

namespace dae
{
	class GameObject;

	class Transform final
	{
	public:
		explicit Transform(GameObject* owner = nullptr) : m_Owner(owner) {}

		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
		const glm::vec3& GetWorldPosition() const;
		const glm::vec3& GetRotation() const { return m_LocalRotation; }

		void SetLocalPosition(const glm::vec3& pos);
		void SetWorldPosition(const glm::vec3& pos);
		void SetRotation(const glm::vec3& rot);
		void SetPositionDirty() { m_PositionIsDirty = true; }

	private:
		void UpdateWorldPosition();

		GameObject* m_Owner;
		glm::vec3 m_LocalPosition{ 0,0,0 };
		glm::vec3 m_WorldPosition{ 0,0,0 };
		glm::vec3 m_LocalRotation{ 0,0,0 };
		bool m_PositionIsDirty{ true };

		friend class GameObject;
	};
}
