#include "Sandbox.hpp"

#include <iostream>

#include "../objects/ParticleSystemCPU.hpp"

void Sandbox::Initialize()
{
	AddObject(new ParticleSystemCPU("Particle system", 5000, 5, {
		7.0f,                                                      // lifespan
		0.0f,                                                      // lifespan variation
		glm::vec3(0.0f),                                           // position
		glm::vec3(0.0f, 0.5f, 0.0f),                               // position variation
		glm::vec3(1.0f, 1.0f, 0.0f),                               // velocity
		glm::vec3(1.0f, 1.0f, 0.0f),                               // velocity variation
		glm::vec3(0.0f),                                           // rotation
		glm::vec3(0.0f),                                           // rotation variation
		glm::vec3(0.0f),                                           // angular velocity
		glm::vec3(0.0f, 0.0f, 15.0f),                              // angular velocity variation
		glm::vec4(205.0f / 255, 169.0f / 255, 250.0f / 255, 1.0f), // color begin
		glm::vec4(71.0f / 255, 169.0f / 255, 1.0f, 1.0f),          // color end
		0.5f,                                                      // size begin
		0.0f,                                                      // size end
		0.0f                                                       // size variation
	}));

	Camera& camera = GetCamera();
	camera.SetTranslation({ 8.0f, 8.0f, 0.0f });
	camera.SetOrhographicProperties(2);

	std::cout << "Initialize" << std::endl;
}

void Sandbox::OnDestroy()
{
	std::cout << "Destroy" << std::endl;
}
