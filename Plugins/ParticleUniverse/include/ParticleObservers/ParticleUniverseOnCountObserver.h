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

#ifndef __PU_ONCOUNT_OBSERVER_H__
#define __PU_ONCOUNT_OBSERVER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseCommon.h"
#include "ParticleUniverseObserver.h"

namespace ParticleUniverse
{
	/** This class is used to observe if an x number of Particles have been emitted.
	@remarks
		OnCountObserver supports 2 possible situations. If the comparison operator is set to 
		'less than', the _observe() function returns true until the counter exceeds the threshold.
		If the comparison operator is set to 'greater than', the _observe() function only returns 
		true if the counter exceeds the threshold.
    */
	class _ParticleUniverseExport OnCountObserver : public ParticleObserver
	{
		protected:
			uint mCount;
			uint mThreshold;
			ComparisionOperator mCompare;

		public:
			// Constants
			static const uint DEFAULT_THRESHOLD;
			static const ComparisionOperator DEFAULT_COMPARE;

			OnCountObserver(void);
	        virtual ~OnCountObserver(void) {};

			/** 
	        */
			virtual void _notifyStart (void);

			/** 
	        */
			virtual bool _observe (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

			/** 
	        */
			uint getThreshold(void) const {return mThreshold;};
			void setThreshold(uint threshold){mThreshold = threshold;};

			/** 
	        */
			const ComparisionOperator getCompare(void) const {return mCompare;};
			void setCompare(ComparisionOperator op){mCompare = op;};

			/** Copy attributes to another observer.
	        */
			virtual void copyAttributesTo (ParticleObserver* observer);
	};

}
#endif
