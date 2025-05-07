#pragma once
#include "Component.h"

namespace dae
{
	class TeleportComponent final : public Component
	{
	public:
		TeleportComponent(GameObject* owner, float leftBound, float rightBound);
		~TeleportComponent() override = default;

		void Update(float deltaTime) override;

	private:
		float m_LeftBound;
		float m_RightBound;
	};
}
