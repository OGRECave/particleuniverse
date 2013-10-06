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

#include "ParticleAffectors/ParticleUniverseParticleFollower.h"

namespace ParticleUniverse
{
	// Constants
	const Real ParticleFollower::DEFAULT_MAX_DISTANCE = 3.40282e+038f;
	const Real ParticleFollower::DEFAULT_MIN_DISTANCE = 10.0f;

	//-----------------------------------------------------------------------
	ParticleFollower::ParticleFollower(void) : 
		ParticleAffector(),
		mMinDistance(DEFAULT_MIN_DISTANCE),
		mMaxDistance(DEFAULT_MAX_DISTANCE),
		mPositionPreviousParticle(Vector3::ZERO),
		mFirst(false)
	{
	}
	//-----------------------------------------------------------------------
	void ParticleFollower::copyAttributesTo (ParticleAffector* affector)
	{
		ParticleAffector::copyAttributesTo(affector);

		ParticleFollower* particleFollower = static_cast<ParticleFollower*>(affector);
		particleFollower->mMaxDistance = mMaxDistance;
		particleFollower->mMinDistance = mMinDistance;
	}
	//-----------------------------------------------------------------------
	void ParticleFollower::_firstParticle(ParticleTechnique* particleTechnique, 
		Particle* particle, 
		Real timeElapsed)
	{
		// The first particle should go its own way
		mFirst = true;
	}
	//-----------------------------------------------------------------------
	void ParticleFollower::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		if (!mFirst)
		{
			// Change in V 1.3.1
			// Only proceed if it isn´t the first one. Compensate for scaling.
//			Real distance = (particle->position / _mAffectorScale).distance(mPositionPreviousParticle / _mAffectorScale);
			Real distance = (particle->position).distance(mPositionPreviousParticle);
			Real avgScale = 0.3333f * (_mAffectorScale.x + _mAffectorScale.y + _mAffectorScale.z);
			Real scaledMinDistance = avgScale * mMinDistance;
			if (distance > scaledMinDistance && distance < avgScale * mMaxDistance)
			{
				// This particle drifts too much from the previous one; correct it!
				Real f = scaledMinDistance/distance;
				particle->position = mPositionPreviousParticle + f * (particle->position - mPositionPreviousParticle);
			}
		}

		mPositionPreviousParticle = particle->position;
		mFirst = false;
	}
	//-----------------------------------------------------------------------
	Real ParticleFollower::getMaxDistance(void) const
	{
		return mMaxDistance;
	}
	//-----------------------------------------------------------------------
	void ParticleFollower::setMaxDistance(Real maxDistance)
	{
		mMaxDistance = maxDistance;
	}
	//-----------------------------------------------------------------------
	Real ParticleFollower::getMinDistance(void) const
	{
		return mMinDistance;
	}
	//-----------------------------------------------------------------------
	void ParticleFollower::setMinDistance(Real minDistance)
	{
		mMinDistance = minDistance;
	}
}
