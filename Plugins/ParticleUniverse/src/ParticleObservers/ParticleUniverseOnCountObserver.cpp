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

#include "ParticleObservers/ParticleUniverseOnCountObserver.h"

namespace ParticleUniverse
{
	// Constants
	const uint OnCountObserver::DEFAULT_THRESHOLD = 0;
	const ComparisionOperator OnCountObserver::DEFAULT_COMPARE = CO_LESS_THAN;

	//-----------------------------------------------------------------------
	OnCountObserver::OnCountObserver(void) : ParticleObserver(),
		mCount(1),
		mThreshold(DEFAULT_THRESHOLD),
		mCompare(DEFAULT_COMPARE)
	{
	}
	//-----------------------------------------------------------------------
	void OnCountObserver::_notifyStart (void)
	{
		ParticleObserver::_notifyStart();

		// At a restart the counter must be reset again.
		mCount = 1;
	}
	//-----------------------------------------------------------------------
	bool OnCountObserver::_observe (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		if (!particle)
			return false;

		if (!particle->hasEventFlags(Particle::PEF_EMITTED))
			return false;
		
		if (mCompare == CO_GREATER_THAN)
		{
			if (mCount > mThreshold)
			{
				return true;
			}
			else
			{
				mCount++;
				return false;
			}
		}
		else if (mCompare == CO_LESS_THAN)
		{
			if (mCount < mThreshold)
			{
				mCount++;
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (mCount == mThreshold)
			{
				mCount++;
				return true;
			}
			else
			{
				mCount++;
				return false;
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	void OnCountObserver::copyAttributesTo(ParticleObserver* observer)
	{
		ParticleObserver::copyAttributesTo(observer);

		OnCountObserver* onCountObserver = static_cast<OnCountObserver*>(observer);
		onCountObserver->mCount = mCount;
		onCountObserver->mThreshold = mThreshold;
		onCountObserver->mCompare = mCompare;
	}
}
