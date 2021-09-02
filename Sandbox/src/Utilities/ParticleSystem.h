#pragma once

#include <TituEngine.h>
#include <glm/vec2.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

struct ParticleProperties
{
	glm::vec2 posStart;
	glm::vec2 velocity;
	glm::vec4 colorStart, colorEnd;
	glm::vec2 sizeStart, sizeEnd;
	float rotationSpeed;
	float lifeTime;
};

class ParticleSystem
{
public:
	ParticleSystem();
	ParticleSystem(int poolSize);

	void OnUpdate(TituEngine::Timestep ts);
	void OnRender();
	void OnRender(TituEngine::SubTexture2D* textures[], uint32_t size);

	void Emit(const ParticleProperties& particleProps);

private:

	struct Particle
	{
		bool active;
		glm::vec2 velocity;
		glm::vec2 position;
		float rotation;
		float angularSpeed;
		glm::vec2 sizeCurrent, sizeStart, sizeEnd;
		glm::vec4 colorCurrent, colorStart, colorEnd;
		float timeRemaining, lifeTime;
	};

	std::vector<Particle> m_ParticlesPool;
	uint32_t m_PoolIndex;

};
