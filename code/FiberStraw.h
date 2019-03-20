#pragma once

#include <stdlib.h>
#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <time.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include <vector>
#include <iostream>

#define NUM_PARTICLES 5
//float restLength = 1;

struct FiberStraw
{
	
	glm::vec3 positions[5];
	glm::vec3 velocities[5];
	std::vector<std::pair<int, int>> connexions;
	FiberStraw() {
		float x = (float)((rand() % 9000) - 4500) / 1000;
		float z = (float)((rand() % 9000) - 4500) / 1000;
		for (int i = 0; i < NUM_PARTICLES; i++) {
			positions[i] = glm::vec3(x, i - 1, z);
		}
		for (int i = 0; i < NUM_PARTICLES; i++) {
			velocities[i] = glm::vec3((float)((rand() % 10) - 5), 0, (float)((rand() % 10) - 5));
		}

		for (int i = 0; i < NUM_PARTICLES; i++)
		{
			for (int j = -2; j < 3; j++)
			{
				if (i + j >= 0 && i + j < NUM_PARTICLES && j != 0)
				{
					connexions.push_back(std::pair<int, int>{i, i + j});

				}
			}			
		}
	}
	
};

struct ForceActuator { virtual glm::vec3 computeForce(float mass, const glm::vec3& position) = 0;};

struct GravityForce : ForceActuator
{
	glm::vec3 computeForce(float mass, const glm::vec3& position)
	{
		return glm::vec3(0, -9.81f, 0) * mass;
	}
};

struct WindForce : ForceActuator
{
	glm::vec3 computeForce(float mass, const glm::vec3& position)
	{
		return glm::vec3(2, 0, 2) * mass;
	}
};

glm::vec3 springforce(const glm::vec3& P1, const glm::vec3& V1, const glm::vec3& P2, const glm::vec3& V2, float L0, float ke, float kd);

glm::vec3 computeForces(FiberStraw& fiber, int idx, const std::vector<ForceActuator*>& force_acts);

struct Collider {
	virtual bool checkCollision(const glm::vec3& prev_pos,	const glm::vec3& next_pos) = 0;
	virtual void getPlane(glm::vec3& normal, float& d) = 0;
	void computeCollision(glm::vec3& old_pos, glm::vec3& new_pos) {
		//...
	}
};
struct PlaneCol : Collider {
	//...
};
struct SphereCol : Collider {
	//...
};

void verlet(float dt, FiberStraw& fiber, const std::vector<Collider*>& colliders, const std::vector<ForceActuator*>& force_acts);