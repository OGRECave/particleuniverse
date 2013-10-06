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

#ifndef __PU_PHYSICS_EXTERN_H__
#define __PU_PHYSICS_EXTERN_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseExtern.h"
#include "ParticleUniversePhysicsActor.h"

namespace ParticleUniverse
{
	/** PhysicsExtern is a pure virtual class that must be subclassed. The child class forms the connection between Particle Universe and
		the physics engine.
	*/
	class _ParticleUniverseExport PhysicsExtern : public Extern
	{
		public:
			PhysicsExtern(void) : Extern() {};
			virtual ~PhysicsExtern(void) {};

		/** As soon as a particle is emitted it can be processed by the PhysicsExtern. One of the actions is to assign a PhysicsActor and
			shape to the particle.
		*/
		virtual void _initParticleForEmission(Particle* particle) = 0;

		/** As soon as a particle is expired it can be processed by the PhysicsExtern. One of the actions is to remove a PhysicsActor from
			the particle.
		*/
		virtual void _initParticleForExpiration(Particle* particle) = 0;
	};

}
#endif
