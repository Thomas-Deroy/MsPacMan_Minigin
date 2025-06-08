#pragma once
#include "GameObject.h"
#include "Component.h"
#include <memory>

namespace dae
{
	class PowerPelletComponent : public Component
	{
    public:
        PowerPelletComponent(GameObject* owner) : Component(owner) {};

		void Update(float deltaTime) override
		{
			m_Timer += deltaTime;
			if (m_Timer >= 0.2f)
			{
				m_Timer -= 0.2f; 
				m_Visible = !m_Visible;

				GetOwner()->GetComponent<RenderComponent>()->SetVisible(m_Visible);
			}
		}
    private:
		float m_Timer = 0.f;
		bool m_Visible = true;
    };
}