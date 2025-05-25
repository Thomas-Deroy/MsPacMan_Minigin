#include "GameObject.h"
#include "Component.h"
#include "RenderComponent.h"
#include <stdexcept>
#include <iostream>

namespace dae
{
    GameObject::GameObject()
    {
        m_transform.m_Owner = this;
    }

    GameObject::~GameObject()
    {
        SetParent(nullptr, false);

        for (auto* child : m_Children)
        {
            child->m_Parent = nullptr;
        }
        m_Children.clear();

        m_transform.m_Owner = nullptr;
    }

    void GameObject::Update(float deltaTime)
    {
        if (!m_IsActive) return;

        for (const auto& component : m_Components)
        {
            component->Update(deltaTime);
        }

        m_PreviousPosition = GetPosition();

        m_transform.SetLocalPosition({
            m_transform.GetLocalPosition().x + m_VelocityX * deltaTime,
            m_transform.GetLocalPosition().y + m_VelocityY * deltaTime,
            m_transform.GetLocalPosition().z
            });

        m_VelocityX = 0;
        m_VelocityY = 0;
    }

    void GameObject::FixedUpdate()
    {
        for (const auto& component : m_Components)
        {
            component->FixedUpdate();
        }
    }

    void GameObject::Render() const
    {
        for (const auto& component : m_Components)
        {
            component->Render();
        }
    }

    void GameObject::SetPosition(float x, float y, float z)
    {
        m_transform.SetLocalPosition({ x, y, z });
        m_transform.SetPositionDirty();
    }

    glm::vec3 GameObject::GetPosition() const
    {
        return m_transform.GetWorldPosition();
    }

    Transform& GameObject::GetTransform()
    {
        return m_transform;
    }

    const Transform& GameObject::GetTransform() const
    {
        return m_transform;
    }

    void GameObject::SetRenderLayer(int layer) 
    { 
        m_RenderLayer = layer; 
    }

    int GameObject::GetRenderLayer() const 
    { 
        return m_RenderLayer; 
    }


    void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
    {
        if (parent == this || m_Parent == parent || IsChildOf(parent))
            return;

        glm::vec3 worldPos = m_transform.GetWorldPosition();

        if (m_Parent)
        {
            auto& siblings = m_Parent->m_Children;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
        }

        m_Parent = parent;

        if (m_Parent)
        {
            m_Parent->m_Children.push_back(this);

            if (keepWorldPosition)
                m_transform.SetLocalPosition(worldPos - m_Parent->GetTransform().GetWorldPosition());
        }
        else
        {
            m_transform.SetLocalPosition(worldPos);
        }

        m_transform.SetPositionDirty();
    }

    bool GameObject::IsChildOf(const GameObject* potentialParent) const
    {
        if (!potentialParent) return false;
        for (auto* p = m_Parent; p != nullptr; p = p->m_Parent)
        {
            if (p == potentialParent)
                return true;
        }
        return false;
    }

    GameObject* GameObject::GetParent() const
    {
        return m_Parent;
    }

    const std::vector<GameObject*>& GameObject::GetChildren() const
    {
        return m_Children;
    }

    std::vector<GameObject*>& GameObject::GetChildren()
    {
        return m_Children;
    }

    GameObject* GameObject::GetChildAt(size_t index) const
    {
        if (index >= m_Children.size()) throw std::out_of_range("Child index out of bounds");
        return m_Children[index];
    }

    size_t GameObject::GetChildCount() const
    {
        return m_Children.size();
    }
}
