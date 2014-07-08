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

#include "ParticleUniverseAffector.h"

namespace ParticleUniverse
{
	// Constants
	const bool ParticleAffector::DEFAULT_ENABLED = true;
	const Vector3 ParticleAffector::DEFAULT_POSITION(0, 0, 0);
	const ParticleAffector::AffectSpecialisation ParticleAffector::DEFAULT_SPECIALISATION = ParticleAffector::AFSP_DEFAULT;

	//-----------------------------------------------------------------------
	ParticleAffector::ParticleAffector(void) :
		Particle(),
		IAlias(),
		IElement(),
		mParentTechnique(0),
		mName(BLANK_STRING),
		_mAffectorScale(Vector3::UNIT_SCALE),
		mAffectSpecialisation(ParticleAffector::AFSP_DEFAULT)
	{
		particleType = PT_AFFECTOR;
		mAliasType = AT_AFFECTOR;
	};
	//-----------------------------------------------------------------------
	void ParticleAffector::_notifyStart (void)
	{
		setEnabled(mOriginalEnabled);
	}
	//-----------------------------------------------------------------------
	void ParticleAffector::_notifyRescaled(const Vector3& scale)
	{
		_mAffectorScale = scale;
	}
	//-----------------------------------------------------------------------
	void ParticleAffector::_initForEmission(void)
	{
		// The affector itself is emitted.
		Particle::_initForEmission();

		// Emitting an affector is similar as starting one.
		_notifyStart();
	}
	//-----------------------------------------------------------------------
	void ParticleAffector::_initForExpiration(ParticleTechnique* technique, Real timeElapsed)
	{
		// The affector itself is expired.
		Particle::_initForExpiration(technique, timeElapsed);

		// Expiring an affector is similar as stopping one.
		_notifyStop();
	}
	//-----------------------------------------------------------------------
	void ParticleAffector::copyAttributesTo(ParticleAffector* affector)
	{
		copyParentAttributesTo(affector);
	}
	//-----------------------------------------------------------------------
	void ParticleAffector::copyParentAttributesTo(ParticleAffector* affector)
	{
		// Copy Parent attributes
		Particle::copyAttributesTo(affector);

		affector->setName(mName);
		affector->mParentTechnique = mParentTechnique;
		affector->_mAffectorScale = _mAffectorScale;
		affector->mAffectSpecialisation = mAffectSpecialisation;
		affector->mExcludedEmitters = list<String>(mExcludedEmitters); // Use copy constructor
	}
	//-----------------------------------------------------------------------
	void ParticleAffector::setParentTechnique(ParticleTechnique* parentTechnique)
	{
		mParentTechnique = parentTechnique;
	}
	//-----------------------------------------------------------------------
	void ParticleAffector::_processParticle(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed, bool firstParticle)
	{
		// Call the _firstParticle() function if the first particle in the update loop is encountered.
		if (firstParticle)
		{
			// Perform a precalculation at the first particle
			_firstParticle(particleTechnique, particle, timeElapsed);
		}

		if (!mExcludedEmitters.empty() && particle->parentEmitter)
		{
			// Return if the emitter which emits this particle is part of the list
			String emitterName = particle->parentEmitter->getName();
			list<String>::iterator it;
			it = std::find(mExcludedEmitters.begin(), mExcludedEmitters.end(), emitterName);
			if (it != mExcludedEmitters.end())
			{
				return;
			}
		}

		_affect(particleTechnique, particle, timeElapsed);
	}
	//-----------------------------------------------------------------------
	void ParticleAffector::addEmitterToExclude(const String& emitterName)
	{
		list<String>::iterator it;
		it = std::find(mExcludedEmitters.begin(), mExcludedEmitters.end(), emitterName);
		if (it == mExcludedEmitters.end())
		{
			mExcludedEmitters.push_back(emitterName);
		}
	}
	//-----------------------------------------------------------------------
	void ParticleAffector::removeEmitterToExclude(const String& emitterName)
	{
		list<String>::iterator it;
		it = std::find(mExcludedEmitters.begin(), mExcludedEmitters.end(), emitterName);
		if (it != mExcludedEmitters.end())
		{
			mExcludedEmitters.erase(it);
		}
	}
	//-----------------------------------------------------------------------
	void ParticleAffector::removeAllEmittersToExclude(void)
	{
		mExcludedEmitters.clear();
	}
	//-----------------------------------------------------------------------
	const list<String>& ParticleAffector::getEmittersToExclude(void) const
	{
		return mExcludedEmitters;
	}
	//-----------------------------------------------------------------------
	bool ParticleAffector::hasEmitterToExclude(const String& emitterName)
	{
		list<String>::iterator it;
		it = std::find(mExcludedEmitters.begin(), mExcludedEmitters.end(), emitterName);
		return (it != mExcludedEmitters.end());
	}
	//-----------------------------------------------------------------------
	const Vector3& ParticleAffector::getDerivedPosition(void)
	{
		if (mMarkedForEmission)
		{
			// Use the affector position, because it is emitted
			// If a particle is emitted, position and derived position are the same
			mDerivedPosition = position;
		}
		else
		{
			// Add the techniques' derived position
			mDerivedPosition = mParentTechnique->getDerivedPosition() + 
				mParentTechnique->getParentSystem()->getDerivedOrientation() * (_mAffectorScale * position);
		}
		return mDerivedPosition;
	}
	//-----------------------------------------------------------------------
	Real ParticleAffector::_calculateAffectSpecialisationFactor (const Particle* particle)
	{
		// Assume that particle->totalTimeToLive != 0, which is reasonable
		switch (mAffectSpecialisation)
		{
			case AFSP_DEFAULT:
				return 1.0f;
				break;

			// This means that older particles will be affected MORE than just emitted particles
			case AFSP_TTL_INCREASE:
				{
					if (particle)
					{
						return particle->timeFraction;
					}
					else
					{
						return 1.0f;
					}
				}
				break;

			// This means that older particles will be affected LESS than just emitted particles
			case AFSP_TTL_DECREASE:
				{
					if (particle)
					{
						return 1.0f - particle->timeFraction;
					}
					else
					{
						return 1.0f;
					}
				}
				break;

			default:
				return 1.0f;
				break;
		}
	}
}
