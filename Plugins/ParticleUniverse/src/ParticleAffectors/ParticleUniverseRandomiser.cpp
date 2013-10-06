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

#include "ParticleAffectors/ParticleUniverseRandomiser.h"

namespace ParticleUniverse
{
	// Constants
	const Vector3 Randomiser::DEFAULT_MAX_DEVIATION(0, 0, 0);
	const Real Randomiser::DEFAULT_TIME_STEP = 0.0f;
	const bool Randomiser::DEFAULT_RANDOM_DIRECTION = true;

	//-----------------------------------------------------------------------
	Randomiser::Randomiser(void) : 
		ParticleAffector(),
		mMaxDeviationX(DEFAULT_MAX_DEVIATION.x),
		mMaxDeviationY(DEFAULT_MAX_DEVIATION.y),
		mMaxDeviationZ(DEFAULT_MAX_DEVIATION.z),
		mTimeSinceLastUpdate(0.0f),
		mTimeStep(DEFAULT_TIME_STEP),
		mUpdate(true),
		mRandomDirection(DEFAULT_RANDOM_DIRECTION)
	{
	}
	//-----------------------------------------------------------------------
	Real Randomiser::getMaxDeviationX(void) const
	{
		return mMaxDeviationX;
	}
	//-----------------------------------------------------------------------
	void Randomiser::setMaxDeviationX(Real maxDeviationX)
	{
		mMaxDeviationX = maxDeviationX;
	}
	//-----------------------------------------------------------------------
	Real Randomiser::getMaxDeviationY(void) const
	{
		return mMaxDeviationY;
	}
	//-----------------------------------------------------------------------
	void Randomiser::setMaxDeviationY(Real maxDeviationY)
	{
		mMaxDeviationY = maxDeviationY;
	}
	//-----------------------------------------------------------------------
	Real Randomiser::getMaxDeviationZ(void) const
	{
		return mMaxDeviationZ;
	}
	//-----------------------------------------------------------------------
	void Randomiser::setMaxDeviationZ(Real maxDeviationZ)
	{
		mMaxDeviationZ = maxDeviationZ;
	}
	//-----------------------------------------------------------------------
	Real Randomiser::getTimeStep(void) const
	{
		return mTimeStep;
	}
	//-----------------------------------------------------------------------
	void Randomiser::setTimeStep(Real timeStep)
	{
		mTimeStep = timeStep;
		mTimeSinceLastUpdate = timeStep;
	}
	//-----------------------------------------------------------------------
	bool Randomiser::isRandomDirection(void) const
	{
		return mRandomDirection;
	}
	//-----------------------------------------------------------------------
	void Randomiser::setRandomDirection(bool randomDirection)
	{
		mRandomDirection = randomDirection;
	}
	//-----------------------------------------------------------------------
	void Randomiser::_preProcessParticles(ParticleTechnique* technique, Real timeElapsed)
	{
		if (technique->getNumberOfEmittedParticles() > 0)
		{
			mTimeSinceLastUpdate += timeElapsed;
			if (mTimeSinceLastUpdate > mTimeStep)
			{
				mTimeSinceLastUpdate -= mTimeStep;
				mUpdate = true;
			}
		}
	}
	//-----------------------------------------------------------------------
	void Randomiser::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		if (mUpdate)
		{
			if (mRandomDirection)
			{
				// Random direction: Change the direction after each update
				particle->direction += Vector3(Math::RangeRandom(-mMaxDeviationX, mMaxDeviationX),
					Math::RangeRandom(-mMaxDeviationY, mMaxDeviationY),
					Math::RangeRandom(-mMaxDeviationZ, mMaxDeviationZ));
			}
			else
			{
				// Explicitly check on 'freezed', because it passes the techniques' validation.
				if (particle->isFreezed())
					return;

				// Random position: Add the position deviation after each update
				particle->position += _mAffectorScale * Vector3(Math::RangeRandom(-mMaxDeviationX, mMaxDeviationX),
					Math::RangeRandom(-mMaxDeviationY, mMaxDeviationY),
					Math::RangeRandom(-mMaxDeviationZ, mMaxDeviationZ));
			}
		}
	}
	//-----------------------------------------------------------------------
	void Randomiser::_postProcessParticles(ParticleTechnique* technique, Real timeElapsed)
	{
		mUpdate = false;
	}
	//-----------------------------------------------------------------------
	void Randomiser::copyAttributesTo (ParticleAffector* affector)
	{
		ParticleAffector::copyAttributesTo(affector);

		Randomiser* randomiser = static_cast<Randomiser*>(affector);
		randomiser->mMaxDeviationX = mMaxDeviationX;
		randomiser->mMaxDeviationY = mMaxDeviationY;
		randomiser->mMaxDeviationZ = mMaxDeviationZ;
		randomiser->setTimeStep(mTimeStep); // Also sets time since last update to appropriate value
		randomiser->mRandomDirection = mRandomDirection;
	}

}
