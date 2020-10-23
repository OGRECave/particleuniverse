/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/AlignAffectorPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
AlignAffectorPropertyWindow::AlignAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
AlignAffectorPropertyWindow::AlignAffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
AlignAffectorPropertyWindow::AlignAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::AlignAffector* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void AlignAffectorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::AlignAffector* affector = static_cast<ParticleUniverse::AlignAffector*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_AFFECTOR_ALIGN_RESIZE)
	{
		affector->setResize(prop->DoGetValue().GetBool());
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void AlignAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::AlignAffector* alignAffector = static_cast<ParticleUniverse::AlignAffector*>(affector);
	
	// Resize: bool
	doSetBool(PRNL_AFFECTOR_ALIGN_RESIZE, alignAffector->isResize());
}
//-----------------------------------------------------------------------
void AlignAffectorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_AFFECTOR_ALIGN_RESIZE = _("Resize");

	mHelpHtml = wxT("AffectorAlign.html");

	// Resize: bool
	Append(wxBoolProperty(PRNL_AFFECTOR_ALIGN_RESIZE, 
		PRNL_AFFECTOR_ALIGN_RESIZE, 
		ParticleUniverse::AlignAffector::DEFAULT_RESIZE));
}