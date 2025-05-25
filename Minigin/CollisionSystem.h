#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <mutex>
#include <SDL.h>
#include "GameObject.h"
#include "Singleton.h"
#include "EventSystem.h"

namespace dae
{
	class ColliderComponent;

	enum class CollisionLayer : unsigned int
	{
		Default = make_sdbm_hash("Default"),
		Friendly = make_sdbm_hash("Friendly"),
		Enemy = make_sdbm_hash("Enemy"),
		Wall = make_sdbm_hash("Wall"),
		Object = make_sdbm_hash("Object")
	};

	struct CollisionEventArg : public EventArg
	{
		ColliderComponent* self{};
		ColliderComponent* other{};
		CollisionEventArg(ColliderComponent* self, ColliderComponent* other)
			: self(self), other(other) {
		}
	};

	class CollisionSystem final : public Singleton<CollisionSystem>
	{
	public:
		void RegisterCollider(ColliderComponent* collider);
		void UnregisterCollider(ColliderComponent* collider);
		void Update();

		void SetLayerMask(CollisionLayer layer, unsigned int mask);
		unsigned int GetLayerMask(CollisionLayer layer) const;

	private:
		std::vector<ColliderComponent*> m_Colliders;
		std::mutex m_CollidersMutex;

		using ColliderPair = std::pair<ColliderComponent*, ColliderComponent*>;
		struct PairHash
		{
			std::size_t operator()(const ColliderPair& p) const;
		};

		std::unordered_set<ColliderPair, PairHash> m_PreviousCollisions;
		std::unordered_map<CollisionLayer, unsigned int> m_LayerMasks;

		void SendCollisionEvents(ColliderComponent* a, ColliderComponent* b, bool wasColliding);
		void CleanupColliders();
		ColliderPair MakePair(ColliderComponent* a, ColliderComponent* b);

	};
}
