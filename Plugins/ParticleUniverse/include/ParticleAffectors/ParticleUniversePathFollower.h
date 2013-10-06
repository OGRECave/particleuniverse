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

#ifndef __PU_PATH_FOLLOWER_H__
#define __PU_PATH_FOLLOWER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseAffector.h"
#include "ParticleUniverseSimpleSpline.h"

namespace ParticleUniverse
{
	/** This affector let particles move along a line. The line is derived from a number of points in 3D space.
		Using a spline interpolation, the line becomes smooth.
    */
	class _ParticleUniverseExport PathFollower : public ParticleAffector
	{
		protected:
			SimpleSpline mSpline;

		public:
			PathFollower(void) : 
				ParticleAffector()
			{
			};
	        virtual ~PathFollower(void) {};

			/** 
	        */
			void addPoint(const Vector3& point);

			/** Clear all points
	        */
			void clearPoints (void);

			/** 
	        */
			unsigned short getNumPoints(void) const {return mSpline.getNumPoints();};
				
			/** 
	        */
			const Vector3& getPoint(unsigned short index) const {return mSpline.getPoint(index);};
			
			/** 
	        */
			virtual void _affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

			/** @copydoc ParticleAffector::copyAttributesTo */
			virtual void copyAttributesTo (ParticleAffector* affector);
	};

}
#endif
