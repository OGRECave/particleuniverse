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

#include "ParticleObservers/ParticleUniverseOnTimeObserver.h"

namespace ParticleUniverse
{
	// Constants
	const Real OnTimeObserver::DEFAULT_THRESHOLD = 0.0f;
	const bool OnTimeObserver::DEFAULT_SINCE_START_SYSTEM = false;

	//-----------------------------------------------------------------------
	OnTimeObserver::OnTimeObserver(void) : ParticleObserver(),
		mThreshold(DEFAULT_THRESHOLD),
		mCompare(CO_GREATER_THAN),
		mSinceStartSystem(DEFAULT_SINCE_START_SYSTEM)
	{
	};
	//-----------------------------------------------------------------------
	void OnTimeObserver::_preProcessParticles(ParticleTechnique* technique, Real timeElapsed)
	{
		// Call parent
		ParticleObserver::_preProcessParticles(technique, timeElapsed);

		// Also observe if there are no particles emitted, because some of the event handlers do not only
		// perform an action on a particle.
		if (technique->getNumberOfEmittedParticles() == 0)
		{
			_handleObserve(technique, 0, timeElapsed);
		}
	}
	//-----------------------------------------------------------------------
	bool OnTimeObserver::_observe (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		if (mCompare == CO_GREATER_THAN)
		{
			if (mSinceStartSystem)
			{
				// Validate whether time since start of the particle system > threshold
				return (mParentTechnique->getParentSystem()->getTimeElapsedSinceStart() > mThreshold);
			}
			else
			{
				// Validate whether time since start of the particle emission > threshold
				return (particle && (particle->totalTimeToLive - particle->timeToLive) > mThreshold);
			}
		}
		else if (mCompare == CO_LESS_THAN)
		{
			if (mSinceStartSystem)
			{
				// Validate whether time since start of the particle system < threshold
				return (mParentTechnique->getParentSystem()->getTimeElapsedSinceStart() < mThreshold);
			}
			else
			{
				// Validate whether time since start of the particle emission < threshold
				return (particle && (particle->totalTimeToLive - particle->timeToLive) < mThreshold);
			}
		}
		else
		{
			// Equals
			if (mSinceStartSystem)
			{
				// Validate whether time since start of the particle system == threshold
				return almostEquals(mParentTechnique->getParentSystem()->getTimeElapsedSinceStart(), mThreshold, 0.01f);
			}
			else
			{
				// Validate whether time since start of the particle emission == threshold
				return particle && almostEquals((particle->totalTimeToLive - particle->timeToLive), mThreshold, 0.01f);
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	void OnTimeObserver::copyAttributesTo(ParticleObserver* observer)
	{
		ParticleObserver::copyAttributesTo(observer);

		OnTimeObserver* onTimeObserver = static_cast<OnTimeObserver*>(observer);
		onTimeObserver->mThreshold = mThreshold;
		onTimeObserver->mCompare = mCompare;
		onTimeObserver->mSinceStartSystem = mSinceStartSystem;
	}
}
