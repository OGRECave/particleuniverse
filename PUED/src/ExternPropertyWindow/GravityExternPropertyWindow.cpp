/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ExternPropertyWindow/GravityExternPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "AffectorPropertyWindow/GravityAffectorPropertyWindow.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
GravityExternPropertyWindow::GravityExternPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ExternPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
GravityExternPropertyWindow::GravityExternPropertyWindow(ExternPropertyWindow* externPropertyWindow) :
	ExternPropertyWindow(externPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
GravityExternPropertyWindow::GravityExternPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::GravityExtern* externObject) :
	ExternPropertyWindow(parent, owner, (static_cast<ParticleUniverse::Extern*>(externObject))->getName())
{
	copyAttributesFromExtern(externObject);
}
//-----------------------------------------------------------------------
void GravityExternPropertyWindow::copyAttributesFromExtern(ParticleUniverse::Extern* externObject)
{
	ExternPropertyWindow::copyAttributesFromExtern(externObject);

	// Copy properties from Extern to property window
	ParticleUniverse::GravityExtern* gravityExtern = static_cast<ParticleUniverse::GravityExtern*>(externObject);

	// Distance Threshold: ParticleUniverse::Real
	doSetDouble(PRNL_EXTERN_THRESHOLD, gravityExtern->getDistanceThreshold());

	// Gravity: ParticleUniverse::Real
	doSetDouble(PRNL_GRAVITY, gravityExtern->getGravity());
}
//-----------------------------------------------------------------------
void GravityExternPropertyWindow::copyAttributeToExtern(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::Extern* ext = static_cast<ParticleUniverse::Extern*>(mOwner->getPUElement());
	ParticleUniverse::GravityExtern* externObject = static_cast<ParticleUniverse::GravityExtern*>(ext);

	if (propertyName == PRNL_EXTERN_THRESHOLD)
	{
		// Distance Threshold: ParticleUniverse::Real
		ParticleUniverse::Attachable* attachable = static_cast<ParticleUniverse::Attachable*>(mOwner->getPUElement());
		attachable->setDistanceThreshold(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_GRAVITY)
	{
		// Gravity: ParticleUniverse::Real
		externObject->setGravity(prop->DoGetValue().GetDouble());
	}
	else
	{
		// Update extern with another attribute
		ExternPropertyWindow::copyAttributeToExtern(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void GravityExternPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	// TODO
	PRNL_EXTERN_THRESHOLD = _("Distance threshold");

	mHelpHtml = wxT("ExternGravity.html");

	// Distance Threshold: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_EXTERN_THRESHOLD, PRNL_EXTERN_THRESHOLD, std::numeric_limits<float>::max()));
	SetPropertyEditor(PRNL_EXTERN_THRESHOLD, wxPG_EDITOR(SpinCtrl));

	// Gravity: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_GRAVITY, PRNL_GRAVITY, ParticleUniverse::GravityAffector::DEFAULT_GRAVITY));
	SetPropertyEditor(PRNL_GRAVITY, wxPG_EDITOR(SpinCtrl));
}