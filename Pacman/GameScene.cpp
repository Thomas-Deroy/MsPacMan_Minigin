#include "GameScene.h"

const int WIDTH = 448;
const int HEIGHT = 576;

void LoadGameScene(dae::Scene& scene, dae::SceneManager& sceneManager, dae::LevelManager& levelManager, dae::GameMode gameMode)
{
	if (sceneManager.GetActiveSceneName() == "Game")
	{
		// --- ENGINE --- //

		auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
		auto retroFont = dae::ResourceManager::GetInstance().LoadFont("RetroFont.ttf", 18);
		auto& input = dae::InputManager::GetInstance();
		input.RequestClearAllBinds();
		auto& event = EventSystem::GetInstance();
		auto& soundSystem = dae::ServiceLocator::GetSoundSystem();

		// --- GAME --- //

		auto& highScoreManager = HighScoreManager::GetInstance();
		highScoreManager.ReadHighScore();

		soundSystem.LoadSound(1, "../Data/MSPC_Siren0.wav");
		soundSystem.LoadSound(2, "../Data/MSPC_EatDot.wav");
		soundSystem.Play(1, 0.2f, true);

		// --- LEVEL --- //

		//auto go = std::make_unique<dae::GameObject>();
		//go->AddComponent<dae::RenderComponent>("MSPC_Level1.tga", 2.0f);
		//go->SetPosition(0.f, 40.f);
		//scene.Add(go);

		levelManager.LoadLevelManager(&scene, 1);
		dae::LevelBuilder* builder = levelManager.GetLevelBuilder();

		auto highScoreText = std::make_unique<dae::GameObject>();
		highScoreText->AddComponent<dae::TextComponent>("HIGH SCORE", retroFont);
		highScoreText->SetPosition((WIDTH / 2) - 75.f, 1.0f);
		scene.Add(highScoreText);

		auto highScore = std::make_unique<dae::GameObject>();
		highScore->AddComponent<dae::TextComponent>(highScoreManager.GetTopScoreDisplay(), retroFont);
		highScore->SetPosition((WIDTH / 2) - 50.f, 20.0f);
		scene.Add(highScore);

		auto blackScreen = std::make_unique<dae::GameObject>();
		blackScreen->AddComponent<dae::RenderComponent>("MSPC_BlackScreen.tga", 2.0f);
		blackScreen->SetPosition(0.f, 40.f);
		blackScreen->GetComponent<dae::RenderComponent>()->SetVisible(false);
		scene.Add(blackScreen);


		// --- DAE WATERMARK --- //

		auto go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::RenderComponent>("logo.tga", 0.4f);
		go->SetPosition((WIDTH / 2) - 40.f, (HEIGHT / 2) - 25.f);
		scene.Add(go);

		auto to = std::make_unique<dae::GameObject>();
		to->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
		to->SetPosition((WIDTH / 2) - 115.0f, 215.0f);
		scene.Add(to);

		auto fpsCounter = std::make_unique<dae::GameObject>();
		fpsCounter->SetPosition(350, 10);
		fpsCounter->AddComponent<dae::FPSComponent>(font);
		scene.Add(fpsCounter);

		// --- GHOSTS --- //

		auto blinky = std::make_unique<dae::GameObject>(); // Red Ghost
		auto blinkyPtr = blinky.get();
		blinky->SetPosition(150, 200);
		blinky->AddComponent<dae::SpriteComponent>("MSPC_Ghost_Spritesheet.tga", 6, 4, 0.1f, 2.0f);
		blinky->GetComponent<dae::SpriteComponent>()->SetLooping(false);
		blinky->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Enemy, false, true);
		blinky->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Ghost, builder);
		blinky->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(builder);

		auto pinky = std::make_unique<dae::GameObject>(); // Pink Ghost
		auto pinkyPtr = pinky.get();
		pinky->SetPosition(100, 200);
		pinky->AddComponent<dae::SpriteComponent>("MSPC_Ghost_Spritesheet.tga", 6, 4, 0.1f, 2.0f);
		pinky->GetComponent<dae::SpriteComponent>()->SetLooping(false);
		pinky->GetComponent<dae::SpriteComponent>()->SetAnimationRow(1);
		pinky->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Enemy, false, true);
		pinky->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Ghost, builder);
		pinky->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(builder);

		auto inky = std::make_unique<dae::GameObject>(); // Cyan Ghost
		auto inkyPtr = inky.get();
		inky->SetPosition(200, 200);
		inky->AddComponent<dae::SpriteComponent>("MSPC_Ghost_Spritesheet.tga", 6, 4, 0.1f, 2.0f);
		inky->GetComponent<dae::SpriteComponent>()->SetLooping(false);
		inky->GetComponent<dae::SpriteComponent>()->SetAnimationRow(2);
		inky->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Enemy, false, true);
		inky->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Ghost, builder);
		inky->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(builder);

		auto clyde = std::make_unique<dae::GameObject>(); // Orange Ghost
		auto clydePtr = clyde.get();
		clyde->SetPosition(150, 200);
		clyde->AddComponent<dae::SpriteComponent>("MSPC_Ghost_Spritesheet.tga", 6, 4, 0.1f, 2.0f);
		clyde->GetComponent<dae::SpriteComponent>()->SetLooping(false);
		clyde->GetComponent<dae::SpriteComponent>()->SetAnimationRow(3);
		clyde->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Enemy, false, true);
		clyde->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Ghost, builder);
		clyde->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(builder);

		// --- PACMAN & MsPACMAN --- //

		auto pacman = std::make_unique<dae::GameObject>();
		auto pacmanPtr = pacman.get();
		if (gameMode == dae::GameMode::TwoPlayer) {
			pacman->AddComponent<dae::SpriteComponent>("MSPC_Pacman_Spritesheet.tga", 4, 3, 0.1f, 2.0f);
			pacman->SetPosition(200, 304);
			pacman->AddComponent<dae::HealthComponent>(3, HealthChanged, PlayerDied);
			pacman->AddComponent<dae::PointsComponent>(PointsChanged);
			pacman->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Friendly, true, true);
			pacman->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Pacman, builder);
			pacman->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(builder);
			scene.Add(pacman);
		}

		auto mspacman = std::make_unique<dae::GameObject>();
		auto mspacmanPtr = mspacman.get();
		mspacman->AddComponent<dae::SpriteComponent>("MSPC_Mspacman_Spritesheet.tga", 4, 3, 0.1f, 2.0f);
		mspacman->SetPosition(208.f, 400.f);
		mspacman->AddComponent<dae::HealthComponent>(3, HealthChanged, PlayerDied);
		mspacman->AddComponent<dae::PointsComponent>(PointsChanged);
		mspacman->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Friendly, true, true);
		mspacman->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Pacman, builder);
		mspacman->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(builder);
		scene.Add(mspacman);

		// --- ADD GHOSTS --- //

		clyde->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Clyde);
		scene.Add(clyde);
		inky->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Inky);
		scene.Add(inky);
		pinky->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Pinky);
		scene.Add(pinky);
		if (gameMode == dae::GameMode::Versus) blinky->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Blinky, true);
		else blinky->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Blinky);
		scene.Add(blinky);

		// --- EVENTS --- //

		// Highscore
		event.Subscribe(PlayerDied, [mspacmanPtr](const Event&) {
			if (mspacmanPtr)
			{
				auto& highScoreManager = HighScoreManager::GetInstance();
				auto points = mspacmanPtr->GetComponent<dae::PointsComponent>();
				if (points->GetScore() >= highScoreManager.GetTopScore())
				{
					highScoreManager.SaveHighScore("XXX", points->GetScore());
				}
			}
			});

		// Pacman eating pellets
		event.Subscribe(OnTriggerEnter, [&soundSystem, mspacmanPtr, pacmanPtr, blinkyPtr, pinkyPtr, inkyPtr, clydePtr, gameMode, builderPtr = builder](const Event& e) {
			auto* collisionArgs = static_cast<dae::CollisionEventArg*>(e.args[0]);
			if (!collisionArgs || !collisionArgs->other || !collisionArgs->self) return;

			bool isPelletCollectedByMsPacman =
				(collisionArgs->self->GetLayer() == dae::CollisionLayer::Object &&
					collisionArgs->other == mspacmanPtr->GetComponent<dae::ColliderComponent>()) ||
				(collisionArgs->other->GetLayer() == dae::CollisionLayer::Object &&
					collisionArgs->self == mspacmanPtr->GetComponent<dae::ColliderComponent>());

			bool isPelletCollectedByPacman = false;
			if (gameMode == dae::GameMode::TwoPlayer) {
				isPelletCollectedByPacman =
					(collisionArgs->self->GetLayer() == dae::CollisionLayer::Object &&
						collisionArgs->other == pacmanPtr->GetComponent<dae::ColliderComponent>()) ||
					(collisionArgs->other->GetLayer() == dae::CollisionLayer::Object &&
						collisionArgs->self == pacmanPtr->GetComponent<dae::ColliderComponent>());
			}

			if (isPelletCollectedByMsPacman || isPelletCollectedByPacman)
			{
				dae::ColliderComponent* pelletCollider =
					(collisionArgs->self->GetLayer() == dae::CollisionLayer::Object)
					? collisionArgs->self : collisionArgs->other;

				dae::GameObject* pellet = pelletCollider->GetOwner();
				if (pellet->GetComponent<dae::PowerPelletComponent>())
				{
					blinkyPtr->GetComponent<dae::GhostAIComponent>()->SetFrightened(true);
					pinkyPtr->GetComponent<dae::GhostAIComponent>()->SetFrightened(true);
					inkyPtr->GetComponent<dae::GhostAIComponent>()->SetFrightened(true);
					clydePtr->GetComponent<dae::GhostAIComponent>()->SetFrightened(true);
				}
				soundSystem.Play(2, 1.0f, false);

				auto pointsComponent = mspacmanPtr->GetComponent<dae::PointsComponent>();
				if (pointsComponent) pointsComponent->AddPoints(10);

				builderPtr->RemovePellet(pellet);
			}
		});


		// Ghost attack / eaten event
		event.Subscribe(OnTriggerEnter, [&soundSystem, mspacmanPtr, pacmanPtr, gameMode, builderPtr = builder](const Event& e) {
			auto* collisionArgs = static_cast<dae::CollisionEventArg*>(e.args[0]);
			if (!collisionArgs || !collisionArgs->other || !collisionArgs->self) return;

			bool isGhostCollision =
				(collisionArgs->self == mspacmanPtr->GetComponent<dae::ColliderComponent>() && collisionArgs->other->GetLayer() == dae::CollisionLayer::Enemy) ||
				(collisionArgs->other == mspacmanPtr->GetComponent<dae::ColliderComponent>() && collisionArgs->self->GetLayer() == dae::CollisionLayer::Enemy);

			if (isGhostCollision)
			{
				auto ghostAI = collisionArgs->self->GetOwner()->GetComponent<dae::GhostAIComponent>();
				auto ghostMovement = collisionArgs->self->GetOwner()->GetComponent<dae::MovementComponent>();
				if (ghostAI->GetFrightened() && ghostMovement) // Eatable Ghost
				{
					ghostAI->QueueStateChange(std::make_unique<dae::GhostEatenState>());
					ghostMovement->SetTargetDestination(builderPtr->GetCenterNode());
				}
				else
				{
					if (ghostAI->GetEaten()) return;

					// Take damage
					auto health = mspacmanPtr->GetComponent<dae::HealthComponent>();
					if (health) health->TakeDamage(1);

					mspacmanPtr->SetPosition(208.f, 400.f); // TEMPORARY
					auto move = mspacmanPtr->GetComponent<dae::MovementComponent>();
					move->SetTargetNode(nullptr);
					move->FindAndSetStartNode(builderPtr);
				}
			}
		});

		// Pacman eating all pellets -> Next Level
		event.Subscribe(AllPelletsEaten, [&levelManager, &scene, mspacmanPtr, pacmanPtr, blinkyPtr, pinkyPtr, inkyPtr, clydePtr, gameMode, builderPtr = builder](const Event&) {
			scene.Pause();
			blinkyPtr->GetComponent<dae::MovementComponent>()->Reset();
			pinkyPtr->GetComponent<dae::MovementComponent>()->Reset();
			inkyPtr->GetComponent<dae::MovementComponent>()->Reset();
			clydePtr->GetComponent<dae::MovementComponent>()->Reset();
			mspacmanPtr->GetComponent<dae::MovementComponent>()->Reset();

			mspacmanPtr->SetPosition(208.f, 400.f);
			levelManager.NextLevel();

			mspacmanPtr->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(levelManager.GetLevelBuilder());
			if(gameMode == dae::GameMode::TwoPlayer) pacmanPtr->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(builderPtr);
			blinkyPtr->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(builderPtr);
			pinkyPtr->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(builderPtr);
			inkyPtr->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(builderPtr);
			clydePtr->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(builderPtr);
			scene.Resume();
			std::cout << "Next Level" << std::endl;
		});

		// --- UI --- //

		auto livesDisplayMsPacMan = std::make_unique<dae::GameObject>();
		livesDisplayMsPacMan->SetPosition(5.f, 538.f);
		livesDisplayMsPacMan->AddComponent<dae::RenderComponent>();
		livesDisplayMsPacMan->AddComponent<dae::LivesDisplayComponent>(mspacmanPtr, &scene);
		scene.Add(livesDisplayMsPacMan);

		auto scoreDisplayMsPacMan = std::make_unique<dae::GameObject>();
		scoreDisplayMsPacMan->SetPosition(20.f, 10.f);
		scoreDisplayMsPacMan->AddComponent<dae::TextComponent>("", retroFont);
		scoreDisplayMsPacMan->AddComponent<dae::PointsDisplayComponent>(mspacmanPtr);
		scene.Add(scoreDisplayMsPacMan);

		//auto saveHighScore = std::make_unique<dae::GameObject>();
		//auto saveHighScorePtr = saveHighScore.get();
		//saveHighScore->SetPosition(100.f, 520.f);
		//saveHighScore->AddComponent<dae::TextComponent>("", retroFont);
		//saveHighScore->AddComponent<dae::NameInputComponent>(highScoreManager.GetTopScore());
		//scene.Add(saveHighScore);

		//input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_UP, dae::KeyState::Pressed, std::make_unique<dae::LetterUpCommand>(saveHighScorePtr));
		//input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_DOWN, dae::KeyState::Pressed, std::make_unique<dae::LetterDownCommand>(saveHighScorePtr));
		//input.BindCommand(SDL_CONTROLLER_BUTTON_A, dae::KeyState::Pressed, std::make_unique<dae::ConfirmLetterCommand>(saveHighScorePtr));


		// --- INPUTS --- //

		if (gameMode == dae::GameMode::TwoPlayer) {
			input.BindCommand(SDLK_w, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(pacmanPtr, glm::vec2(0.f, -1.f)));
			input.BindCommand(SDLK_s, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(pacmanPtr, glm::vec2(0.f, 1.f)));
			input.BindCommand(SDLK_a, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(pacmanPtr, glm::vec2(-1.f, 0.f)));
			input.BindCommand(SDLK_d, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(pacmanPtr, glm::vec2(1.f, 0.f)));
		}
		else if (gameMode == dae::GameMode::Versus) {
			input.BindCommand(SDLK_w, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(blinkyPtr, glm::vec2(0.f, -1.f)));
			input.BindCommand(SDLK_s, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(blinkyPtr, glm::vec2(0.f, 1.f)));
			input.BindCommand(SDLK_a, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(blinkyPtr, glm::vec2(-1.f, 0.f)));
			input.BindCommand(SDLK_d, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(blinkyPtr, glm::vec2(1.f, 0.f)));
		}

		input.BindCommand(SDLK_UP, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(0.f, -1.f)));
		input.BindCommand(SDLK_DOWN, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(0.f, 1.f)));
		input.BindCommand(SDLK_LEFT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(-1.f, 0.f)));
		input.BindCommand(SDLK_RIGHT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(1.f, 0.f)));

		input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_UP, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(0.f, -1.f)));
		input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_DOWN, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(0.f, 1.f)));
		input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_LEFT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(-1.f, 0.f)));
		input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(1.f, 0.f)));
		std::cout << "GameScene loaded" << std::endl;
	}
}