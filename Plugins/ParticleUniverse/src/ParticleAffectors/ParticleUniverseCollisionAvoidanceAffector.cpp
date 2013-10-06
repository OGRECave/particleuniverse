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

#include "ParticleAffectors/ParticleUniverseCollisionAvoidanceAffector.h"

namespace ParticleUniverse
{
	// Constants
	const Real CollisionAvoidanceAffector::DEFAULT_RADIUS = 100.0f;

	//-----------------------------------------------------------------------
	CollisionAvoidanceAffector::CollisionAvoidanceAffector(void) : 
		ParticleAffector(),
		mRadius(DEFAULT_RADIUS)
	{
	}
	//-----------------------------------------------------------------------
	Real CollisionAvoidanceAffector::getRadius(void) const
	{
		return mRadius;
	}
	//-----------------------------------------------------------------------
	void CollisionAvoidanceAffector::setRadius(Real radius)
	{
		mRadius = radius;
	}
	//-----------------------------------------------------------------------
	void CollisionAvoidanceAffector::_prepare(ParticleTechnique* particleTechnique)
	{
		// Activate spatial hashing
		particleTechnique->setSpatialHashingUsed(true);
	}
	//-----------------------------------------------------------------------
	void CollisionAvoidanceAffector::_unprepare(ParticleTechnique* particleTechnique)
	{
		// Deactivate spatial hashing
		particleTechnique->setSpatialHashingUsed(false);
	}
	//-----------------------------------------------------------------------
	void CollisionAvoidanceAffector::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		// Determine neighbouring particles.
		SpatialHashTable<Particle*>* hashtable = particleTechnique->getSpatialHashTable();
		if (hashtable)
		{
			SpatialHashTable<Particle*>::HashTableCell cell = hashtable->getCell(particle->position);
			if (cell.empty())
				return;

			unsigned int size = static_cast<unsigned int>(cell.size());
			Vector3 displacement = Vector3::ZERO;
			Vector3 diff = Vector3::ZERO;
			for (unsigned int i = 0; i < size; ++i)
			{
				Particle* p = cell[i];

				// Don't check if it is the same particle
				if (particle != p)
				{
					// Validate whether the neighbouring particle is within range
					diff = p->position - particle->position;
					if (diff.length() < mRadius)
					{
						displacement -= diff;
					}
				}
			}
			particle->direction += displacement * timeElapsed;
		}
	}
	//-----------------------------------------------------------------------
	void CollisionAvoidanceAffector::copyAttributesTo (ParticleAffector* affector)
	{
		ParticleAffector::copyAttributesTo(affector);
		CollisionAvoidanceAffector* collisionAvoidanceAffector = static_cast<CollisionAvoidanceAffector*>(affector);
		collisionAvoidanceAffector->mRadius = mRadius;
	}
}
