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

#ifndef __PU_SPHERE_SURFACE_EMITTER_H__
#define __PU_SPHERE_SURFACE_EMITTER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseEmitter.h"

namespace ParticleUniverse
{
	/** The SphereSurfaceEmitter emits particles from the surface of a sphere (instead within the sphere´s
		volume). The particles are emitted in a direction perpendicular to the tangentvector where 
		the particle emits. Using the angle attribute, the direction can be influenced.
    */
	class _ParticleUniverseExport SphereSurfaceEmitter : public ParticleEmitter
	{
		protected:
			Real mRadius;
			Vector3 mRandomVector;

		public:
			// Constants
			static const Real DEFAULT_RADIUS;

			SphereSurfaceEmitter(void);
	        virtual ~SphereSurfaceEmitter(void) {};

			/** 
	        */
			const Real getRadius(void) const;
			void setRadius(const Real radius);

			/** 
	        */
			virtual void _initParticlePosition(Particle* particle);

			/** 
	        */
			virtual void _initParticleDirection(Particle* particle);
			
			/** 
	        */
			virtual void copyAttributesTo (ParticleEmitter* emitter);
	};

}
#endif
