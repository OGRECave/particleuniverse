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

#ifndef __PU_ONRANDOM_OBSERVER_H__
#define __PU_ONRANDOM_OBSERVER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseCommon.h"
#include "ParticleUniverseObserver.h"

namespace ParticleUniverse
{
	/** This class is used to observe whether a random generated number is within a specified interval.
    */
	class _ParticleUniverseExport OnRandomObserver : public ParticleObserver
	{
		protected:
			Real mThreshold; // Value between 0..1

		public:
			// Constants
			static const Real DEFAULT_THRESHOLD;

			OnRandomObserver(void);
	        virtual ~OnRandomObserver(void) {};

			/** See ParticleObserver::_preProcessParticles()
	        */
			virtual void _preProcessParticles(ParticleTechnique* technique, Real timeElapsed);

			/** See ParticleObserver::_processParticle()
	        */
			virtual void _processParticle(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed, bool firstParticle);

			/** 
	        */
			virtual bool _observe (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

			/** 
	        */
			Real getThreshold(void) const {return mThreshold;};
			void setThreshold(Real threshold){mThreshold = threshold;};

			/** Copy attributes to another observer.
	        */
			virtual void copyAttributesTo (ParticleObserver* observer);
	};

}
#endif
