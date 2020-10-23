/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "EventHandlerPropertyWindow/DoScaleEventHandlerPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
DoScaleEventHandlerPropertyWindow::DoScaleEventHandlerPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	EventHandlerPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
DoScaleEventHandlerPropertyWindow::DoScaleEventHandlerPropertyWindow(EventHandlerPropertyWindow* eventHandlerPropertyWindow) :
	EventHandlerPropertyWindow(eventHandlerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
DoScaleEventHandlerPropertyWindow::DoScaleEventHandlerPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::DoScaleEventHandler* eventHandler) :
	EventHandlerPropertyWindow(parent, owner, eventHandler->getName())
{
	copyAttributesFromEventHandler(eventHandler);
}
//-----------------------------------------------------------------------
void DoScaleEventHandlerPropertyWindow::copyAttributeToEventHandler(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::DoScaleEventHandler* handler = static_cast<ParticleUniverse::DoScaleEventHandler*>(mOwner->getPUElement());
	if (!handler)
		return;

	if (propertyName == PRNL_SCALE_FRACTION)
	{
		// Scale Fraction: Ogre:: Real
		handler->setScaleFraction(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_SCALE_TYPE)
	{
		// Scale Type: List
		wxString scaleType = prop->GetValueAsString();
		if (scaleType == SC_TIME_TO_LIVE)
		{
			handler->setScaleType(ParticleUniverse::DoScaleEventHandler::ST_TIME_TO_LIVE);
		}
		else if (scaleType == SC_VELOCITY)
		{
			handler->setScaleType(ParticleUniverse::DoScaleEventHandler::ST_VELOCITY);
		}
	}
	else
	{
		// Update handler with another attribute
		EventHandlerPropertyWindow::copyAttributeToEventHandler(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void DoScaleEventHandlerPropertyWindow::copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler* eventHandler)
{
	EventHandlerPropertyWindow::copyAttributesFromEventHandler(eventHandler);

	// Copy properties from eventHandler to property window
	ParticleUniverse::DoScaleEventHandler* doScaleEventHandler = static_cast<ParticleUniverse::DoScaleEventHandler*>(eventHandler);

	// Scale Fraction: Ogre:: Real
	doSetDouble(PRNL_SCALE_FRACTION, doScaleEventHandler->getScaleFraction());

	// Scale Type: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_SCALE_TYPE);
	ParticleUniverse::DoScaleEventHandler::ScaleType scaleType = doScaleEventHandler->getScaleType();
	wxString scaleTypeString = SC_TIME_TO_LIVE;
	if (scaleType == ParticleUniverse::DoScaleEventHandler::ST_VELOCITY)
	{
		scaleTypeString = SC_VELOCITY;
	}
	propTo->SetValueFromString(scaleTypeString);
}
//-----------------------------------------------------------------------
void DoScaleEventHandlerPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_SCALE_FRACTION = _("Scale fraction");
	PRNL_SCALE_TYPE = _("Scale type");
	SC_TIME_TO_LIVE = _("Time to live");
	SC_VELOCITY = _("Velocity");

	mHelpHtml = wxT("EventHandlerDoScale.html");

	// Scale Fraction: Ogre:: Real
	Append(wxFloatProperty(PRNL_SCALE_FRACTION, 
		PRNL_SCALE_FRACTION, 
		ParticleUniverse::DoScaleEventHandler::DEFAULT_SCALE_FRACTION));
	SetPropertyEditor(PRNL_SCALE_FRACTION, wxPG_EDITOR(SpinCtrl));

	// Scale Type: List
	mScaleTypes.Add(SC_TIME_TO_LIVE);
	mScaleTypes.Add(SC_VELOCITY);
	wxPGId pid = Append(wxEnumProperty(PRNL_SCALE_TYPE, PRNL_SCALE_TYPE, mScaleTypes));
}