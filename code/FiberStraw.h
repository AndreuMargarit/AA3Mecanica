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
extern float restLength;
extern float mass;
extern float ke;
extern float kd;
extern float bounceCoefficient;


glm::vec3 getNormal(const glm::vec3& vector1, const glm::vec3& vector2) {

	glm::vec3 normal;

	normal.x = vector1.y * vector2.z + vector2.y * vector1.z;
	normal.y = -(vector1.x * vector2.z + vector2.x * vector1.z);
	normal.z = vector1.x * vector2.y + vector2.x * vector1.y;

	return normal;
}

float getD(glm::vec3 normal, glm::vec3 point) {
	return -(normal.x * point.x + normal.y * point.y + normal.z * point.z);
}


struct FiberStraw
{
	glm::vec3 lastPositions[5];
	glm::vec3 positions[5];
	glm::vec3 velocities[5];
	std::vector<std::pair<int, int>> connexions;
	FiberStraw() {
		float x = (float)((rand() % 9000) - 4500) / 1000;
		float z = (float)((rand() % 9000) - 4500) / 1000;
		for (int i = 0; i < NUM_PARTICLES; i++) {
			positions[i] = glm::vec3(x, i - 1, z);
			lastPositions[i] = positions[i];
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
		glm::vec3 normal;
		float d;
		getPlane(normal, d);
		 
		new_pos = new_pos - (1 + bounceCoefficient) * (glm::dot(normal, new_pos) + d) * normal;
		old_pos = old_pos - (1+bounceCoefficient) * (glm::dot(normal, old_pos) + d) * normal;
		//Calcular el fregament (moure el  old_pos en direcció al vector del punt anterior i nou en el pla)
	}
};
struct PlaneCol : Collider {
	glm::vec3 point, vector1, vector2;
	
	PlaneCol(std::vector<glm::vec3> points) {
		point = points[0];
		vector1 = points[1] - points[0];
		vector2 = points[2] - points[0];
	}
	void getPlane(glm::vec3& normal, float& d)
	{
		normal = getNormal(vector1, vector2);
		glm::normalize(normal);
		d = getD(normal, point);
	}

	bool CheckCollision(const glm::vec3& prev_pos, const glm::vec3& next_pos)
	{
		glm::vec3 normal;
		float d;
		getPlane(normal, d);
		float value_prev_pos = normal.x * prev_pos.x + normal.y * prev_pos.y + normal.z * prev_pos.z + d;
		float value_next_pos = normal.x * next_pos.x + normal.y * next_pos.y + normal.z * next_pos.z + d;

		return(value_prev_pos <= 0 && value_next_pos > 0) || (value_prev_pos >= 0 && value_next_pos < 0);

	}

};
struct SphereCol : Collider {
	//...
};


void verlet(float dt, FiberStraw& fiber, /*const std::vector<Collider*>& colliders,*/ const std::vector<ForceActuator*>& force_acts);