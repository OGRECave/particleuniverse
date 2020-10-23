/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ObserverPropertyWindow/OnVelocityObserverPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
OnVelocityObserverPropertyWindow::OnVelocityObserverPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ObserverPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnVelocityObserverPropertyWindow::OnVelocityObserverPropertyWindow(ObserverPropertyWindow* observerPropertyWindow) :
	ObserverPropertyWindow(observerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnVelocityObserverPropertyWindow::OnVelocityObserverPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::OnVelocityObserver* observer) :
	ObserverPropertyWindow(parent, owner, observer->getName())
{
	copyAttributesFromObserver(observer);
}
//-----------------------------------------------------------------------
void OnVelocityObserverPropertyWindow::copyAttributeToObserver(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::OnVelocityObserver* observer = static_cast<ParticleUniverse::OnVelocityObserver*>(mOwner->getPUElement());
	if (!observer)
		return;

	if (propertyName == PRNL_ON_VELOCITY_COMPARE)
	{
		// Velocity threshold - compare: List
		wxString compare = prop->GetValueAsString();
		if (compare == PRNL_COMPARE_LESS_THAN)
		{
			observer->setCompare(ParticleUniverse::CO_LESS_THAN);
		}
		else if (compare == PRNL_COMPARE_GREATER_THAN)
		{
			observer->setCompare(ParticleUniverse::CO_GREATER_THAN);
		}
		else if (compare == PRNL_COMPARE_EQUALS)
		{
			observer->setCompare(ParticleUniverse::CO_EQUALS);
		}
	}
	else if (propertyName == PRNL_ON_VELOCITY_THRESHOLD)
	{
		// Velocity threshold - value: ParticleUniverse::Real
		observer->setThreshold(prop->DoGetValue().GetDouble());
	}
	else
	{
		// Update observer with another attribute
		ObserverPropertyWindow::copyAttributeToObserver(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void OnVelocityObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver* observer)
{
	ObserverPropertyWindow::copyAttributesFromObserver(observer);

	// Copy properties from observer to property window
	ParticleUniverse::OnVelocityObserver* onVelocityObserver = static_cast<ParticleUniverse::OnVelocityObserver*>(observer);

	// Velocity threshold - compare: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_ON_VELOCITY_COMPARE);
	ParticleUniverse::ComparisionOperator compare = onVelocityObserver->getCompare();
	wxString compareString = PRNL_COMPARE_LESS_THAN;
	if (compare == ParticleUniverse::CO_GREATER_THAN)
	{
		compareString = PRNL_COMPARE_GREATER_THAN;
	}
	else if (compare == ParticleUniverse::CO_EQUALS)
	{
		compareString = PRNL_COMPARE_EQUALS;
	}
	propTo->SetValueFromString(compareString);

	// Velocity threshold - value: ParticleUniverse::Real
	doSetDouble(PRNL_ON_VELOCITY_THRESHOLD, onVelocityObserver->getThreshold());
}
//-----------------------------------------------------------------------
void OnVelocityObserverPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_ON_VELOCITY_COMPARE = _("Velocity compare");
	PRNL_ON_VELOCITY_THRESHOLD = _("Velocity threshold");
	PRNL_COMPARE_LESS_THAN = _("Less than");
	PRNL_COMPARE_GREATER_THAN = _("Greater than");
	PRNL_COMPARE_EQUALS = _("Equals");

	mHelpHtml = wxT("ObserverOnVelocity.html");

	// Velocity threshold - compare: List
	mCompare.Add(PRNL_COMPARE_LESS_THAN);
	mCompare.Add(PRNL_COMPARE_GREATER_THAN);
	mCompare.Add(PRNL_COMPARE_EQUALS);
	Append(wxEnumProperty(PRNL_ON_VELOCITY_COMPARE, PRNL_ON_VELOCITY_COMPARE, mCompare));

	// Velocity threshold - value: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_ON_VELOCITY_THRESHOLD, PRNL_ON_VELOCITY_THRESHOLD, ParticleUniverse::OnVelocityObserver::DEFAULT_VELOCITY_THRESHOLD));
	SetPropertyEditor(PRNL_ON_VELOCITY_THRESHOLD, wxPG_EDITOR(SpinCtrl));
}