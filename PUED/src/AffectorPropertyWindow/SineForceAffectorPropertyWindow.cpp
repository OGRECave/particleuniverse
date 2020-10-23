/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/SineForceAffectorPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
SineForceAffectorPropertyWindow::SineForceAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
SineForceAffectorPropertyWindow::SineForceAffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
SineForceAffectorPropertyWindow::SineForceAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::SineForceAffector* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void SineForceAffectorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::SineForceAffector* affector = static_cast<ParticleUniverse::SineForceAffector*>(mOwner->getPUElement());
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
	else if (propertyName == PRNL_FREQ_MIN)
	{
		// Minimum Frequency: ParticleUniverse::Real
		affector->setFrequencyMin(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_FREQ_MAX)
	{
		// Maximum Frequency: ParticleUniverse::Real
		affector->setFrequencyMax(prop->DoGetValue().GetDouble());
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void SineForceAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::SineForceAffector* sineForceAffector = static_cast<ParticleUniverse::SineForceAffector*>(affector);

	// Force vector: Ogre::Vector3
	doSetVector3(PRNL_FORCE_VECTOR, sineForceAffector->getForceVector());

	// Force Application: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_LINEAR_FORCE_APPLICATION);
	ParticleUniverse::BaseForceAffector::ForceApplication application = sineForceAffector->getForceApplication();
	wxString applicationString = APP_ADD;
	if (application == ParticleUniverse::BaseForceAffector::FA_AVERAGE)
	{
		applicationString = APP_AVG;
	}
	propTo->SetValueFromString(applicationString);

	// Minimum Frequency: ParticleUniverse::Real
	doSetDouble(PRNL_FREQ_MIN, sineForceAffector->getFrequencyMin());

	// Maximum Frequency: ParticleUniverse::Real
	doSetDouble(PRNL_FREQ_MAX, sineForceAffector->getFrequencyMax());
}
//-----------------------------------------------------------------------
void SineForceAffectorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_FREQ_MIN = _("Minimum frequency");
	PRNL_FREQ_MAX = _("Maximum frequency");
	PRNL_FORCE_VECTOR = _("Force vector");
	PRNL_LINEAR_FORCE_APPLICATION = _("Application");
	APP_ADD = _("Add");
	APP_AVG = _("Average");
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");

	mHelpHtml = wxT("AffectorSineForce.html");

	// Force vector: Ogre::Vector3
	appendVector3(PRNL_FORCE_VECTOR, PRNL_FORCE_VECTOR, ParticleUniverse::SineForceAffector::DEFAULT_FORCE_VECTOR);

	// Force Application: 
	mApplication.Add(APP_ADD);
	mApplication.Add(APP_AVG);
	wxPGId pid = Append(wxEnumProperty(PRNL_LINEAR_FORCE_APPLICATION, PRNL_LINEAR_FORCE_APPLICATION, mApplication));

	// Minimum Frequency: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_FREQ_MIN, PRNL_FREQ_MIN, ParticleUniverse::SineForceAffector::DEFAULT_FREQ_MIN));
	SetPropertyEditor(PRNL_FREQ_MIN, wxPG_EDITOR(SpinCtrl));

	// Maximum Frequency: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_FREQ_MAX, PRNL_FREQ_MAX, ParticleUniverse::SineForceAffector::DEFAULT_FREQ_MAX));
	SetPropertyEditor(PRNL_FREQ_MAX, wxPG_EDITOR(SpinCtrl));
}