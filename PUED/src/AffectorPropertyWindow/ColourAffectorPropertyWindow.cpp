/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/ColourAffectorPropertyWindow.h"
#include "AffectorPropertyWindow/ColourAffectorDialog.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/ogre/utils.h"
#include "wx/propgrid/advprops.h"

ParentPropertyTimeAndColour::ParentPropertyTimeAndColour(
	const wxString& label, 
	const wxString& name) :
	wxParentPropertyClass(label, name),
	mTimeAndColour(0)
{
}
//-----------------------------------------------------------------------
bool ParentPropertyTimeAndColour::OnEvent (wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
	if (event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED)
	{
		// Show dialog
		AffectorPropertyWindow* affectorPropertyWindow	= static_cast<AffectorPropertyWindow*>(propgrid);
		if (!affectorPropertyWindow)
			return true;

		ParticleUniverse::ColourAffector* affector = static_cast<ParticleUniverse::ColourAffector*>(affectorPropertyWindow->getOwner()->getPUElement());
		if (!affector)
			return true;

		ParticleUniverse::ColourAffector::ColourMap map = affector->getTimeAndColour();
		ColourAffectorDialog dlg(map, propgrid, wxID_ANY, wxT("Colour Affector"), wxDefaultPosition, wxSize(580, 220));
		if (dlg.ShowModal() == wxID_OK)
		{
			affector->clearColourMap();
			ParticleUniverse::ColourAffector::ColourMap& map = dlg.getColourMap();
			ParticleUniverse::ColourAffector::ColourMap::iterator it;
			ParticleUniverse::ColourAffector::ColourMap::iterator itEnd = map.end();
			for (it = map.begin(); it != itEnd; ++it)
			{
				// Add to affector
				affector->addColour(it->first, it->second);
			}
		}
	}

	return true;
}
//-----------------------------------------------------------------------
int ParentPropertyTimeAndColour::getNumberOfTimeAndColourEntries(void)
{
	return mTimeAndColour;
}
//-----------------------------------------------------------------------
void ParentPropertyTimeAndColour::reset(void)
{
	mTimeAndColour = 0;
	this->Empty();
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ColourAffectorPropertyWindow::ColourAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
ColourAffectorPropertyWindow::ColourAffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
ColourAffectorPropertyWindow::ColourAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::ColourAffector* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void ColourAffectorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::ColourAffector* affector = static_cast<ParticleUniverse::ColourAffector*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_COLOUR_OPERATION)
	{
		// Colour Operation: List of types
		wxString operation = prop->GetValueAsString();
		if (operation == COP_SET)
		{
			affector->setColourOperation(ParticleUniverse::ColourAffector::CAO_SET);
		}
		else if (operation == COP_MULTIPLY)
		{
			affector->setColourOperation(ParticleUniverse::ColourAffector::CAO_MULTIPLY);
		}
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void ColourAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::ColourAffector* colourAffector = static_cast<ParticleUniverse::ColourAffector*>(affector);

	// Colour Operation: List of types
	wxPGProperty* propTo = GetPropertyPtr(PRNL_COLOUR_OPERATION);
	ParticleUniverse::ColourAffector::ColourOperation colourOperation = colourAffector->getColourOperation();
	wxString colourOperationString = COP_SET;
	if (colourOperation == ParticleUniverse::ColourAffector::CAO_MULTIPLY)
	{
		colourOperationString = COP_MULTIPLY;
	}
	propTo->SetValueFromString(colourOperationString);
}
//-----------------------------------------------------------------------
void ColourAffectorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_TIME_AND_COLOUR = _("Time and Colour");
	PRNL_TIME_AND_COLOUR_PARENT = _("Time/Colour");
	PRNL_TIME_AND_COLOUR_TIME = _("Time instance");
	PRNL_TIME_AND_COLOUR_COLOUR = _("Colour instance");
	PRNL_COLOUR_OPERATION = _("Colour operation");
	COP_SET = _("Set");
	COP_MULTIPLY = _("Multiply");

	mHelpHtml = wxT("AffectorColour.html");

	// Time and Colour: List
	wxPGId pid = Append(new ParentPropertyTimeAndColour(PRNL_TIME_AND_COLOUR, PRNL_TIME_AND_COLOUR));
	SetPropertyEditor(pid, wxPG_EDITOR(TextCtrlAndButton)); // Add a button

	// Colour Operation: List of types
	mColourOperation.Add(COP_SET);
	mColourOperation.Add(COP_MULTIPLY);
	Append(wxEnumProperty(PRNL_COLOUR_OPERATION, PRNL_COLOUR_OPERATION, mColourOperation));
}