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

#include "ParticleObservers/ParticleUniverseOnRandomObserver.h"

namespace ParticleUniverse
{
	// Constants
	const Real OnRandomObserver::DEFAULT_THRESHOLD = 0.5f;

	//-----------------------------------------------------------------------
	OnRandomObserver::OnRandomObserver(void) : 
		ParticleObserver(),
		mThreshold(DEFAULT_THRESHOLD)
	{
	}
	//-----------------------------------------------------------------------
	void OnRandomObserver::_preProcessParticles(ParticleTechnique* technique, Real timeElapsed)
	{
		// Call parent
		ParticleObserver::_preProcessParticles(technique, timeElapsed);

		// Handle the _observe here
		_handleObserve(technique, 0, timeElapsed);
	}
	//-----------------------------------------------------------------------
	void OnRandomObserver::_processParticle(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed, bool firstParticle)
	{
		// No need to determine per particle
	}
	//-----------------------------------------------------------------------
	bool OnRandomObserver::_observe (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		return (Math::UnitRandom() > mThreshold);
	}
	//-----------------------------------------------------------------------
	void OnRandomObserver::copyAttributesTo(ParticleObserver* observer)
	{
		ParticleObserver::copyAttributesTo(observer);
		OnRandomObserver* onRandomObserver = static_cast<OnRandomObserver*>(observer);
		onRandomObserver->mThreshold = mThreshold;
	}
}
