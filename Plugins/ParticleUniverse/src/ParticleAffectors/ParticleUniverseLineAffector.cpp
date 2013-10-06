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

#include "ParticleAffectors/ParticleUniverseLineAffector.h"

namespace ParticleUniverse
{
	// Constants
	const Real LineAffector::DEFAULT_MAX_DEVIATION = 1.0f;
	const Real LineAffector::DEFAULT_TIME_STEP = 0.1f;
	const Vector3 LineAffector::DEFAULT_END(0, 0, 0);
	const Real LineAffector::DEFAULT_DRIFT = 0.0f;

	//-----------------------------------------------------------------------
	LineAffector::LineAffector(void) : 
		ParticleAffector(),
		mMaxDeviation(DEFAULT_MAX_DEVIATION),
		_mScaledMaxDeviation(1.0f),
		mEnd(DEFAULT_END),
		mTimeSinceLastUpdate(0.0f),
		mTimeStep(DEFAULT_TIME_STEP),
		mUpdate(true),
		mFirst(true),
		mDrift(DEFAULT_DRIFT),
		mOneMinusDrift(1.0f)
	{
	}
	//-----------------------------------------------------------------------
	Real LineAffector::getMaxDeviation(void) const
	{
		return mMaxDeviation;
	}
	//-----------------------------------------------------------------------
	void LineAffector::setMaxDeviation(Real maxDeviation)
	{
		mMaxDeviation = maxDeviation;
		_mScaledMaxDeviation = mMaxDeviation * _mAffectorScale.length();
	}
	//-----------------------------------------------------------------------
	const Vector3& LineAffector::getEnd(void) const
	{
		return mEnd;
	}
	//-----------------------------------------------------------------------
	void LineAffector::setEnd(const Vector3& end)
	{
		mEnd = end;
	}
	//-----------------------------------------------------------------------
	Real LineAffector::getTimeStep(void) const
	{
		return mTimeStep;
	}
	//-----------------------------------------------------------------------
	void LineAffector::setTimeStep(Real timeStep)
	{
		mTimeStep = timeStep;
	}
	//-----------------------------------------------------------------------
	Real LineAffector::getDrift(void) const
	{
		return mDrift;
	}
	//-----------------------------------------------------------------------
	void LineAffector::setDrift(Real drift)
	{
		mDrift = drift;
		mOneMinusDrift = 1.0f - drift;
	}
	//-----------------------------------------------------------------------
	void LineAffector::_notifyRescaled(const Vector3& scale)
	{
		ParticleAffector::_notifyRescaled(scale);
		_mScaledMaxDeviation = mMaxDeviation * scale.length();
	}
	//-----------------------------------------------------------------------
	void LineAffector::_firstParticle(ParticleTechnique* particleTechnique, 
		Particle* particle, 
		Real timeElapsed)
	{
		// The first particle should stay on its place
		mFirst = true;
	}
	//-----------------------------------------------------------------------
	void LineAffector::_preProcessParticles(ParticleTechnique* technique, Real timeElapsed)
	{
		if (technique->getNumberOfEmittedParticles() > 0)
		{
			mTimeSinceLastUpdate += timeElapsed;
			while (mTimeSinceLastUpdate > mTimeStep)
			{
				mTimeSinceLastUpdate -= mTimeStep;
				mUpdate = true;
			}
		}
		mParentTechnique->getParentSystem()->rotationOffset(mEnd); // Always update
	}
	//-----------------------------------------------------------------------
	void LineAffector::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		mParentTechnique->getParentSystem()->rotationOffset(particle->originalPosition); // Always update
		if (mUpdate && Math::UnitRandom() > 0.5 && !mFirst)
		{
			// Generate a random vector perpendicular on the line
			Vector3 perpendicular = mEnd.crossProduct(Vector3(Math::RangeRandom(-1, 1), 
				Math::RangeRandom(-1, 1), 
				Math::RangeRandom(-1, 1)));
			perpendicular.normalise();

			// Determine a random point near the line.
			Vector3 targetPosition = particle->originalPosition + _mScaledMaxDeviation * Math::UnitRandom() * perpendicular;

			/** Set the new position.
			@remarks
				This affector already takes rotational offset of the particle system into account. This means that there is no need
				to set the particle system to keep_local to 'true'. The reason is that this is a specialized affector that calculates
				a new particle position instead of a direction.
			*/
			particle->position = mDrift * targetPosition + mOneMinusDrift * particle->position;
			mParentTechnique->getParentSystem()->rotationOffset(particle->position);
		}
		mFirst = false;
	}
	//-----------------------------------------------------------------------
	void LineAffector::_postProcessParticles(ParticleTechnique* technique, Real timeElapsed)
	{
		mUpdate = false;
	}
	//-----------------------------------------------------------------------
	void LineAffector::copyAttributesTo (ParticleAffector* affector)
	{
		ParticleAffector::copyAttributesTo(affector);

		LineAffector* lineAffector = static_cast<LineAffector*>(affector);

		lineAffector->setMaxDeviation(mMaxDeviation);
		lineAffector->mEnd = mEnd;
		lineAffector->mTimeStep = mTimeStep;
		lineAffector->mDrift = mDrift;
		lineAffector->mOneMinusDrift = mOneMinusDrift;
	}
}
