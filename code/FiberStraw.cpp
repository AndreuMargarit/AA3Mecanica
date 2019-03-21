#include "FiberStraw.h"

glm::vec3 springforce(const glm::vec3 & P1, const glm::vec3 & V1, const glm::vec3 & P2, const glm::vec3 & V2, float L0, float ke, float kd)
{
	 
	return -glm::vec3(((ke *(glm::length(P1 - P2) - L0) + kd* glm::dot((V1-V2), ((P1-P2)/glm::length(P1-P2)))) * ((P1 - P2) / glm::length(P1 - P2))));
}

glm::vec3 computeForces(FiberStraw & fiber, int idx, const std::vector<ForceActuator*>& force_acts)
{
	glm::vec3 totalF = glm::vec3(0,0,0);

	std::vector<int> conn;
	for (int i = 0; i < fiber.connexions.size(); i++)
		if (fiber.connexions[i].first == idx) conn.push_back(fiber.connexions[i].second);

	for (int i = 0; i < conn.size(); i++)
	{
		if (abs(idx - conn[i]) != 1)
			totalF += springforce(fiber.positions[idx], fiber.velocities[idx], fiber.positions[conn[i]], fiber.velocities[conn[i]], restLength * 2, ke, kd);
		else
			totalF += springforce(fiber.positions[idx], fiber.velocities[idx], fiber.positions[conn[i]], fiber.velocities[conn[i]], restLength, ke, kd);
	}

	for (int i = 0; i < force_acts.size(); i++)
	{
		totalF += force_acts[i]->computeForce(mass, glm::vec3(0, 0, 0));
	}

	return totalF;
}

void verlet(float dt, FiberStraw & fiber, /*const std::vector<Collider*>& colliders,*/ const std::vector<ForceActuator*>& force_acts)
{
	glm::vec3 aux;
	for (int i = 1; i < NUM_PARTICLES; i++)
	{
		aux = fiber.positions[i];
		fiber.positions[i] = fiber.positions[i] + (fiber.positions[i] - fiber.lastPositions[i]) + (computeForces(fiber, i, force_acts) / mass) * pow(dt, 2);
		fiber.lastPositions[i] = aux;
	}
}
