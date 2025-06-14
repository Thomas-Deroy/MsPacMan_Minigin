#pragma once
#include "Component.h"
#include "LevelManager.h"
#include <vector>
#include <string>

namespace dae
{
    class RenderComponent;
    class GameObject;
    class Scene;

    class FruitDisplayComponent : public Component
    {
    public:
        FruitDisplayComponent(GameObject* owner, Scene* scene, LevelManager* levelManager);

        void AddFruitIcon();
    private:
        Scene* m_Scene;
		LevelManager* m_LevelManager;

        float m_IconSpacing = 18.f;
		float m_IconPositionX = 410.f;
    };
}
