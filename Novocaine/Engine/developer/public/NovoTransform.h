#pragma once
#include "glm/glm.hpp"

#define WORLD_UP glm::vec3(0.0f, 1.0f, 0.0f)

struct NovoDirection {
	NovoDirection(glm::vec3 Rotation) {
		// determin the cosine and sine values of the rotations
		float CosX = cos(glm::radians(Rotation.x));
		float CosY = cos(glm::radians(Rotation.y));
		float SinX = sin(glm::radians(Rotation.x));
		float SinY = sin(glm::radians(Rotation.y));

		// find the vector using the axis angles
		Forward.x = CosY * CosX;
		Forward.y = SinX;
		Forward.z = SinY * CosX;

		// we need to normalise direction values to make sure they dont consider distance
		Forward = glm::normalize(Forward);

		// cross product gives the axis perpendicular to two given axis
		Right = glm::normalize(glm::cross(Forward, WORLD_UP));

		Up = glm::normalize(glm::cross(Right, Forward));
	}

	glm::vec3 Forward;
	glm::vec3 Right;
	glm::vec3 Up;
};

struct NovoTransform {
	NovoTransform() {
		Location = glm::vec3(0.0f);
		Rotation = glm::vec3(0.0f);
		Scale = glm::vec3(1.0f);
	}

	// Get all 3 directions
	NovoDirection GetDirections() {
		return NovoDirection(Rotation);
	}

	// Get all forrward vector based on the rotation
	glm::vec3 GetForward() {
		return NovoDirection(Rotation).Forward;
	}

	// Get all right vector based on the rotation
	glm::vec3 GetRight() {
		return NovoDirection(Rotation).Right;
	}
	
	// Get all Up vector based on the rotation
	glm::vec3 GetUp() {
		return NovoDirection(Rotation).Up;
	}

	glm::vec3 Location;
	glm::vec3 Rotation;
	glm::vec3 Scale;


};