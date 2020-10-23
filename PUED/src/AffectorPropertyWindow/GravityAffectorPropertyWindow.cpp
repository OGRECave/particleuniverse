/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/GravityAffectorPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
GravityAffectorPropertyWindow::GravityAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
GravityAffectorPropertyWindow::GravityAffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
GravityAffectorPropertyWindow::GravityAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::GravityAffector* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void GravityAffectorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::GravityAffector* affector = static_cast<ParticleUniverse::GravityAffector*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_GRAVITY)
	{
		// Gravity: ParticleUniverse::Real
		affector->setGravity(prop->DoGetValue().GetDouble());
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void GravityAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::GravityAffector* gravityAffector = static_cast<ParticleUniverse::GravityAffector*>(affector);

	// Gravity: ParticleUniverse::Real
	doSetDouble(PRNL_GRAVITY, gravityAffector->getGravity());
}
//-----------------------------------------------------------------------
void GravityAffectorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_GRAVITY = _("Gravity");

	mHelpHtml = wxT("AffectorGravity.html");

	// Gravity: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_GRAVITY, PRNL_GRAVITY, ParticleUniverse::GravityAffector::DEFAULT_GRAVITY));
	SetPropertyEditor(PRNL_GRAVITY, wxPG_EDITOR(SpinCtrl));
}