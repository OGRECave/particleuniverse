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

#ifndef __PU_GEOMETRY_ROTATOR_H__
#define __PU_GEOMETRY_ROTATOR_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseAffector.h"
#include "ParticleUniverseDynamicAttribute.h"
#include "ParticleUniverseDynamicAttributeFactory.h"

namespace ParticleUniverse
{
	/** The GeometryRotator rotates particles around its orientation axis. The rotation speed can be 
		adjusted and can be set to a 'global' rotation speed, which affects all particles in the
		Particle Technique the same way. It is also possible to use the rotation speed of the particles
		itself.
    */
	class _ParticleUniverseExport GeometryRotator : public ParticleAffector
	{
		public:
			// Constants
			static const bool DEFAULT_USE_OWN;
			static const Real DEFAULT_ROTATION_SPEED;
			static const Vector3 DEFAULT_ROTATION_AXIS;

			GeometryRotator(void);
	        virtual ~GeometryRotator(void);

			/** Returns the rotation speed. This is the speed controlled by the affector. Besides
				the default rotation speed, it is also possible to use the particles own rotation speed.
	        */
			DynamicAttribute* getRotationSpeed(void) const;

			/** 
	        */
			void setRotationSpeed(DynamicAttribute* dynRotationSpeed);

			/** Returns an indication whether the rotation speed is the same for all particles in this 
				particle technique, or whether the rotation speed of the particle itself is used.
	        */
			bool useOwnRotationSpeed (void) const;

			/** Set the indication whether rotation speed of the particle itself is used.
	        */
			void setUseOwnRotationSpeed (bool useOwnRotationSpeed);

			/** 
	        */
			const Vector3& getRotationAxis(void) const;

			/** 
	        */
			void setRotationAxis(const Vector3& rotationAxis);

			/** 
	        */
			void resetRotationAxis(void);

			/** Returns a rotation speed value, depending on the type of dynamic attribute.
	        */
			Real _calculateRotationSpeed (Particle* particle);

			/** @copydoc ParticleAffector::copyAttributesTo */
			virtual void copyAttributesTo (ParticleAffector* affector);

			/** @copydoc ParticleAffector::_initParticleForEmission */
			virtual void _initParticleForEmission(Particle* particle);

			/** @copydoc ParticleAffector::_affect */
			virtual void _affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

		protected:
			Real mScaledRotationSpeed;
			bool mUseOwnRotationSpeed;
			DynamicAttribute* mDynRotationSpeed;
			Quaternion mQ;
			Vector3 mRotationAxis;
			bool mRotationAxisSet;

			/** Helper factory
			*/
			DynamicAttributeFactory mDynamicAttributeFactory;
			DynamicAttributeHelper mDynamicAttributeHelper;
	};

}
#endif
