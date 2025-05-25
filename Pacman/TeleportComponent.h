#pragma once
#include "Component.h"
#include "LevelBuilder.h"
#include "MovementComponent.h"

namespace dae
{
	class TeleportComponent final : public Component
	{
	public:
		TeleportComponent(GameObject* owner, float leftBound, float rightBound, LevelBuilder* builder);
		~TeleportComponent() override = default;

		void Update(float deltaTime) override;

	private:
		float m_LeftBound;
		float m_RightBound;
		MovementComponent* m_MovementComponent{ nullptr };
		LevelBuilder* m_LevelBuilder;
	};
}
