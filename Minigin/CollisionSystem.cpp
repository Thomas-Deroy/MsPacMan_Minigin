#include "CollisionSystem.h"
#include "ColliderComponent.h"
#include "EventSystem.h"
#include <algorithm>

namespace dae
{
    void CollisionSystem::RegisterCollider(ColliderComponent* collider)
    {
        std::lock_guard<std::mutex> lock(m_CollidersMutex);
        m_Colliders.push_back(collider);
    }

    void CollisionSystem::UnregisterCollider(ColliderComponent* collider)
    {
        std::lock_guard<std::mutex> lock(m_CollidersMutex);
        auto it = std::remove(m_Colliders.begin(), m_Colliders.end(), collider);
        m_Colliders.erase(it, m_Colliders.end());
    }

    void CollisionSystem::Update()
    {
        CleanupColliders();

        std::unordered_set<ColliderPair, PairHash> currentCollisions;

        for (size_t i = 0; i < m_Colliders.size(); ++i)
        {
            auto* a = m_Colliders[i];
            if (!a) continue;

            for (size_t j = i + 1; j < m_Colliders.size(); ++j)
            {
                auto* b = m_Colliders[j];
                if (!b) continue;


                if (a->IsOverlapping(*b))
                {
                    auto key = MakePair(a, b);
                    currentCollisions.insert(key);

                    bool wasColliding = m_PreviousCollisions.find(key) != m_PreviousCollisions.end();

                    SendCollisionEvents(a, b, wasColliding);
                }
            }
        }

		// OnTriggerExit
        for (const auto& prev : m_PreviousCollisions)
        {
            if (currentCollisions.find(prev) == currentCollisions.end())
            {
                CollisionEventArg exitArg{ prev.first, prev.second };
                Event exitEvent(OnTriggerExit);
                exitEvent.args[0] = &exitArg;
                exitEvent.nbArgs = 1;
                EventSystem::GetInstance().Notify(exitEvent);
            }
        }

        m_PreviousCollisions = std::move(currentCollisions);
    }

    void CollisionSystem::CleanupColliders()
    {
        std::lock_guard<std::mutex> lock(m_CollidersMutex);
        m_Colliders.erase(
            std::remove_if(m_Colliders.begin(), m_Colliders.end(),
                [](auto* c) { return c == nullptr; }),
            m_Colliders.end());
    }

    void CollisionSystem::SendCollisionEvents(ColliderComponent* a, ColliderComponent* b, bool wasColliding)
    {
        if (!wasColliding)
        {
            // OnTriggerEnter
            CollisionEventArg enterArg{ a, b };
            Event enterEvent(OnTriggerEnter);
            enterEvent.args[0] = &enterArg;
            enterEvent.nbArgs = 1;
            EventSystem::GetInstance().Notify(enterEvent);
        }

        // OnTriggerStay
        CollisionEventArg stayArg{ a, b };
        Event stayEvent(OnTriggerStay);
        stayEvent.args[0] = &stayArg;
        stayEvent.nbArgs = 1;
        EventSystem::GetInstance().Notify(stayEvent);

        // Layer-specific events
        SendLayerSpecificEvent(b, a);
    }

    void CollisionSystem::SendLayerSpecificEvent(ColliderComponent* self, ColliderComponent* other)
    {
        EventId eventId = OnTriggerDefault;

        switch (other->GetLayer())
        {
        case CollisionLayer::Friendly: eventId = OnTriggerFriendly; break;
        case CollisionLayer::Enemy: eventId = OnTriggerEnemy; break;
        case CollisionLayer::Wall: eventId = OnTriggerWall; break;
        case CollisionLayer::Object: eventId = OnTriggerObject; break;
        default: break;
        }

        CollisionEventArg arg{ self, other };
        Event event(eventId);
        event.args[0] = &arg;
        event.nbArgs = 1;
        EventSystem::GetInstance().Notify(event);
    }

    CollisionSystem::ColliderPair CollisionSystem::MakePair(ColliderComponent* a, ColliderComponent* b)
    {
        return (a < b) ? std::make_pair(a, b) : std::make_pair(b, a);
    }

    std::size_t CollisionSystem::PairHash::operator()(const ColliderPair& p) const
    {
        return std::hash<ColliderComponent*>()(p.first) ^ std::hash<ColliderComponent*>()(p.second);
    }

    void CollisionSystem::SetLayerMask(CollisionLayer layer, unsigned int mask)
    {
        m_LayerMasks[layer] = mask;
    }

    unsigned int CollisionSystem::GetLayerMask(CollisionLayer layer) const
    {
        auto it = m_LayerMasks.find(layer);
        return (it != m_LayerMasks.end()) ? it->second : 0xFFFFFFFF;
    }

    bool CollisionSystem::WouldCollide(const ColliderComponent* self, const glm::vec2& newPos, CollisionLayer collideWithLayer) const
    {
        if (!self)
            return false;

        const SDL_Rect selfBounds = self->GetBoundsAtPosition(newPos);

        for (auto* other : m_Colliders)
        {
            if (!other || other == self)
                continue;


            if (other->GetLayer() != collideWithLayer)
                continue;

            const SDL_Rect otherBounds = other->GetBoundsAtPosition(other->GetWorldPosition());

            if (ColliderComponent::RectsOverlap(selfBounds, otherBounds))
                return true;
        }

        return false;
    }

}
