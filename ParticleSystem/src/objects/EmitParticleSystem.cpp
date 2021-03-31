#include "EmitParticleSystem.hpp"

#include <imgui.h>

#include "ParticleSystemException.hpp"

EmitParticleSystem::EmitParticleSystem(
	const std::string& name,
	unsigned int particlesPerFrame,
	ParticleProperties particleProperties
) : SceneObject(name), particleProperties(particleProperties), particlesPerFrame(particlesPerFrame) { }

void EmitParticleSystem::Update()
{
	Compute();
	Emit(particlesPerFrame);
}

void EmitParticleSystem::UI()
{
	SceneObject::UI();

	ImGui::DragFloat("Lifespan", &particleProperties.lifespan, 0.2f, 0.2f);
	ImGui::DragFloat("Lifespan variation", &particleProperties.lifespanVariation, 0.2f, 0.0f);
	ImGui::DragFloat3("Position", &particleProperties.position[0]);
	ImGui::DragFloat3("Position variation", &particleProperties.positionVariation[0]);
	ImGui::DragFloat3("Velocity", &particleProperties.velocity[0]);
	ImGui::DragFloat3("Velocity variation", &particleProperties.velocityVariation[0]);
	ImGui::DragFloat3("Rotation", &particleProperties.rotation[0]);
	ImGui::DragFloat3("Rotation variation", &particleProperties.rotationVariation[0]);
	ImGui::DragFloat3("Angular velocity", &particleProperties.angularVelocity[0]);
	ImGui::DragFloat3("Angular velocity variation", &particleProperties.angularVelocityVariation[0]);
	ImGui::DragFloat4("Color begin", &particleProperties.colorBegin[0]);
	ImGui::DragFloat4("Color end", &particleProperties.colorEnd[0]);
	ImGui::DragFloat2("Size begin", &particleProperties.sizeBegin[0]);
	ImGui::DragFloat2("Size end", &particleProperties.sizeEnd[0]);
	ImGui::DragFloat2("Size variation", &particleProperties.sizeVariation[0]);
}
