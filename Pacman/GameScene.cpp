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
		auto bigRetroFont = dae::ResourceManager::GetInstance().LoadFont("RetroFont.ttf", 40);
		auto& input = dae::InputManager::GetInstance();
		input.RequestClearAllBinds();
		auto& event = EventSystem::GetInstance();
		auto& soundSystem = dae::ServiceLocator::GetSoundSystem();

		// --- GAME --- //

		auto& highScoreManager = HighScoreManager::GetInstance();
		highScoreManager.ReadHighScore();

		soundSystem.LoadSound(1, "../Data/MSPC_Siren0.wav");
		soundSystem.LoadSound(2, "../Data/MSPC_EatDot.wav");
		soundSystem.LoadSound(3, "../Data/MSPC_Start.wav");
		soundSystem.LoadSound(4, "../Data/MSPC_EatGhost.wav");
		soundSystem.LoadSound(5, "../Data/MSPC_Death.wav");
		soundSystem.LoadSound(6, "../Data/MSPC_Fright.wav");

		soundSystem.Play(3, 0.2f, false);

		soundSystem.Play(1, 0.2f, true);

		scene.Timer(4.f, StartTimer);

		// --- LEVEL --- //

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
		auto blackScreenPtr = blackScreen.get();
		blackScreen->AddComponent<dae::RenderComponent>("MSPC_BlackScreen.tga", 2.0f);
		blackScreen->SetPosition(0.f, 40.f);
		blackScreen->GetComponent<dae::RenderComponent>()->SetColor(0.f, 0.f, 0.f, 0.f);
		blackScreen->SetRenderLayer(3);
		scene.Add(blackScreen);

		auto readyText = std::make_unique<dae::GameObject>();
		auto readyTextPtr = readyText.get();
		readyText->AddComponent<dae::TextComponent>("READY!", retroFont);
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
		blinky->SetPosition(170.f, 200.f);
		blinky->AddComponent<dae::SpriteComponent>("MSPC_Ghost_Spritesheet.tga", 6, 4, 0.1f, 2.0f);
		blinky->GetComponent<dae::SpriteComponent>()->SetLooping(false);
		blinky->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Enemy, false, true);
		blinky->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Ghost, builder);
		blinky->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(builder);

		auto pinky = std::make_unique<dae::GameObject>(); // Pink Ghost
		auto pinkyPtr = pinky.get();
		pinky->SetPosition(190.f, 240.f);
		pinky->AddComponent<dae::SpriteComponent>("MSPC_Ghost_Spritesheet.tga", 6, 4, 0.1f, 2.0f);
		pinky->GetComponent<dae::SpriteComponent>()->SetLooping(false);
		pinky->GetComponent<dae::SpriteComponent>()->SetAnimationRow(1);
		pinky->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Enemy, false, true);
		pinky->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Ghost, builder);
		pinky->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(builder);

		auto inky = std::make_unique<dae::GameObject>(); // Cyan Ghost
		auto inkyPtr = inky.get();
		inky->SetPosition(150.f, 240.f);
		inky->AddComponent<dae::SpriteComponent>("MSPC_Ghost_Spritesheet.tga", 6, 4, 0.1f, 2.0f);
		inky->GetComponent<dae::SpriteComponent>()->SetLooping(false);
		inky->GetComponent<dae::SpriteComponent>()->SetAnimationRow(2);
		inky->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Enemy, false, true);
		inky->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Ghost, builder);
		inky->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(builder);

		auto clyde = std::make_unique<dae::GameObject>(); // Orange Ghost
		auto clydePtr = clyde.get();
		clyde->SetPosition(240.f, 240.f);
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
			levelManager.AddLevelObjects(pacmanPtr);
		}

		auto mspacman = std::make_unique<dae::GameObject>();
		auto mspacmanPtr = mspacman.get();
		mspacman->AddComponent<dae::SpriteComponent>("MSPC_Mspacman_Spritesheet.tga", 4, 3, 0.1f, 2.0f);
		mspacman->SetPosition(194.f, 395.f);
		mspacman->AddComponent<dae::AnimationRotationComponent>();
		mspacman->AddComponent<dae::HealthComponent>(1, HealthChanged, PlayerDied);
		mspacman->AddComponent<dae::PointsComponent>(PointsChanged);
		mspacman->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Friendly, true, true);
		mspacman->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Pacman, builder);
		mspacman->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(builder);
		scene.Add(mspacman);
		levelManager.AddLevelObjects(mspacmanPtr);
		// --- ADD GHOSTS --- //

		clyde->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Clyde);
		clyde->GetComponent<dae::GhostAIComponent>()->SetStartDelay(12.f);
		scene.Add(clyde);
		inky->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Inky);
		inky->GetComponent<dae::GhostAIComponent>()->SetStartDelay(8.f);
		scene.Add(inky);
		pinky->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Pinky);
		pinky->GetComponent<dae::GhostAIComponent>()->SetStartDelay(4.f);
		scene.Add(pinky);
		if (gameMode == dae::GameMode::Versus) blinky->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Blinky, true);
		else blinky->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Blinky);
		scene.Add(blinky);

		levelManager.AddLevelObjects(clydePtr);
		levelManager.AddLevelObjects(inkyPtr);
		levelManager.AddLevelObjects(pinkyPtr);
		levelManager.AddLevelObjects(blinkyPtr);

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

		static int ghostsEatenInFrightened = 0;
		event.Subscribe(GhostFrightenedOver, [&soundSystem](const Event&) {
			soundSystem.Unmute(1);
			ghostsEatenInFrightened = 0;
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
					ghostsEatenInFrightened = 0;
					soundSystem.Play(6, 0.5f, false);
					soundSystem.Mute(1); 
				}
				soundSystem.Play(2, 1.0f, false);

				auto pointsComponent = mspacmanPtr->GetComponent<dae::PointsComponent>();
				if (pointsComponent) pointsComponent->AddPoints(10);

				builderPtr->RemovePellet(pellet);
			}
		});

		// Ghost attack / eaten event
		event.Subscribe(OnTriggerEnter, [&soundSystem, &scene , ghostScorePtr ,mspacmanPtr, pacmanPtr, gameMode, builderPtr = builder](const Event& e) {
			auto* collisionArgs = static_cast<dae::CollisionEventArg*>(e.args[0]);
			if (!collisionArgs || !collisionArgs->other || !collisionArgs->self) return;

			bool isGhostCollision = (collisionArgs->other == mspacmanPtr->GetComponent<dae::ColliderComponent>() 
									&& collisionArgs->self->GetLayer() == dae::CollisionLayer::Enemy);

			if (isGhostCollision)
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
					ghostAI->QueueStateChange(std::make_unique<dae::GhostEatenState>());
					ghostMovement->SetTargetDestination(builderPtr->GetCenterNode());
				}
				else
				{
					scene.Timer(0.5f, AnimationTimer);
					soundSystem.Mute(1); 
					scene.Pause();
				}
			}
		});

		event.Subscribe(AnimationTimer, [&scene, &soundSystem, pacmanPtr, mspacmanPtr, blinkyPtr, pinkyPtr, inkyPtr, clydePtr](const Event&) {
			scene.Resume();
			blinkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(false);
			pinkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(false);
			inkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(false);
			clydePtr->GetComponent<dae::SpriteComponent>()->SetVisible(false);
			mspacmanPtr->GetComponent<dae::AnimationRotationComponent>()->Rotate(2, 45.f);
			soundSystem.Play(5, 0.5f, false);
			scene.Pause();
			scene.Timer(1.f, DeadAnimationTimer);
		});

		event.Subscribe(DeadAnimationTimer, [&scene, &soundSystem, pacmanPtr ,mspacmanPtr, blinkyPtr, pinkyPtr, inkyPtr, clydePtr, gameMode, builderPtr = builder](const Event&) {
			mspacmanPtr->GetComponent<dae::HealthComponent>()->TakeDamage(1);
			auto move = mspacmanPtr->GetComponent<dae::MovementComponent>();
			mspacmanPtr->SetPosition(194.f, 395.f); 
			move->SetTargetNode(nullptr);
			move->FindAndSetStartNode(builderPtr);

			blinkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);
			pinkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);
			inkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);
			clydePtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);

			soundSystem.Unmute(1);
			scene.Resume();
		});

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
					ghostAI->QueueStateChange(std::make_unique<dae::GhostChaseState>());
				}
			}
		});

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
		event.Subscribe(AllPelletsEaten, [&levelManager, &scene, mspacmanPtr, pacmanPtr, blinkyPtr, pinkyPtr, inkyPtr, clydePtr, gameMode, blackScreenPtr, builderPtr = builder](const Event&) {
			scene.Pause();
			blinkyPtr->GetComponent<dae::MovementComponent>()->Reset();
			pinkyPtr->GetComponent<dae::MovementComponent>()->Reset();
			inkyPtr->GetComponent<dae::MovementComponent>()->Reset();
			clydePtr->GetComponent<dae::MovementComponent>()->Reset();
			mspacmanPtr->GetComponent<dae::MovementComponent>()->Reset();

			mspacmanPtr->SetPosition(208.f, 400.f);
			blackScreenPtr->GetComponent<dae::RenderComponent>()->SetFadeEffect(dae::FadeEffect::FadeInAndOut, 4.f);
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

		event.Subscribe(StartTimer, [&scene, readyTextPtr](const Event&) {
			readyTextPtr->GetComponent<dae::TextComponent>()->SetVisible(false);
			scene.Resume();
		});

		event.Subscribe(EatenAnimationTimer, [&scene, blinkyPtr, pinkyPtr, inkyPtr, clydePtr, ghostScorePtr](const Event&) {
			scene.Resume();
			blinkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);
			pinkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);
			inkyPtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);
			clydePtr->GetComponent<dae::SpriteComponent>()->SetVisible(true);
			ghostScorePtr->GetComponent<dae::SpriteComponent>()->SetVisible(false);
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

		auto saveHighScore = std::make_unique<dae::GameObject>();
		auto saveHighScorePtr = saveHighScore.get();
		saveHighScore->SetPosition(120.f, 250.f);
		saveHighScore->AddComponent<dae::TextComponent>("", bigRetroFont);
		saveHighScore->AddComponent<dae::NameInputComponent>(highScoreManager.GetTopScore());
		saveHighScore->GetComponent<dae::TextComponent>()->SetVisible(false);
		scene.Add(saveHighScore);

		event.Subscribe(PlayerDied, [&levelManager, &scene, &input, saveHighScorePtr](const Event&) {
			scene.Pause();
			scene.ResumeObject(saveHighScorePtr);
			saveHighScorePtr->GetComponent<dae::TextComponent>()->SetVisible(true);

			input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_UP, dae::KeyState::Pressed, std::make_unique<dae::LetterUpCommand>(saveHighScorePtr));
			input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_DOWN, dae::KeyState::Pressed, std::make_unique<dae::LetterDownCommand>(saveHighScorePtr));
			input.BindCommand(SDL_CONTROLLER_BUTTON_A, dae::KeyState::Pressed, std::make_unique<dae::ConfirmLetterCommand>(saveHighScorePtr));
		});

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

		input.BindCommand(SDLK_F1, dae::KeyState::Pressed, std::make_unique<dae::CheatCommand>(&scene, &levelManager, &soundSystem, 1));
		input.BindCommand(SDLK_F2, dae::KeyState::Pressed, std::make_unique<dae::CheatCommand>(&scene, &levelManager, &soundSystem, 2));
		input.BindCommand(SDLK_F3, dae::KeyState::Pressed, std::make_unique<dae::CheatCommand>(&scene, &levelManager, &soundSystem, 3));

		std::cout << "GameScene loaded" << std::endl;
		scene.Pause();
		scene.ResumeObject(readyTextPtr);
	}
}