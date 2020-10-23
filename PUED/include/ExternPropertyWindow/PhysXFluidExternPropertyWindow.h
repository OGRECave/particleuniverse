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

#ifndef __PUED_PHYSX_FLUID_EXTERN_PROPERTY_WINDOW_H__
#define __PUED_PHYSX_FLUID_EXTERN_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseExternPropertyWindow.h"
#include "Externs/ParticleUniversePhysXFluidExtern.h"

// Property names
static wxString PRNL_PHYSX_REST_PARTICLE_PER_METER = wxT("");
static wxString PRNL_PHYSX_REST_DENSITY = wxT("");
static wxString PRNL_PHYSX_KERNEL_RADIUS_MULTIPLIER = wxT("");
static wxString PRNL_PHYSX_MOTION_LIMIT_MULTIPLIER = wxT("");
static wxString PRNL_PHYSX_COLLISION_DISTANCE_MULTIPLIER = wxT("");
static wxString PRNL_PHYSX_PACKET_SIZE_MULTIPLIER = wxT("");
static wxString PRNL_PHYSX_STIFFNESS = wxT("");
static wxString PRNL_PHYSX_VISCOSITY = wxT("");
static wxString PRNL_PHYSX_SURFACE_TENSION = wxT("");
static wxString PRNL_PHYSX_DAMPING = wxT("");
static wxString PRNL_PHYSX_EXTERNAL_ACCELERATION = wxT("");
static wxString PRNL_PHYSX_RESTITUTION_FOR_STATIC_SHAPES = wxT("");
static wxString PRNL_PHYSX_DYNAMIC_FRICTION_FOR_STATIC_SHAPES = wxT("");
static wxString PRNL_PHYSX_STATIC_FRICTION_FOR_STATIC_SHAPES = wxT("");
static wxString PRNL_PHYSX_ATTRACTION_FOR_STATIC_SHAPES = wxT("");
static wxString PRNL_PHYSX_RESTITUTION_FOR_DYNAMIC_SHAPES = wxT("");
static wxString PRNL_PHYSX_DYNAMIC_FRICTION_FOR_DYNAMIC_SHAPES = wxT("");
static wxString PRNL_PHYSX_STATIC_FRICTION_FOR_DYNAMIC_SHAPES = wxT("");
static wxString PRNL_PHYSX_ATTRACTION_FOR_DYNAMIC_SHAPES = wxT("");
static wxString PRNL_PHYSX_COLLISION_RESPONSE_COEFFICIENT = wxT("");
static wxString PRNL_PHYSX_SIMULATION_METHOD = wxT("");
static wxString PRNL_PHYSX_COLLISION_METHOD = wxT("");
static wxString PRNL_PHYSX_FLAG_VISUALIZATION = wxT("");
static wxString PRNL_PHYSX_FLAG_DISABLE_GRAVITY = wxT("");
static wxString PRNL_PHYSX_FLAG_COLLISION_TWOWAY = wxT("");
static wxString PRNL_PHYSX_FLAG_FLUID_ENABLED = wxT("");
static wxString PRNL_PHYSX_FLAG_HARDWARE = wxT("");
static wxString PRNL_PHYSX_FLAG_PRIORITY_MODE = wxT("");
static wxString PRNL_PHYSX_FLAG_PROJECT_TO_PLANE = wxT("");
static wxString PRNL_PHYSX_FLAG_STRICT_COOKING_FORMAT = wxT("");
static wxString PHYSX_INTERCOLLISION = wxT("");
static wxString PHYSX_NOINTERCOLLISION = wxT("");
static wxString PHYSX_MIX_INTERCOLLISION = wxT("");
static wxString PHYSX_STATIC = wxT("");
static wxString PHYSX_DYNAMIC = wxT("");

/**	
*/
class PhysXFluidExternPropertyWindow : public ExternPropertyWindow
{
	public:
		/**
			Constructor: Pass a parent window, the owner component and name
		*/
		PhysXFluidExternPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name);

		/**
			Constructor: Pass an extern window; its settings will be copied.
		*/
		PhysXFluidExternPropertyWindow(ExternPropertyWindow* externPropertyWindow);

		/**
			Constructor: Pass an extern; its settings will be copied.
		*/
		PhysXFluidExternPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::PhysXFluidExtern* externObject);

		/**
			Destructor
		*/
		~PhysXFluidExternPropertyWindow(void){};

		/**
			See ExternPropertyWindow
		*/
		virtual void copyAttributeToExtern(wxPGProperty* prop, wxString propertyName);
		
		/**
			See ExternPropertyWindow
		*/
		virtual void copyAttributesFromExtern(ParticleUniverse::Extern* externObject);

		/**
			See ExternPropertyWindow
		*/
		virtual void onPropertyChanged(wxPropertyGridEvent& event);

	protected:
		/**
			Initialises all box extern properties
		*/
		virtual void _initProperties(void);

};

#endif

