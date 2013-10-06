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

#ifndef __PU_ALIGN_AFFECTOR_H__
#define __PU_ALIGN_AFFECTOR_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseAffector.h"

namespace ParticleUniverse
{
	/** Aligns the orientation of a particle towards the previous particle and adjusts the height of
		the particle, which becomes the length between the two particles. And how do we benefit from 
		this? Well, with the right renderer settings you could get a chain of particles, each connected
		to the previous, making use of the particle orientation.
		
		We get good results with a billboard renderer (which - btw - doesn't take the particle 
		orientation into account by default). Use the billboard type 'oriented shape', which is a type
		that isn't a standard billboard type of Ogre. It has been added to allow the billboard renderer
		take advantage of the particles' orientation. Also use for instance 'bottom center' as the 
		billboard origin, which neatly connects the billboards.

		Using the AlignAffector is a step in the direction to generate electric beams.
    */
	class _ParticleUniverseExport AlignAffector : public ParticleAffector
	{
		protected:
			Particle* mPreviousParticle;
			bool mResize;

		public:
			// Constants
			static const bool DEFAULT_RESIZE;

			AlignAffector(void);
	        virtual ~AlignAffector(void){};

			/** @copydoc ParticleAffector::copyAttributesTo */
			virtual void copyAttributesTo (ParticleAffector* affector);

			/** See setResize().
	        */
			bool isResize(void) const;

			/** Set resize. This attribute determines whether the size of the particle must be changed
				according to its alignment with the previous particle.
	        */
			void setResize(bool resize);

			/** @copydoc ParticleAffector::_firstParticle */
			virtual void _firstParticle(ParticleTechnique* particleTechnique, 
				Particle* particle, 
				Real timeElapsed);

			/** @copydoc ParticleAffector::_affect */
			virtual void _affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);
	};

}
#endif
