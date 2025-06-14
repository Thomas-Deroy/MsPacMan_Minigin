#include "GameScene.h"

const int WIDTH = 448;
const int HEIGHT = 576;

const glm::vec2 POSITION_MSPACMAN(194.f, 395.f);
const glm::vec2 POSITION_PACMAN(200.f, 304.f);

const glm::vec2 POSITION_BLINKY(190.f, 200.f);
const glm::vec2 POSITION_PINKY(190.f, 240.f);
const glm::vec2 POSITION_INKY(150.f, 240.f);
const glm::vec2 POSITION_Sue(240.f, 240.f);



void LoadGameScene(dae::Scene& scene, dae::SceneManager& sceneManager, dae::LevelManager& levelManager, dae::GameMode gameMode)
{
	if (sceneManager.GetActiveSceneName() == "Game")
	{
		// --- ENGINE --- //

		auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
		auto retroFont = dae::ResourceManager::GetInstance().LoadFont("RetroFont.ttf", 18);
		auto bigRetroFont = dae::ResourceManager::GetInstance().LoadFont("RetroFont.ttf", 40);
		auto& input = dae::InputManager::GetInstance();
		input.RequestClearAllBinds();
		auto& event = EventSystem::GetInstance();
		auto& soundSystem = dae::ServiceLocator::GetSoundSystem();

		// --- GAME --- //

		static auto& highScoreManager = dae::HighScoreManager::GetInstance();
		highScoreManager.ReadHighScore();

		//Loading sounds
		soundSystem.LoadSound(1, "MSPC_Siren0.wav");
		soundSystem.LoadSound(2, "MSPC_EatDot.wav");
		soundSystem.LoadSound(3, "MSPC_Start.wav");
		soundSystem.LoadSound(4, "MSPC_EatGhost.wav");
		soundSystem.LoadSound(5, "MSPC_Death.wav");
		soundSystem.LoadSound(6, "MSPC_Fright.wav");
		soundSystem.LoadSound(7, "MSPC_EatFruit.wav");
		soundSystem.LoadSound(8, "MSPC_FruitBounce.wav");
		soundSystem.LoadSound(9, "MSPC_Extend.wav");

		// Fruit Bounce sound
		soundSystem.Play(8, 0.2f, true);
		soundSystem.Mute(8);

		// Intro Animation
		scene.Timer(4.f, StartTimer);
		soundSystem.Play(3, 0.2f, false);

		// --- LEVEL --- //

		levelManager.LoadLevelManager(&scene, &event, &soundSystem, 1);
		dae::LevelBuilder* builder = levelManager.GetLevelBuilder();

		auto highScoreText = std::make_unique<dae::GameObject>();
		auto highScoreTextPtr = highScoreText.get();
		highScoreText->AddComponent<dae::TextComponent>("HIGH SCORE", retroFont);
		highScoreText->SetPosition(160.f, 1.0f);
		scene.Add(highScoreText);

		auto highScore = std::make_unique<dae::GameObject>();
		auto highScorePtr = highScore.get();
		highScore->AddComponent<dae::TextComponent>(highScoreManager.GetTopScoreDisplay(), retroFont);
		highScore->SetPosition(165.f, 20.0f);
		scene.Add(highScore);

		auto readyText = std::make_unique<dae::GameObject>();
		auto readyTextPtr = readyText.get();
		readyText->AddComponent<dae::TextComponent>("READY!", retroFont);
		readyText->GetComponent<dae::TextComponent>()->SetColor(1.f, 1.f, 0.f);
		readyText->SetPosition(185.f,310.f);
		readyText->SetRenderLayer(3);
		scene.Add(readyText);

		auto ghostScore = std::make_unique<dae::GameObject>();
		auto ghostScorePtr = ghostScore.get();
		ghostScore->AddComponent<dae::SpriteComponent>("testScore.tga", 1, 4, 0.1f, 2.0f);
		ghostScore->GetComponent<dae::SpriteComponent>()->SetLooping(false);
		ghostScore->GetComponent<dae::SpriteComponent>()->SetAnimationColumn(1);
		ghostScore->GetComponent<dae::SpriteComponent>()->SetVisible(false);
		ghostScore->SetRenderLayer(2);
		ghostScore->SetPosition(0.f, 0.f);
		scene.Add(ghostScore);

		// --- FPS --- //

		auto fpsCounter = std::make_unique<dae::GameObject>();
		fpsCounter->SetPosition(350, 10);
		fpsCounter->AddComponent<dae::FPSComponent>(font);
		scene.Add(fpsCounter);

		// --- GHOSTS --- //

		auto blinky = std::make_unique<dae::GameObject>(); // Red Ghost
		auto blinkyPtr = blinky.get();
		blinky->SetPosition(POSITION_BLINKY);
		blinky->AddComponent<dae::SpriteComponent>("MSPC_Ghost_Spritesheet.tga", 6, 4, 0.1f, 2.0f);
		blinky->GetComponent<dae::SpriteComponent>()->SetLooping(false);
		blinky->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Enemy, false);
		blinky->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Ghost, builder);
		blinky->GetComponent<dae::MovementComponent>()->FindAndSetNode(builder);

		auto pinky = std::make_unique<dae::GameObject>(); // Pink Ghost
		auto pinkyPtr = pinky.get();
		pinky->SetPosition(POSITION_PINKY);
		pinky->AddComponent<dae::SpriteComponent>("MSPC_Ghost_Spritesheet.tga", 6, 4, 0.1f, 2.0f);
		pinky->GetComponent<dae::SpriteComponent>()->SetLooping(false);
		pinky->GetComponent<dae::SpriteComponent>()->SetAnimationRow(1);
		pinky->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Enemy, false);
		pinky->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Ghost, builder);
		pinky->GetComponent<dae::MovementComponent>()->FindAndSetNode(builder);

		auto inky = std::make_unique<dae::GameObject>(); // Cyan Ghost
		auto inkyPtr = inky.get();
		inky->SetPosition(POSITION_INKY);
		inky->AddComponent<dae::SpriteComponent>("MSPC_Ghost_Spritesheet.tga", 6, 4, 0.1f, 2.0f);
		inky->GetComponent<dae::SpriteComponent>()->SetLooping(false);
		inky->GetComponent<dae::SpriteComponent>()->SetAnimationRow(2);
		inky->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Enemy, false);
		inky->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Ghost, builder);
		inky->GetComponent<dae::MovementComponent>()->FindAndSetNode(builder);

		auto Sue = std::make_unique<dae::GameObject>(); // Orange Ghost
		auto SuePtr = Sue.get();
		Sue->SetPosition(POSITION_Sue);
		Sue->AddComponent<dae::SpriteComponent>("MSPC_Ghost_Spritesheet.tga", 6, 4, 0.1f, 2.0f);
		Sue->GetComponent<dae::SpriteComponent>()->SetLooping(false);
		Sue->GetComponent<dae::SpriteComponent>()->SetAnimationRow(3);
		Sue->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Enemy, false);
		Sue->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Ghost, builder);
		Sue->GetComponent<dae::MovementComponent>()->FindAndSetNode(builder);

		// --- PACMAN & MsPACMAN --- //

		auto pacman = std::make_unique<dae::GameObject>();
		auto pacmanPtr = pacman.get();
		if (gameMode == dae::GameMode::TwoPlayer) {
			pacman->AddComponent<dae::SpriteComponent>("MSPC_Pacman_Spritesheet.tga", 4, 3, 0.1f, 2.0f);
			pacman->SetPosition(POSITION_PACMAN);
			pacman->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Friendly, false);
			pacman->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Pacman, builder);
			pacman->GetComponent<dae::MovementComponent>()->FindAndSetNode(builder);
			pacman->AddComponent<dae::AnimationRotationComponent>();
			scene.Add(pacman);
			levelManager.AddLevelObjects(pacmanPtr, dae::CharacterType::Pacman);
		}

		auto mspacman = std::make_unique<dae::GameObject>();
		auto mspacmanPtr = mspacman.get();
		mspacman->AddComponent<dae::SpriteComponent>("MSPC_Mspacman_Spritesheet.tga", 4, 3, 0.1f, 2.0f);
		mspacman->SetPosition(POSITION_MSPACMAN);
		mspacman->AddComponent<dae::AnimationRotationComponent>();
		mspacman->AddComponent<dae::HealthComponent>(3);
		mspacman->AddComponent<dae::PointsComponent>();
		mspacman->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Friendly, false);
		mspacman->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Pacman, builder);
		mspacman->GetComponent<dae::MovementComponent>()->FindAndSetNode(builder);
		scene.Add(mspacman);
		levelManager.AddLevelObjects(mspacmanPtr, dae::CharacterType::MsPacman);

		// --- ADD GHOSTS --- //

		if(gameMode == dae::GameMode::TwoPlayer) Sue->AddComponent<dae::GhostAIComponent>(pacmanPtr, dae::GhostType::Sue);
		else Sue->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Sue);
		Sue->GetComponent<dae::GhostAIComponent>()->SetStartDelay(12.f);
		scene.Add(Sue);

		inky->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Inky);
		inky->GetComponent<dae::GhostAIComponent>()->SetStartDelay(8.f);
		scene.Add(inky);

		if (gameMode == dae::GameMode::TwoPlayer) pinky->AddComponent<dae::GhostAIComponent>(pacmanPtr, dae::GhostType::Pinky);
		else pinky->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Pinky);
		pinky->GetComponent<dae::GhostAIComponent>()->SetStartDelay(4.f);
		scene.Add(pinky);

		if (gameMode == dae::GameMode::Versus) blinky->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Blinky, true);
		else blinky->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Blinky);
		scene.Add(blinky);

		levelManager.AddLevelObjects(SuePtr, dae::CharacterType::GhostSue);
		levelManager.AddLevelObjects(inkyPtr, dae::CharacterType::GhostInky);
		levelManager.AddLevelObjects(pinkyPtr, dae::CharacterType::GhostPinky);
		levelManager.AddLevelObjects(blinkyPtr, dae::CharacterType::GhostBlinky);

		// --- FRUIT --- //

		auto fruit = std::make_unique<dae::GameObject>();
		auto fruitPtr = fruit.get();
		fruit->AddComponent<dae::SpriteComponent>("MSPC_Fruit_Spritesheet.tga", 6, 3, 0.1f, 2.0f);
		fruit->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Object, false);
		fruit->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::None, builder);
		fruit->GetComponent<dae::MovementComponent>()->SetIgnoreTunnels(true);
		fruit->GetComponent<dae::MovementComponent>()->FindAndSetNode(builder);
		fruit->AddComponent<dae::FruitComponent>(builder);
		fruit->GetComponent<dae::FruitComponent>()->ChangeState(std::make_unique<dae::FruitHiddenState>());
		fruit->GetComponent<dae::SpriteComponent>()->SetAnimationRow(0);
		fruit->SetActive(true);
		scene.Add(fruit);
		levelManager.AddLevelObjects(fruitPtr,  dae::CharacterType::Fruit);
		
		// --- UI --- //

		auto fruitDisplay = std::make_unique<dae::GameObject>();
		auto fruitDisplayPtr = fruitDisplay.get();
		fruitDisplay->SetPosition(5.f, 10.f);
		fruitDisplay->AddComponent<dae::RenderComponent>();
		fruitDisplay->AddComponent<dae::FruitDisplayComponent>(&scene, &levelManager);
		scene.Add(fruitDisplay);


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

		auto saveHighScore = std::make_unique<dae::GameObject>();
		auto saveHighScorePtr = saveHighScore.get();
		saveHighScore->SetPosition(120.f, 250.f);
		saveHighScore->AddComponent<dae::TextComponent>("", bigRetroFont);
		saveHighScore->AddComponent<dae::NameInputComponent>(highScoreManager.GetTopScore());
		saveHighScore->GetComponent<dae::TextComponent>()->SetVisible(false);
		scene.Add(saveHighScore);

		// --- INPUTS --- //

		if (gameMode == dae::GameMode::TwoPlayer) {
			if (input.IsControllerConnected(1)) 
			{
				input.BindCommand(1, SDL_CONTROLLER_BUTTON_DPAD_UP, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(pacmanPtr, glm::vec2(0.f, -1.f)));
				input.BindCommand(1, SDL_CONTROLLER_BUTTON_DPAD_DOWN, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(pacmanPtr, glm::vec2(0.f, 1.f)));
				input.BindCommand(1, SDL_CONTROLLER_BUTTON_DPAD_LEFT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(pacmanPtr, glm::vec2(-1.f, 0.f)));
				input.BindCommand(1, SDL_CONTROLLER_BUTTON_DPAD_RIGHT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(pacmanPtr, glm::vec2(1.f, 0.f)));
			}
			else
			{
				input.BindCommand(SDLK_UP, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(pacmanPtr, glm::vec2(0.f, -1.f)));
				input.BindCommand(SDLK_DOWN, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(pacmanPtr, glm::vec2(0.f, 1.f)));
				input.BindCommand(SDLK_LEFT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(pacmanPtr, glm::vec2(-1.f, 0.f)));
				input.BindCommand(SDLK_RIGHT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(pacmanPtr, glm::vec2(1.f, 0.f)));
			}
		}
		else if (gameMode == dae::GameMode::Versus) {
			if (input.IsControllerConnected(1)) 
			{
				input.BindCommand(1, SDL_CONTROLLER_BUTTON_DPAD_UP, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(blinkyPtr, glm::vec2(0.f, -1.f)));
				input.BindCommand(1, SDL_CONTROLLER_BUTTON_DPAD_DOWN, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(blinkyPtr, glm::vec2(0.f, 1.f)));
				input.BindCommand(1, SDL_CONTROLLER_BUTTON_DPAD_LEFT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(blinkyPtr, glm::vec2(-1.f, 0.f)));
				input.BindCommand(1, SDL_CONTROLLER_BUTTON_DPAD_RIGHT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(blinkyPtr, glm::vec2(1.f, 0.f)));
			}
			else
			{
				input.BindCommand(SDLK_UP, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(blinkyPtr, glm::vec2(0.f, -1.f)));
				input.BindCommand(SDLK_DOWN, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(blinkyPtr, glm::vec2(0.f, 1.f)));
				input.BindCommand(SDLK_LEFT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(blinkyPtr, glm::vec2(-1.f, 0.f)));
				input.BindCommand(SDLK_RIGHT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(blinkyPtr, glm::vec2(1.f, 0.f)));
			}
		}

		if (input.IsControllerConnected(0))
		{
			input.BindCommand(0, SDL_CONTROLLER_BUTTON_DPAD_UP, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(0.f, -1.f)));
			input.BindCommand(0, SDL_CONTROLLER_BUTTON_DPAD_DOWN, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(0.f, 1.f)));
			input.BindCommand(0, SDL_CONTROLLER_BUTTON_DPAD_LEFT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(-1.f, 0.f)));
			input.BindCommand(0, SDL_CONTROLLER_BUTTON_DPAD_RIGHT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(1.f, 0.f)));
		}
		else
		{
			input.BindCommand(SDLK_w, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(0.f, -1.f)));
			input.BindCommand(SDLK_s, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(0.f, 1.f)));
			input.BindCommand(SDLK_a, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(-1.f, 0.f)));
			input.BindCommand(SDLK_d, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(1.f, 0.f)));
		}

		// Cheat Inputs
		input.BindCommand(SDLK_F1, dae::KeyState::Pressed, std::make_unique<dae::CheatCommand>(&scene, &levelManager, &soundSystem, 1));
		input.BindCommand(SDLK_F2, dae::KeyState::Pressed, std::make_unique<dae::CheatCommand>(&scene, &levelManager, &soundSystem, 2));
		input.BindCommand(SDLK_F3, dae::KeyState::Pressed, std::make_unique<dae::CheatCommand>(&scene, &levelManager, &soundSystem, 3));

		scene.Pause();
		scene.ResumeObject(readyTextPtr);
		scene.ResumeObject(highScoreTextPtr);
		scene.ResumeObject(highScorePtr);
		

		// === EVENTS === //

		// --- Highscore --- //
		event.Subscribe(PlayerDied, [&levelManager, &scene, &input, mspacmanPtr, saveHighScorePtr](const Event&) {
			scene.Pause();
			scene.ResumeObject(saveHighScorePtr);

			saveHighScorePtr->GetComponent<dae::TextComponent>()->SetVisible(true);

			if (input.IsControllerConnected(0))
			{
				input.BindCommand(0, SDL_CONTROLLER_BUTTON_DPAD_UP, dae::KeyState::Pressed, std::make_unique<dae::LetterUpCommand>(saveHighScorePtr));
				input.BindCommand(0, SDL_CONTROLLER_BUTTON_DPAD_DOWN, dae::KeyState::Pressed, std::make_unique<dae::LetterDownCommand>(saveHighScorePtr));
				input.BindCommand(0, SDL_CONTROLLER_BUTTON_A, dae::KeyState::Pressed, std::make_unique<dae::ConfirmLetterCommand>(saveHighScorePtr));
			}
			else 
			{
				input.BindCommand(SDLK_UP, dae::KeyState::Pressed, std::make_unique<dae::LetterUpCommand>(saveHighScorePtr));
				input.BindCommand(SDLK_DOWN, dae::KeyState::Pressed, std::make_unique<dae::LetterDownCommand>(saveHighScorePtr));
				input.BindCommand(SDLK_LEFT, dae::KeyState::Pressed, std::make_unique<dae::ConfirmLetterCommand>(saveHighScorePtr));
			}
		});

		event.Subscribe(NameInputCompleted, [mspacmanPtr, saveHighScorePtr](const Event&) {
			auto points = mspacmanPtr->GetComponent<dae::PointsComponent>();
			auto nameInput = saveHighScorePtr->GetComponent<dae::NameInputComponent>();

			if (points && nameInput && points->GetScore() >= highScoreManager.GetTopScore()) {
				highScoreManager.SaveHighScore(nameInput->GetName(), points->GetScore());
			}
		});

		// --- Ghost Event --- //

		// Ending ghost frightened
		static int ghostsEatenInFrightened = 0;

		event.Subscribe(GhostFrightenedOver, [&soundSystem](const Event&) {
			soundSystem.Unmute(1);
			ghostsEatenInFrightened = 0;
		});

		// Ghost Attack / Eaten 
		event.Subscribe(OnTriggerEnter, [&soundSystem, &scene, ghostScorePtr, mspacmanPtr, pacmanPtr, gameMode, builderPtr = builder](const Event& e) {
			auto* collisionArgs = static_cast<dae::CollisionEventArg*>(e.args[0]);
			if (!collisionArgs || !collisionArgs->other || !collisionArgs->self) return;

			bool isGhostCollisionMsPacman = (collisionArgs->other == mspacmanPtr->GetComponent<dae::ColliderComponent>()
				&& collisionArgs->self->GetLayer() == dae::CollisionLayer::Enemy);

			bool isGhostCollisionPacman = false;
			if (gameMode == dae::GameMode::TwoPlayer) {
				isGhostCollisionPacman = (collisionArgs->other == pacmanPtr->GetComponent<dae::ColliderComponent>()
					&& collisionArgs->self->GetLayer() == dae::CollisionLayer::Enemy);
			}

			if (isGhostCollisionMsPacman || isGhostCollisionPacman)
			{
				auto ghost = collisionArgs->self->GetOwner();
				auto ghostAI = collisionArgs->self->GetOwner()->GetComponent<dae::GhostAIComponent>();
				auto ghostMovement = collisionArgs->self->GetOwner()->GetComponent<dae::MovementComponent>();

				if (ghostAI->GetEaten()) return;

				if (ghostAI->GetFrightened() && ghostMovement) // Eatable Ghost
				{
					soundSystem.Play(4, 0.5f, false);

					ghostScorePtr->GetComponent<dae::SpriteComponent>()->SetAnimationColumn(ghostsEatenInFrightened);
					++ghostsEatenInFrightened;
					int pointsAwarded = 200 * (1 << (ghostsEatenInFrightened - 1));
					mspacmanPtr->GetComponent<dae::PointsComponent>()->AddPoints(pointsAwarded);

					scene.Pause();
					scene.Timer(1.f, EatenAnimationTimer);

					ghost->GetComponent<dae::SpriteComponent>()->SetVisible(false);
					ghostScorePtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);

					ghostScorePtr->SetPosition(ghost->GetPosition().x - 4.f, ghost->GetPosition().y - 4.f, 0.0f);
					ghostAI->ChangeState(std::make_unique<dae::GhostEatenState>());
					ghostMovement->SetTargetDestination(builderPtr->GetCenterNode());
				}
				else // Attack on player
				{
					scene.Timer(0.5f, AnimationTimer);
					soundSystem.Mute(1);
					scene.Pause();
				}
			}
		});
		
		// -- MsPacman & Pacman Events -- //
		
		// Eating pellets
		event.Subscribe(OnTriggerEnter, [&soundSystem, mspacmanPtr, pacmanPtr, blinkyPtr, pinkyPtr, inkyPtr, SuePtr, fruitDisplayPtr, fruitPtr, gameMode, builderPtr = builder](const Event& e) {
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

				if (pellet == builderPtr->GetLeftFruitTriggerArea() || pellet == builderPtr->GetRightFruitTriggerArea() || pellet == builderPtr->GetGhostHouse())
					return;

				if (pellet->GetComponent<dae::PowerPelletComponent>())
				{
					blinkyPtr->GetComponent<dae::GhostAIComponent>()->SetFrightened(true);
					pinkyPtr->GetComponent<dae::GhostAIComponent>()->SetFrightened(true);
					inkyPtr->GetComponent<dae::GhostAIComponent>()->SetFrightened(true);
					SuePtr->GetComponent<dae::GhostAIComponent>()->SetFrightened(true);
					ghostsEatenInFrightened = 0;
					soundSystem.Play(6, 0.5f, false);
					soundSystem.Mute(1);
				}
				else if (pellet->GetComponent<dae::FruitComponent>())
				{
					mspacmanPtr->GetComponent<dae::PointsComponent>()->AddPoints(300);

					soundSystem.Play(7, 0.5f, false);
					soundSystem.Mute(8);

					fruitPtr->GetComponent<dae::FruitComponent>()->ChangeState(std::make_unique<dae::FruitHiddenState>());
					fruitDisplayPtr->GetComponent<dae::FruitDisplayComponent>()->AddFruitIcon();
					return;
				}
				else
				{
					soundSystem.Play(2, 1.0f, false);
					mspacmanPtr->GetComponent<dae::PointsComponent>()->AddPoints(10);
				}

				builderPtr->RemovePellet(pellet);
			}
		});

		// -- Timers -- //

		event.Subscribe(AnimationTimer, [&scene, &soundSystem, pacmanPtr, mspacmanPtr, blinkyPtr, pinkyPtr, inkyPtr, SuePtr, fruitPtr, gameMode](const Event&) {
			scene.Resume();
			blinkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(false);
			pinkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(false);
			inkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(false);
			SuePtr->GetComponent<dae::SpriteComponent>()->SetVisible(false);
			fruitPtr->GetComponent<dae::FruitComponent>()->ChangeState(std::make_unique<dae::FruitHiddenState>());

			soundSystem.Play(5, 0.5f, false);
			scene.Pause();

			// Rotate animation
			scene.ResumeObject(mspacmanPtr);
			mspacmanPtr->GetComponent<dae::AnimationRotationComponent>()->Rotate(2, 45.f);
			mspacmanPtr->GetComponent<dae::MovementComponent>()->Reset();

			if (gameMode == dae::GameMode::TwoPlayer) 
			{
				scene.ResumeObject(pacmanPtr);
				pacmanPtr->GetComponent<dae::AnimationRotationComponent>()->Rotate(2, 45.f);
				pacmanPtr->GetComponent<dae::MovementComponent>()->Reset();
			}

			scene.Timer(3.f, DeadAnimationTimer);
		});

		event.Subscribe(DeadAnimationTimer, [&scene, &soundSystem, pacmanPtr, mspacmanPtr, blinkyPtr, pinkyPtr, inkyPtr, SuePtr, gameMode, builderPtr = builder](const Event&) {
			mspacmanPtr->GetComponent<dae::HealthComponent>()->TakeDamage(1);
			auto move = mspacmanPtr->GetComponent<dae::MovementComponent>();
			mspacmanPtr->SetPosition(POSITION_MSPACMAN);
			move->SetTargetNode(nullptr);
			move->FindAndSetNode(builderPtr);

			if(gameMode == dae::GameMode::TwoPlayer) {
				pacmanPtr->SetPosition(POSITION_PACMAN);
				pacmanPtr->GetComponent<dae::MovementComponent>()->SetTargetNode(nullptr);
				pacmanPtr->GetComponent<dae::MovementComponent>()->FindAndSetNode(builderPtr);
			}

			blinkyPtr->SetPosition(POSITION_BLINKY);
			blinkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);
			blinkyPtr->GetComponent<dae::MovementComponent>()->Reset(builderPtr);

			pinkyPtr->SetPosition(POSITION_PINKY);
			pinkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);
			pinkyPtr->GetComponent<dae::MovementComponent>()->Reset(builderPtr);
			pinkyPtr->GetComponent<dae::GhostAIComponent>()->SetStartDelay(4.f);

			inkyPtr->SetPosition(POSITION_INKY);
			inkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);
			inkyPtr->GetComponent<dae::MovementComponent>()->Reset(builderPtr);
			inkyPtr->GetComponent<dae::GhostAIComponent>()->SetStartDelay(8.f);

			SuePtr->SetPosition(POSITION_Sue);
			SuePtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);
			SuePtr->GetComponent<dae::MovementComponent>()->Reset(builderPtr);
			SuePtr->GetComponent<dae::GhostAIComponent>()->SetStartDelay(12.f);

			soundSystem.Unmute(1);
			if(!mspacmanPtr->GetComponent<dae::HealthComponent>()->IsDead())
				scene.Resume();
			});

		// --- Collision events --- //

		// Ghost returning to house after being eaten
		event.Subscribe(OnTriggerEnter, [builderPtr = builder](const Event& e) {
			auto* collisionArgs = static_cast<dae::CollisionEventArg*>(e.args[0]);
			if (!collisionArgs || !collisionArgs->other || !collisionArgs->self) return;
			bool isGhostCollision = (collisionArgs->self == builderPtr->GetGhostHouse()->GetComponent<dae::ColliderComponent>()
				&& collisionArgs->other->GetLayer() == dae::CollisionLayer::Enemy);

			if (isGhostCollision)
			{
				auto ghostAI = collisionArgs->other->GetOwner()->GetComponent<dae::GhostAIComponent>();
				if (ghostAI->GetEaten())
				{
					ghostAI->ChangeState(std::make_unique<dae::GhostChaseState>());
				}
			}
			});

		// Safe Guard for when ghost are stuck in the ghost house
		event.Subscribe(OnTriggerStay, [builderPtr = builder](const Event& e) {
			auto* collisionArgs = static_cast<dae::CollisionEventArg*>(e.args[0]);
			if (!collisionArgs || !collisionArgs->other || !collisionArgs->self) return;

			bool isGhostCollision = (collisionArgs->self == builderPtr->GetGhostHouse()->GetComponent<dae::ColliderComponent>()
				&& collisionArgs->other->GetLayer() == dae::CollisionLayer::Enemy);

			if (isGhostCollision)
			{
				auto ghostMovement = collisionArgs->other->GetOwner()->GetComponent<dae::MovementComponent>();
				ghostMovement->SetIgnoreNodePreferences(true);
			}
		});

		event.Subscribe(OnTriggerExit, [builderPtr = builder](const Event& e) {
			auto* collisionArgs = static_cast<dae::CollisionEventArg*>(e.args[0]);
			if (!collisionArgs || !collisionArgs->other || !collisionArgs->self) return;
			bool isGhostCollision = (collisionArgs->self == builderPtr->GetGhostHouse()->GetComponent<dae::ColliderComponent>()
				&& collisionArgs->other->GetLayer() == dae::CollisionLayer::Enemy);

			if (isGhostCollision)
			{
				auto ghostMovement = collisionArgs->other->GetOwner()->GetComponent<dae::MovementComponent>();
				ghostMovement->SetIgnoreNodePreferences(false);
			}
		});

		// Pacman eating all pellets -> Next Level
		event.Subscribe(AllPelletsEaten, [&levelManager, &scene, mspacmanPtr, pacmanPtr, blinkyPtr, pinkyPtr, inkyPtr, SuePtr, gameMode, builderPtr = builder](const Event&) {
			levelManager.NextLevel();
		});

		event.Subscribe(OnTriggerStay, [builderPtr = builder](const Event& e) {
			auto* collisionArgs = static_cast<dae::CollisionEventArg*>(e.args[0]);
			if (!collisionArgs || !collisionArgs->other || !collisionArgs->self) return;

			if(collisionArgs->other->GetLayer() != dae::CollisionLayer::Object)
				return;

			bool isFruitCollision = (collisionArgs->self == builderPtr->GetLeftFruitTriggerArea()->GetComponent<dae::ColliderComponent>()
									&& collisionArgs->other->GetLayer() == dae::CollisionLayer::Object) ||
									(collisionArgs->self == builderPtr->GetRightFruitTriggerArea()->GetComponent<dae::ColliderComponent>()
									&& collisionArgs->other->GetLayer() == dae::CollisionLayer::Object);

			if(!isFruitCollision)
				return;

			auto* fruitComponent = collisionArgs->other->GetOwner()->GetComponent<dae::FruitComponent>();
			if (!fruitComponent)
				return;

			if (fruitComponent->GetEnterState())
			{
				EventSystem::GetInstance().Notify(Event{ FruitEnterScene });
				if (collisionArgs->self == builderPtr->GetLeftFruitTriggerArea()->GetComponent<dae::ColliderComponent>()) fruitComponent->GetMovement()->SetNextDirection(glm::vec2(1.f, 0.f));
				if (collisionArgs->self == builderPtr->GetRightFruitTriggerArea()->GetComponent<dae::ColliderComponent>()) fruitComponent->GetMovement()->SetNextDirection(glm::vec2(-1.f, 0.f));
			}

			if (fruitComponent->GetExitState())
			{
				EventSystem::GetInstance().Notify(Event{ FruitExitScene });
				fruitComponent->ChangeState(std::make_unique<dae::FruitHiddenState>());
			}
		});

		event.Subscribe(FruitEnterScene, [&soundSystem](const Event&) {
			soundSystem.Unmute(8);
			});

		event.Subscribe(FruitExitScene, [&soundSystem](const Event&) {
			soundSystem.Mute(8);
			});

		event.Subscribe(StartTimer, [&scene, &soundSystem, readyTextPtr](const Event&) {
			readyTextPtr->GetComponent<dae::TextComponent>()->SetVisible(false);
			soundSystem.Play(1, 0.2f, true);
			scene.Resume();
		});

		event.Subscribe(EatenAnimationTimer, [&scene, blinkyPtr, pinkyPtr, inkyPtr, SuePtr, ghostScorePtr](const Event&) {
			scene.Resume();
			blinkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);
			pinkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);
			inkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);
			SuePtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);
			ghostScorePtr->GetComponent<dae::SpriteComponent>()->SetVisible(false);
			});

		event.Subscribe(ExtraLife, [&soundSystem, mspacmanPtr](const Event&) {
			soundSystem.Play(9, 0.5f, false);
			mspacmanPtr->GetComponent<dae::HealthComponent>()->AddHeal(1);
		});
	}
}