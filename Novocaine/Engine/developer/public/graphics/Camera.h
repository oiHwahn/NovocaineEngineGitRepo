#pragma once
#include "NovoTransform.h"

class Camera {
public:
	Camera(float x = 0.0f, float y = 0.0f, float z = 0.0f);

	// game logic for camera
	void Update();

	// move the camera in a direction based on camera speed and scale
	void AddMovementInput(glm::vec3 Direction, float Scale = 1.0f);

	// rotate camera base on rotation values
	void AddRotation(glm::vec3 Rotation);

public:
	// Field of view
	float FOV;

	// closest that a 3D camera can render on the camera
	float NearClip;

	// furthest that a 3D camera can render on the camera

	float FarClip;

	// speed of camera 
	float Speed;

	// the speed of rotation
	float LookSensitivity;

	// Location, rotation in world space
	NovoTransform Transform;


};