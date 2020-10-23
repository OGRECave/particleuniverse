/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"

#ifdef PU_PHYSICS_PHYSX

#include "ExternPropertyWindow/PhysXFluidExternPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "Externs/ParticleUniversePhysXMath.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
PhysXFluidExternPropertyWindow::PhysXFluidExternPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ExternPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
PhysXFluidExternPropertyWindow::PhysXFluidExternPropertyWindow(ExternPropertyWindow* externPropertyWindow) :
	ExternPropertyWindow(externPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
PhysXFluidExternPropertyWindow::PhysXFluidExternPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::PhysXFluidExtern* externObject) :
	ExternPropertyWindow(parent, owner, (static_cast<ParticleUniverse::Extern*>(externObject))->getName())
{
	copyAttributesFromExtern(externObject);
}
//-----------------------------------------------------------------------
void PhysXFluidExternPropertyWindow::copyAttributeToExtern(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::Extern* ext = static_cast<ParticleUniverse::Extern*>(mOwner->getPUElement());
	ParticleUniverse::PhysXFluidExtern* externObject = static_cast<ParticleUniverse::PhysXFluidExtern*>(ext);
	NxFluidDesc nxFluidDesc = externObject->mFluidDesc;

	if (propertyName == PRNL_PHYSX_REST_PARTICLE_PER_METER)
	{
		// Rest particle per meter: NxReal
		externObject->mFluidDesc.restParticlesPerMeter = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_REST_DENSITY)
	{
		// Rest density: NxReal
		externObject->mFluidDesc.restDensity = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_KERNEL_RADIUS_MULTIPLIER)
	{
		// Kernel radius multiplier: NxReal
		externObject->mFluidDesc.kernelRadiusMultiplier = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_MOTION_LIMIT_MULTIPLIER)
	{
		// Motion limit multiplier: NxReal
		externObject->mFluidDesc.motionLimitMultiplier = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_COLLISION_DISTANCE_MULTIPLIER)
	{
		// Collision distance multiplier: NxReal
		externObject->mFluidDesc.collisionDistanceMultiplier = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_PACKET_SIZE_MULTIPLIER)
	{
		// Packet size multiplier: NxU32
		externObject->mFluidDesc.packetSizeMultiplier = prop->DoGetValue().GetLong();
	}
	else if (propertyName == PRNL_PHYSX_STIFFNESS)
	{
		// Stiffness: NxReal
		externObject->mFluidDesc.stiffness = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_VISCOSITY)
	{
		// Viscosity: NxReal
		externObject->mFluidDesc.viscosity = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_SURFACE_TENSION)
	{
		// Surface tension: NxReal
		externObject->mFluidDesc.surfaceTension = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_DAMPING)
	{
		// Damping: NxReal
		externObject->mFluidDesc.damping = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_EXTERNAL_ACCELERATION + PRNL_X ||
		propertyName == PRNL_PHYSX_EXTERNAL_ACCELERATION + PRNL_Y ||
		propertyName == PRNL_PHYSX_EXTERNAL_ACCELERATION + PRNL_Z)
	{
		// External acceleration: NxVec3
		Ogre::Vector3 v3;
		v3 = doGetVector3(PRNL_PHYSX_EXTERNAL_ACCELERATION, v3);
		externObject->mFluidDesc.externalAcceleration = ParticleUniverse::PhysXMath::convert(v3);
	}
	else if (propertyName == PRNL_PHYSX_RESTITUTION_FOR_STATIC_SHAPES)
	{
		// Restitution for static shapes: NxReal
		externObject->mFluidDesc.restitutionForStaticShapes = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_DYNAMIC_FRICTION_FOR_STATIC_SHAPES)
	{
		// Dynamic friction for static shapes: NxReal
		externObject->mFluidDesc.dynamicFrictionForStaticShapes = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_STATIC_FRICTION_FOR_STATIC_SHAPES)
	{
		// Static friction for static shapes: NxReal
		externObject->mFluidDesc.staticFrictionForStaticShapes = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_ATTRACTION_FOR_STATIC_SHAPES)
	{
		// Attraction for static shapes: NxReal
		externObject->mFluidDesc.attractionForStaticShapes = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_RESTITUTION_FOR_DYNAMIC_SHAPES)
	{
		// Restitution for dynamic shapes: NxReal
		externObject->mFluidDesc.restitutionForDynamicShapes = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_DYNAMIC_FRICTION_FOR_DYNAMIC_SHAPES)
	{
		// Dynamic friction for dynamic shapes: NxReal
		externObject->mFluidDesc.dynamicFrictionForDynamicShapes = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_STATIC_FRICTION_FOR_DYNAMIC_SHAPES)
	{
		// Static friction for dynamic shapes: NxReal
		externObject->mFluidDesc.staticFrictionForDynamicShapes = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_ATTRACTION_FOR_DYNAMIC_SHAPES)
	{
		// Attraction for dynamic shapes: NxReal
		externObject->mFluidDesc.attractionForDynamicShapes = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_COLLISION_RESPONSE_COEFFICIENT)
	{
		// Collision response coefficient: NxReal
		externObject->mFluidDesc.collisionResponseCoefficient = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_COLLISION_GROUP)
	{
		// Collision group: NxCollisionGroup
		externObject->mFluidDesc.collisionGroup = prop->DoGetValue().GetLong();
	}
	else if (propertyName == PRNL_PHYSX_SIMULATION_METHOD)
	{
		// Simulation method: List
		wxString simulationMethod = prop->GetValueAsString();
		if (simulationMethod == PHYSX_INTERCOLLISION)
		{
			externObject->mFluidDesc.simulationMethod = NX_F_SPH;
		}
		else if (simulationMethod == PHYSX_NOINTERCOLLISION)
		{
			externObject->mFluidDesc.simulationMethod = NX_F_NO_PARTICLE_INTERACTION;
		}
		else if (simulationMethod == PHYSX_MIX_INTERCOLLISION)
		{
			externObject->mFluidDesc.simulationMethod = NX_F_MIXED_MODE;
		}
	}
	else if (propertyName == PRNL_PHYSX_COLLISION_METHOD)
	{
		// Collision method: List
		wxString collisionMethod = prop->GetValueAsString();
		if (collisionMethod == PHYSX_STATIC)
		{
			externObject->mFluidDesc.collisionMethod = NX_F_STATIC;
		}
		else if (collisionMethod == PHYSX_DYNAMIC)
		{
			externObject->mFluidDesc.collisionMethod = NX_F_DYNAMIC;
		}
	}
	else if (propertyName == PRNL_PHYSX_FLAG_VISUALIZATION)
	{
		// Visualisation: bool
		bool val =  prop->DoGetValue().GetBool();
		if (val)
		{
			externObject->mFluidDesc.flags |= NX_FF_VISUALIZATION;
		}
		else
		{
			externObject->mFluidDesc.flags &= ~NX_FF_VISUALIZATION;
		}
	}
	else if (propertyName == PRNL_PHYSX_FLAG_DISABLE_GRAVITY)
	{
		// Disable gravity: bool
		bool val =  prop->DoGetValue().GetBool();
		if (val)
		{
			externObject->mFluidDesc.flags |= NX_FF_DISABLE_GRAVITY;
		}
		else
		{
			externObject->mFluidDesc.flags &= ~NX_FF_DISABLE_GRAVITY;
		}
	}
	else if (propertyName == PRNL_PHYSX_FLAG_COLLISION_TWOWAY)
	{
		// Collision twoway: bool
		bool val =  prop->DoGetValue().GetBool();
		if (val)
		{
			externObject->mFluidDesc.flags |= NX_FF_COLLISION_TWOWAY;
		}
		else
		{
			externObject->mFluidDesc.flags &= ~NX_FF_COLLISION_TWOWAY;
		}
	}
	else if (propertyName == PRNL_PHYSX_FLAG_FLUID_ENABLED)
	{
		// Fluid enabled: bool
		bool val =  prop->DoGetValue().GetBool();
		if (val)
		{
			externObject->mFluidDesc.flags |= NX_FF_ENABLED;
		}
		else
		{
			externObject->mFluidDesc.flags &= ~NX_FF_ENABLED;
		}
	}
	else if (propertyName == PRNL_PHYSX_FLAG_HARDWARE)
	{
		// Hardware: bool
		bool val =  prop->DoGetValue().GetBool();
		if (val)
		{
			externObject->mFluidDesc.flags |= NX_FF_HARDWARE;
		}
		else
		{
			externObject->mFluidDesc.flags &= ~NX_FF_HARDWARE;
		}
	}
	else if (propertyName == PRNL_PHYSX_FLAG_PRIORITY_MODE)
	{
		// Priority mode: bool
		bool val =  prop->DoGetValue().GetBool();
		if (val)
		{
			externObject->mFluidDesc.flags |= NX_FF_PRIORITY_MODE;
		}
		else
		{
			externObject->mFluidDesc.flags &= ~NX_FF_PRIORITY_MODE;
		}
	}
	else if (propertyName == PRNL_PHYSX_FLAG_PROJECT_TO_PLANE)
	{
		// Project to plane: bool
		bool val =  prop->DoGetValue().GetBool();
		if (val)
		{
			externObject->mFluidDesc.flags |= NX_FF_PROJECT_TO_PLANE;
		}
		else
		{
			externObject->mFluidDesc.flags &= ~NX_FF_PROJECT_TO_PLANE;
		}
	}
	else if (propertyName == PRNL_PHYSX_FLAG_STRICT_COOKING_FORMAT)
	{
		// Strict cooking format: bool
		bool val =  prop->DoGetValue().GetBool();
		if (val)
		{
			externObject->mFluidDesc.flags |= NX_FF_FORCE_STRICT_COOKING_FORMAT;
		}
		else
		{
			externObject->mFluidDesc.flags &= ~NX_FF_FORCE_STRICT_COOKING_FORMAT;
		}
	}
	else
	{
		// Update extern with another attribute
		ExternPropertyWindow::copyAttributeToExtern(prop, propertyName);
	}

	if (!externObject->mFluidDesc.isValid())
	{
		// Reset to original value if not OK, to prevent that the editor crashes. This will leave the property in an incorrect state (value).
		externObject->mFluidDesc = nxFluidDesc;
	}
}
//-----------------------------------------------------------------------
void PhysXFluidExternPropertyWindow::copyAttributesFromExtern(ParticleUniverse::Extern* externObject)
{
	ExternPropertyWindow::copyAttributesFromExtern(externObject);

	// Copy properties from Extern to property window
	ParticleUniverse::PhysXFluidExtern* physXFluidExtern = static_cast<ParticleUniverse::PhysXFluidExtern*>(externObject);

	// Rest particle per meter: NxReal
	doSetDouble(PRNL_PHYSX_REST_PARTICLE_PER_METER, physXFluidExtern->mFluidDesc.restParticlesPerMeter);

	// Rest density: NxReal
	doSetDouble(PRNL_PHYSX_REST_DENSITY, physXFluidExtern->mFluidDesc.restDensity);

	// Kernel radius multiplier: NxReal
	doSetDouble(PRNL_PHYSX_KERNEL_RADIUS_MULTIPLIER, physXFluidExtern->mFluidDesc.kernelRadiusMultiplier);

	// Motion limit multiplier: NxReal
	doSetDouble(PRNL_PHYSX_MOTION_LIMIT_MULTIPLIER, physXFluidExtern->mFluidDesc.motionLimitMultiplier);

	// Collision distance multiplier: NxReal
	doSetDouble(PRNL_PHYSX_COLLISION_DISTANCE_MULTIPLIER, physXFluidExtern->mFluidDesc.collisionDistanceMultiplier);

	// Packet size multiplier: NxU32
	doSetLong(PRNL_PHYSX_PACKET_SIZE_MULTIPLIER, physXFluidExtern->mFluidDesc.packetSizeMultiplier);

	// Stiffness: NxReal
	doSetDouble(PRNL_PHYSX_STIFFNESS, physXFluidExtern->mFluidDesc.stiffness);

	// Viscosity: NxReal
	doSetDouble(PRNL_PHYSX_VISCOSITY, physXFluidExtern->mFluidDesc.viscosity);

	// Surface tension: NxReal
	doSetDouble(PRNL_PHYSX_SURFACE_TENSION, physXFluidExtern->mFluidDesc.surfaceTension);

	// Damping: NxReal
	doSetDouble(PRNL_PHYSX_DAMPING, physXFluidExtern->mFluidDesc.damping);
	
	// External acceleration: NxVec3
	doSetVector3(PRNL_PHYSX_EXTERNAL_ACCELERATION, ParticleUniverse::PhysXMath::convert(physXFluidExtern->mFluidDesc.externalAcceleration));

	// Restitution for static shapes: NxReal
	doSetDouble(PRNL_PHYSX_RESTITUTION_FOR_STATIC_SHAPES, physXFluidExtern->mFluidDesc.restitutionForStaticShapes);

	// Dynamic friction for static shapes: NxReal
	doSetDouble(PRNL_PHYSX_DYNAMIC_FRICTION_FOR_STATIC_SHAPES, physXFluidExtern->mFluidDesc.dynamicFrictionForStaticShapes);

	// Static friction for static shapes: NxReal
	doSetDouble(PRNL_PHYSX_STATIC_FRICTION_FOR_STATIC_SHAPES, physXFluidExtern->mFluidDesc.staticFrictionForStaticShapes);

	// Attraction for static shapes: NxReal
	doSetDouble(PRNL_PHYSX_ATTRACTION_FOR_STATIC_SHAPES, physXFluidExtern->mFluidDesc.attractionForStaticShapes);

	// Restitution for dynamic shapes: NxReal
	doSetDouble(PRNL_PHYSX_RESTITUTION_FOR_DYNAMIC_SHAPES, physXFluidExtern->mFluidDesc.restitutionForDynamicShapes);

	// Dynamic friction for dynamic shapes: NxReal
	doSetDouble(PRNL_PHYSX_DYNAMIC_FRICTION_FOR_DYNAMIC_SHAPES, physXFluidExtern->mFluidDesc.dynamicFrictionForDynamicShapes);

	// Static friction for dynamic shapes: NxReal
	doSetDouble(PRNL_PHYSX_STATIC_FRICTION_FOR_DYNAMIC_SHAPES, physXFluidExtern->mFluidDesc.staticFrictionForDynamicShapes);

	// Attraction for dynamic shapes: NxReal
	doSetDouble(PRNL_PHYSX_ATTRACTION_FOR_DYNAMIC_SHAPES, physXFluidExtern->mFluidDesc.attractionForDynamicShapes);

	// Collision response coefficient: NxReal
	doSetDouble(PRNL_PHYSX_COLLISION_RESPONSE_COEFFICIENT, physXFluidExtern->mFluidDesc.collisionResponseCoefficient);

	// Collision group: NxCollisionGroup
	doSetLong(PRNL_PHYSX_COLLISION_GROUP, physXFluidExtern->mFluidDesc.collisionGroup);

	// Simulation method: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_PHYSX_SIMULATION_METHOD);
	NxU32 simulationMethod = physXFluidExtern->mFluidDesc.simulationMethod;
	wxString simulationMethodString = PHYSX_INTERCOLLISION;
	if (simulationMethod == NX_F_NO_PARTICLE_INTERACTION)
	{
		simulationMethodString = PHYSX_NOINTERCOLLISION;
	}
	else if (simulationMethod == NX_F_NO_PARTICLE_INTERACTION)
	{
		simulationMethodString = PHYSX_MIX_INTERCOLLISION;
	}
	propTo->SetValueFromString(simulationMethodString);

	// Collision method: List
	propTo = GetPropertyPtr(PRNL_PHYSX_COLLISION_METHOD);
	NxU32 collisionMethod = physXFluidExtern->mFluidDesc.collisionMethod;
	wxString collisionMethodString = PHYSX_STATIC;
	if (collisionMethod == NX_F_DYNAMIC)
	{
		collisionMethodString = PHYSX_DYNAMIC;
	}
	propTo->SetValueFromString(simulationMethodString);

	// Visualisation: bool
	doSetBool(PRNL_PHYSX_FLAG_VISUALIZATION, (physXFluidExtern->mFluidDesc.flags & NX_FF_VISUALIZATION));

	// Disable gravity: bool
	doSetBool(PRNL_PHYSX_FLAG_DISABLE_GRAVITY, (physXFluidExtern->mFluidDesc.flags & NX_FF_DISABLE_GRAVITY));

	// Collision twoway: bool
	doSetBool(PRNL_PHYSX_FLAG_COLLISION_TWOWAY, (physXFluidExtern->mFluidDesc.flags & NX_FF_COLLISION_TWOWAY));

	// Fluid enabled: bool
	doSetBool(PRNL_PHYSX_FLAG_FLUID_ENABLED, (physXFluidExtern->mFluidDesc.flags & NX_FF_ENABLED));

	// Hardware: bool
	doSetBool(PRNL_PHYSX_FLAG_HARDWARE, (physXFluidExtern->mFluidDesc.flags & NX_FF_HARDWARE));

	// Priority mode: bool
	doSetBool(PRNL_PHYSX_FLAG_PRIORITY_MODE, (physXFluidExtern->mFluidDesc.flags & NX_FF_PRIORITY_MODE));

	// Project to plane: bool
	doSetBool(PRNL_PHYSX_FLAG_PROJECT_TO_PLANE, (physXFluidExtern->mFluidDesc.flags & NX_FF_PROJECT_TO_PLANE));

	// Strict cooking format: bool
	doSetBool(PRNL_PHYSX_FLAG_STRICT_COOKING_FORMAT, (physXFluidExtern->mFluidDesc.flags & NX_FF_FORCE_STRICT_COOKING_FORMAT));
}
//-----------------------------------------------------------------------
void PhysXFluidExternPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_PHYSX_REST_PARTICLE_PER_METER = _("Rest particle per meter");
	PRNL_PHYSX_REST_DENSITY = _("Rest density");
	PRNL_PHYSX_KERNEL_RADIUS_MULTIPLIER = _("Kernel radius multiplier");
	PRNL_PHYSX_MOTION_LIMIT_MULTIPLIER = _("Motion limit multiplier");
	PRNL_PHYSX_COLLISION_DISTANCE_MULTIPLIER = _("Collision dist. multiplier");
	PRNL_PHYSX_PACKET_SIZE_MULTIPLIER = _("Packet size multiplier");
	PRNL_PHYSX_STIFFNESS = _("Stiffness");
	PRNL_PHYSX_VISCOSITY = _("Viscosity");
	PRNL_PHYSX_SURFACE_TENSION = _("Surface tension");
	PRNL_PHYSX_DAMPING = _("Damping");
	PRNL_PHYSX_EXTERNAL_ACCELERATION = _("External acceleration");
	PRNL_PHYSX_RESTITUTION_FOR_STATIC_SHAPES = _("Restitution static shapes");
	PRNL_PHYSX_DYNAMIC_FRICTION_FOR_STATIC_SHAPES = _("Dyn. friction static shapes");
	PRNL_PHYSX_STATIC_FRICTION_FOR_STATIC_SHAPES = _("Static friction static shapes");
	PRNL_PHYSX_ATTRACTION_FOR_STATIC_SHAPES = _("Attraction static shapes");
	PRNL_PHYSX_RESTITUTION_FOR_DYNAMIC_SHAPES = _("Restitution dynamic shapes");
	PRNL_PHYSX_DYNAMIC_FRICTION_FOR_DYNAMIC_SHAPES = _("Dyn. friction dyn. shapes");
	PRNL_PHYSX_STATIC_FRICTION_FOR_DYNAMIC_SHAPES = _("Static friction dyn. shapes");
	PRNL_PHYSX_ATTRACTION_FOR_DYNAMIC_SHAPES = _("Attraction dynamic shapes");
	PRNL_PHYSX_COLLISION_RESPONSE_COEFFICIENT = _("Collision response coeff.");
	PRNL_PHYSX_SIMULATION_METHOD = _("Simulation method");
	PRNL_PHYSX_COLLISION_METHOD = _("Collision method");
	PRNL_PHYSX_FLAG_VISUALIZATION = _("Visualisation");
	PRNL_PHYSX_FLAG_DISABLE_GRAVITY = _("Disable gravity");
	PRNL_PHYSX_FLAG_COLLISION_TWOWAY = _("Collision twoway");
	PRNL_PHYSX_FLAG_FLUID_ENABLED = _("Fluid enabled");
	PRNL_PHYSX_FLAG_HARDWARE = _("Hardware");
	PRNL_PHYSX_FLAG_PRIORITY_MODE = _("Priority mode");
	PRNL_PHYSX_FLAG_PROJECT_TO_PLANE = _("Project to plane");
	PRNL_PHYSX_FLAG_STRICT_COOKING_FORMAT = _("Strict cooking format");
	PHYSX_INTERCOLLISION = _("Intercollision");
	PHYSX_NOINTERCOLLISION = _("Non-intercollision");
	PHYSX_MIX_INTERCOLLISION = _("Mixed intercollision");
	PHYSX_STATIC = _("Static");
	PHYSX_DYNAMIC = _("Dynamic");

	mHelpHtml = wxT("ExternPhysXFluid.html");

	NxFluidDesc fluidDesc;
	fluidDesc.setToDefault();

	// Rest particle per meter: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_REST_PARTICLE_PER_METER, PRNL_PHYSX_REST_PARTICLE_PER_METER, fluidDesc.restParticlesPerMeter));
	SetPropertyEditor(PRNL_PHYSX_REST_PARTICLE_PER_METER, wxPG_EDITOR(SpinCtrl));

	// Rest density: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_REST_DENSITY, PRNL_PHYSX_REST_DENSITY, fluidDesc.restDensity));
	SetPropertyEditor(PRNL_PHYSX_REST_DENSITY, wxPG_EDITOR(SpinCtrl));

	// Kernel radius multiplier: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_KERNEL_RADIUS_MULTIPLIER, PRNL_PHYSX_KERNEL_RADIUS_MULTIPLIER, fluidDesc.kernelRadiusMultiplier));
	SetPropertyEditor(PRNL_PHYSX_KERNEL_RADIUS_MULTIPLIER, wxPG_EDITOR(SpinCtrl));

	// Motion limit multiplier: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_MOTION_LIMIT_MULTIPLIER, PRNL_PHYSX_MOTION_LIMIT_MULTIPLIER, fluidDesc.motionLimitMultiplier));
	SetPropertyEditor(PRNL_PHYSX_MOTION_LIMIT_MULTIPLIER, wxPG_EDITOR(SpinCtrl));

	// Collision distance multiplier: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_COLLISION_DISTANCE_MULTIPLIER, PRNL_PHYSX_COLLISION_DISTANCE_MULTIPLIER, fluidDesc.collisionDistanceMultiplier));
	SetPropertyEditor(PRNL_PHYSX_COLLISION_DISTANCE_MULTIPLIER, wxPG_EDITOR(SpinCtrl));

	// Packet size multiplier: NxU32
	Append(wxUIntProperty(PRNL_PHYSX_PACKET_SIZE_MULTIPLIER, PRNL_PHYSX_PACKET_SIZE_MULTIPLIER, fluidDesc.packetSizeMultiplier));
	SetPropertyEditor(PRNL_PHYSX_PACKET_SIZE_MULTIPLIER, wxPG_EDITOR(SpinCtrl));

	// Stiffness: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_STIFFNESS, PRNL_PHYSX_STIFFNESS, fluidDesc.stiffness));
	SetPropertyEditor(PRNL_PHYSX_STIFFNESS, wxPG_EDITOR(SpinCtrl));

	// Viscosity: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_VISCOSITY, PRNL_PHYSX_VISCOSITY, fluidDesc.viscosity));
	SetPropertyEditor(PRNL_PHYSX_VISCOSITY, wxPG_EDITOR(SpinCtrl));

	// Surface tension: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_SURFACE_TENSION, PRNL_PHYSX_SURFACE_TENSION, fluidDesc.surfaceTension));
	SetPropertyEditor(PRNL_PHYSX_SURFACE_TENSION, wxPG_EDITOR(SpinCtrl));

	// Damping: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_DAMPING, PRNL_PHYSX_DAMPING, fluidDesc.damping));
	SetPropertyEditor(PRNL_PHYSX_DAMPING, wxPG_EDITOR(SpinCtrl));

	// External acceleration: NxVec3
	appendVector3(PRNL_PHYSX_EXTERNAL_ACCELERATION, PRNL_PHYSX_EXTERNAL_ACCELERATION, 
		ParticleUniverse::PhysXMath::convert(fluidDesc.externalAcceleration));

	// Restitution for static shapes: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_RESTITUTION_FOR_STATIC_SHAPES, PRNL_PHYSX_RESTITUTION_FOR_STATIC_SHAPES, fluidDesc.restitutionForStaticShapes));
	SetPropertyEditor(PRNL_PHYSX_RESTITUTION_FOR_STATIC_SHAPES, wxPG_EDITOR(SpinCtrl));

	// Dynamic friction for static shapes: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_DYNAMIC_FRICTION_FOR_STATIC_SHAPES, PRNL_PHYSX_DYNAMIC_FRICTION_FOR_STATIC_SHAPES, fluidDesc.dynamicFrictionForStaticShapes));
	SetPropertyEditor(PRNL_PHYSX_DYNAMIC_FRICTION_FOR_STATIC_SHAPES, wxPG_EDITOR(SpinCtrl));

	// Static friction for static shapes: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_STATIC_FRICTION_FOR_STATIC_SHAPES, PRNL_PHYSX_STATIC_FRICTION_FOR_STATIC_SHAPES, fluidDesc.staticFrictionForStaticShapes));
	SetPropertyEditor(PRNL_PHYSX_STATIC_FRICTION_FOR_STATIC_SHAPES, wxPG_EDITOR(SpinCtrl));

	// Attraction for static shapes: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_ATTRACTION_FOR_STATIC_SHAPES, PRNL_PHYSX_ATTRACTION_FOR_STATIC_SHAPES, fluidDesc.attractionForStaticShapes));
	SetPropertyEditor(PRNL_PHYSX_ATTRACTION_FOR_STATIC_SHAPES, wxPG_EDITOR(SpinCtrl));

	// Restitution for dynamic shapes: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_RESTITUTION_FOR_DYNAMIC_SHAPES, PRNL_PHYSX_RESTITUTION_FOR_DYNAMIC_SHAPES, fluidDesc.restitutionForDynamicShapes));
	SetPropertyEditor(PRNL_PHYSX_RESTITUTION_FOR_DYNAMIC_SHAPES, wxPG_EDITOR(SpinCtrl));

	// Dynamic friction for dynamic shapes: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_DYNAMIC_FRICTION_FOR_DYNAMIC_SHAPES, PRNL_PHYSX_DYNAMIC_FRICTION_FOR_DYNAMIC_SHAPES, fluidDesc.dynamicFrictionForDynamicShapes));
	SetPropertyEditor(PRNL_PHYSX_DYNAMIC_FRICTION_FOR_DYNAMIC_SHAPES, wxPG_EDITOR(SpinCtrl));

	// Static friction for dynamic shapes: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_STATIC_FRICTION_FOR_DYNAMIC_SHAPES, PRNL_PHYSX_STATIC_FRICTION_FOR_DYNAMIC_SHAPES, fluidDesc.staticFrictionForDynamicShapes));
	SetPropertyEditor(PRNL_PHYSX_STATIC_FRICTION_FOR_DYNAMIC_SHAPES, wxPG_EDITOR(SpinCtrl));

	// Attraction for dynamic shapes: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_ATTRACTION_FOR_DYNAMIC_SHAPES, PRNL_PHYSX_ATTRACTION_FOR_DYNAMIC_SHAPES, fluidDesc.attractionForDynamicShapes));
	SetPropertyEditor(PRNL_PHYSX_ATTRACTION_FOR_DYNAMIC_SHAPES, wxPG_EDITOR(SpinCtrl));

	// Collision response coefficient: NxReal
	Append(wxFloatProperty(PRNL_PHYSX_COLLISION_RESPONSE_COEFFICIENT, PRNL_PHYSX_COLLISION_RESPONSE_COEFFICIENT, fluidDesc.collisionResponseCoefficient));
	SetPropertyEditor(PRNL_PHYSX_COLLISION_RESPONSE_COEFFICIENT, wxPG_EDITOR(SpinCtrl));

	// Collision group: NxCollisionGroup
	Append(wxUIntProperty(PRNL_PHYSX_COLLISION_GROUP, PRNL_PHYSX_COLLISION_GROUP, fluidDesc.collisionGroup));
	SetPropertyEditor(PRNL_PHYSX_COLLISION_GROUP, wxPG_EDITOR(SpinCtrl));

	// Simulation method: List
	wxArrayString simulationMethod;
	simulationMethod.Add(PHYSX_INTERCOLLISION);
	simulationMethod.Add(PHYSX_NOINTERCOLLISION);
	simulationMethod.Add(PHYSX_MIX_INTERCOLLISION);
	wxPGId pid = Append(wxEnumProperty(PRNL_PHYSX_SIMULATION_METHOD, PRNL_PHYSX_SIMULATION_METHOD, simulationMethod));

	// Collision method: List
	wxArrayString collisionMethod;
	collisionMethod.Add(PHYSX_STATIC);
	collisionMethod.Add(PHYSX_DYNAMIC);
	wxPGId pid2 = Append(wxEnumProperty(PRNL_PHYSX_COLLISION_METHOD, PRNL_PHYSX_COLLISION_METHOD, collisionMethod));

	// Visualisation: bool
	Append(wxBoolProperty(PRNL_PHYSX_FLAG_VISUALIZATION, PRNL_PHYSX_FLAG_VISUALIZATION, fluidDesc.flags & NX_FF_VISUALIZATION));

	// Disable gravity: bool
	Append(wxBoolProperty(PRNL_PHYSX_FLAG_DISABLE_GRAVITY, PRNL_PHYSX_FLAG_DISABLE_GRAVITY, fluidDesc.flags & NX_FF_DISABLE_GRAVITY));

	// Collision twoway: bool
	Append(wxBoolProperty(PRNL_PHYSX_FLAG_COLLISION_TWOWAY, PRNL_PHYSX_FLAG_COLLISION_TWOWAY, fluidDesc.flags & NX_FF_COLLISION_TWOWAY));

	// Fluid enabled: bool
	Append(wxBoolProperty(PRNL_PHYSX_FLAG_FLUID_ENABLED, PRNL_PHYSX_FLAG_FLUID_ENABLED, fluidDesc.flags & NX_FF_ENABLED));

	// Hardware: bool
	Append(wxBoolProperty(PRNL_PHYSX_FLAG_HARDWARE, PRNL_PHYSX_FLAG_HARDWARE, fluidDesc.flags & NX_FF_HARDWARE));

	// Priority mode: bool
	Append(wxBoolProperty(PRNL_PHYSX_FLAG_PRIORITY_MODE, PRNL_PHYSX_FLAG_PRIORITY_MODE, fluidDesc.flags & NX_FF_PRIORITY_MODE));

	// Project to plane: bool
	Append(wxBoolProperty(PRNL_PHYSX_FLAG_PROJECT_TO_PLANE, PRNL_PHYSX_FLAG_PROJECT_TO_PLANE, fluidDesc.flags & NX_FF_PROJECT_TO_PLANE));

	// Strict cooking format: bool
	Append(wxBoolProperty(PRNL_PHYSX_FLAG_STRICT_COOKING_FORMAT, PRNL_PHYSX_FLAG_STRICT_COOKING_FORMAT, fluidDesc.flags & NX_FF_FORCE_STRICT_COOKING_FORMAT));
}
//-----------------------------------------------------------------------
void PhysXFluidExternPropertyWindow::onPropertyChanged(wxPropertyGridEvent& event)
{
	// Perform additional validations.
	if (!_validatePropertyFloatPositive(event.GetPropertyPtr(), PRNL_PHYSX_STIFFNESS))
		return;
	if (!_validatePropertyFloatPositive(event.GetPropertyPtr(), PRNL_PHYSX_SURFACE_TENSION))
		return;
	if (!_validatePropertyFloatPositive(event.GetPropertyPtr(), PRNL_PHYSX_VISCOSITY))
		return;
	if (!_validatePropertyFloatPositive(event.GetPropertyPtr(), PRNL_PHYSX_DAMPING))
		return;
	if (!_validatePropertyFloatPositive(event.GetPropertyPtr(), PRNL_PHYSX_COLLISION_RESPONSE_COEFFICIENT))
		return;
	if (!_validatePropertyFloatPositive(event.GetPropertyPtr(), PRNL_PHYSX_COLLISION_GROUP))
		return;

	ExternPropertyWindow::onPropertyChanged(event);
}

#endif //PU_PHYSICS_PHYSX