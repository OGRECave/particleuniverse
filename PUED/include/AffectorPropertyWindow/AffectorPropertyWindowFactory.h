/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2014 Henry van Merode. All rights reserved.

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

#ifndef __PUED_AFFECTOR_PROPERTY_FACTORY_H__
#define __PUED_AFFECTOR_PROPERTY_FACTORY_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseAffectorPropertyWindow.h"
#include "AffectorPropertyWindow/AlignAffectorPropertyWindow.h"
#include "AffectorPropertyWindow/BoxColliderPropertyWindow.h"
#include "AffectorPropertyWindow/CollisionAvoidanceAffectorPropertyWindow.h"
#include "AffectorPropertyWindow/ColourAffectorPropertyWindow.h"
#include "AffectorPropertyWindow/ForceFieldAffectorPropertyWindow.h"
#include "AffectorPropertyWindow/FlockCenteringAffectorPropertyWindow.h"
#include "AffectorPropertyWindow/GeometryRotatorPropertyWindow.h"
#include "AffectorPropertyWindow/GravityAffectorPropertyWindow.h"
#include "AffectorPropertyWindow/InterParticleColliderPropertyWindow.h"
#include "AffectorPropertyWindow/JetAffectorPropertyWindow.h"
#include "AffectorPropertyWindow/LineAffectorPropertyWindow.h"
#include "AffectorPropertyWindow/LinearForceAffectorPropertyWindow.h"
#include "AffectorPropertyWindow/ParticleFollowerPropertyWindow.h"
#include "AffectorPropertyWindow/PathFollowerPropertyWindow.h"
#include "AffectorPropertyWindow/PlaneColliderPropertyWindow.h"
#include "AffectorPropertyWindow/RandomiserPropertyWindow.h"
#include "AffectorPropertyWindow/ScaleAffectorPropertyWindow.h"
#include "AffectorPropertyWindow/ScaleVelocityAffectorPropertyWindow.h"
#include "AffectorPropertyWindow/SineForceAffectorPropertyWindow.h"
#include "AffectorPropertyWindow/SphereColliderPropertyWindow.h"
#include "AffectorPropertyWindow/TextureAnimatorPropertyWindow.h"
#include "AffectorPropertyWindow/TextureRotatorPropertyWindow.h"
#include "AffectorPropertyWindow/VelocityMatchingAffectorPropertyWindow.h"
#include "AffectorPropertyWindow/VortexAffectorPropertyWindow.h"

/**	
*/
class AffectorPropertyWindowFactory
{
	public:
		/**
			Constructor / Destructor
		*/
		AffectorPropertyWindowFactory(void)
		{
			// Affector
			CST_AFFECTOR_ALIGN = ogre2wxTranslate(AFFECTOR_ALIGN);
			CST_AFFECTOR_BOX_COLLIDER = ogre2wxTranslate(AFFECTOR_BOX_COLLIDER);
			CST_AFFECTOR_COLLISION_AVOIDANCE = ogre2wxTranslate(AFFECTOR_COLLISION_AVOIDANCE);
			CST_AFFECTOR_COLOUR = ogre2wxTranslate(AFFECTOR_COLOUR);
			CST_AFFECTOR_FLOCK_CENTERING = ogre2wxTranslate(AFFECTOR_FLOCK_CENTERING);
			CST_AFFECTOR_FORCEFIELD = ogre2wxTranslate(AFFECTOR_FORCEFIELD);
			CST_AFFECTOR_GEOMETRY_ROTATOR = ogre2wxTranslate(AFFECTOR_GEOMETRY_ROTATOR);
			CST_AFFECTOR_GRAVITY = ogre2wxTranslate(AFFECTOR_GRAVITY);
			CST_AFFECTOR_INTER_PARTICLE_COLLIDER = ogre2wxTranslate(AFFECTOR_INTER_PARTICLE_COLLIDER);
			CST_AFFECTOR_JET = ogre2wxTranslate(AFFECTOR_JET);
			CST_AFFECTOR_LINE = ogre2wxTranslate(AFFECTOR_LINE);
			CST_AFFECTOR_LINEAR_FORCE = ogre2wxTranslate(AFFECTOR_LINEAR_FORCE);
			CST_AFFECTOR_PARTICLE_FOLLOWER = ogre2wxTranslate(AFFECTOR_PARTICLE_FOLLOWER);
			CST_AFFECTOR_PATH_FOLLOWER = ogre2wxTranslate(AFFECTOR_PATH_FOLLOWER);
			CST_AFFECTOR_PLANE_COLLIDER = ogre2wxTranslate(AFFECTOR_PLANE_COLLIDER);
			CST_AFFECTOR_RANDOMISER = ogre2wxTranslate(AFFECTOR_RANDOMISER);
			CST_AFFECTOR_SCALE = ogre2wxTranslate(AFFECTOR_SCALE);
			CST_AFFECTOR_SCALE_VELOCITY = ogre2wxTranslate(AFFECTOR_SCALE_VELOCITY);
			CST_AFFECTOR_SINE_FORCE = ogre2wxTranslate(AFFECTOR_SINE_FORCE);
			CST_AFFECTOR_SPHERE_COLLIDER = ogre2wxTranslate(AFFECTOR_SPHERE_COLLIDER);
			CST_AFFECTOR_TEXTURE_ANIMATOR = ogre2wxTranslate(AFFECTOR_TEXTURE_ANIMATOR);
			CST_AFFECTOR_TEXTURE_ROTATOR = ogre2wxTranslate(AFFECTOR_TEXTURE_ROTATOR);
			CST_AFFECTOR_VELOCITY_MATCHING = ogre2wxTranslate(AFFECTOR_VELOCITY_MATCHING);
			CST_AFFECTOR_VORTEX = ogre2wxTranslate(AFFECTOR_VORTEX);
		};
		~AffectorPropertyWindowFactory(void){};

