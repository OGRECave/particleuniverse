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

#ifndef __PU_VORTEX_AFFECTOR_H__
#define __PU_VORTEX_AFFECTOR_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseAffector.h"
#include "ParticleUniverseDynamicAttribute.h"
#include "ParticleUniverseDynamicAttributeFactory.h"

namespace ParticleUniverse
{
	/** The VortexAffector rotates particles around a given rotation axis.
    */
	class _ParticleUniverseExport VortexAffector : public ParticleAffector
	{
		public:
			// Constants
			static const Vector3 DEFAULT_ROTATION_VECTOR;
			static const Real DEFAULT_ROTATION_SPEED;
		
			VortexAffector(void);
	        virtual ~VortexAffector(void);

			/** 
	        */
			const Vector3& getRotationVector(void) const;

			/** 
	        */
			void setRotationVector(const Vector3& rotationVector);

			/** 
	        */
			DynamicAttribute* getRotationSpeed(void) const;

			/** 
	        */
			void setRotationSpeed(DynamicAttribute* dynRotationSpeed);

			/** @copydoc ParticleAffector::copyAttributesTo */
			virtual void copyAttributesTo (ParticleAffector* affector);

			/** 
	        */
			virtual void _preProcessParticles(ParticleTechnique* particleTechnique, Real timeElapsed);

			/** 
	        */
			virtual void _affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

		protected:
			Vector3 mRotationVector;
			Quaternion mRotation;
			DynamicAttribute* mDynRotationSpeed;
			DynamicAttributeHelper mDynamicAttributeHelper;
			DynamicAttributeFactory mDynamicAttributeFactory;

			/** 
	        */
			Radian _calculateRotationSpeed(void);
	};

}
#endif
