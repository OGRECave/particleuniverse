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

#ifndef __PU_LINE_AFFECTOR_H__
#define __PU_LINE_AFFECTOR_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseAffector.h"

namespace ParticleUniverse
{
	/** Affects a particle depending on a line shape. Particles are getting a new position along the line.
    */
	class _ParticleUniverseExport LineAffector : public ParticleAffector
	{
		protected:
			Real mMaxDeviation;
			Real _mScaledMaxDeviation;
			Vector3 mEnd;
			Real mTimeSinceLastUpdate;
			Real mTimeStep;
			Real mDrift;
			Real mOneMinusDrift;
			bool mUpdate;
			bool mFirst;

		public:
			// Constants
			static const Real DEFAULT_MAX_DEVIATION;
			static const Real DEFAULT_TIME_STEP;
			static const Vector3 DEFAULT_END;
			static const Real DEFAULT_DRIFT;
			
			LineAffector(void);
	        virtual ~LineAffector(void) {};

			/** 
	        */
			Real getMaxDeviation(void) const;
			void setMaxDeviation(Real maxDeviation);

			/** 
	        */
			const Vector3& getEnd(void) const;
			void setEnd(const Vector3& end);

			/** 
	        */
			Real getTimeStep(void) const;
			void setTimeStep(Real timeStep);

			/** 
	        */
			Real getDrift(void) const;
			void setDrift(Real drift);

			/**
	        */
			virtual void _notifyRescaled(const Vector3& scale);

			/** 
	        */
			virtual void _firstParticle(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);
			/** 
	        */
			virtual void _preProcessParticles(ParticleTechnique* particleTechnique, Real timeElapsed);

			/** 
	        */
			virtual void _affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

			/** 
	        */
			virtual void _postProcessParticles(ParticleTechnique* technique, Real timeElapsed);

			/** @copydoc ParticleAffector::copyAttributesTo */
			virtual void copyAttributesTo (ParticleAffector* affector);
	};

}
#endif
