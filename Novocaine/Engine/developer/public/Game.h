#pragma once

class GraphicsEngine;
class Texture;
class Mesh;
class Input;

// DEBUG
class Game {
public:
	// create a game instance or return the one we already created
	static Game* GetGameInstance();

	// destroy the game instance
	static void DestroyGameInstance();

	// start/run application/gameloop
	void Run();

	// close the whole app
	void CloseGame();

	// Get tge defaykt engine texture from the graphics engine
	Texture* GetDefaultTexture() const;

	// super accurate delta time
	double GetDeltaTime() const { return DeltaTime; }

	// less accurate delta time
	float GetDeltaTimeF() const { return static_cast<float>(DeltaTime); }

	// get input for the game
	Input* GetGameInput() const { return GameInput; }

private:
	Game();
	~Game() {}

	// when the app first loads
	// will initialise all the required dependencies
	bool Initialise();

	// runs at the start of play (after initialisation)
	void BeginPlay();

	// listen and detect for input
	void ProcessInput();

	// handle the games logic
	void Update();

	// render graphics to the screen
	void Draw();

	// uninitialise and delete all memory
	void CleanupGame();

private:
	// if false app runs, if true app closes
	bool bIsGameOver;

	// hold the graphcis engine
	GraphicsEngine* Graphics;

	// Holds the input logic for the game
	Input* GameInput;

	// the time between each frame
	double DeltaTime;

	// DEBUG MESHES
	Mesh* Cube1;
	Mesh* Cube2;
};