/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/CollisionAvoidanceAffectorPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
CollisionAvoidanceAffectorPropertyWindow::CollisionAvoidanceAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
CollisionAvoidanceAffectorPropertyWindow::CollisionAvoidanceAffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
CollisionAvoidanceAffectorPropertyWindow::CollisionAvoidanceAffectorPropertyWindow(
	wxWindow* parent, 
	EditComponent* owner, 
	ParticleUniverse::CollisionAvoidanceAffector* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void CollisionAvoidanceAffectorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::CollisionAvoidanceAffector* affector = static_cast<ParticleUniverse::CollisionAvoidanceAffector*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_COLLISON_AVOIDANCE_RADIUS)
	{
		// Avoidance Radius: ParticleUniverse::Real
		affector->setRadius(prop->DoGetValue().GetDouble());
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void CollisionAvoidanceAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::CollisionAvoidanceAffector* collisionAvoidanceAffector = static_cast<ParticleUniverse::CollisionAvoidanceAffector*>(affector);

	// Avoidance Radius: ParticleUniverse::Real
	doSetDouble(PRNL_COLLISON_AVOIDANCE_RADIUS, collisionAvoidanceAffector->getRadius());
}
//-----------------------------------------------------------------------
void CollisionAvoidanceAffectorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_COLLISON_AVOIDANCE_RADIUS = _("Avoidance radius");

	mHelpHtml = wxT("AffectorCollisionAvoidance.html");

	// Avoidance Radius: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_COLLISON_AVOIDANCE_RADIUS, PRNL_COLLISON_AVOIDANCE_RADIUS, ParticleUniverse::CollisionAvoidanceAffector::DEFAULT_RADIUS));
	SetPropertyEditor(PRNL_COLLISON_AVOIDANCE_RADIUS, wxPG_EDITOR(SpinCtrl));
}