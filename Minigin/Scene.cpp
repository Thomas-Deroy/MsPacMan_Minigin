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

void Scene::Update(float deltaTime)
{
    std::vector<GameObject*> toRemove;

    dae::CollisionSystem::GetInstance().Update();

    for (auto& object : m_objects)
    {
        if (object->IsMarkedForDestroy())
        {
            toRemove.push_back(object.get());
            continue;
        }
        if (object->IsActive())  
        {
            object->Update(deltaTime);
        }
    }

    for (auto* object : toRemove)
    {
        Remove(object);
    }

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


