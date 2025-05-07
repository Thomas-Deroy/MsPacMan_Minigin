#include "ColliderComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include <SDL.h>

namespace dae
{
	ColliderComponent::ColliderComponent(GameObject* owner, const glm::vec2& size, const glm::vec2& offset, CollisionLayer layer, bool isTrigger, bool debugDraw)
		: Component(owner), m_Size(size), m_Offset(offset), m_Layer(layer), m_IsTrigger(isTrigger), m_DebugDraw(debugDraw)
	{
		CollisionSystem::GetInstance().RegisterCollider(this);
	}

	ColliderComponent::~ColliderComponent() 
	{
		//CollisionSystem::GetInstance().UnregisterCollider(this);
	}

	void ColliderComponent::Update(float) {}

	void ColliderComponent::Render() const
	{
		if (!m_DebugDraw) return;

		glm::vec2 pos = GetPosition();
		SDL_Rect rect{ static_cast<int>(pos.x), static_cast<int>(pos.y),
					   static_cast<int>(m_Size.x), static_cast<int>(m_Size.y) };

		SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
	}

	glm::vec2 ColliderComponent::GetPosition() const
	{
		const auto& worldPos = GetOwner()->GetTransform().GetWorldPosition();
		return { worldPos.x + m_Offset.x, worldPos.y + m_Offset.y };
	}

	glm::vec2 ColliderComponent::GetSize() const
	{
		return m_Size;
	}

	glm::vec2 ColliderComponent::GetCenter() const
	{
		const auto pos = GetPosition();
		return { pos.x + m_Size.x / 2.f, pos.y + m_Size.y / 2.f };
	}

	bool ColliderComponent::IsOverlapping(const ColliderComponent& other) const
	{
		const auto posA = GetPosition();
		const auto posB = other.GetPosition();

		bool overlapX = posA.x < posB.x + other.m_Size.x && posA.x + m_Size.x > posB.x;
		bool overlapY = posA.y < posB.y + other.m_Size.y && posA.y + m_Size.y > posB.y;

		return overlapX && overlapY;
	}

	glm::vec2 ColliderComponent::GetWorldPosition() const
	{
		glm::vec3 ownerPos = m_Owner->GetPosition();
		glm::vec3 offset3D(m_Offset.x, m_Offset.y, 0.0f);
		return ownerPos + offset3D;
	}


	SDL_Rect ColliderComponent::GetBoundsAtPosition(const glm::vec2& pos) const
	{
		return SDL_Rect{
			static_cast<int>(pos.x + m_Offset.x),
			static_cast<int>(pos.y + m_Offset.y),
			static_cast<int>(m_Size.x),
			static_cast<int>(m_Size.y)
		};
	}

	bool ColliderComponent::RectsOverlap(const SDL_Rect& a, const SDL_Rect& b)
	{
		return !(a.x + a.w <= b.x || b.x + b.w <= a.x || a.y + a.h <= b.y || b.y + b.h <= a.y);
	}
}
