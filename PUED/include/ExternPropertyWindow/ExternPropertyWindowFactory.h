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

#ifndef __PUED_EXTERN_PROPERTY_FACTORY_H__
#define __PUED_EXTERN_PROPERTY_FACTORY_H__

#include "wx/ogre/prerequisites.h"
#include "ExternPropertyWindow/BoxColliderExternPropertyWindow.h"
#include "ExternPropertyWindow/GravityExternPropertyWindow.h"
#include "ExternPropertyWindow/SphereColliderExternPropertyWindow.h"
#include "ExternPropertyWindow/VortexExternPropertyWindow.h"

#ifdef PU_PHYSICS_PHYSX
#include "ExternPropertyWindow/PhysXActorExternPropertyWindow.h"
#include "ExternPropertyWindow/PhysXFluidExternPropertyWindow.h"
#endif //PU_PHYSICS_PHYSX

/**	
*/
class ExternPropertyWindowFactory
{
	public:
		/**
			Constructor / Destructor
		*/
		ExternPropertyWindowFactory(void)
		{
			CST_EXTERN_BOX_COLLIDER = ogre2wxTranslate(EXTERN_BOX_COLLIDER);
			CST_EXTERN_SPHERE_COLLIDER = ogre2wxTranslate(EXTERN_SPHERE_COLLIDER);
			CST_EXTERN_GRAVITY = ogre2wxTranslate(EXTERN_GRAVITY);
			CST_EXTERN_PHYSX_ACTOR = ogre2wxTranslate(EXTERN_PHYSX_ACTOR);
			CST_EXTERN_PHYSX_FLUID = ogre2wxTranslate(EXTERN_PHYSX_FLUID);
			CST_EXTERN_VORTEX = ogre2wxTranslate(EXTERN_VORTEX);
		};
		~ExternPropertyWindowFactory(void){};

		/**
			Create a specific Extern Property Window
		*/
		ExternPropertyWindow* createExternPropertyWindow(
			wxWindow* parent, 
			EditComponent* owner, 
			const Ogre::String& name,
			ComponentSubType subType)
		{
			if (subType == CST_UNDEFINED)
			{
				return new ExternPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_EXTERN_BOX_COLLIDER)
			{
				return new BoxColliderExternPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_EXTERN_GRAVITY)
			{
				return new GravityExternPropertyWindow(parent, owner, name);
			}
#ifdef PU_PHYSICS_PHYSX
			else if (subType == CST_EXTERN_PHYSX_ACTOR)
			{
				return new PhysXActorExternPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_EXTERN_PHYSX_FLUID)
			{
				return new PhysXFluidExternPropertyWindow(parent, owner, name);
			}
#endif //PU_PHYSICS_PHYSX
			else if (subType == CST_EXTERN_SPHERE_COLLIDER)
			{
				return new SphereColliderExternPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_EXTERN_VORTEX)
			{
				return new VortexExternPropertyWindow(parent, owner, name);
			}
			else
			{
				return new ExternPropertyWindow(parent, owner, name);
			}
			return 0;
		};

		/**
			Create a specific Extern Property Window and propagate the attributes from the ExternPropertyWindow
		*/
		ExternPropertyWindow* createExternPropertyWindow(ComponentSubType subType, ExternPropertyWindow* externPropertyWindow)
		{
			if (subType == CST_UNDEFINED)
			{
				return new ExternPropertyWindow(externPropertyWindow);
			}
			else if (subType == CST_EXTERN_BOX_COLLIDER)
			{
				return new BoxColliderExternPropertyWindow(externPropertyWindow);
			}
			else if (subType == CST_EXTERN_GRAVITY)
			{
				return new GravityExternPropertyWindow(externPropertyWindow);
			}
#ifdef PU_PHYSICS_PHYSX
			else if (subType == CST_EXTERN_PHYSX_ACTOR)
			{
				return new PhysXActorExternPropertyWindow(externPropertyWindow);
			}
			else if (subType == CST_EXTERN_PHYSX_FLUID)
			{
				return new PhysXFluidExternPropertyWindow(externPropertyWindow);
			}
#endif //PU_PHYSICS_PHYSX
			else if (subType == CST_EXTERN_SPHERE_COLLIDER)
			{
				return new SphereColliderExternPropertyWindow(externPropertyWindow);
			}
			else if (subType == CST_EXTERN_VORTEX)
			{
				return new VortexExternPropertyWindow(externPropertyWindow);
			}
			else
			{
				return new ExternPropertyWindow(externPropertyWindow);
			}
			return 0;
		};
};

#endif
