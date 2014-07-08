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

namespace ParticleUniverse
{
	Real Particle::DEFAULT_TTL = 10.0f;
	Real Particle::DEFAULT_MASS = 1.0f;
	
	//-----------------------------------------------------------------------
	Particle::~Particle(void)
	{
		ParticleBehaviourIterator it;
		ParticleBehaviourIterator itEnd = mBehaviours.end();
		for (it = mBehaviours.begin(); it != itEnd; ++it)
		{
			ParticleSystemManager::getSingletonPtr()->destroyBehaviour(*it);
		}
	}
	//-----------------------------------------------------------------------
	void Particle::_initForEmission(void)
	{
		mEventFlags = 0;
		timeFraction = 0.0f;

		/*	Note, that this flag must only be set as soon as the particle is emitted. As soon as the particle has
			been moved once, the flag must be removed again.
		*/
		addEventFlags(Particle::PEF_EMITTED);

		// Reset freeze flag
		mFreezed = false;

		// Iterate through all Behaviour objects that are registered at the particle.
		if (mBehaviours.empty())
			return;

		ParticleBehaviourIterator it;
		ParticleBehaviourIterator itEnd = mBehaviours.end();
		for (it = mBehaviours.begin(); it != itEnd; ++it)
		{
			(*it)->_initParticleForEmission(this);
		}
	}
	//-----------------------------------------------------------------------
	bool Particle::isEnabled(void) const
	{
		return mEnabled;
	}
	//-----------------------------------------------------------------------
	void Particle::setEnabled(bool enabled)
	{
		mEnabled = enabled;
		if (!mOriginalEnabledSet)
		{
			// Only one time is permitted
			mOriginalEnabled = enabled;
			mOriginalEnabledSet = true;
		}
	}
	//-----------------------------------------------------------------------
	void Particle::_setOriginalEnabled(bool originalEnabled)
	{
		mOriginalEnabled = originalEnabled;
		mOriginalEnabledSet = true;
	}
	//-----------------------------------------------------------------------
	bool Particle::_getOriginalEnabled(void) const
	{
		return mOriginalEnabled;
	}
	//-----------------------------------------------------------------------
	bool Particle::isFreezed(void) const
	{
		return mFreezed;
	}
	//-----------------------------------------------------------------------
	void Particle::setFreezed(bool freezed)
	{
		mFreezed = freezed;
	}
	//-----------------------------------------------------------------------
	ParticleBehaviour* Particle::getBehaviour(const String& behaviourType)
	{
		if (behaviourType == BLANK_STRING)
			return 0;

		ParticleBehaviourIterator it;
		ParticleBehaviourIterator itEnd = mBehaviours.end();
		for (it = mBehaviours.begin(); it != itEnd; ++it)
		{
			if ((*it)->getBehaviourType() == behaviourType)
			{
				return *it;
			}
		}
		
		return 0;
	}
	//-----------------------------------------------------------------------
	void Particle::copyBehaviours(ParticleBehaviourList& behaviours)
	{
		ParticleBehaviourIterator it;
		ParticleBehaviourIterator itEnd = behaviours.end();
		for (it = behaviours.begin(); it != itEnd; ++it)
		{
			mBehaviours.push_back(ParticleSystemManager::getSingletonPtr()->cloneBehaviour(*it));
		}
	}
	//-----------------------------------------------------------------------
	Real Particle::calculateVelocity(void) const
	{
		if (originalScaledDirectionLength != 0)
		{
			return originalVelocity * (direction.length() / originalScaledDirectionLength);
		}
		else
		{
			// Assume originalScaledDirectionLength to be 1.0 (unit vector)
			return originalVelocity * direction.length();
		}
	}
	//-----------------------------------------------------------------------
	void Particle::copyAttributesTo (Particle* particle)
	{
		// Copy attributes
		particle->position = position;
		particle->originalPosition = originalPosition;
		particle->mDerivedPosition = mDerivedPosition;
		particle->direction = direction;
		particle->originalDirection = originalDirection;
		particle->originalDirectionLength = originalDirectionLength;
		particle->originalScaledDirectionLength = originalScaledDirectionLength;
		particle->originalVelocity = originalVelocity;
		particle->mass = mass;
		particle->timeToLive = timeToLive;
		particle->totalTimeToLive = totalTimeToLive;
		particle->mEventFlags = mEventFlags;
		particle->mMarkedForEmission = mMarkedForEmission;
		particle->mEnabled = mEnabled;
		particle->mOriginalEnabled = mOriginalEnabled;
		particle->mOriginalEnabledSet = mOriginalEnabledSet;
		particle->mFreezed = mFreezed;
		particle->timeFraction = timeFraction;

		// Copy Behaviour
		particle->copyBehaviours(mBehaviours);
	}
	//-----------------------------------------------------------------------
	void Particle::_process(ParticleTechnique* technique, Real timeElapsed)
	{
		// Calculate the time fraction, because it is used in different other components
		timeFraction = (totalTimeToLive - timeToLive) / totalTimeToLive;

		if (mBehaviours.empty())
			return;

		// Iterate through all Behaviour objects that are registered at the particle.
		ParticleBehaviourIterator it;
		ParticleBehaviourIterator itEnd = mBehaviours.end();
		for (it = mBehaviours.begin(); it != itEnd; ++it)
		{
			(*it)->_processParticle(technique, this, timeElapsed);
		}
	}
	//-----------------------------------------------------------------------
	void Particle::_initForExpiration(ParticleTechnique* technique, Real timeElapsed)
	{
		if (mBehaviours.empty())
			return;

		// Iterate through all Behaviour objects that are registered at the particle.
		ParticleBehaviourIterator it;
		ParticleBehaviourIterator itEnd = mBehaviours.end();
		for (it = mBehaviours.begin(); it != itEnd; ++it)
		{
			(*it)->_initParticleForExpiration(technique, this, timeElapsed);
		}
	}

}
