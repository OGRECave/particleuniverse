/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/LinearForceAffectorPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
LinearForceAffectorPropertyWindow::LinearForceAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
LinearForceAffectorPropertyWindow::LinearForceAffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
LinearForceAffectorPropertyWindow::LinearForceAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::LinearForceAffector* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void LinearForceAffectorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::LinearForceAffector* affector = static_cast<ParticleUniverse::LinearForceAffector*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_FORCE_VECTOR + PRNL_X ||
		propertyName == PRNL_FORCE_VECTOR + PRNL_Y ||
		propertyName == PRNL_FORCE_VECTOR + PRNL_Z)
	{
		// Force vector: Ogre::Vector3
		Ogre::Vector3 v3;
		v3 = doGetVector3(PRNL_FORCE_VECTOR, v3);
		affector->setForceVector(v3);
	}
	else if (propertyName == PRNL_LINEAR_FORCE_APPLICATION)
	{
		// Force Application: List
		wxString application = prop->GetValueAsString();
		if (application == APP_ADD)
		{
			affector->setForceApplication(ParticleUniverse::BaseForceAffector::FA_ADD);
		}
		else if (application == APP_AVG)
		{
			affector->setForceApplication(ParticleUniverse::BaseForceAffector::FA_AVERAGE);
		}
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void LinearForceAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::LinearForceAffector* linearForceAffector = static_cast<ParticleUniverse::LinearForceAffector*>(affector);

	// Force vector: Ogre::Vector3
	doSetVector3(PRNL_FORCE_VECTOR, linearForceAffector->getForceVector());

	// Force Application: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_LINEAR_FORCE_APPLICATION);
	ParticleUniverse::BaseForceAffector::ForceApplication application = linearForceAffector->getForceApplication();
	wxString applicationString = APP_ADD;
	if (application == ParticleUniverse::BaseForceAffector::FA_AVERAGE)
	{
		applicationString = APP_AVG;
	}
	propTo->SetValueFromString(applicationString);
}
//-----------------------------------------------------------------------
void LinearForceAffectorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_FORCE_VECTOR = _("Force vector");
	PRNL_LINEAR_FORCE_APPLICATION = _("Application");
	APP_ADD = _("Add");
	APP_AVG = _("Average");
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");

	mHelpHtml = wxT("AffectorLinearForce.html");

	// Force vector: Ogre::Vector3
	appendVector3(PRNL_FORCE_VECTOR, PRNL_FORCE_VECTOR, ParticleUniverse::LinearForceAffector::DEFAULT_FORCE_VECTOR);

	// Force Application: List
	mApplication.Add(APP_ADD);
	mApplication.Add(APP_AVG);
	wxPGId pid = Append(wxEnumProperty(PRNL_LINEAR_FORCE_APPLICATION, PRNL_LINEAR_FORCE_APPLICATION, mApplication));
}