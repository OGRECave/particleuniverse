/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/ParticleFollowerPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
ParticleFollowerPropertyWindow::ParticleFollowerPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
ParticleFollowerPropertyWindow::ParticleFollowerPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
ParticleFollowerPropertyWindow::ParticleFollowerPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::ParticleFollower* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void ParticleFollowerPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::ParticleFollower* affector = static_cast<ParticleUniverse::ParticleFollower*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_MIN_DISTANCE)
	{
		// Min distance: ParticleUniverse::Real
		affector->setMinDistance(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_MAX_DISTANCE)
	{
		// Max distance: ParticleUniverse::Real
		affector->setMaxDistance(prop->DoGetValue().GetDouble());
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void ParticleFollowerPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::ParticleFollower* particleFollower = static_cast<ParticleUniverse::ParticleFollower*>(affector);

	// Min distance: ParticleUniverse::Real
	doSetDouble(PRNL_MIN_DISTANCE, particleFollower->getMinDistance());

	// Max distance: ParticleUniverse::Real
	doSetDouble(PRNL_MAX_DISTANCE, particleFollower->getMaxDistance());
}
//-----------------------------------------------------------------------
void ParticleFollowerPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_MIN_DISTANCE = _("Minimum distance");
	PRNL_MAX_DISTANCE = _("Maximum distance");

	mHelpHtml = wxT("AffectorParticleFollower.html");

	// Min distance: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_MIN_DISTANCE, PRNL_MIN_DISTANCE, ParticleUniverse::ParticleFollower::DEFAULT_MIN_DISTANCE));
	SetPropertyEditor(PRNL_MIN_DISTANCE, wxPG_EDITOR(SpinCtrl));

	// Max distance: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_MAX_DISTANCE, PRNL_MAX_DISTANCE, ParticleUniverse::ParticleFollower::DEFAULT_MAX_DISTANCE));
	SetPropertyEditor(PRNL_MAX_DISTANCE, wxPG_EDITOR(SpinCtrl));
}