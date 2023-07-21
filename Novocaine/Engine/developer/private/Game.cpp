#include "Game.h"
#include "CoreMinimal.h"
#include "GraphicsEngine.h"
#include "SDL2/SDL.h"

Game* Game::GetGameInstance()
{
	// Create a game instance the first time the function runs
	// after the first time since this is a static the variable will remain constant
	static Game* GameInstance = new Game();

	return GameInstance;
}

void Game::DestroyGameInstance()
{
	delete GetGameInstance();
}

void Game::Run()
{
	// if initialise succeeds then game is not over
	bIsGameOver = !Initialise();

	// as long as the game is not over run the game loop
	while (!bIsGameOver) {
		ProcessInput();

		Update();

		Draw();

	}

	// once the game ends cleap up any memory
	CleanupGame();
}

void Game::CloseGame()
{
	bIsGameOver = true;
}

Game::Game()
{
	bIsGameOver = false;
	Graphics = nullptr;
}

Game::~Game()
{
}

bool Game::Initialise()
{
	Graphics = new GraphicsEngine();

	if (!Graphics->Initialise()) {
		return false;
	}

	return true;
}

void Game::ProcessInput()
{
	// TODO: Process the input of the player
	SDL_Event Event;

	while (SDL_PollEvent(&Event)) {
		switch (Event.type) { 
		case SDL_QUIT: 
			CloseGame();
			break;
		default:
			break;
		}
	}
}

void Game::Update()
{
	// TODO: Process the logic of the game
}

void Game::Draw()
{
	// clears prev frame
	Graphics->ClearGraphics();
	// draws new frame
	Graphics->DrawGraphics();
	// presents the frame to the window
	Graphics->PresentGraphics();
}

void Game::CleanupGame()
{
	// destrop the graphics engine
	if (Graphics != nullptr) {
		delete Graphics;
	}
}
