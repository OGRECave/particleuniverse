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

#ifndef __PU_CIRCLE_EMITTER_H__
#define __PU_CIRCLE_EMITTER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseEmitter.h"

namespace ParticleUniverse
{
	/** The CircleEmitter is a ParticleEmitter that emits particles on a circle shape. Particle can be emitted
		random on the circle, but it can also follow the circles' contours.
    */
	class _ParticleUniverseExport CircleEmitter : public ParticleEmitter
	{
		protected:
			Real mRadius;
			Real mCircleAngle;
			Real mOriginalCircleAngle;
			Real mStep;
			Real mX;
			Real mZ;
			bool mRandom;
			Quaternion mOrientation;
			Vector3 mNormal;

		public:
			// Constants
			static const Real DEFAULT_RADIUS;
			static const Real DEFAULT_STEP;
			static const Real DEFAULT_ANGLE;
			static const bool DEFAULT_RANDOM;
			static const Vector3 DEFAULT_NORMAL;

			CircleEmitter(void);
	        virtual ~CircleEmitter(void) {};

			/** 
	        */
			const Real getRadius(void) const;
			void setRadius(const Real radius);

			/** 
	        */
			const Real getCircleAngle(void) const;
			void setCircleAngle(const Real circleAngle);

			/** 
	        */
			const Real getStep(void) const;
			void setStep(const Real step);

			/** 
	        */
			const bool isRandom(void) const;
			void setRandom(const bool random);

			/* 
			*/ 
			const Quaternion& getOrientation(void) const;
			const Vector3& getNormal(void) const;
			void setNormal(const Vector3 &normal); 

			/** See ParticleEmiter
	        */
			void _notifyStart(void);

			/** Determine a particle position on the circle.
	        */
			virtual void _initParticlePosition(Particle* particle);

			/** Determine the particle direction.
	        */
			virtual void _initParticleDirection(Particle* particle);

			/** 
	        */
			virtual void copyAttributesTo (ParticleEmitter* emitter);
	};

}
#endif
