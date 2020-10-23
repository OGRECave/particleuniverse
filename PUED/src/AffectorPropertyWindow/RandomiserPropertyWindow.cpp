/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/RandomiserPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
RandomiserPropertyWindow::RandomiserPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
RandomiserPropertyWindow::RandomiserPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
RandomiserPropertyWindow::RandomiserPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::Randomiser* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void RandomiserPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::Randomiser* affector = static_cast<ParticleUniverse::Randomiser*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_MAX_DEVIATION + PRNL_X ||
		propertyName == PRNL_MAX_DEVIATION + PRNL_Y ||
		propertyName == PRNL_MAX_DEVIATION + PRNL_Z)
	{
		// Max. deviation: Ogre::Vector3
		Ogre::Vector3 v3;
		v3 = doGetVector3(PRNL_MAX_DEVIATION, v3);
		affector->setMaxDeviationX(v3.x);
		affector->setMaxDeviationY(v3.y);
		affector->setMaxDeviationZ(v3.z);
	}
	else if (propertyName == PRNL_RANDOM_DIRECTION)
	{
		// Random direction: bool
		affector->setRandomDirection(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_TIME_STEP)
	{
		// Timestep: ParticleUniverse::Real
		affector->setTimeStep(prop->DoGetValue().GetDouble());
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void RandomiserPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::Randomiser* randomiser = static_cast<ParticleUniverse::Randomiser*>(affector);

	// Max. deviation: Ogre::Vector3
	doSetVector3(PRNL_MAX_DEVIATION, Ogre::Vector3(randomiser->getMaxDeviationX(), 
		randomiser->getMaxDeviationY(),
		randomiser->getMaxDeviationZ()));

	// Random direction: bool
	doSetBool(PRNL_RANDOM_DIRECTION, randomiser->isRandomDirection());

	// Timestep: ParticleUniverse::Real
	doSetDouble(PRNL_TIME_STEP, randomiser->getTimeStep());
}
//-----------------------------------------------------------------------
void RandomiserPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_RANDOM_DIRECTION = _("Random direction");
	PRNL_MAX_DEVIATION = _("Maximum deviation");
	PRNL_TIME_STEP = _("Time step");
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");

	mHelpHtml = wxT("AffectorRandomiser.html");

	// Max. deviation: Ogre::Vector3
	appendVector3(PRNL_MAX_DEVIATION, PRNL_MAX_DEVIATION, ParticleUniverse::Randomiser::DEFAULT_MAX_DEVIATION);

	// Random direction: bool
	Append(wxBoolProperty(PRNL_RANDOM_DIRECTION, PRNL_RANDOM_DIRECTION, ParticleUniverse::Randomiser::DEFAULT_RANDOM_DIRECTION));

	// Timestep: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_TIME_STEP, PRNL_TIME_STEP, ParticleUniverse::Randomiser::DEFAULT_TIME_STEP));
	SetPropertyEditor(PRNL_TIME_STEP, wxPG_EDITOR(SpinCtrl));
}