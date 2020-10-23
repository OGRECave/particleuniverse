/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/FlockCenteringAffectorPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
FlockCenteringAffectorPropertyWindow::FlockCenteringAffectorPropertyWindow(
	wxWindow* parent, 
	EditComponent* owner, 
	const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
FlockCenteringAffectorPropertyWindow::FlockCenteringAffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
FlockCenteringAffectorPropertyWindow::FlockCenteringAffectorPropertyWindow(
	wxWindow* parent, 
	EditComponent* owner, 
	ParticleUniverse::FlockCenteringAffector* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void FlockCenteringAffectorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	// Update affector with another attribute
	AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
}
//-----------------------------------------------------------------------
void FlockCenteringAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// There are no own properties
}
//-----------------------------------------------------------------------
void FlockCenteringAffectorPropertyWindow::_initProperties(void)
{
	mHelpHtml = wxT("AffectorFlockCentering.html");

	// There are no own properties
}