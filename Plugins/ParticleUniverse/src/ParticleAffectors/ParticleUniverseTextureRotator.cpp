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

#include "ParticleAffectors/ParticleUniverseTextureRotator.h"

namespace ParticleUniverse
{
	// Constants
	const bool TextureRotator::DEFAULT_USE_OWN_SPEED = false;
	const Real TextureRotator::DEFAULT_ROTATION_SPEED = 10.0f;
	const Real TextureRotator::DEFAULT_ROTATION = 0.0f;

	//-----------------------------------------------------------------------
	TextureRotator::TextureRotator(void) : 
		ParticleAffector(),
		mUseOwnRotationSpeed(DEFAULT_USE_OWN_SPEED),
		mScaledRotationSpeed(Radian(0)),
		twoPiRad(Radian(Math::TWO_PI))
	{
		mDynRotation = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
		(static_cast<DynamicAttributeFixed*>(mDynRotation))->setValue(DEFAULT_ROTATION);
		mDynRotationSpeed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
		(static_cast<DynamicAttributeFixed*>(mDynRotationSpeed))->setValue(DEFAULT_ROTATION_SPEED);
	}
	//-----------------------------------------------------------------------
	TextureRotator::~TextureRotator(void)
	{
		if (mDynRotation)
		{
			PU_DELETE_T(mDynRotation, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
			mDynRotation = 0;
		}

		if (mDynRotationSpeed)
		{
			PU_DELETE_T(mDynRotationSpeed, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
			mDynRotationSpeed = 0;
		}
	}
	//-----------------------------------------------------------------------
	bool TextureRotator::useOwnRotationSpeed (void) const
	{
		return mUseOwnRotationSpeed;
	}
	//-----------------------------------------------------------------------
	void TextureRotator::setUseOwnRotationSpeed (bool useOwnRotationSpeed)
	{
		mUseOwnRotationSpeed = useOwnRotationSpeed;
	}
	//-----------------------------------------------------------------------
	DynamicAttribute* TextureRotator::getRotation(void) const
	{
		return mDynRotation;
	}
	//-----------------------------------------------------------------------
	void TextureRotator::setRotation(DynamicAttribute* dynRotation)
	{
		if (mDynRotation)
			PU_DELETE_T(mDynRotation, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);

		mDynRotation = dynRotation;
	}
	//-----------------------------------------------------------------------
	DynamicAttribute* TextureRotator::getRotationSpeed(void) const
	{
		return mDynRotationSpeed;
	}
	//-----------------------------------------------------------------------
	void TextureRotator::setRotationSpeed(DynamicAttribute* dynRotationSpeed)
	{
		if (mDynRotationSpeed)
			PU_DELETE_T(mDynRotationSpeed, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);

		mDynRotationSpeed = dynRotationSpeed;
	}
	//-----------------------------------------------------------------------
	Radian TextureRotator::_calculateRotation(void)
	{
		return Radian(mDynamicAttributeHelper.calculate(mDynRotation, mParentTechnique->getParentSystem()->getTimeElapsedSinceStart()));
	}
	//-----------------------------------------------------------------------
	Radian TextureRotator::_calculateRotationSpeed(Particle* particle)
	{
		return Radian(mDynamicAttributeHelper.calculate(mDynRotationSpeed, particle->timeFraction));
	}
	//-----------------------------------------------------------------------
	void TextureRotator::_initParticleForEmission(Particle* particle)
	{
		// Only continue if the particle is a visual particle
		if (particle->particleType != Particle::PT_VISUAL)
			return;

		// Set initial random zRotation
		VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
		visualParticle->zRotation = _calculateRotation();
		if (particle->parentEmitter->getParentTechnique()->getRenderer())
		{
			// Assume that all parents exist. That must be the case otherwise particles are not emitted.
			particle->parentEmitter->getParentTechnique()->getRenderer()->_notifyParticleZRotated();
		}

		// Set the zRotationSpeed
		visualParticle->zRotationSpeed = _calculateRotationSpeed(particle);
	}
	//-----------------------------------------------------------------------
	void TextureRotator::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		// Only continue if the particle is a visual particle
		if (particle->particleType != Particle::PT_VISUAL)
			return;

		VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
		if (mUseOwnRotationSpeed)
		{
			// Use scaled rotationspeed and adjust the speed according to the velocity
			mScaledRotationSpeed = visualParticle->zRotationSpeed * timeElapsed;
		}
		else
		{
			// Scale speed
			mScaledRotationSpeed = _calculateRotationSpeed(particle) * timeElapsed;
		}

		visualParticle->zRotation += mScaledRotationSpeed;
		visualParticle->zRotation = visualParticle->zRotation > twoPiRad ? visualParticle->zRotation - twoPiRad : visualParticle->zRotation;
		if (particleTechnique->getRenderer())
		{
			particleTechnique->getRenderer()->_notifyParticleZRotated();
		}
	}
	//-----------------------------------------------------------------------
	void TextureRotator::copyAttributesTo (ParticleAffector* affector)
	{
		ParticleAffector::copyAttributesTo(affector);

		TextureRotator* textureRotator = static_cast<TextureRotator*>(affector);
		textureRotator->setRotation(mDynamicAttributeFactory.cloneDynamicAttribute(getRotation()));
		textureRotator->setRotationSpeed(mDynamicAttributeFactory.cloneDynamicAttribute(getRotationSpeed()));
		textureRotator->mUseOwnRotationSpeed = mUseOwnRotationSpeed;
	}

}
