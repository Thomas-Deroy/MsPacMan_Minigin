#include "MenuScene.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "LevelManager.h"

#include "TextComponent.h"
#include "RenderComponent.h"
#include "UIMovementComponent.h"

#include "UIMoveCommand.h"
#include "GameModeCommand.h"

void LoadMenuScene(dae::Scene& scene, dae::SceneManager& sceneManager, dae::LevelManager& levelManager)
{
	if (sceneManager.GetActiveSceneName() == "Menu")
	{
		auto retroFont = dae::ResourceManager::GetInstance().LoadFont("RetroFont.ttf", 18);
		auto& input = dae::InputManager::GetInstance();

		auto startScreenImage = std::make_unique<dae::GameObject>();
		startScreenImage->AddComponent<dae::RenderComponent>("MSPC_StartScreen.tga", 2.0f);
		startScreenImage->SetPosition(20.f, 100.f);
		scene.Add(startScreenImage);

		auto onePlayerText = std::make_unique<dae::GameObject>();
		onePlayerText->AddComponent<dae::TextComponent>("1 Player", retroFont);
		onePlayerText->SetPosition(180.f, 240.f);
		scene.Add(onePlayerText);

		auto twoPlayerText = std::make_unique<dae::GameObject>();
		twoPlayerText->AddComponent<dae::TextComponent>("2 Player", retroFont);
		twoPlayerText->SetPosition(180.f, 280.f);
		scene.Add(twoPlayerText);

		auto versusText = std::make_unique<dae::GameObject>();
		versusText->AddComponent<dae::TextComponent>("Versus", retroFont);
		versusText->SetPosition(180.f, 320.f);
		scene.Add(versusText);

		auto modeSelect = std::make_unique<dae::GameObject>();
		auto modeSelectPtr = modeSelect.get();
		modeSelect->SetPosition(150.f, 242.f);
		modeSelect->AddComponent<dae::RenderComponent>("MSPC_UISelector.tga", 2.0f);
		modeSelect->AddComponent<dae::UIMovementComponent>(3, 40.f, glm::vec2{ 0, 1 });
		auto* uiMovement = modeSelect->GetComponent<dae::UIMovementComponent>();
		scene.Add(modeSelect);

		// --- INPUTS --- //

		input.BindCommand(SDLK_w, dae::KeyState::Pressed, std::make_unique<dae::UIMoveCommand>(modeSelectPtr, -1));
		input.BindCommand(SDLK_s, dae::KeyState::Pressed, std::make_unique<dae::UIMoveCommand>(modeSelectPtr, +1));

		input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_UP, dae::KeyState::Pressed, std::make_unique<dae::UIMoveCommand>(modeSelectPtr, -1));
		input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_DOWN, dae::KeyState::Pressed, std::make_unique<dae::UIMoveCommand>(modeSelectPtr, +1));

		input.BindCommand(SDLK_SPACE, dae::KeyState::Pressed, std::make_unique<dae::GameModeCommand>(uiMovement, sceneManager, levelManager, input));
		input.BindCommand(SDL_CONTROLLER_BUTTON_A, dae::KeyState::Pressed, std::make_unique<dae::GameModeCommand>(uiMovement, sceneManager, levelManager, input));
	}
}