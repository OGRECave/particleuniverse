/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "EventHandlerPropertyWindow/DoFreezeEventHandlerPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
DoFreezeEventHandlerPropertyWindow::DoFreezeEventHandlerPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	EventHandlerPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
DoFreezeEventHandlerPropertyWindow::DoFreezeEventHandlerPropertyWindow(EventHandlerPropertyWindow* eventHandlerPropertyWindow) :
	EventHandlerPropertyWindow(eventHandlerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
DoFreezeEventHandlerPropertyWindow::DoFreezeEventHandlerPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::DoFreezeEventHandler* eventHandler) :
	EventHandlerPropertyWindow(parent, owner, eventHandler->getName())
{
	copyAttributesFromEventHandler(eventHandler);
}
//-----------------------------------------------------------------------
void DoFreezeEventHandlerPropertyWindow::copyAttributeToEventHandler(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	// Update handler with another attribute
	EventHandlerPropertyWindow::copyAttributeToEventHandler(prop, propertyName);
}
//-----------------------------------------------------------------------
void DoFreezeEventHandlerPropertyWindow::copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler* eventHandler)
{
	EventHandlerPropertyWindow::copyAttributesFromEventHandler(eventHandler);
}
//-----------------------------------------------------------------------
void DoFreezeEventHandlerPropertyWindow::_initProperties(void)
{
	mHelpHtml = wxT("EventHandlerDoFreeze.html");

	// This event handler doesn't have any properties
}