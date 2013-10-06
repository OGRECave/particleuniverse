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

#ifndef __PU_ONTIME_OBSERVER_H__
#define __PU_ONTIME_OBSERVER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseCommon.h"
#include "ParticleUniverseObserver.h"

namespace ParticleUniverse
{
	/** The OnTimeObserver observers how much time has been elapsed. This can be both the particles own time
		and the time since the ParticleSystem was started.
    */
	class _ParticleUniverseExport OnTimeObserver : public ParticleObserver
	{
		protected:
			Real mThreshold;
			ComparisionOperator mCompare;
			bool mSinceStartSystem;

		public:
			// Constants
			static const Real DEFAULT_THRESHOLD;
			static const bool DEFAULT_SINCE_START_SYSTEM;

			OnTimeObserver(void);
	        virtual ~OnTimeObserver(void) {};

			/** In case there are no particles, but the observation returns true, the event handlers must still be
				called.
	        */
			virtual void _preProcessParticles(ParticleTechnique* technique, Real timeElapsed);

			/** 
	        */
			virtual bool _observe (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

			/** 
	        */
			Real getThreshold(void) const {return mThreshold;};
			void setThreshold(Real threshold){mThreshold = threshold;};

			/** 
	        */
			const ComparisionOperator getCompare(void) const {return mCompare;};
			void setCompare(ComparisionOperator op){mCompare = op;};

			/** 
	        */
			bool isSinceStartSystem(void) const {return mSinceStartSystem;};
			void setSinceStartSystem(bool sinceStartSystem){mSinceStartSystem = sinceStartSystem;};

			/** Copy attributes to another observer.
	        */
			virtual void copyAttributesTo (ParticleObserver* observer);
	};

}
#endif
