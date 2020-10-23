/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/LineAffectorPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
LineAffectorPropertyWindow::LineAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
LineAffectorPropertyWindow::LineAffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
LineAffectorPropertyWindow::LineAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::LineAffector* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void LineAffectorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::LineAffector* affector = static_cast<ParticleUniverse::LineAffector*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_END + PRNL_X ||
		propertyName == PRNL_END + PRNL_Y ||
		propertyName == PRNL_END + PRNL_Z)
	{
		// Line end: Ogre::Vector3
		Ogre::Vector3 v3;
		v3 = doGetVector3(PRNL_END, v3);
		affector->setEnd(v3);
	}
	else if (propertyName == PRNL_MAX_DEVIATION)
	{
		// Maximum Deviation: ParticleUniverse::Real
		affector->setMaxDeviation(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_TIME_STEP)
	{
		// Time Step: ParticleUniverse::Real
		affector->setTimeStep(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_DRIFT)
	{
		// Drift: ParticleUniverse::Real
		affector->setDrift(prop->DoGetValue().GetDouble());
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void LineAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::LineAffector* lineAffector = static_cast<ParticleUniverse::LineAffector*>(affector);

	// Line end: Ogre::Vector3
	doSetVector3(PRNL_END, lineAffector->getEnd());

	// Maximum Deviation: ParticleUniverse::Real
	doSetDouble(PRNL_MAX_DEVIATION, lineAffector->getMaxDeviation());

	// Time Step: ParticleUniverse::Real
	doSetDouble(PRNL_TIME_STEP, lineAffector->getTimeStep());

	// Drift: ParticleUniverse::Real
	doSetDouble(PRNL_DRIFT, lineAffector->getDrift());
}
//-----------------------------------------------------------------------
void LineAffectorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_END = _("End position");
	PRNL_DRIFT = _("Drift");
	PRNL_MAX_DEVIATION = _("Maximum deviation");
	PRNL_TIME_STEP = _("Time step");
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");


	mHelpHtml = wxT("AffectorLine.html");

	// Line end: Ogre::Vector3
	appendVector3(PRNL_END, PRNL_END, ParticleUniverse::LineAffector::DEFAULT_END);

	// Maximum Deviation: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_MAX_DEVIATION, PRNL_MAX_DEVIATION, ParticleUniverse::LineAffector::DEFAULT_MAX_DEVIATION));
	SetPropertyEditor(PRNL_MAX_DEVIATION, wxPG_EDITOR(SpinCtrl));

	// Time Step: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_TIME_STEP, PRNL_TIME_STEP, ParticleUniverse::LineAffector::DEFAULT_TIME_STEP));
	SetPropertyEditor(PRNL_TIME_STEP, wxPG_EDITOR(SpinCtrl));

	// Drift: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_DRIFT, PRNL_DRIFT, ParticleUniverse::LineAffector::DEFAULT_DRIFT));
	SetPropertyEditor(PRNL_DRIFT, wxPG_EDITOR(SpinCtrl));
}
