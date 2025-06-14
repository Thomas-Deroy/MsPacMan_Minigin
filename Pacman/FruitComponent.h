#pragma once
#include "Component.h"
#include "GameObject.h"
#include "ColliderComponent.h"
#include "SpriteComponent.h"
#include "IFruitState.h"
#include "FruitExitState.h"
#include "FruitEnterState.h"
#include "LevelBuilder.h"
#include <memory>

namespace dae
{
	class MovementComponent;

	class FruitComponent : public Component
	{
	public:
		FruitComponent(GameObject* owner, LevelBuilder* builder);

		void Update(float deltaTime) override;

		ColliderComponent* GetCollider() const { return GetOwner()->GetComponent<ColliderComponent>(); };
		SpriteComponent* GetSprite() const { return GetOwner()->GetComponent<SpriteComponent>(); };
		MovementComponent* GetMovement() const { return GetOwner()->GetComponent<MovementComponent>(); };
		LevelBuilder* GetLevelBuilder() const;
		float GetStateTimer() const { return m_StateTimer; }
		bool GetExitState() { return typeid(*m_CurrentState) == typeid(FruitExitState); }
		bool GetEnterState() { return typeid(*m_CurrentState) == typeid(FruitEnterState); }

		void ResetStateTimer();
		void ChangeState(std::unique_ptr<IFruitState> newState);

	private:
		float m_DirectionCooldown{};
		LevelBuilder* m_LevelBuilder;

		std::unique_ptr<IFruitState> m_CurrentState;
		float m_StateTimer{};
	};
}
