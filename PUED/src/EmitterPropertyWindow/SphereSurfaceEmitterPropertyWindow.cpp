/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "EmitterPropertyWindow/SphereSurfaceEmitterPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
SphereSurfaceEmitterPropertyWindow::SphereSurfaceEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	EmitterPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
SphereSurfaceEmitterPropertyWindow::SphereSurfaceEmitterPropertyWindow(EmitterPropertyWindow* emitterPropertyWindow) :
	EmitterPropertyWindow(emitterPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
SphereSurfaceEmitterPropertyWindow::SphereSurfaceEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::SphereSurfaceEmitter* emitter) :
	EmitterPropertyWindow(parent, owner, emitter->getName())
{
	copyAttributesFromEmitter(emitter);
}
//-----------------------------------------------------------------------
void SphereSurfaceEmitterPropertyWindow::copyAttributeToEmitter(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::SphereSurfaceEmitter* sphereSurfaceEmitter = static_cast<ParticleUniverse::SphereSurfaceEmitter*>(mOwner->getPUElement());
	if (!sphereSurfaceEmitter)
		return;

	if (propertyName == PRNL_SPHERE_SURFACE_EMITTER_RADIUS)
	{
		// Update emitter with Radius
		copyRadiusToSphereSurfaceEmitter(prop, sphereSurfaceEmitter);
	}
	else
	{
		// Update emitter with another attribute
		EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void SphereSurfaceEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter* emitter)
{
	EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

	// Copy properties from emitter to property window
	ParticleUniverse::SphereSurfaceEmitter* sphereSurfaceEmitter = static_cast<ParticleUniverse::SphereSurfaceEmitter*>(emitter);

	// Radius: ParticleUniverse::Real
	doSetDouble(PRNL_SPHERE_SURFACE_EMITTER_RADIUS, sphereSurfaceEmitter->getRadius());
}
//-----------------------------------------------------------------------
void SphereSurfaceEmitterPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_SPHERE_SURFACE_EMITTER_RADIUS = _("Sphere Radius");

	mHelpHtml = wxT("EmitterSphereSurface.html");

	// Radius: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_SPHERE_SURFACE_EMITTER_RADIUS, PRNL_SPHERE_SURFACE_EMITTER_RADIUS, ParticleUniverse::SphereSurfaceEmitter::DEFAULT_RADIUS));
	SetPropertyEditor(PRNL_SPHERE_SURFACE_EMITTER_RADIUS, wxPG_EDITOR(SpinCtrl));
}
//-----------------------------------------------------------------------
void SphereSurfaceEmitterPropertyWindow::copyRadiusToSphereSurfaceEmitter(wxPGProperty* prop, ParticleUniverse::SphereSurfaceEmitter* sphereSurfaceEmitter)
{
	sphereSurfaceEmitter->setRadius(prop->DoGetValue().GetDouble());
}