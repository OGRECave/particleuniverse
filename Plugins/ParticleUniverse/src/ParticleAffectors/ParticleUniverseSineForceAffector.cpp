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

#include "ParticleAffectors/ParticleUniverseSineForceAffector.h"

namespace ParticleUniverse
{
	// Constants
	const Real SineForceAffector::DEFAULT_FREQ_MIN = 1.0f;
	const Real SineForceAffector::DEFAULT_FREQ_MAX = 1.0f;

	//-----------------------------------------------------------------------
	SineForceAffector::SineForceAffector(void) : 
		BaseForceAffector(),
		mAngle(361),
		mFrequencyMin(DEFAULT_FREQ_MIN),
		mFrequencyMax(DEFAULT_FREQ_MAX),
		mFrequency(1.0f)
	{
	}
	//-----------------------------------------------------------------------
	void SineForceAffector::copyAttributesTo (ParticleAffector* affector)
	{
		BaseForceAffector::copyAttributesTo(affector);

		SineForceAffector* sineForceAffector = static_cast<SineForceAffector*>(affector);
		sineForceAffector->mFrequencyMin = mFrequencyMin;
		sineForceAffector->mFrequencyMax = mFrequencyMax;
		sineForceAffector->mFrequency = mFrequency;
		sineForceAffector->mAngle = mAngle;
	}
	//-----------------------------------------------------------------------
	void SineForceAffector::_preProcessParticles(ParticleTechnique* particleTechnique, Real timeElapsed)
	{
		// Scale by time
		mAngle += mFrequency * timeElapsed;
		Real sineValue = Math::Sin(Radian(mAngle));
		mScaledVector = mForceVector * timeElapsed * sineValue;

		if (mAngle > Math::TWO_PI)
		{
			mAngle = 0.0f;

			if (mFrequencyMin != mFrequencyMax)
			{
				mFrequency = Math::RangeRandom(mFrequencyMin, mFrequencyMax);
			}
		}
	}
	//-----------------------------------------------------------------------
	const Real SineForceAffector::getFrequencyMin(void) const
	{
		return mFrequencyMin;
	}
	//-----------------------------------------------------------------------
	void SineForceAffector::setFrequencyMin(const Real frequencyMin)
	{
		mFrequencyMin = frequencyMin;
		if (frequencyMin > mFrequencyMax)
		{
			mFrequency = frequencyMin;
		}
	}
	//-----------------------------------------------------------------------
	const Real SineForceAffector::getFrequencyMax(void) const
	{
		return mFrequencyMax;
	}
	//-----------------------------------------------------------------------
	void SineForceAffector::setFrequencyMax(const Real frequencyMax)
	{
		mFrequencyMax = frequencyMax;
		mFrequency = frequencyMax;
	}
	//-----------------------------------------------------------------------
	void SineForceAffector::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		// Affect the direction
		if (mForceApplication == FA_ADD)
		{
			particle->direction += mScaledVector;
		}
		else
		{
			particle->direction = (particle->direction + mForceVector) / 2;
		}
	}

}
