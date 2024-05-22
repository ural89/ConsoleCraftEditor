#pragma once
#include "ParticleObject.h"
#include "Core/../Core.h"
class GE_API BlastParticle : public ParticleObject
{
public:
	BlastParticle(Scene& scene, int particleType, class Vector2 startVelocity) :
		ParticleObject(scene,  particleType) {
		this->velocity = startVelocity;
	};
	void Update(float deltaTime) override;
	void OnCollidedBorder(int border) override;
private:
	Vector2 velocity;
};

