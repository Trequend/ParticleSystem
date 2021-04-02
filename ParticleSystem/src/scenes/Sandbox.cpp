#include "Sandbox.hpp"

#include <iostream>

#include "../objects/ParticleSystemCPU.hpp"

void Sandbox::Initialize()
{
	AddObject(new ParticleSystemCPU("Particle system", 5000, 5, {
		15.0f,                       // lifespan
		0.0f,                        // lifespan variation
		glm::vec3(0.0f),             // position
		glm::vec3(0.2f),             // position variation
		glm::vec3(1.0f, 1.0f, 0.0f), // velocity
		glm::vec3(1.5f, 1.5f, 0.0f), // velocity variation
		glm::vec3(0.0f),             // rotation
		glm::vec3(0.0f),             // rotation variation
		glm::vec3(0.0f),             // angular velocity
		glm::vec3(0.0f),             // angular velocity variation
		glm::vec4(1.0f),             // color begin
		glm::vec4(0.0f),             // color end
		1.0f,                        // size begin
		0.0f,                        // size end
		0.0f                         // size variation
	}));
	std::cout << "Initialize" << std::endl;
}

void Sandbox::OnDestroy()
{
	std::cout << "Destroy" << std::endl;
}
