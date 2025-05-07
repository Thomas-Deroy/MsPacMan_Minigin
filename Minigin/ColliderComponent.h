#pragma once
#include "Component.h"
#include "GameObject.h"
#include "CollisionSystem.h"

namespace dae
{
	class ColliderComponent final : public Component
	{
	public:
		ColliderComponent(GameObject* owner, const glm::vec2& size, const glm::vec2& offset = {}, CollisionLayer layer = CollisionLayer::Default, bool isTrigger = false, bool debugDraw = false);
		~ColliderComponent();

		void Update(float) override;
		void Render() const override;

		glm::vec2 GetPosition() const;
		glm::vec2 GetSize() const;
		glm::vec2 GetCenter() const;
		bool IsOverlapping(const ColliderComponent& other) const;
		bool IsTrigger() const { return m_IsTrigger; }
		CollisionLayer GetLayer() const { return m_Layer; }
		glm::vec2 GetWorldPosition() const; 

		SDL_Rect GetBoundsAtPosition(const glm::vec2& pos) const;
		static bool RectsOverlap(const SDL_Rect& a, const SDL_Rect& b);

	private:
		glm::vec2 m_Size{};
		glm::vec2 m_Offset{};
		bool m_DebugDraw{};
		bool m_IsTrigger{};
		CollisionLayer m_Layer{};
	};
}
