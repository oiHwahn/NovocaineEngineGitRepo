#include "Input.h"
#include "Game.h"

Input::Input()
{
	KeyboardState = nullptr;
}

void Input::ProcessInput()
{
	if (KeyboardState == nullptr)
		KeyboardState = SDL_GetKeyboardState(NULL);

	// reset the delta each frame to make sure we got 0
	MouseXDelta = MouseYDelta = 0;
	ScrollDelta = 0.0f;
	
	SDL_Event Event;

	while (SDL_PollEvent(&Event)) {
		switch (Event.type) {
		case SDL_KEYUP :
			KeyboardState = SDL_GetKeyboardState(NULL);
			break;
		case SDL_KEYDOWN :
			KeyboardState = SDL_GetKeyboardState(NULL);
			break;
		case SDL_MOUSEBUTTONDOWN :
			SetMouseState(Event.button.button, true);
			break;
		case SDL_MOUSEBUTTONUP:
			SetMouseState(Event.button.button, false );
			break;
		case SDL_MOUSEMOTION:
			MouseXDelta += Event.motion.xrel;
			MouseYDelta += Event.motion.yrel;
			break;
		case SDL_MOUSEWHEEL:
			ScrollDelta = +Event.wheel.preciseY;
			break;
		case SDL_QUIT:
			Game::GetGameInstance()->CloseGame();
			break;
		default:
			break;
		}
	}
}

bool Input::IsKeyDown(SDL_Scancode KeyCode)
{
	return KeyboardState[KeyCode];
}

bool Input::IsMouseButtonDown(NovoUint Button)
{
	// making sure the button isnt out of the array scope
	if (Button > MAX_MOUSE_BUTTONS)
		return false;

	return MouseState [Button];
}

MouseData Input::GetMouseData() const
{
	return MouseData(MouseXDelta, MouseYDelta, ScrollDelta);
}

void Input::SetCursorInvisible(bool bIsVisible)
{
	// set the cursor to be visible by default
	SDL_bool RelativeMode = SDL_FALSE;

	// turn the cursor off and activate relative mode
	if (!bIsVisible)
		RelativeMode = SDL_TRUE;

	SDL_SetRelativeMouseMode(RelativeMode);
}



void Input::SetMouseState(NovoUint Button, bool bState)
{
	if (Button > MAX_MOUSE_BUTTONS)
		return;

	MouseState[Button] = bState;
}
