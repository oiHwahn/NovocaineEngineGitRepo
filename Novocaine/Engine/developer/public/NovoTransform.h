#pragma once
#include "glm/glm.hpp"


struct NovoTransform {
	NovoTransform() {
		Location = glm::vec3(0.0f);
		Rotation = glm::vec3(0.0f);
		Scale = glm::vec3(1.0f);

	}



	glm::vec3 Location;
	glm::vec3 Rotation;
	glm::vec3 Scale;


};