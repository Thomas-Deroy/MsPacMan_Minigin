#include "TeleportComponent.h"
#include "GameObject.h"

namespace dae
{
	TeleportComponent::TeleportComponent(GameObject* owner, float leftBound, float rightBound, LevelBuilder* builder)
		: Component(owner), m_LeftBound(leftBound), m_RightBound(rightBound), m_LevelBuilder(builder)
	{
		m_MovementComponent = owner->GetComponent<MovementComponent>();
	}

	void TeleportComponent::Update(float)
	{
		glm::vec3 pos = GetOwner()->GetTransform().GetLocalPosition();

		if (pos.x < m_LeftBound)
		{
			pos.x = m_RightBound;
			GetOwner()->GetTransform().SetLocalPosition(pos);
			m_MovementComponent->SetTargetNode(nullptr);
			m_MovementComponent->FindAndSetStartNode(m_LevelBuilder);
		}

		if (pos.x > m_RightBound)
		{
			std::cout << "Teleporting left" << std::endl;
			pos.x = m_LeftBound;
			GetOwner()->GetTransform().SetLocalPosition(pos);
			m_MovementComponent->SetTargetNode(nullptr);
			m_MovementComponent->FindAndSetStartNode(m_LevelBuilder);
		}
	}
}
