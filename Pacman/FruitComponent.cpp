#include "FruitComponent.h"
#include "MovementComponent.h"
#include <cstdlib>
#include <ctime>

namespace dae
{
	FruitComponent::FruitComponent(GameObject* owner, LevelBuilder* builder)
		: Component(owner), m_LevelBuilder(builder)
	{
		GetMovement()->SetSpeedMultiplier(0.5f);
	}

	void FruitComponent::Update(float deltaTime)
	{
		if (!m_CurrentState) return;

		m_StateTimer += deltaTime;
		m_DirectionCooldown -= deltaTime;

		if (auto newState = m_CurrentState->Update(*this, deltaTime))
		{
			ChangeState(std::move(newState));
		}
	}

	void FruitComponent::ChangeState(std::unique_ptr<IFruitState> newState)
	{
		if (m_CurrentState)
			m_CurrentState->Exit(*this);

		m_CurrentState = std::move(newState);

		if (m_CurrentState)
			m_CurrentState->Enter(*this);
	}

	void FruitComponent::ResetStateTimer()
	{
		m_StateTimer = 0.0f;
	}

	LevelBuilder* FruitComponent::GetLevelBuilder() const
	{
		return m_LevelBuilder;
	}

}
