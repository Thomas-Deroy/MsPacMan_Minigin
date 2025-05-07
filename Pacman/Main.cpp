#include <SDL.h>

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"

#include "SDLSoundSystem.h"
#include "ServiceLocator.h"

#include "GameScene.h"
#include "MenuScene.h"

#include <iostream>

const int WIDTH = 448;
const int HEIGHT = 576;

void load()
{
    dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());

    auto& sceneManager = dae::SceneManager::GetInstance();
    auto& gameScene = sceneManager.CreateScene("Game");
    auto& menuScene = sceneManager.CreateScene("Menu");

    sceneManager.SetActiveScene("Game");

    LoadGameScene(gameScene, sceneManager, dae::GameMode::OnePlayer);
    LoadMenuScene(menuScene);
}

int main(int, char* [])
{
	dae::Minigin engine("../Data/", WIDTH, HEIGHT);
	engine.Run(load);
	return 0;
}
