/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ObserverPropertyWindow/OnCountObserverPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
OnCountObserverPropertyWindow::OnCountObserverPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ObserverPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnCountObserverPropertyWindow::OnCountObserverPropertyWindow(ObserverPropertyWindow* observerPropertyWindow) :
	ObserverPropertyWindow(observerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnCountObserverPropertyWindow::OnCountObserverPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::OnCountObserver* observer) :
	ObserverPropertyWindow(parent, owner, observer->getName())
{
	copyAttributesFromObserver(observer);
}
//-----------------------------------------------------------------------
void OnCountObserverPropertyWindow::copyAttributeToObserver(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::OnCountObserver* observer = static_cast<ParticleUniverse::OnCountObserver*>(mOwner->getPUElement());
	if (!observer)
		return;

	if (propertyName == PRNL_ON_COUNT_COMPARE)
	{
		// Count Threshold Compare: List
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
	else if (propertyName == PRNL_ON_COUNT_THRESHOLD)
	{
		// Count Threshold Value: ParticleUniverse::uint
		observer->setThreshold(prop->DoGetValue().GetLong());
	}
	else
	{
		// Update observer with another attribute
		ObserverPropertyWindow::copyAttributeToObserver(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void OnCountObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver* observer)
{
	ObserverPropertyWindow::copyAttributesFromObserver(observer);

	// Copy properties from observer to property window
	ParticleUniverse::OnCountObserver* onCountObserver = static_cast<ParticleUniverse::OnCountObserver*>(observer);

	// Count Threshold Compare: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_ON_COUNT_COMPARE);
	ParticleUniverse::ComparisionOperator compare = onCountObserver->getCompare();
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

	// Count Threshold Value: ParticleUniverse::uint
	doSetUint16(PRNL_ON_COUNT_THRESHOLD, onCountObserver->getThreshold());
}
//-----------------------------------------------------------------------
void OnCountObserverPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_ON_COUNT_COMPARE = _("On count compare");
	PRNL_ON_COUNT_THRESHOLD = _("On count threshold");
	PRNL_COMPARE_LESS_THAN = _("Less than");
	PRNL_COMPARE_GREATER_THAN = _("Greater than");
	PRNL_COMPARE_EQUALS = _("Equals");

	mHelpHtml = wxT("ObserverOnCount.html");

	// Count Threshold Compare: List
	mCompare.Add(PRNL_COMPARE_LESS_THAN);
	mCompare.Add(PRNL_COMPARE_GREATER_THAN);
	mCompare.Add(PRNL_COMPARE_EQUALS);
	Append(wxEnumProperty(PRNL_ON_COUNT_COMPARE, PRNL_ON_COUNT_COMPARE, mCompare));

	// Count Threshold Value: ParticleUniverse::uint
	Append(wxUIntProperty(PRNL_ON_COUNT_THRESHOLD, PRNL_ON_COUNT_THRESHOLD, ParticleUniverse::OnCountObserver::DEFAULT_THRESHOLD));
	SetPropertyEditor(PRNL_ON_COUNT_THRESHOLD, wxPG_EDITOR(SpinCtrl));
}
