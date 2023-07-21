#pragma once
#include "CoreMinimal.h"
#include "SDL2/SDL.h"

// making our max mouse button number more readable 
#define MAX_MOUSE_BUTTONS SDL_BUTTON_X2

struct MouseData {
	MouseData()
		: MouseXDelta(0), MouseYDelta(0), ScrollDelta(0.0f) {}

	MouseData(int mousexdelta, float mouseydelta, float scrolldelta)
		: MouseXDelta(mousexdelta), MouseYDelta (mouseydelta), ScrollDelta(scrolldelta) {}

	int MouseXDelta, MouseYDelta;
	int ScrollDelta;
};

class Input {
public: 
	Input();

	// process inputs
	void ProcessInput();

	// check if key is down
	bool IsKeyDown(SDL_Scancode KeyCode);
	
	// check if a mouse button is down
	bool IsMouseButtonDown(NovoUint Button);

	// return the data of the mouse
	MouseData GetMouseData() const;

	// should cursor be visible
	void SetCursorInvisible(bool bIsVisible);

private:
	// set the mouse button state
	void SetMouseState(NovoUint Button, bool bState);

private: 
	// holds the state of the keyboard
	const unsigned char* KeyboardState;

	// holds the state of the mouse buttons
	bool MouseState[SDL_BUTTON_X2] = { false };
	
	// amount the mouse has moved in either x or y
	int MouseXDelta, MouseYDelta;

	// amout the scroll wheel has moved in either direction
	float ScrollDelta;
};