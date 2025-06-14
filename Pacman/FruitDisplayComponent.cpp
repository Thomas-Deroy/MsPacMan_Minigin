#include "FruitDisplayComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "Scene.h"

namespace dae
{
    FruitDisplayComponent::FruitDisplayComponent(GameObject* owner, Scene* scene, LevelManager* levelManager)
		: Component(owner), m_Scene(scene), m_LevelManager(levelManager)
    {
    }

    void FruitDisplayComponent::AddFruitIcon()
    {
        auto icon = std::make_unique<GameObject>();

        switch (m_LevelManager->GetCurrentLevel())
        {
        case 1:
            icon->AddComponent<RenderComponent>("../Data/MSPC_Cherry.tga", 2.f);
            break;
        case 2:
            icon->AddComponent<RenderComponent>("../Data/MSPC_Strawberry.tga", 2.f);
            break;
        case 3:
            icon->AddComponent<RenderComponent>("../Data/MSPC_Orange.tga", 2.f);
			break;
        case 5:
            icon->AddComponent<RenderComponent>("../Data/MSPC_Pretzel.tga", 2.f);
			break;
        case 7:
			icon->AddComponent<RenderComponent>("../Data/MSPC_Pear.tga", 2.f);
            break;
        case 10:
            icon->AddComponent<RenderComponent>("../Data/MSPC_Banana.tga", 2.f);
			break;
        default:
            break;
        }

        icon->SetPosition(m_IconPositionX, 540.f);
        m_Scene->Add(icon);

        m_IconPositionX -= m_IconSpacing;
    }
}
