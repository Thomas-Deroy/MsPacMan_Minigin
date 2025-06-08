#include "Scene.h"
#include "GameObject.h"
#include <algorithm>


using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name)
	: m_name(name)
{
}

Scene::~Scene() = default;

const std::string& dae::Scene::GetName() const
{
    return m_name;
}

void Scene::Add(std::unique_ptr<GameObject>& object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(GameObject* obj) {
    m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
        [obj](const std::unique_ptr<GameObject>& o) {
            return o.get() == obj;
        }),
        m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

bool Scene::Contains(GameObject* obj) const {
    for (const auto& gameObject : m_objects) {
        if (gameObject.get() == obj) return true;
    }
    return false;
}

void dae::Scene::Pause()
{
    for (auto& object : m_objects)
    {
        object->SetActive(false);
    }
}

void dae::Scene::Resume()
{
    for (auto& object : m_objects)
    {
        object->SetActive(true);
    }
}

void dae::Scene::ResumeObject(GameObject* object)
{
    if (object == nullptr) return;

    for (auto& obj : m_objects)
    {
        if (obj.get() == object)
        {
            obj->SetActive(true);
            break;
        }
    }
}

void Scene::Timer(float durationSeconds, EventId eventId)
{
    m_PendingTimers.push_back(TimerTask{ durationSeconds, eventId });
}

void Scene::Update(float deltaTime)
{
    for (auto it = m_TimerTasks.begin(); it != m_TimerTasks.end(); )
    {
        it->Remaining -= deltaTime;
        if (it->Remaining <= 0.f)
        {
            Event event(it->EventToFire);
            EventSystem::GetInstance().Notify(event);
            it = m_TimerTasks.erase(it);
        }
        else
        {
            ++it;
        }
    }

    if (!m_PendingTimers.empty())
    {
        m_TimerTasks.insert(m_TimerTasks.end(), m_PendingTimers.begin(), m_PendingTimers.end());
        m_PendingTimers.clear();
    }

    for (auto& object : m_objects)
    {
        if (object->IsActive())
        {
            object->Update(deltaTime);
        }
    }

    m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
        [](const std::unique_ptr<GameObject>& obj)
        {
            return obj->IsMarkedForDestroy();
        }),
        m_objects.end());

    dae::CollisionSystem::GetInstance().Update();
}

void Scene::Render() const
{
    std::vector<const GameObject*> sortedObjects;
    for (const auto& object : m_objects)
    {
        sortedObjects.push_back(object.get());
    }

    std::sort(sortedObjects.begin(), sortedObjects.end(),
        [](const GameObject* a, const GameObject* b)
        {
            return a->GetRenderLayer() < b->GetRenderLayer();
        });

    for (const auto* object : sortedObjects)
    {
        object->Render();
    }
}


