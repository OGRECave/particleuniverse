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

#include "ParticleObservers/ParticleUniverseOnVelocityObserver.h"

namespace ParticleUniverse
{
	// Constants
	const Real OnVelocityObserver::DEFAULT_VELOCITY_THRESHOLD = 0.0f;

	//-----------------------------------------------------------------------
	OnVelocityObserver::OnVelocityObserver(void) : 
		ParticleObserver(),
		mThreshold(DEFAULT_VELOCITY_THRESHOLD),
		mCompare(CO_LESS_THAN)
	{
	};
	//-----------------------------------------------------------------------
	bool OnVelocityObserver::_observe (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		if (!particle)
			return false;

		// Compensate for the scaled velocity
		Real scaleVelocity = particleTechnique->getParentSystem()->getScaleVelocity();
		if (mCompare == CO_GREATER_THAN)
		{
			// Changed in V 1.3.1
			return (particle->calculateVelocity()) > (scaleVelocity * mThreshold);
		}
		else if (mCompare == CO_LESS_THAN)
		{
			return (particle->calculateVelocity()) < (scaleVelocity * mThreshold);
		}
		else
		{
			// Equals
			return almostEquals(particle->calculateVelocity(), (scaleVelocity * mThreshold), 0.01f);
		}

		return false;
	}
	//-----------------------------------------------------------------------
	void OnVelocityObserver::copyAttributesTo(ParticleObserver* observer)
	{
		ParticleObserver::copyAttributesTo(observer);

		OnVelocityObserver* onVelocityObserver = static_cast<OnVelocityObserver*>(observer);
		onVelocityObserver->mThreshold = mThreshold;
		onVelocityObserver->mCompare = mCompare;
	}
}
