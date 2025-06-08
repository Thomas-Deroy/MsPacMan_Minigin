#pragma once
#include <string>
#include <vector>
#include <memory>
#include "CollisionSystem.h"

namespace dae
{
	class SceneManager; 
	class GameObject;

	struct TimerTask
	{
		float Remaining;
		EventId EventToFire;
	};


	class Scene final
	{
	public:

		void Add(std::unique_ptr<GameObject>& object);
		void Remove(GameObject* object);
		void RemoveAll();
		bool Contains(GameObject* object) const; 

		void Update(float);
		void Render() const;

		void Pause(); 
		void Resume();
		void ResumeObject(GameObject* object);

		void Timer(float durationSeconds, EventId eventId);

		const std::string& GetName() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		explicit Scene(const std::string& name);
	private:
		std::string m_name;
		std::vector<std::unique_ptr<GameObject>> m_objects;
		std::vector<TimerTask> m_PendingTimers;
		std::vector<TimerTask> m_TimerTasks;

		static unsigned int m_idCounter;
	};
}
