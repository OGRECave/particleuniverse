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

#ifndef __PU_PHYSICS_ACTOR_H__
#define __PU_PHYSICS_ACTOR_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniversePhysicsShape.h"

namespace ParticleUniverse
{
	/** PhysicsActorDesc contains the properties of the actor and is used to create a actor.
	*/
	class _ParticleUniverseExport PhysicsActorDesc
	{
		public:
			// Constructor
			PhysicsActorDesc(void) : 
				position(Vector3::ZERO),
				direction(Vector3::ZERO),
				orientation(Quaternion::IDENTITY),
				mass(1.0f),
				collisionGroup(0){};

			// Destructor
			virtual ~PhysicsActorDesc(void) {}

			// Public attributes
			Vector3 position;
			Vector3 direction;
			Quaternion orientation;
			Real mass;
			uint16 collisionGroup;
	};

	/** A PhysicsActor is an element of the physical world and has no visual representation by itself. It only has physical properties. 
		The PhysicsActor is an abstraction class of the real underlying physics engine, such as PhysX, Havok or Bullet.
	*/
	class _ParticleUniverseExport PhysicsActor
	{
		public:
			// Constructor
			PhysicsActor(void) : 
				position(Vector3::ZERO),
				direction(Vector3::ZERO),
				orientation(Quaternion::IDENTITY),
				mass(1.0f),
				collisionGroup(0),
				contactPoint(Vector3::ZERO),
				contactNormal(Vector3::ZERO){};

			// Destructor
			virtual ~PhysicsActor(void) {};

			// Public attributes
			Vector3 position;
			Vector3 direction;
			Quaternion orientation;
			Real mass;
			uint16 collisionGroup;
			Vector3 contactPoint;
			Vector3 contactNormal;
	};

}
#endif
