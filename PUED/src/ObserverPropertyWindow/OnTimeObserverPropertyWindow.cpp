/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ObserverPropertyWindow/OnTimeObserverPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
OnTimeObserverPropertyWindow::OnTimeObserverPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ObserverPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnTimeObserverPropertyWindow::OnTimeObserverPropertyWindow(ObserverPropertyWindow* observerPropertyWindow) :
	ObserverPropertyWindow(observerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnTimeObserverPropertyWindow::OnTimeObserverPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::OnTimeObserver* observer) :
	ObserverPropertyWindow(parent, owner, observer->getName())
{
	copyAttributesFromObserver(observer);
}
//-----------------------------------------------------------------------
void OnTimeObserverPropertyWindow::copyAttributeToObserver(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::OnTimeObserver* observer = static_cast<ParticleUniverse::OnTimeObserver*>(mOwner->getPUElement());
	if (!observer)
		return;

	if (propertyName == PRNL_ON_TIME_COMPARE)
	{
		// Time Threshold - Compare: List
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
	else if (propertyName == PRNL_ON_TIME_THRESHOLD)
	{
		// Time Threshold - Value: ParticleUniverse::Real
		observer->setThreshold(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_SINCE_START_SYSTEM)
	{
		// Since Start System: bool
		observer->setSinceStartSystem(prop->DoGetValue().GetBool());
	}
	else
	{
		// Update observer with another attribute
		ObserverPropertyWindow::copyAttributeToObserver(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void OnTimeObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver* observer)
{
	ObserverPropertyWindow::copyAttributesFromObserver(observer);

	// Copy properties from observer to property window
	ParticleUniverse::OnTimeObserver* onTimeObserver = static_cast<ParticleUniverse::OnTimeObserver*>(observer);

	// Time Threshold - Compare: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_ON_TIME_COMPARE);
	ParticleUniverse::ComparisionOperator compare = onTimeObserver->getCompare();
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

	// Time Threshold - Value: ParticleUniverse::Real
	doSetDouble(PRNL_ON_TIME_THRESHOLD, onTimeObserver->getThreshold());

	// Since Start System: bool
	doSetBool(PRNL_SINCE_START_SYSTEM, onTimeObserver->isSinceStartSystem());
}
//-----------------------------------------------------------------------
void OnTimeObserverPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_ON_TIME_COMPARE = _("On time compare");
	PRNL_ON_TIME_THRESHOLD = _("On time threshold");
	PRNL_SINCE_START_SYSTEM = _("Since start system");
	PRNL_COMPARE_LESS_THAN = _("Less than");
	PRNL_COMPARE_GREATER_THAN = _("Greater than");
	PRNL_COMPARE_EQUALS = _("Equals");

	mHelpHtml = wxT("ObserverOnTime.html");

	// Time Threshold - Compare: List
	mCompare.Add(PRNL_COMPARE_LESS_THAN);
	mCompare.Add(PRNL_COMPARE_GREATER_THAN);
	mCompare.Add(PRNL_COMPARE_EQUALS);
	Append(wxEnumProperty(PRNL_ON_TIME_COMPARE, PRNL_ON_TIME_COMPARE, mCompare));

	// Time Threshold - Value: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_ON_TIME_THRESHOLD, PRNL_ON_TIME_THRESHOLD, ParticleUniverse::OnTimeObserver::DEFAULT_THRESHOLD));
	SetPropertyEditor(PRNL_ON_TIME_THRESHOLD, wxPG_EDITOR(SpinCtrl));

	// Since Start System: bool
	Append(wxBoolProperty(PRNL_SINCE_START_SYSTEM, PRNL_SINCE_START_SYSTEM, ParticleUniverse::OnTimeObserver::DEFAULT_SINCE_START_SYSTEM));
}