#include "graphics/Camera.h"
#include "Game.h"
#include "Input.h"

#define DEFAULT_FOV 70.0f

Camera::Camera(float x, float y, float z)
{
	FOV = DEFAULT_FOV;
	NearClip = 0.01f;
	FarClip = 1000.0f;
	Speed = 5.0f;
	LookSensitivity = 70.0f;
	Transform.Location = glm::vec3(x, y, z);
}

void Camera::Update()
{
	Input* GameInput = Game::GetGameInstance()->GetGameInput();
	float DeltaTime = Game::GetGameInstance()->GetDeltaTimeF();

	// Move camera forward if W is down
	if (GameInput->IsKeyDown(SDL_SCANCODE_W)) {
		AddMovementInput(Transform.GetForward());
	}

	// Move camera back if S is down
	if (GameInput->IsKeyDown(SDL_SCANCODE_S)) {
		AddMovementInput(-Transform.GetForward());
	}

	// Move camera forward if A is down
	if (GameInput->IsKeyDown(SDL_SCANCODE_A)) {
		AddMovementInput(-Transform.GetRight());

	}

	// Move camera back if D is down
	if (GameInput->IsKeyDown(SDL_SCANCODE_D)) {
		AddMovementInput(Transform.GetRight());
	}

	// move the camera up based on world rotation
	if (GameInput->IsKeyDown(SDL_SCANCODE_Q)) {
		AddMovementInput(WORLD_UP);
	}

	// move the camera down based on world rotation
	if (GameInput->IsKeyDown(SDL_SCANCODE_E)) {
		AddMovementInput(-WORLD_UP);
	}

	// activate rotational mode
	if (GameInput->IsMouseButtonDown(SDL_BUTTON_RIGHT)) {
		// hide the cursor
		GameInput->SetCursorInvisible(false);

		// rotation the pitch - mouse y looks up and down
		float RotX = -GameInput->GetMouseData().MouseYDelta * DeltaTime;
		// rotation the yaw - mouse x looks left and right
		float RotY = GameInput->GetMouseData().MouseXDelta * DeltaTime;

		// create a vec 3 for the final rotation and consider sensitivity
		glm::vec3 FinalRotation = glm::vec3(RotX, RotY, 0.0f) * LookSensitivity;

		// change the rotation of the camera
		AddRotation(FinalRotation);

		// change the fov when scrolling
		if (GameInput->GetMouseData().ScrollDelta != 0) {
			// get new fov
			float NewFOV = FOV + -GameInput->GetMouseData().ScrollDelta * 5.0f;
			
			// make sure it cant go below 0
			NewFOV = std::max(NewFOV, 0.0f);
			NewFOV = std::min(NewFOV, 180.0f);


			// set the fov to the new fov
			FOV = NewFOV;

			NOVO_MSG_LOG("Camera", FOV)
		}
	}
	else {
		// show the cursor
		GameInput->SetCursorInvisible(true);

		// reset fov if we changed it
		if (FOV != DEFAULT_FOV)
			FOV = DEFAULT_FOV;
	}

}

void Camera::AddMovementInput(glm::vec3 Direction, float Scale)
{
	// adjust the direction for the speed value
	glm::vec3 SpeedDirection = Direction * Speed;

	// get the speed of the camera and account for time
	glm::vec3 DeltaSpeed = SpeedDirection * Game::GetGameInstance()->GetDeltaTimeF();

	// move the camera location based on direction and speed then scale the value
	Transform.Location += DeltaSpeed * Scale;
}

void Camera::AddRotation(glm::vec3 Rotation)
{
	// rotate the pitch
	Transform.Rotation.x += Rotation.x;

	// min chooses the smaller number of the two capping x at 89
	Transform.Rotation.x = std::min(Transform.Rotation.x, 89.0f);
	// max chooses the larger number of the two capping x at -89 minimum
	Transform.Rotation.x = std::max(Transform.Rotation.x, -89.0f);

	// rotate the yaw
	Transform.Rotation.y += Rotation.y;

	// make sure the rotation resets when it hits 360.0f
	Transform.Rotation.y = glm::mod(Transform.Rotation.y, 360.0f);

}
