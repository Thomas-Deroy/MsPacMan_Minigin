#pragma once
#include <SDL.h>

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

// --- ENGINE --- //

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
#include "ColliderComponent.h"

// --- GAME --- //

#include "HighScoreManager.h"
#include "GameEvents.h"
#include "LevelBuilder.h"
#include "LevelManager.h"

// Components
#include "GhostAIComponent.h"
#include "PowerPelletComponent.h"
#include "FruitDisplayComponent.h"
#include "NameInputComponent.h"
#include "FruitComponent.h"
#include "LivesDisplayComponent.h"
#include "HealthComponent.h"
#include "PointsDisplayComponent.h"
#include "PointsComponent.h"
#include "AnimationRotationComponent.h"

// States
#include "GhostEatenState.h"
#include "GhostChaseState.h"
#include "FruitWanderState.h"
#include "FruitEnterState.h"
#include "FruitHiddenState.h"

// Commands
#include "MoveCommand.h"
#include "TakeDamageCommand.h"
#include "AddPointsCommand.h"
#include "NameInputCommand.h"
#include "CheatCommand.h"

#include <iostream>

#include "MenuScene.h"

namespace dae {
    class Scene;
}

void LoadGameScene(dae::Scene& scene, dae::SceneManager& sceneManager, dae::LevelManager& levelManager, dae::GameMode gameMode);
