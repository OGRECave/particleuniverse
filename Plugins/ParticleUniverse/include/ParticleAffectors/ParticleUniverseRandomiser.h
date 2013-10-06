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

#ifndef __PU_RANDOMISER_H__
#define __PU_RANDOMISER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseAffector.h"

namespace ParticleUniverse
{
	/** Randomises the position or the direction of a particle.
    */
	class _ParticleUniverseExport Randomiser : public ParticleAffector
	{
		protected:
			Real mMaxDeviationX;
			Real mMaxDeviationY;
			Real mMaxDeviationZ;
			Real mTimeSinceLastUpdate;
			Real mTimeStep;
			bool mRandomDirection;
			bool mUpdate;

		public:
			// Constants
			static const Vector3 DEFAULT_MAX_DEVIATION;
			static const Real DEFAULT_TIME_STEP;
			static const bool DEFAULT_RANDOM_DIRECTION;

			Randomiser(void);
	        virtual ~Randomiser(void) {};

			/** 
	        */
			Real getMaxDeviationX(void) const;
			void setMaxDeviationX(const Real maxDeviationX);

			/** 
	        */
			Real getMaxDeviationY(void) const;
			void setMaxDeviationY(const Real maxDeviationZ);

			/** 
	        */
			Real getMaxDeviationZ(void) const;
			void setMaxDeviationZ(const Real maxDeviationZ);

			/** 
	        */
			Real getTimeStep(void) const;
			void setTimeStep(const Real timeStep);

			/** 
	        */
			bool isRandomDirection(void) const;
			void setRandomDirection(bool randomDirection);

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
