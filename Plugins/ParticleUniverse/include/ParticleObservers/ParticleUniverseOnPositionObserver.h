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

#ifndef __PU_ONPOSITION_OBSERVER_H__
#define __PU_ONPOSITION_OBSERVER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseCommon.h"
#include "ParticleUniverseObserver.h"

namespace ParticleUniverse
{
	/** This class is used to observe whether a Particle reaches a certain position.
    */
	class _ParticleUniverseExport OnPositionObserver : public ParticleObserver
	{
		protected:
			Real mPositionXThreshold;
			Real mPositionYThreshold;
			Real mPositionZThreshold;
			bool mPositionXThresholdSet;
			bool mPositionYThresholdSet;
			bool mPositionZThresholdSet;
			ComparisionOperator mComparePositionX;
			ComparisionOperator mComparePositionY;
			ComparisionOperator mComparePositionZ;

		public:
			// Constants
			static const Vector3 DEFAULT_POSITION_THRESHOLD;

			OnPositionObserver(void);
	        virtual ~OnPositionObserver(void) {};

			/** 
	        */
			virtual bool _observe (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

			/** 
			 */
			void setPositionXThreshold(Real threshold){mPositionXThreshold = threshold; mPositionXThresholdSet = true;};
			void setPositionYThreshold(Real threshold){mPositionYThreshold = threshold; mPositionYThresholdSet = true;};
			void setPositionZThreshold(Real threshold){mPositionZThreshold = threshold; mPositionZThresholdSet = true;};

			/** 
			 */
			Real getPositionXThreshold(void) const {return mPositionXThreshold;};
			Real getPositionYThreshold(void) const {return mPositionYThreshold;};
			Real getPositionZThreshold(void) const {return mPositionZThreshold;};

			/** 
			 */
			bool isPositionXThresholdSet(void) const {return mPositionXThresholdSet;};
			bool isPositionYThresholdSet(void) const {return mPositionYThresholdSet;};
			bool isPositionZThresholdSet(void) const {return mPositionZThresholdSet;};

			/** 
			 */
			void resetPositionXThreshold(void) {mPositionXThresholdSet = false;};
			void resetPositionYThreshold(void) {mPositionYThresholdSet = false;};
			void resetPositionZThreshold(void) {mPositionZThresholdSet = false;};

			/** 
			 */
			void setComparePositionX(ComparisionOperator op){mComparePositionX = op;};
			void setComparePositionY(ComparisionOperator op){mComparePositionY = op;};
			void setComparePositionZ(ComparisionOperator op){mComparePositionZ = op;};

			/** 
			 */
			const ComparisionOperator getComparePositionX(void) const {return mComparePositionX;};
			const ComparisionOperator getComparePositionY(void) const {return mComparePositionY;};
			const ComparisionOperator getComparePositionZ(void) const {return mComparePositionZ;};

			/** Copy attributes to another observer.
	        */
			virtual void copyAttributesTo (ParticleObserver* observer);
	};

}
#endif
