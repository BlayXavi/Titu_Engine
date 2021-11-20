#pragma once

#include <glm/glm.hpp>
#include <iostream>

namespace TituEngine {
	namespace Math
	{
		bool DecomposeTransform(const glm::mat4 transform, glm::vec3& translation, glm::vec3& rotation);
		bool DecomposeTransform(const glm::mat4 transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);


	}
}


std::ostream& operator << (std::ostream& os, const glm::vec3& vector);
