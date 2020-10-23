/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "EventHandlerPropertyWindow/DoStopSystemEventHandlerPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
DoStopSystemEventHandlerPropertyWindow::DoStopSystemEventHandlerPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	EventHandlerPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
DoStopSystemEventHandlerPropertyWindow::DoStopSystemEventHandlerPropertyWindow(EventHandlerPropertyWindow* eventHandlerPropertyWindow) :
	EventHandlerPropertyWindow(eventHandlerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
DoStopSystemEventHandlerPropertyWindow::DoStopSystemEventHandlerPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::DoStopSystemEventHandler* eventHandler) :
	EventHandlerPropertyWindow(parent, owner, eventHandler->getName())
{
	copyAttributesFromEventHandler(eventHandler);
}
//-----------------------------------------------------------------------
void DoStopSystemEventHandlerPropertyWindow::copyAttributeToEventHandler(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	// Update handler with another attribute
	EventHandlerPropertyWindow::copyAttributeToEventHandler(prop, propertyName);
}
//-----------------------------------------------------------------------
void DoStopSystemEventHandlerPropertyWindow::copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler* eventHandler)
{
	EventHandlerPropertyWindow::copyAttributesFromEventHandler(eventHandler);
}
//-----------------------------------------------------------------------
void DoStopSystemEventHandlerPropertyWindow::_initProperties(void)
{
	mHelpHtml = wxT("EventHandlerDoStopSystem.html");

	// This event handler doesn't have any properties
}