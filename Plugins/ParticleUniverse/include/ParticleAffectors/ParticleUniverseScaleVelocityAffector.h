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

#ifndef __PU_SCALE_VELOCITY_AFFECTOR_H__
#define __PU_SCALE_VELOCITY_AFFECTOR_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseDynamicAttribute.h"
#include "ParticleUniverseDynamicAttributeFactory.h"
#include "ParticleUniverseAffector.h"

namespace ParticleUniverse
{
	/** Scales the velocity of a particle. This can be a linear scale, but scaling that changes over time alos is possible.
    */
	class _ParticleUniverseExport ScaleVelocityAffector : public ParticleAffector
	{
		public:
			// Constants
			static const Real DEFAULT_VELOCITY_SCALE;

			ScaleVelocityAffector(void);
	        virtual ~ScaleVelocityAffector(void);

			/** @copydoc ParticleAffector::copyAttributesTo */
			virtual void copyAttributesTo (ParticleAffector* affector);

			/** 
	        */
			virtual void _affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

			/** 
	        */
			DynamicAttribute* getDynScaleVelocity(void) const {return mDynScaleVelocity;};
			void setDynScaleVelocity(DynamicAttribute* dynScaleVelocity);
			void resetDynScaleVelocity(bool resetToDefault = true);

			/** 
	        */
			bool isSinceStartSystem(void) const {return mSinceStartSystem;};
			void setSinceStartSystem(bool sinceStartSystem){mSinceStartSystem = sinceStartSystem;};

			/** 
	        */
			bool isStopAtFlip(void) const {return mStopAtFlip;};
			void setStopAtFlip(bool stopAtFlip){mStopAtFlip = stopAtFlip;};

	protected:
			DynamicAttribute* mDynScaleVelocity;
			DynamicAttributeFactory mDynamicAttributeFactory;
			DynamicAttributeHelper mDynamicAttributeHelper;
			bool mSinceStartSystem;
			bool mStopAtFlip;
	};

}
#endif
