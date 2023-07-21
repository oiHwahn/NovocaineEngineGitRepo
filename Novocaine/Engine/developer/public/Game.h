#pragma once

class GraphicsEngine;

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



private:
	Game();
	~Game();

	// when the app first loads
	// will initialise all the required dependencies
	bool Initialise();

	// listen and detect for input
	void ProcessInput();

	// handle the games logic
	void Update();

	// render graphics to the screen
	void Draw();

	// uninitialise and delete all memory
	void CleanupGame();

private:
	bool bIsGameOver;

	// hold the graphcis engine
	GraphicsEngine* Graphics;
};