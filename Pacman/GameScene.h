#pragma once
#include <SDL.h>

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

// ENGINE

#include "Minigin.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "CollisionSystem.h"
#include "EventSystem.h"

#include "SDLSoundSystem.h"
#include "ServiceLocator.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "SpriteComponent.h"
#include "FPSComponent.h"
#include "RotateComponent.h"
#include "ColliderComponent.h"

// GAME

#include "LivesDisplayComponent.h"
#include "HealthComponent.h"
#include "PointsDisplayComponent.h"
#include "PointsComponent.h"
#include "AnimationRotationComponent.h"

#include "GameEvents.h"
#include "LevelBuilder.h"
#include "LevelManager.h"
#include "HighScoreManager.h"

#include "TeleportComponent.h"
#include "GhostAIComponent.h"
#include "PowerPelletComponent.h"
#include "GhostEatenState.h"
#include "GhostChaseState.h"
#include "NameInputComponent.h"

#include "MoveCommand.h"
#include "TakeDamageCommand.h"
#include "AddPointsCommand.h"
#include "LetterUpCommand.h"
#include "CheatCommand.h"

#include <iostream>

#include "MenuScene.h"

namespace dae {
    class Scene;
}

void LoadGameScene(dae::Scene& scene, dae::SceneManager& sceneManager, dae::LevelManager& levelManager, dae::GameMode gameMode);
