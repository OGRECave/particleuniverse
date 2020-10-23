/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "EventHandlerPropertyWindow/DoEnableComponentEventHandlerPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/ogre/utils.h"

//-----------------------------------------------------------------------
DoEnableComponentEventHandlerPropertyWindow::DoEnableComponentEventHandlerPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	EventHandlerPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
DoEnableComponentEventHandlerPropertyWindow::DoEnableComponentEventHandlerPropertyWindow(EventHandlerPropertyWindow* eventHandlerPropertyWindow) :
	EventHandlerPropertyWindow(eventHandlerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
DoEnableComponentEventHandlerPropertyWindow::DoEnableComponentEventHandlerPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::DoEnableComponentEventHandler* eventHandler) :
	EventHandlerPropertyWindow(parent, owner, eventHandler->getName())
{
	copyAttributesFromEventHandler(eventHandler);
}
//-----------------------------------------------------------------------
void DoEnableComponentEventHandlerPropertyWindow::copyAttributeToEventHandler(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::DoEnableComponentEventHandler* handler = static_cast<ParticleUniverse::DoEnableComponentEventHandler*>(mOwner->getPUElement());
	if (!handler)
		return;

	if (propertyName == PRNL_ENABLE_COMPONENT_TRUE_FALSE)
	{
		// Enable/Disable: bool
		handler->setComponentEnabled(prop->DoGetValue().GetBool());
	}
	else
	{
		// Update handler with another attribute
		EventHandlerPropertyWindow::copyAttributeToEventHandler(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void DoEnableComponentEventHandlerPropertyWindow::copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler* eventHandler)
{
	EventHandlerPropertyWindow::copyAttributesFromEventHandler(eventHandler);

	// Copy properties from eventHandler to property window
	ParticleUniverse::DoEnableComponentEventHandler* doEnableComponentEventHandler = static_cast<ParticleUniverse::DoEnableComponentEventHandler*>(eventHandler);

	// Enable/Disable: bool
	doSetBool(PRNL_ENABLE_COMPONENT_TRUE_FALSE, doEnableComponentEventHandler->isComponentEnabled());
}
//-----------------------------------------------------------------------
void DoEnableComponentEventHandlerPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_ENABLE_COMPONENT_TRUE_FALSE = _("Enable component");

	mHelpHtml = wxT("EventHandlerDoEnableComponent.html");

	// Component to enable/disable: Realised by means of a connection
	
	// Enable/Disable: bool
	Append(wxBoolProperty(PRNL_ENABLE_COMPONENT_TRUE_FALSE, PRNL_ENABLE_COMPONENT_TRUE_FALSE, true));
}