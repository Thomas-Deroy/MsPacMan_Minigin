#pragma once
#include "Component.h"
#include "GameObject.h"
#include "CollisionSystem.h"

namespace dae
{
	class ColliderComponent final : public Component
	{
	public:
		ColliderComponent(GameObject* owner, const glm::vec2& size, const glm::vec2& offset = {}, CollisionLayer layer = CollisionLayer::Default, bool debugDraw = false);
		~ColliderComponent();

		void Update(float deltaTime) override;
		void Render() const override;

		glm::vec2 GetPosition() const;
		glm::vec2 GetSize() const;
		glm::vec2 GetCenter() const;

		CollisionLayer GetLayer() const { return m_Layer; }
		void SetLayer(CollisionLayer layer) { m_Layer = layer; }

		bool IsOverlapping(const ColliderComponent& other) const;
	private:
		glm::vec2 m_Size{};
		glm::vec2 m_Offset{};
		bool m_DebugDraw{};
		CollisionLayer m_Layer{};
	};
}
