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

#ifndef __PU_GRAVITY_EXTERN_H__
#define __PU_GRAVITY_EXTERN_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleAffectors/ParticleUniverseGravityAffector.h"
#include "ParticleUniverseAttachable.h"

namespace ParticleUniverse
{
	/** The GravityExtern is a wrapper of the GravityAffector, adding the functionality of a MovableObject.
		The GravityExtern can be attached to another SceneNode than the one where the ParticleSystem at 
		which the GravityExtern is registered, is attached. This makes it possible to affect the particles 
		in the Particle System, while both SceneNodes moves different from each other. This approach makes
		it possible to simulate something like a helicopter (SceneNode to which the GravityExtern is 
		attached) that flies over a certain area and moves the leaves on the ground (Particle System attached
		to another SceneNode).
    */
	class _ParticleUniverseExport GravityExtern : public Attachable, public GravityAffector
	{
		public:
		GravityExtern(void) : 
			Attachable(),
			GravityAffector() {};
	        virtual ~GravityExtern(void) {};

			/** The _preProcessParticles() function sets the position and mDerivedPosition attributes to
				the actual world position of the node to which it is attached.
	        */
			virtual void _preProcessParticles(ParticleTechnique* technique, Real timeElapsed);

			/** Processes a particle.
	        */
			virtual void _interface(ParticleTechnique* technique, 
				Particle* particle, 
				Real timeElapsed);

			/** Copy both the Extern and the derived GravityAffector properties.
	        */
			virtual void copyAttributesTo (Extern* externObject);

		protected:
	};

}
#endif
