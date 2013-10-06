/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2013 Henry van Merode. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniversePCH.h"

#ifndef PARTICLE_UNIVERSE_EXPORTS
#define PARTICLE_UNIVERSE_EXPORTS
#endif

#include "ParticleAffectors/ParticleUniverseInterParticleCollider.h"

namespace ParticleUniverse
{
	// Constants
	const Real InterParticleCollider::DEFAULT_ADJUSTMENT = 1.0f;
	const InterParticleCollider::InterParticleCollisionResponse InterParticleCollider::DEFAULT_COLLISION_RESPONSE = 
		InterParticleCollider::IPCR_AVERAGE_VELOCITY;

	//-----------------------------------------------------------------------
	InterParticleCollider::InterParticleCollider(void) : 
		BaseCollider(),
		mAdjustment(DEFAULT_ADJUSTMENT),
		mInterParticleCollisionResponse(DEFAULT_COLLISION_RESPONSE)
	{
	}
	//-----------------------------------------------------------------------
	Real InterParticleCollider::getAdjustment(void) const
	{
		return mAdjustment;
	}
	//-----------------------------------------------------------------------
	void InterParticleCollider::setAdjustment(Real adjustment)
	{
		mAdjustment = adjustment;
	}
	//-----------------------------------------------------------------------
	InterParticleCollider::InterParticleCollisionResponse InterParticleCollider::getInterParticleCollisionResponse(void) const
	{
		return mInterParticleCollisionResponse;
	}
	//-----------------------------------------------------------------------
	void InterParticleCollider::setInterParticleCollisionResponse(InterParticleCollider::InterParticleCollisionResponse interParticleCollisionResponse)
	{
		mInterParticleCollisionResponse = interParticleCollisionResponse;
	}
	//-----------------------------------------------------------------------
	void InterParticleCollider::_prepare(ParticleTechnique* particleTechnique)
	{
		// Activate spatial hashing
		particleTechnique->setSpatialHashingUsed(true);
	}
	//-----------------------------------------------------------------------
	void InterParticleCollider::_unprepare(ParticleTechnique* particleTechnique)
	{
		// Deactivate spatial hashing
		particleTechnique->setSpatialHashingUsed(false);
	}
	//-----------------------------------------------------------------------
	void InterParticleCollider::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		// Fast rejection: only visible, moving particles are able to collide, unless they are colliding already
		// Changed && into || in V1.3.1
		if (particle->particleType != Particle::PT_VISUAL || 
			particle->hasEventFlags(PEF_COLLIDED) || 
			particle->direction == Vector3::ZERO)
		{
			return;
		}

		// Determine whether neighbour particles are colliding.
		SpatialHashTable<Particle*>* hashtable = particleTechnique->getSpatialHashTable();
		if (hashtable)
		{
			SpatialHashTable<Particle*>::HashTableCell cell = hashtable->getCell(particle->position);
			if (cell.empty())
				return;

			unsigned int size = static_cast<unsigned int>(cell.size());
			for (unsigned int i = 0; i < size; ++i)
			{
				Particle* p = cell[i];

				// Don't check if it is the same particle or the particle is already colliding.
				if (particle != p  && !p->hasEventFlags(PEF_COLLIDED))
				{
					// Check for collision
					if (_validateAndExecuteSphereCollision(particle, p, timeElapsed))
					{
						return;
					}
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	bool InterParticleCollider::_validateAndExecuteSphereCollision (Particle* particle1, Particle* particle2, Real timeElapsed)
	{
		VisualParticle* vp1 = static_cast<VisualParticle*>(particle1);
		VisualParticle* vp2 = static_cast<VisualParticle*>(particle2);
		if ((vp1->position - vp2->position).length() < mAdjustment * (vp1->radius + vp2->radius))
		{
			/** Collision detected.
			@remarks
				The collision response calculation isn't accurate, but gives an acceptable result.
			*/
			Vector3 n = vp1->position - vp2->position;
			n.normalise();
			switch(mInterParticleCollisionResponse)
			{
				case IPCR_AVERAGE_VELOCITY:
				{
					// Use average velocity; this keeps the particles in movement.
					Real velocity1 = vp1->direction.length();
					Real velocity2 = vp2->direction.length();
					Real averageVelocity = 0.5f * (velocity1 + velocity2);
					vp1->direction = averageVelocity * vp2->mass * n;
					vp2->direction = averageVelocity * vp1->mass * -n;
				}
				break;

				case IPCR_ANGLE_BASED_VELOCITY:
				{
					// The new velocity is based on the angle between original direction and new direction.
					// Note, that this usually means that the velocity decreases.
					Real velocity1 = Math::Abs((vp1->direction).dotProduct(n));
					Real velocity2 = Math::Abs((vp2->direction).dotProduct(n));
					vp1->direction = velocity1 * vp2->mass * n;
					vp2->direction = velocity2 * vp1->mass * -n;
				}
				break;
			}
			vp1->direction *= mBouncyness;
			vp2->direction *= mBouncyness;
			vp1->addEventFlags(Particle::PEF_COLLIDED);
			vp2->addEventFlags(Particle::PEF_COLLIDED);
			return true;
		}

		return false;
	}
	//-----------------------------------------------------------------------
	void InterParticleCollider::copyAttributesTo (ParticleAffector* affector)
	{
		BaseCollider::copyAttributesTo(affector);
		InterParticleCollider* interParticleCollider = static_cast<InterParticleCollider*>(affector);
		interParticleCollider->mAdjustment = mAdjustment;
		interParticleCollider->mInterParticleCollisionResponse = mInterParticleCollisionResponse;
	}
}
