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

#include "ParticleAffectors/ParticleUniversePathFollower.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	void PathFollower::copyAttributesTo (ParticleAffector* affector)
	{
		ParticleAffector::copyAttributesTo(affector);
		PathFollower* pathFollower = static_cast<PathFollower*>(affector);
		unsigned short numPoints = mSpline.getNumPoints();
		pathFollower->mSpline.clear();
		for (unsigned short i = 0; i < numPoints; ++i)
		{
			pathFollower->mSpline.addPoint(mSpline.getPoint(i));
		}
	}
	//-----------------------------------------------------------------------
	void PathFollower::addPoint(const Vector3& point)
	{
		mSpline.addPoint(point);
	}
	//-----------------------------------------------------------------------
	void PathFollower::clearPoints (void)
	{
		mSpline.clear();
	}
	//-----------------------------------------------------------------------
	void PathFollower::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		if (mSpline.getNumPoints() > 0)
		{
			// PU 1.4
			Real timeLeft = particle->totalTimeToLive - particle->timeToLive;
			Real timeFraction = timeLeft / particle->totalTimeToLive;
			Ogre::Real timeFractionPlusDelta = (timeLeft + timeElapsed ) / particle->totalTimeToLive;
			timeFractionPlusDelta = timeFractionPlusDelta < 1.0f ? timeFractionPlusDelta : 1.0f;
			particle->position += _mAffectorScale * (mSpline.interpolate(timeFractionPlusDelta) - mSpline.interpolate(timeFraction));
		}
	}
}
