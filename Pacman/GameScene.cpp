#include "GameScene.h"

const int WIDTH = 448;
const int HEIGHT = 576;

void LoadGameScene(dae::Scene& scene, dae::SceneManager& sceneManager, dae::GameMode)
{
	// --- ENGINE --- //

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	auto retroFont = dae::ResourceManager::GetInstance().LoadFont("RetroFont.ttf", 18);
	auto& input = dae::InputManager::GetInstance();
	auto& event = EventSystem::GetInstance();
	auto& soundSystem = dae::ServiceLocator::GetSoundSystem();

	// --- GAME --- //

	auto& highScoreManager = HighScoreManager::GetInstance();
	highScoreManager.ReadHighScore();

	soundSystem.LoadSound(1, "../Data/MSPC_Siren0.wav");
	soundSystem.LoadSound(2, "../Data/MSPC_EatDot.wav");
	if (sceneManager.GetActiveSceneName() == "Game") soundSystem.Play(1, 0.5f, true);

	// LEVEL
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("MSPC_Level1.tga", 2.0f);
	go->SetPosition(0.f, 40.f);
	scene.Add(go);

	static dae::LevelBuilder builder;
	builder.LoadLevelFromFile("../Data/Level1.txt", &scene, -0.4f, -0.4f);

	auto highScoreText = std::make_unique<dae::GameObject>();
	highScoreText->AddComponent<dae::TextComponent>("HIGH SCORE", retroFont);
	highScoreText->SetPosition((WIDTH / 2) - 75.f, 1.0f);
	scene.Add(highScoreText);

	auto highScore = std::make_unique<dae::GameObject>();
	highScore->AddComponent<dae::TextComponent>(highScoreManager.GetTopScoreDisplay(), retroFont);
	highScore->SetPosition((WIDTH / 2) - 50.f, 20.0f);
	scene.Add(highScore);

	// DAE STUFF
	go = std::make_unique<dae::GameObject>();
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

	auto blinky = std::make_unique<dae::GameObject>();
	blinky->SetPosition(150, 200);
	blinky->AddComponent<dae::SpriteComponent>("MSPC_Ghost_Spritesheet.tga", 6, 4, 0.1f, 2.0f);
	blinky->GetComponent<dae::SpriteComponent>()->SetAnimationColumn(0);
	blinky->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Enemy, false, true);
	blinky->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Ghost);
	blinky->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(&builder);

	auto pinky = std::make_unique<dae::GameObject>();
	pinky->SetPosition(100, 200);
	pinky->AddComponent<dae::SpriteComponent>("MSPC_Ghost_Spritesheet.tga", 6, 4, 0.1f, 2.0f);
	pinky->GetComponent<dae::SpriteComponent>()->SetAnimationColumn(0);
	pinky->GetComponent<dae::SpriteComponent>()->SetAnimationRow(1);
	pinky->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Enemy, false, true);
	pinky->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Ghost);
	pinky->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(&builder);

	auto inky = std::make_unique<dae::GameObject>();
	inky->SetPosition(200, 200);
	inky->AddComponent<dae::SpriteComponent>("MSPC_Ghost_Spritesheet.tga", 6, 4, 0.1f, 2.0f);
	inky->GetComponent<dae::SpriteComponent>()->SetAnimationColumn(0);
	inky->GetComponent<dae::SpriteComponent>()->SetAnimationRow(2);
	inky->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Enemy, false, true);
	inky->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Ghost);
	inky->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(&builder);

	auto clyde = std::make_unique<dae::GameObject>();
	clyde->SetPosition(150, 250);
	clyde->AddComponent<dae::SpriteComponent>("MSPC_Ghost_Spritesheet.tga", 6, 4, 0.1f, 2.0f);
	clyde->GetComponent<dae::SpriteComponent>()->SetAnimationColumn(0);
	clyde->GetComponent<dae::SpriteComponent>()->SetAnimationRow(3);
	clyde->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Enemy, false, true);
	clyde->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Ghost);
	clyde->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(&builder);

	// --- PACMAN & MsPACMAN --- //

	auto pacman = std::make_unique<dae::GameObject>();
	auto pacmanPtr = pacman.get();
	pacman->AddComponent<dae::SpriteComponent>("MSPC_Pacman_Spritesheet.tga", 4, 3, 0.1f, 2.0f);
	pacman->SetPosition(200, 304);
	pacman->AddComponent<dae::HealthComponent>(3, HealthChanged, PlayerDied);
	pacman->AddComponent<dae::PointsComponent>(PointsChanged);
	pacman->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Friendly, true, true);
	pacman->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Pacman);
	pacman->AddComponent<dae::TeleportComponent>(-32.f, static_cast<float>(WIDTH));
	pacman->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(&builder);
	scene.Add(pacman);

	auto mspacman = std::make_unique<dae::GameObject>();
	auto mspacmanPtr = mspacman.get();
	mspacman->AddComponent<dae::SpriteComponent>("MSPC_Mspacman_Spritesheet.tga", 4, 3, 0.1f, 2.0f);
	mspacman->SetPosition(208.f, 400.f);
	mspacman->AddComponent<dae::HealthComponent>(3, HealthChanged, PlayerDied);
	mspacman->AddComponent<dae::PointsComponent>(PointsChanged);
	mspacman->AddComponent<dae::ColliderComponent>(glm::vec2(16.f, 16.f), glm::vec2(8.f, 8.f), dae::CollisionLayer::Friendly, true, true);
	mspacman->AddComponent<dae::MovementComponent>(100.f, dae::AnimationType::Pacman);
	mspacman->AddComponent<dae::TeleportComponent>(-32.0f, static_cast<float>(WIDTH));
	//mspacman->GetComponent<dae::MovementComponent>()->FindAndSetStartNode(&builder);
	scene.Add(mspacman);

	// --- ADD GHOSTS --- //

	clyde->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Clyde);
	scene.Add(clyde);
	inky->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Inky);
	scene.Add(inky);
	pinky->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Pinky);
	scene.Add(pinky);
	blinky->AddComponent<dae::GhostAIComponent>(mspacmanPtr, dae::GhostType::Blinky);
	scene.Add(blinky);

	// EVENTS

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


	event.Subscribe(OnTriggerEnter, [&soundSystem, mspacmanPtr, pacmanPtr](const Event& e) {
		auto* collisionArgs = static_cast<dae::CollisionEventArg*>(e.args[0]);
		if (!collisionArgs || !collisionArgs->other || !collisionArgs->self) return;

		// Only if the "self" is the pellet (Object) and "other" is Friendly (Ms. Pac-Man)
		bool isPelletCollectedByAPacman =
			(collisionArgs->self->GetLayer() == dae::CollisionLayer::Object &&
				collisionArgs->other == mspacmanPtr->GetComponent<dae::ColliderComponent>()) ||
			(collisionArgs->other->GetLayer() == dae::CollisionLayer::Object &&
				collisionArgs->self == mspacmanPtr->GetComponent<dae::ColliderComponent>()) ||
			(collisionArgs->self->GetLayer() == dae::CollisionLayer::Object &&
				collisionArgs->other == pacmanPtr->GetComponent<dae::ColliderComponent>()) ||
			(collisionArgs->other->GetLayer() == dae::CollisionLayer::Object &&
				collisionArgs->self == pacmanPtr->GetComponent<dae::ColliderComponent>());

		if (isPelletCollectedByAPacman)
		{
			dae::ColliderComponent* pelletCollider =
				(collisionArgs->self->GetLayer() == dae::CollisionLayer::Object)
				? collisionArgs->self : collisionArgs->other;

			dae::GameObject* pellet = pelletCollider->GetOwner();
			soundSystem.Play(2, 1.0f, false);

			auto pointsComponent = mspacmanPtr->GetComponent<dae::PointsComponent>();
			if (pointsComponent) pointsComponent->AddPoints(10);

			builder.RemovePellet(pellet);
		}
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

	// --- INPUTS --- //

	input.BindCommand(SDLK_w, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(pacmanPtr, glm::vec2(0.f, -1.f)));
	input.BindCommand(SDLK_s, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(pacmanPtr, glm::vec2(0.f, 1.f)));
	input.BindCommand(SDLK_a, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(pacmanPtr, glm::vec2(-1.f, 0.f)));
	input.BindCommand(SDLK_d, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(pacmanPtr, glm::vec2(1.f, 0.f)));

	input.BindCommand(SDLK_c, dae::KeyState::Pressed, std::make_unique<dae::TakeDamageCommand>(mspacmanPtr, 1));
	input.BindCommand(SDLK_z, dae::KeyState::Pressed, std::make_unique<dae::AddPointsCommand>(mspacmanPtr, 10));
	input.BindCommand(SDLK_x, dae::KeyState::Pressed, std::make_unique<dae::AddPointsCommand>(mspacmanPtr, 100));

	input.BindCommand(SDLK_UP, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(0.f, -1.f)));
	input.BindCommand(SDLK_DOWN, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(0.f, 1.f)));
	input.BindCommand(SDLK_LEFT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(-1.f, 0.f)));
	input.BindCommand(SDLK_RIGHT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(1.f, 0.f)));

	//input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_UP, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(0.f, -1.f)));
	//input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_DOWN, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(0.f, 1.f)));
	//input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_LEFT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(-1.f, 0.f)));
	//input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(mspacmanPtr, glm::vec2(1.f, 0.f)));

	//input.BindCommand(SDL_CONTROLLER_BUTTON_X, dae::KeyState::Pressed, std::make_unique<dae::TakeDamageCommand>(mspacmanPtr, 1));
	//input.BindCommand(SDL_CONTROLLER_BUTTON_A, dae::KeyState::Pressed, std::make_unique<dae::AddPointsCommand>(mspacmanPtr, 10));
	//input.BindCommand(SDL_CONTROLLER_BUTTON_B, dae::KeyState::Pressed, std::make_unique<dae::AddPointsCommand>(mspacmanPtr, 100));
}