		/**
			Create a specific Affector Property Window
		*/
		AffectorPropertyWindow* createAffectorPropertyWindow(
			wxWindow* parent, 
			EditComponent* owner, 
			const Ogre::String& name,
			ComponentSubType subType)
		{
			if (subType == CST_UNDEFINED)
			{
				return new AffectorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_ALIGN)
			{
				return new AlignAffectorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_BOX_COLLIDER)
			{
				return new BoxColliderPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_COLLISION_AVOIDANCE)
			{
				return new CollisionAvoidanceAffectorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_COLOUR)
			{
				return new ColourAffectorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_FLOCK_CENTERING)
			{
				return new FlockCenteringAffectorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_FORCEFIELD)
			{
				return new ForceFieldAffectorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_GEOMETRY_ROTATOR)
			{
				return new GeometryRotatorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_GRAVITY)
			{
				return new GravityAffectorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_INTER_PARTICLE_COLLIDER)
			{
				return new InterParticleColliderPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_JET)
			{
				return new JetAffectorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_LINE)
			{
				return new LineAffectorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_LINEAR_FORCE)
			{
				return new LinearForceAffectorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_PARTICLE_FOLLOWER)
			{
				return new ParticleFollowerPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_PATH_FOLLOWER)
			{
				return new PathFollowerPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_PLANE_COLLIDER)
			{
				return new PlaneColliderPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_RANDOMISER)
			{
				return new RandomiserPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_SCALE)
			{
				return new ScaleAffectorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_SCALE_VELOCITY)
			{
				return new ScaleVelocityAffectorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_SINE_FORCE)
			{
				return new SineForceAffectorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_SPHERE_COLLIDER)
			{
				return new SphereColliderPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_TEXTURE_ANIMATOR)
			{
				return new TextureAnimatorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_TEXTURE_ROTATOR)
			{
				return new TextureRotatorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_VELOCITY_MATCHING)
			{
				return new VelocityMatchingAffectorPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_AFFECTOR_VORTEX)
			{
				return new VortexAffectorPropertyWindow(parent, owner, name);
			}
			else
			{
				return new AffectorPropertyWindow(parent, owner, name);
			}

			return 0;
		};

		/**
			Create a specific Affector Property Window and propagate the attributes from the AffectorPropertyWindow
		*/
		AffectorPropertyWindow* createAffectorPropertyWindow(ComponentSubType subType, AffectorPropertyWindow* affectorPropertyWindow)
		{
			if (subType == CST_UNDEFINED)
			{
				return new AffectorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_ALIGN)
			{
				return new AlignAffectorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_BOX_COLLIDER)
			{
				return new BoxColliderPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_COLLISION_AVOIDANCE)
			{
				return new CollisionAvoidanceAffectorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_COLOUR)
			{
				return new ColourAffectorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_FLOCK_CENTERING)
			{
				return new FlockCenteringAffectorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_FORCEFIELD)
			{
				return new ForceFieldAffectorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_GEOMETRY_ROTATOR)
			{
				return new GeometryRotatorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_GRAVITY)
			{
				return new GravityAffectorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_INTER_PARTICLE_COLLIDER)
			{
				return new InterParticleColliderPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_JET)
			{
				return new JetAffectorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_LINE)
			{
				return new LineAffectorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_LINEAR_FORCE)
			{
				return new LinearForceAffectorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_PARTICLE_FOLLOWER)
			{
				return new ParticleFollowerPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_PATH_FOLLOWER)
			{
				return new PathFollowerPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_PLANE_COLLIDER)
			{
				return new PlaneColliderPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_RANDOMISER)
			{
				return new RandomiserPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_SCALE)
			{
				return new ScaleAffectorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_SCALE_VELOCITY)
			{
				return new ScaleVelocityAffectorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_SINE_FORCE)
			{
				return new SineForceAffectorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_SPHERE_COLLIDER)
			{
				return new SphereColliderPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_TEXTURE_ANIMATOR)
			{
				return new TextureAnimatorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_TEXTURE_ROTATOR)
			{
				return new TextureRotatorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_VELOCITY_MATCHING)
			{
				return new VelocityMatchingAffectorPropertyWindow(affectorPropertyWindow);
			}
			else if (subType == CST_AFFECTOR_VORTEX)
			{
				return new VortexAffectorPropertyWindow(affectorPropertyWindow);
			}
			else
			{
				return new AffectorPropertyWindow(affectorPropertyWindow);
			}
			return 0;
		};
};

#endif

