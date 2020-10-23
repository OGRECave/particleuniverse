/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ObserverPropertyWindow/OnPositionObserverPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
OnPositionObserverPropertyWindow::OnPositionObserverPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ObserverPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnPositionObserverPropertyWindow::OnPositionObserverPropertyWindow(ObserverPropertyWindow* observerPropertyWindow) :
	ObserverPropertyWindow(observerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnPositionObserverPropertyWindow::OnPositionObserverPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::OnPositionObserver* observer) :
	ObserverPropertyWindow(parent, owner, observer->getName())
{
	copyAttributesFromObserver(observer);
}
//-----------------------------------------------------------------------
void OnPositionObserverPropertyWindow::copyAttributeToObserver(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::OnPositionObserver* observer = static_cast<ParticleUniverse::OnPositionObserver*>(mOwner->getPUElement());
	if (!observer)
		return;

	if (propertyName == PRNL_ON_POSITION_X_COMPARE)
	{
		// Compare X: List
		wxString compare = prop->GetValueAsString();
		if (compare == PRNL_COMPARE_NOT_USED)
		{
			observer->setComparePositionX(ParticleUniverse::CO_LESS_THAN);
			observer->setPositionXThreshold(ParticleUniverse::OnPositionObserver::DEFAULT_POSITION_THRESHOLD.x);
			observer->resetPositionXThreshold();
		}
		else if (compare == PRNL_COMPARE_LESS_THAN)
		{
			observer->setComparePositionX(ParticleUniverse::CO_LESS_THAN);
			observer->setPositionXThreshold(observer->getPositionXThreshold()); // Forces that compare is used
		}
		else if (compare == PRNL_COMPARE_GREATER_THAN)
		{
			observer->setComparePositionX(ParticleUniverse::CO_GREATER_THAN);
			observer->setPositionXThreshold(observer->getPositionXThreshold()); // Forces that compare is used
		}
		else if (compare == PRNL_COMPARE_EQUALS)
		{
			observer->setComparePositionX(ParticleUniverse::CO_EQUALS);
			observer->setPositionXThreshold(observer->getPositionXThreshold()); // Forces that compare is used
		}
	}
	else if (propertyName == PRNL_ON_POSITION_X)
	{
		// Position X: ParticleUniverse::Real
		observer->setPositionXThreshold(prop->DoGetValue().GetDouble());

		// Adjust the compare operator
		prop = GetPropertyPtr(PRNL_ON_POSITION_X_COMPARE);
		if (prop)
		{
			wxString compareString = PRNL_COMPARE_LESS_THAN;
			if (observer->getComparePositionX() == ParticleUniverse::CO_GREATER_THAN)
			{
				compareString = PRNL_COMPARE_GREATER_THAN;
			}
			else if (observer->getComparePositionX() == ParticleUniverse::CO_EQUALS)
			{
				compareString = PRNL_COMPARE_EQUALS;
			}
			prop->SetValueFromString(compareString);
		}
	}
	else if (propertyName == PRNL_ON_POSITION_Y_COMPARE)
	{
		// Compare Y: List
		wxString compare = prop->GetValueAsString();
		if (compare == PRNL_COMPARE_NOT_USED)
		{
			observer->setComparePositionY(ParticleUniverse::CO_LESS_THAN);
			observer->setPositionYThreshold(ParticleUniverse::OnPositionObserver::DEFAULT_POSITION_THRESHOLD.y);
			observer->resetPositionYThreshold();
		}
		else if (compare == PRNL_COMPARE_LESS_THAN)
		{
			observer->setComparePositionY(ParticleUniverse::CO_LESS_THAN);
			observer->setPositionYThreshold(observer->getPositionYThreshold()); // Forces that compare is used
		}
		else if (compare == PRNL_COMPARE_GREATER_THAN)
		{
			observer->setComparePositionY(ParticleUniverse::CO_GREATER_THAN);
			observer->setPositionYThreshold(observer->getPositionYThreshold()); // Forces that compare is used
		}
		else if (compare == PRNL_COMPARE_EQUALS)
		{
			observer->setComparePositionY(ParticleUniverse::CO_EQUALS);
			observer->setPositionYThreshold(observer->getPositionYThreshold()); // Forces that compare is used
		}
	}
	else if (propertyName == PRNL_ON_POSITION_Y)
	{
		// Position Y: ParticleUniverse::Real
		observer->setPositionYThreshold(prop->DoGetValue().GetDouble());

		// Adjust the compare operator
		prop = GetPropertyPtr(PRNL_ON_POSITION_Y_COMPARE);
		if (prop)
		{
			wxString compareString = PRNL_COMPARE_LESS_THAN;
			if (observer->getComparePositionX() == ParticleUniverse::CO_GREATER_THAN)
			{
				compareString = PRNL_COMPARE_GREATER_THAN;
			}
			else if (observer->getComparePositionX() == ParticleUniverse::CO_EQUALS)
			{
				compareString = PRNL_COMPARE_EQUALS;
			}
			prop->SetValueFromString(compareString);
		}
	}
	else if (propertyName == PRNL_ON_POSITION_Z_COMPARE)
	{
		// Compare Z: List
		wxString compare = prop->GetValueAsString();
		if (compare == PRNL_COMPARE_NOT_USED)
		{
			observer->setComparePositionZ(ParticleUniverse::CO_LESS_THAN);
			observer->setPositionZThreshold(ParticleUniverse::OnPositionObserver::DEFAULT_POSITION_THRESHOLD.z);
			observer->resetPositionZThreshold();
		}
		else if (compare == PRNL_COMPARE_LESS_THAN)
		{
			observer->setComparePositionZ(ParticleUniverse::CO_LESS_THAN);
			observer->setPositionZThreshold(observer->getPositionZThreshold()); // Forces that compare is used
		}
		else if (compare == PRNL_COMPARE_GREATER_THAN)
		{
			observer->setComparePositionZ(ParticleUniverse::CO_GREATER_THAN);
			observer->setPositionZThreshold(observer->getPositionZThreshold()); // Forces that compare is used
		}
		else if (compare == PRNL_COMPARE_EQUALS)
		{
			observer->setComparePositionZ(ParticleUniverse::CO_EQUALS);
			observer->setPositionZThreshold(observer->getPositionZThreshold()); // Forces that compare is used
		}
	}
	else if (propertyName == PRNL_ON_POSITION_Z)
	{
		// Position Z: ParticleUniverse::Real
		observer->setPositionZThreshold(prop->DoGetValue().GetDouble());

		// Adjust the compare operator
		prop = GetPropertyPtr(PRNL_ON_POSITION_Z_COMPARE);
		if (prop)
		{
			wxString compareString = PRNL_COMPARE_LESS_THAN;
			if (observer->getComparePositionX() == ParticleUniverse::CO_GREATER_THAN)
			{
				compareString = PRNL_COMPARE_GREATER_THAN;
			}
			else if (observer->getComparePositionX() == ParticleUniverse::CO_EQUALS)
			{
				compareString = PRNL_COMPARE_EQUALS;
			}
			prop->SetValueFromString(compareString);
		}
	}
	else
	{
		// Update observer with another attribute
		ObserverPropertyWindow::copyAttributeToObserver(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void OnPositionObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver* observer)
{
	ObserverPropertyWindow::copyAttributesFromObserver(observer);

	// Copy properties from observer to property window
	ParticleUniverse::OnPositionObserver* onPositionObserver = static_cast<ParticleUniverse::OnPositionObserver*>(observer);

	// Compare X
	wxPGProperty* propTo = GetPropertyPtr(PRNL_ON_POSITION_X_COMPARE);
	ParticleUniverse::ComparisionOperator compare = onPositionObserver->getComparePositionX();
	wxString compareString = PRNL_COMPARE_NOT_USED;
	if (onPositionObserver->isPositionXThresholdSet())
	{
		if (compare == ParticleUniverse::CO_LESS_THAN)
		{
			compareString = PRNL_COMPARE_LESS_THAN;
		}
		else if (compare == ParticleUniverse::CO_GREATER_THAN)
		{
			compareString = PRNL_COMPARE_GREATER_THAN;
		}
		else if (compare == ParticleUniverse::CO_EQUALS)
		{
			compareString = PRNL_COMPARE_EQUALS;
		}
	}
	propTo->SetValueFromString(compareString);

	// Position X
	doSetDouble(PRNL_ON_POSITION_X, onPositionObserver->getPositionXThreshold());

	// Compare Y
	propTo = GetPropertyPtr(PRNL_ON_POSITION_Y_COMPARE);
	compare = onPositionObserver->getComparePositionY();
	compareString = PRNL_COMPARE_NOT_USED;
	if (onPositionObserver->isPositionYThresholdSet())
	{
		if (compare == ParticleUniverse::CO_LESS_THAN)
		{
			compareString = PRNL_COMPARE_LESS_THAN;
		}
		else if (compare == ParticleUniverse::CO_GREATER_THAN)
		{
			compareString = PRNL_COMPARE_GREATER_THAN;
		}
		else if (compare == ParticleUniverse::CO_EQUALS)
		{
			compareString = PRNL_COMPARE_EQUALS;
		}
	}
	propTo->SetValueFromString(compareString);

	// Position Y
	doSetDouble(PRNL_ON_POSITION_Y, onPositionObserver->getPositionYThreshold());

	// Compare Z
	propTo = GetPropertyPtr(PRNL_ON_POSITION_Z_COMPARE);
	compare = onPositionObserver->getComparePositionZ();
	compareString = PRNL_COMPARE_NOT_USED;
	if (onPositionObserver->isPositionZThresholdSet())
	{
		if (compare == ParticleUniverse::CO_LESS_THAN)
		{
			compareString = PRNL_COMPARE_LESS_THAN;
		}
		else if (compare == ParticleUniverse::CO_GREATER_THAN)
		{
			compareString = PRNL_COMPARE_GREATER_THAN;
		}
		else if (compare == ParticleUniverse::CO_EQUALS)
		{
			compareString = PRNL_COMPARE_EQUALS;
		}
	}
	propTo->SetValueFromString(compareString);

	// Position Z
	doSetDouble(PRNL_ON_POSITION_Z, onPositionObserver->getPositionZThreshold());
}
//-----------------------------------------------------------------------
void OnPositionObserverPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_ON_POSITION_X_COMPARE = _("Position compare x");
	PRNL_ON_POSITION_Y_COMPARE = _("Position compare y");
	PRNL_ON_POSITION_Z_COMPARE = _("Position compare z");
	PRNL_ON_POSITION_X = _("Position.x threshold");
	PRNL_ON_POSITION_Y = _("Position.y threshold");
	PRNL_ON_POSITION_Z = _("Position.z threshold");
	PRNL_COMPARE_NOT_USED = _("Not used");
	PRNL_COMPARE_LESS_THAN = _("Less than");
	PRNL_COMPARE_GREATER_THAN = _("Greater than");
	PRNL_COMPARE_EQUALS = _("Equals");

	mHelpHtml = wxT("ObserverOnPosition.html");

	// Compare X
	mCompareX.Add(PRNL_COMPARE_NOT_USED);
	mCompareX.Add(PRNL_COMPARE_LESS_THAN);
	mCompareX.Add(PRNL_COMPARE_GREATER_THAN);
	mCompareX.Add(PRNL_COMPARE_EQUALS);
	Append(wxEnumProperty(PRNL_ON_POSITION_X_COMPARE, PRNL_ON_POSITION_X_COMPARE, mCompareX));

	// Position X
	Append(wxFloatProperty(PRNL_ON_POSITION_X, PRNL_ON_POSITION_X, ParticleUniverse::OnPositionObserver::DEFAULT_POSITION_THRESHOLD.x));
	SetPropertyEditor(PRNL_ON_POSITION_X, wxPG_EDITOR(SpinCtrl));

	// Compare Y
	mCompareY.Add(PRNL_COMPARE_NOT_USED);
	mCompareY.Add(PRNL_COMPARE_LESS_THAN);
	mCompareY.Add(PRNL_COMPARE_GREATER_THAN);
	mCompareY.Add(PRNL_COMPARE_EQUALS);
	Append(wxEnumProperty(PRNL_ON_POSITION_Y_COMPARE, PRNL_ON_POSITION_Y_COMPARE, mCompareY));

	// Position Y
	Append(wxFloatProperty(PRNL_ON_POSITION_Y, PRNL_ON_POSITION_Y, ParticleUniverse::OnPositionObserver::DEFAULT_POSITION_THRESHOLD.y));
	SetPropertyEditor(PRNL_ON_POSITION_Y, wxPG_EDITOR(SpinCtrl));

	// Compare Z
	mCompareZ.Add(PRNL_COMPARE_NOT_USED);
	mCompareZ.Add(PRNL_COMPARE_LESS_THAN);
	mCompareZ.Add(PRNL_COMPARE_GREATER_THAN);
	mCompareZ.Add(PRNL_COMPARE_EQUALS);
	Append(wxEnumProperty(PRNL_ON_POSITION_Z_COMPARE, PRNL_ON_POSITION_Z_COMPARE, mCompareZ));
	
	// Position Z
	Append(wxFloatProperty(PRNL_ON_POSITION_Z, PRNL_ON_POSITION_Z, ParticleUniverse::OnPositionObserver::DEFAULT_POSITION_THRESHOLD.z));
	SetPropertyEditor(PRNL_ON_POSITION_Z, wxPG_EDITOR(SpinCtrl));
}