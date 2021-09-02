
#include "ParticleSystem.h"
#include <glm/gtx/compatibility.hpp>

#include "tepch.h"

ParticleSystem::ParticleSystem()
	: m_PoolIndex(999)
{
	m_ParticlesPool = std::vector<Particle>(m_PoolIndex + 1);
}

ParticleSystem::ParticleSystem(int poolSize)
	: m_PoolIndex(poolSize)
{
	m_ParticlesPool = std::vector<Particle>(m_PoolIndex + 1);
}

void ParticleSystem::OnUpdate(TituEngine::Timestep ts)
{
	for (size_t i = 0; i < m_ParticlesPool.size(); i++)
	{
		Particle& particle = m_ParticlesPool[i];
		if (!particle.active)
			continue;

		particle.timeRemaining -= ts;
		if (particle.timeRemaining <= 0.0f)
		{
			particle.timeRemaining = 0.0f;
			particle.active = false;
			continue;
		}

		float lifetimeNorm = particle.timeRemaining / particle.lifeTime;
		particle.position += particle.velocity * (float)ts;
		particle.rotation += particle.angularSpeed * ts;
		particle.colorCurrent = glm::lerp(particle.colorEnd, particle.colorStart, lifetimeNorm);
		particle.sizeCurrent = glm::lerp(particle.sizeEnd, particle.sizeStart, lifetimeNorm);
	}
}

void ParticleSystem::OnRender()
{
	for (size_t i = 0; i < m_ParticlesPool.size(); i++)
	{
		Particle particle = m_ParticlesPool[i];
		if (!particle.active)
			continue;

		TituEngine::Renderer2D::DrawQuad(particle.position, particle.rotation, particle.sizeCurrent, particle.colorCurrent);
	}
}

void ParticleSystem::OnRender(TituEngine::SubTexture2D* textures[], uint32_t size)
{
	for (size_t i = 0; i < m_ParticlesPool.size(); i++)
	{
		Particle particle = m_ParticlesPool[i];
		if (!particle.active)
			continue;

		int pI = i % size;
		TituEngine::Renderer2D::DrawQuad({ particle.position.x, particle.position.y, -0.5f}, particle.rotation, particle.sizeCurrent, { 1.0f, 1.0f, 1.0f, 1.0f }, textures[pI]);
		TituEngine::Renderer2D::Flush();
	}
}

void ParticleSystem::Emit(const ParticleProperties& particleProps)
{
	Particle& particle = m_ParticlesPool[m_PoolIndex];
	if (m_PoolIndex == 0) m_PoolIndex = (uint32_t)m_ParticlesPool.size() - 1;
	else --m_PoolIndex;
	particle.position = particleProps.posStart;
	particle.velocity = particleProps.velocity;
	particle.active = true;
	particle.colorCurrent = particleProps.colorStart;
	particle.colorStart = particleProps.colorStart;
	particle.colorEnd = particleProps.colorEnd;
	particle.sizeCurrent = particleProps.sizeStart;
	particle.sizeStart = particleProps.sizeStart;
	particle.sizeEnd = particleProps.sizeEnd;
	particle.lifeTime = particleProps.lifeTime;
	particle.timeRemaining = particleProps.lifeTime;
	particle.angularSpeed = particleProps.rotationSpeed;
	particle.rotation = 0.0f;
}
