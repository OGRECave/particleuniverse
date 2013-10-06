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

#include "ParticleObservers/ParticleUniverseOnQuotaObserver.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool OnQuotaObserver::_observe (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		return mResult;
	}
	//-----------------------------------------------------------------------
	void OnQuotaObserver::_postProcessParticles(ParticleTechnique* particleTechnique, Real timeElapsed)
	{
		mResult = false;
		size_t quota = 0;
		if (mParticleTypeToObserveSet)
		{
			// Type to observe is set, so validate only that one
			switch (mParticleTypeToObserve)
			{
				case Particle::PT_VISUAL:
					quota = particleTechnique->getVisualParticleQuota();
				break;
				case Particle::PT_EMITTER:
					quota = particleTechnique->getEmittedEmitterQuota();
				break;
				case Particle::PT_AFFECTOR:
					quota = particleTechnique->getEmittedAffectorQuota();
				break;
				case Particle::PT_TECHNIQUE:
					quota = particleTechnique->getEmittedTechniqueQuota();
				break;
				case Particle::PT_SYSTEM:
					quota = particleTechnique->getEmittedAffectorQuota();
				break;
			}

			mResult = particleTechnique->getNumberOfEmittedParticles(mParticleTypeToObserve) >= quota;
		}
		else
		{
			// Type to observe is not set, so check them all
			quota = particleTechnique->getVisualParticleQuota() + 
				particleTechnique->getEmittedEmitterQuota() + 
				particleTechnique->getEmittedTechniqueQuota() + 
				particleTechnique->getEmittedAffectorQuota() +
				particleTechnique->getEmittedSystemQuota();
			mResult = particleTechnique->getNumberOfEmittedParticles() >= quota;
		}
	}
}
