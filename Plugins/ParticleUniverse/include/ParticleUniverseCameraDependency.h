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

#ifndef __PU_CAMERA_DEPENDENCY_H__
#define __PU_CAMERA_DEPENDENCY_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseIDependency.h"

namespace ParticleUniverse
{
	/** The CameraDependency class is used to define a relation between an attribute (for example the emission
		rate of a ParticleEmitter) and the camera. The camera distance influences the value of the attribute.
	@remarks
		In case of the emission rate for example, it can be defined that that number of emitted particles 
		decreases if the camera gets further away.
    */
	class _ParticleUniverseExport CameraDependency : public IDependency
	{
		protected:
			Real mThreshold;
			bool mIncrease;

		public:
			// Constants
			static const Real DEFAULT_DISTANCE_THRESHOLD;
			static const bool DEFAULT_INCREASE;

			CameraDependency(void);
			CameraDependency(Real threshold, bool inc);
			virtual ~CameraDependency(void){};

			/** Todo
	        */
			virtual bool affect(Real& baseValue, Real dependencyValue);

			/** Todo
	        */
			const Real getThreshold(void) const {return mThreshold;};
			void setThreshold(const Real threshold) {mThreshold = threshold;};

			/** Todo
	        */
			bool isIncrease(void) const {return mIncrease;};
			void setIncrease(bool increase) {mIncrease = increase;};

			/** Copy attributes to another camera dependency.
	        */
			virtual void copyAttributesTo (CameraDependency* cameraDependency);

	};
}
#endif
