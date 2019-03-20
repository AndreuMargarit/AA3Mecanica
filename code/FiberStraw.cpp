#include "FiberStraw.h"

glm::vec3 springforce(const glm::vec3 & P1, const glm::vec3 & V1, const glm::vec3 & P2, const glm::vec3 & V2, float L0, float ke, float kd)
{

	return -glm::vec3((ke *(glm::length(P1 - P2) - L0) + kd*(V1-V2) * ((P1-P2)/glm::length(P1-P2)) * ((P1 - P2) / glm::length(P1 - P2))));
}

glm::vec3 computeForces(FiberStraw & fiber, int idx, const std::vector<ForceActuator*>& force_acts)
{
	glm::vec3 totalF;

	//for(int i=0;i<fiber.)
	return glm::vec3();
}
