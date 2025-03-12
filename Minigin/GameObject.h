#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <algorithm>
#include "Transform.h"
#include "Component.h"

namespace dae
{
    class GameObject final
    {
    public:
        GameObject();
        ~GameObject();

        void Update(float deltaTime);
        void Render() const;
        void FixedUpdate();

        // POSITION
        void SetPosition(float x, float y, float z = 0.0f);
        glm::vec3 GetPosition() const;

        // TRANSFORM
        Transform& GetTransform();
        const Transform& GetTransform() const;

        // VELOCITY
        void AddVelocity(float x, float y) { m_VelocityX += x; m_VelocityY += y; }
        void SetVelocity(float x, float y) { m_VelocityX = x; m_VelocityY = y; }
        void GetVelocity(float& x, float& y) const { x = m_VelocityX; y = m_VelocityY; }

        // COMPONENTS

        template <typename T, typename... Args>
        T* AddComponent(Args&&... args)
        {
            static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

            auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
            T* ptr = component.get();

            m_ComponentMap[std::type_index(typeid(T))] = ptr;
            m_Components.push_back(std::move(component));

            return ptr;
        }

        template <typename T>
        T* GetComponent() const
        {
            auto it = m_ComponentMap.find(std::type_index(typeid(T)));
            return (it != m_ComponentMap.end()) ? static_cast<T*>(it->second) : nullptr;
        }

        template <typename T>
        void RemoveComponent()
        {
            auto typeIndex = std::type_index(typeid(T));
            auto it = m_ComponentMap.find(typeIndex);
            if (it != m_ComponentMap.end())
            {
                m_ComponentMap.erase(it);
                m_Components.erase(std::remove_if(m_Components.begin(), m_Components.end(),
                    [](const std::unique_ptr<Component>& component) {
                        return dynamic_cast<T*>(component.get()) != nullptr;
                    }), m_Components.end());
            }
        }

        // PARENT-CHILD RELATIONSHIP

        void SetParent(std::shared_ptr<GameObject> parent, bool keepWorldPosition);
        bool IsChildOf(const GameObject* potentialChild) const;

        GameObject* GetParent() const;
        const std::vector<GameObject*>& GetChildren() const;
        GameObject* GetChildAt(size_t index) const;
        size_t GetChildCount() const;

    private:
        Transform m_transform{};
        std::vector<std::unique_ptr<Component>> m_Components{};
        std::unordered_map<std::type_index, Component*> m_ComponentMap{};

        GameObject* m_Parent{ nullptr };
        std::vector<GameObject*> m_Children{};

        float m_VelocityX = 0.0f;
        float m_VelocityY = 0.0f;
    };
}
