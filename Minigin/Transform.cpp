#include "Transform.h"
#include "GameObject.h" 

namespace dae
{
	void Transform::SetLocalPosition(const glm::vec3& pos)
	{
		if (m_LocalPosition != pos)
		{
			m_LocalPosition = pos;
			SetPositionDirty();
		}
	}

	const glm::vec3& Transform::GetWorldPosition() const
	{
		if (m_PositionIsDirty) const_cast<Transform*>(this)->UpdateWorldPosition();

		return m_WorldPosition;
	}

	void dae::Transform::SetRotation(const glm::vec3& rot)
	{
		m_LocalRotation = rot;
		SetPositionDirty();
	}

	void Transform::SetWorldPosition(const glm::vec3& pos)
	{
		if (m_Owner && m_Owner->GetParent())
		{
			glm::vec3 parentWorldPos = m_Owner->GetParent()->GetTransform().GetWorldPosition();
			m_LocalPosition = pos - parentWorldPos;
		}
		else
		{
			m_LocalPosition = pos;
		}

		SetPositionDirty();
	}

	void Transform::UpdateWorldPosition()
	{
		if (m_PositionIsDirty)
		{
			if (m_Owner && m_Owner->GetParent())
				m_WorldPosition = m_Owner->GetParent()->GetTransform().GetWorldPosition() + m_LocalPosition;
			else
				m_WorldPosition = m_LocalPosition;

			m_PositionIsDirty = false;
		}
	}
}

