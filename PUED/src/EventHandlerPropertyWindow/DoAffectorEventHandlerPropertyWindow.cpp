/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "EventHandlerPropertyWindow/DoAffectorEventHandlerPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
DoAffectorEventHandlerPropertyWindow::DoAffectorEventHandlerPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	EventHandlerPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
DoAffectorEventHandlerPropertyWindow::DoAffectorEventHandlerPropertyWindow(EventHandlerPropertyWindow* eventHandlerPropertyWindow) :
	EventHandlerPropertyWindow(eventHandlerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
DoAffectorEventHandlerPropertyWindow::DoAffectorEventHandlerPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::DoAffectorEventHandler* eventHandler) :
	EventHandlerPropertyWindow(parent, owner, eventHandler->getName())
{
	copyAttributesFromEventHandler(eventHandler);
}
//-----------------------------------------------------------------------
void DoAffectorEventHandlerPropertyWindow::copyAttributeToEventHandler(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::DoAffectorEventHandler* handler = static_cast<ParticleUniverse::DoAffectorEventHandler*>(mOwner->getPUElement());
	if (!handler)
		return;

	if (propertyName == PRNL_PRE_POST)
	{
		// Pre/Post Processing: bool
		handler->setPrePost(prop->DoGetValue().GetBool());
	}
	else
	{
		// Update handler with another attribute
		EventHandlerPropertyWindow::copyAttributeToEventHandler(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void DoAffectorEventHandlerPropertyWindow::copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler* eventHandler)
{
	EventHandlerPropertyWindow::copyAttributesFromEventHandler(eventHandler);

	// Copy properties from eventHandler to property window
	ParticleUniverse::DoAffectorEventHandler* doAffectorEventHandler = static_cast<ParticleUniverse::DoAffectorEventHandler*>(eventHandler);

	// Pre/Post Processing: bool
	doSetBool(PRNL_PRE_POST, doAffectorEventHandler->getPrePost());
}
//-----------------------------------------------------------------------
void DoAffectorEventHandlerPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_PRE_POST = _("Perform pre/post processing");

	mHelpHtml = wxT("EventHandlerDoAffector.html");

	// Affector component: Doesn't have a property, because this is established by means of a connection

	// Pre/Post Processing: bool
	Append(wxBoolProperty(PRNL_PRE_POST, PRNL_PRE_POST, ParticleUniverse::DoAffectorEventHandler::DEFAULT_PRE_POST));
}