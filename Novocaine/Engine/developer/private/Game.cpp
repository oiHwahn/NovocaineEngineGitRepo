#include "Game.h"
#include "CoreMinimal.h"
#include "GraphicsEngine.h"
#include "SDL2/SDL.h"
#include "Input.h"

// DEBUG
#include "graphics/ShapeMatrices.h"
#include "graphics/Mesh.h"

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

	if (!bIsGameOver)
		BeginPlay();

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

Texture* Game::GetDefaultTexture() const
{
	return Graphics->GetDefaultTexture();
}

Game::Game()
{
	bIsGameOver = false;
	Graphics = nullptr;
	DeltaTime = 0.0;
	GameInput = nullptr;
	
	// DEBUG
	Cube1 = nullptr;
	Cube2 = nullptr;
}

bool Game::Initialise()
{
	Graphics = new GraphicsEngine();

	if (!Graphics->Initialise()) {
		return false;
	}

	GameInput = new Input();

	return true;
}

void Game::BeginPlay()
{
	// TODO: add anything the needs to load at the start
	Cube1 = Graphics->Create3DShape(novosm::Cube);
	Cube2 = Graphics->Create3DShape(novosm::Cube);

	// change one of the cubes to a differnt texture
	Cube1->BaseColour = Graphics->GetTexture("Engine/developer/textures/T_NovoMarioGrid.png");
	Cube2->BaseColour = Graphics->GetTexture("Engine/developer/textures/T_NovoPortalGrid.png");


	// move cubes away from each other
	Cube1->Transform.Location += glm::vec3(2.0f, 0.0f, 1.0f);
	Cube2->Transform.Location += glm::vec3(2.0f, 0.0f, -1.0f);
}

void Game::ProcessInput()
{
	// TODO: Process the input of the player
	GameInput->ProcessInput();
}

void Game::Update()
{
	// initialise the last frame time as static to remember the last frame
	static double LastFrameTime = 0.0;
	// get the current frame time
	double CurrentFrameTime = static_cast<double>(SDL_GetTicks64());
	// find the delta time milliseconds
	double NewDelta = CurrentFrameTime - LastFrameTime;
	// Set delta timeas seconds
	DeltaTime = NewDelta / 1000.0;
	// Update the last frame time for the next loop
	LastFrameTime = CurrentFrameTime;

	// update any logic in the graphics engine
	Graphics->Update();

	// TODO: Process the logic of the game
	if (Cube1 != nullptr) {
		Cube1->Transform.Rotation.x += GetDeltaTimeF() * 50.0f;
		Cube1->Transform.Rotation.y += GetDeltaTimeF() * 50.0f;
		Cube1->Transform.Rotation.z += GetDeltaTimeF() * 50.0f;
	}

	if (Cube2 != nullptr) {
		Cube2->Transform.Rotation.x -= GetDeltaTimeF() * 50.0f;
		Cube2->Transform.Rotation.y -= GetDeltaTimeF() * 50.0f;
		Cube2->Transform.Rotation.z -= GetDeltaTimeF() * 50.0f;
	}


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
