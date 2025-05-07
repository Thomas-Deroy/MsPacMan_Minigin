#pragma once
#include <SDL.h>

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "CollisionSystem.h"
#include "EngineEvents.h"
#include "EventSystem.h"

#include "MoveCommand.h"
#include "TakeDamageCommand.h"
#include "AddPointsCommand.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "SpriteComponent.h"
#include "FPSComponent.h"
#include "RotateComponent.h"
#include "ColliderComponent.h"

#include "LivesDisplayComponent.h"
#include "HealthComponent.h"
#include "PointsDisplayComponent.h"
#include "PointsComponent.h"

#include "GameEvents.h"
#include "LevelBuilder.h"
#include "HighScoreManager.h"

#include "TeleportComponent.h"
#include "GhostAIComponent.h"

#include <iostream>

#include "SDLSoundSystem.h"
#include "ServiceLocator.h"

#include "MenuScene.h"

namespace dae {
    class Scene;
}

void LoadGameScene(dae::Scene& scene, dae::SceneManager& sceneManager, dae::GameMode gameMode);
