#include "TeleportComponent.h"
#include "GameObject.h"

namespace dae
{
	TeleportComponent::TeleportComponent(GameObject* owner, float leftBound, float rightBound)
		: Component(owner), m_LeftBound(leftBound), m_RightBound(rightBound)
	{
	}

	void TeleportComponent::Update(float)
	{
		glm::vec3 pos = GetOwner()->GetTransform().GetLocalPosition();

		if (pos.x < m_LeftBound)
		{
			pos.x = m_RightBound;
			GetOwner()->GetTransform().SetLocalPosition(pos);
		}
		else if (pos.x > m_RightBound)
		{
			pos.x = m_LeftBound;
			GetOwner()->GetTransform().SetLocalPosition(pos);
		}
	}
}
