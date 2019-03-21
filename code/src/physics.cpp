#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>
#include <glm\gtc\matrix_transform.hpp>
#include "..\FiberStraw.h"

namespace Box {
	void drawCube();
}
namespace Axis {
	void drawAxis();
}

namespace Sphere {
	extern void updateSphere(glm::vec3 pos, float radius = 1.f);
	extern void drawSphere();
}
namespace Capsule {
	extern void updateCapsule(glm::vec3 posA, glm::vec3 posB, float radius = 1.f);
	extern void drawCapsule();
}
namespace Particles {
	extern const int maxParticles;
	extern void updateParticles(int startIdx, int count, float* array_data);
	extern void drawParticles(int startIdx, int count);
}
namespace Mesh {
	extern const int numCols;
	extern const int numRows;
	extern void updateMesh(float* array_data);
	extern void drawMesh();
}
namespace Fiber {
extern const int numVerts;
	extern void updateFiber(float* array_data);
	extern void drawFiber();
}
namespace Cube {
	extern void updateCube(const glm::mat4& transform);
	extern void drawCube();
}



// Boolean variables allow to show/hide the primitives
bool renderSphere = false;
bool renderCapsule = false;
bool renderParticles = false;
bool renderMesh = false;
bool renderFiber = true;
bool renderCube = false;

FiberStraw fiberS[100];

//You may have to change this code
void renderPrims() {
	Box::drawCube();
	Axis::drawAxis();


	if (renderSphere)
		Sphere::drawSphere();
	if (renderCapsule)
		Capsule::drawCapsule();

	if (renderParticles) {
		int startDrawingFromParticle = 0;
		int numParticlesToDraw = Particles::maxParticles;
		Particles::drawParticles(startDrawingFromParticle, numParticlesToDraw);
	}

	if (renderMesh)
		Mesh::drawMesh();
	if (renderFiber)
	{
		for (int i = 0; i < 100; i++)
		{
			Fiber::drawFiber();
			Fiber::updateFiber(&fiberS[i].positions[0].x);
		}
	}

	if (renderCube)
		Cube::drawCube();
}

float restLength = 1;
float mass = 0.1f;
float ke = 5.f;
float kd = 0.3f;
float bounceCoefficient = 0.5;

std::vector<ForceActuator*> forces;

void GUI() {
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);

	// Do your GUI code here....
	{	
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//FrameRate
		ImGui::DragFloat("Rest Length", &restLength, 0.01f, 0.0f, 3.f);
		ImGui::DragFloat("Mass", &mass, 0.01f, 0.1f, 10.f);
		ImGui::DragFloat("Ke", &ke, 0.01f, 0.1f, 10.f);
		ImGui::DragFloat("Kd", &kd, 0.01f, 0.1f, 10.f);
	}
	// .........................
	
	ImGui::End();

	// Example code -- ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	bool show_test_window = false;
	if(show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

void PhysicsInit() {
	
	forces.push_back(new WindForce());
	forces.push_back(new GravityForce());
}

void PhysicsUpdate(float dt) {
	
	for (int i = 0; i < 100; i++)
	{
		verlet(dt, fiberS[i], forces);
		Fiber::updateFiber(&fiberS[i].positions[0].x);
	}
	
}

void PhysicsCleanup() {
	// Do your cleanup code here...
	// ............................
}