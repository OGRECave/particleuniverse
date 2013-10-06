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

#ifndef __PU_BOX_COLLIDER_EXTERN_H__
#define __PU_BOX_COLLIDER_EXTERN_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleAffectors/ParticleUniverseBoxCollider.h"
#include "ParticleUniverseAttachable.h"

namespace ParticleUniverse
{
	/** The BoxColliderExtern is a wrapper of the BoxCollider, adding the functionality of a MovableObject.
		This makes it possible to let particles collide with a BoxCollider that is attached to a different 
		SceneNode, than the ParticleSystem with which particles it collides.
	*/
	class _ParticleUniverseExport BoxColliderExtern : public Attachable, public BoxCollider
	{
		public:
			BoxColliderExtern(void) : 
				Attachable(),
				BoxCollider() {};
			  virtual ~BoxColliderExtern(void) {};

			/** see Extern::_preProcessParticles */
			virtual void _preProcessParticles(ParticleTechnique* technique, Real timeElapsed);

			/** see Extern::_interface */
			virtual void _interface(ParticleTechnique* technique, 
				Particle* particle, 
				Real timeElapsed);

			/** Copy both the Extern and the derived BoxCollider properties.
			*/
			virtual void copyAttributesTo (Extern* externObject);

		protected:
	};

}
#endif
