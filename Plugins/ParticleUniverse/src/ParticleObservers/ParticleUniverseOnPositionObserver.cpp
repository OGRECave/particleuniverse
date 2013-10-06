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

#include "ParticleObservers/ParticleUniverseOnPositionObserver.h"

namespace ParticleUniverse
{
	// Constants
	const Vector3 OnPositionObserver::DEFAULT_POSITION_THRESHOLD(0, 0, 0);

	//-----------------------------------------------------------------------
	OnPositionObserver::OnPositionObserver(void) : 
		ParticleObserver(),
		mPositionXThreshold(DEFAULT_POSITION_THRESHOLD.x),
		mPositionYThreshold(DEFAULT_POSITION_THRESHOLD.y),
		mPositionZThreshold(DEFAULT_POSITION_THRESHOLD.z),
		mComparePositionX(CO_LESS_THAN),
		mComparePositionY(CO_LESS_THAN),
		mComparePositionZ(CO_LESS_THAN),
		mPositionXThresholdSet(false),
		mPositionYThresholdSet(false),
		mPositionZThresholdSet(false)
	{
	}
	//-----------------------------------------------------------------------
	bool OnPositionObserver::_observe (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		if (!particle)
			return false;

		// Added scale in V1.3.1
		if (mPositionXThresholdSet && mComparePositionX == CO_LESS_THAN && particle->position.x < mPositionXThreshold * _mObserverScale.x)
			return true;
		if (mPositionXThresholdSet && mComparePositionX == CO_GREATER_THAN && particle->position.x > mPositionXThreshold * _mObserverScale.x)
			return true;
		if (mPositionXThresholdSet && mComparePositionX == CO_EQUALS && particle->position.x == mPositionXThreshold * _mObserverScale.x)
			return true;

		if (mPositionYThresholdSet && mComparePositionY == CO_LESS_THAN && particle->position.y < mPositionYThreshold * _mObserverScale.y)
			return true;
		if (mPositionYThresholdSet && mComparePositionY == CO_GREATER_THAN && particle->position.y > mPositionYThreshold * _mObserverScale.y)
			return true;
		if (mPositionYThresholdSet && mComparePositionY == CO_EQUALS && particle->position.y == mPositionYThreshold * _mObserverScale.y)
			return true;

		if (mPositionZThresholdSet && mComparePositionZ == CO_LESS_THAN && particle->position.z < mPositionZThreshold * _mObserverScale.z)
			return true;
		if (mPositionZThresholdSet && mComparePositionZ == CO_GREATER_THAN && particle->position.z > mPositionZThreshold * _mObserverScale.z)
			return true;
		if (mPositionZThresholdSet && mComparePositionZ == CO_EQUALS && particle->position.z == mPositionZThreshold * _mObserverScale.z)
			return true;

		return false;
	}
	//-----------------------------------------------------------------------
	void OnPositionObserver::copyAttributesTo(ParticleObserver* observer)
	{
		ParticleObserver::copyAttributesTo(observer);

		OnPositionObserver* onPositionObserver = static_cast<OnPositionObserver*>(observer);
		onPositionObserver->mPositionXThreshold = mPositionXThreshold;
		onPositionObserver->mPositionYThreshold = mPositionYThreshold;
		onPositionObserver->mPositionZThreshold = mPositionZThreshold;
		onPositionObserver->mComparePositionX = mComparePositionX;
		onPositionObserver->mComparePositionY = mComparePositionY;
		onPositionObserver->mComparePositionZ = mComparePositionZ;
		onPositionObserver->mPositionXThresholdSet = mPositionXThresholdSet;
		onPositionObserver->mPositionYThresholdSet = mPositionYThresholdSet;
		onPositionObserver->mPositionZThresholdSet = mPositionZThresholdSet;
	}

}
