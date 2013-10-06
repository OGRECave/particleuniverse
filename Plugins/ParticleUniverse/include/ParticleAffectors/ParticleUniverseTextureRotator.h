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

#ifndef __PU_TEXTURE_ROTATOR_H__
#define __PU_TEXTURE_ROTATOR_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseAffector.h"
#include "ParticleUniverseDynamicAttribute.h"
#include "ParticleUniverseDynamicAttributeFactory.h"

namespace ParticleUniverse
{
	/** The TextureRotator rotates the texture(s) of a particle. In general it is possible to support individual
		rotation of each particle texture-set - the same as in the geometry rotator, setting 
		mUseOwnRotationSpeed to true -, but in practice this isn't really usable, because usually all particles
		share the same material.
    */
	class _ParticleUniverseExport TextureRotator : public ParticleAffector
	{
		public:
			// Constants
			static const bool DEFAULT_USE_OWN_SPEED;
			static const Real DEFAULT_ROTATION_SPEED;
			static const Real DEFAULT_ROTATION;

			TextureRotator(void);
	        virtual ~TextureRotator(void);

			/** Returns an indication whether the 2D rotation speed is the same for all particles in this 
				particle technique, or whether the 2D rotation speed of the particle itself is used.
	        */
			bool useOwnRotationSpeed (void) const;

			/** Set the indication whether the 2D rotation speed of the particle itself is used.
	        */
			void setUseOwnRotationSpeed (bool useOwnRotationSpeed);

			/** Returns the rotation speed. This is the speed controlled by the affector.
	        */
			DynamicAttribute* getRotationSpeed(void) const;

			/** 
	        */
			void setRotationSpeed(DynamicAttribute* dynRotationSpeed);

			/** Returns the rotation defined in the the affector.
	        */
			DynamicAttribute* getRotation(void) const;

			/** 
	        */
			void setRotation(DynamicAttribute* dynRotation);

			/** @copydoc ParticleAffector::copyAttributesTo */
			virtual void copyAttributesTo (ParticleAffector* affector);

			/** Returns a rotation set in the affector, depending on the type of dynamic attribute.
	        */
			Radian _calculateRotation (void);

			/** Returns a rotation speed value, depending on the type of dynamic attribute.
	        */
			Radian _calculateRotationSpeed (Particle* particle);

			/** @copydoc ParticleAffector::_initParticleForEmission */
			virtual void _initParticleForEmission(Particle* particle);

			/** @copydoc ParticleAffector::_affect */
			virtual void _affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

		protected:
			bool mUseOwnRotationSpeed;
			Radian mScaledRotationSpeed;
			Radian twoPiRad;
			DynamicAttribute* mDynRotation;
			DynamicAttribute* mDynRotationSpeed;

			/** Helper factory
			*/
			DynamicAttributeFactory mDynamicAttributeFactory;
			DynamicAttributeHelper mDynamicAttributeHelper;
	};

}
#endif
