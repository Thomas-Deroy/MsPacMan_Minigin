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

		void SetLocalPosition(const glm::vec3& pos);
		void SetWorldPosition(const glm::vec3& pos);
		void SetPositionDirty() { m_PositionIsDirty = true; }

	private:
		void UpdateWorldPosition();

		GameObject* m_Owner;
		glm::vec3 m_LocalPosition{};
		glm::vec3 m_WorldPosition{};
		bool m_PositionIsDirty{ true };

		friend class GameObject;
	};
}
