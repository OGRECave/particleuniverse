/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2014 Henry van Merode. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseEventHandlerPropertyWindow.h"
#include "ParticleUniverseEditComponent.h"
#include "ParticleUniverseSystemManager.h"
#include "wx/ogre/utils.h"

//-----------------------------------------------------------------------
EventHandlerPropertyWindow::EventHandlerPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) : PropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
EventHandlerPropertyWindow::EventHandlerPropertyWindow(EventHandlerPropertyWindow* eventHandlerPropertyWindow) : PropertyWindow(
	eventHandlerPropertyWindow->GetParent(), 
	eventHandlerPropertyWindow->getOwner(), 
	eventHandlerPropertyWindow->getComponentName())
{
	_initProperties();
	copyAttributesFromPropertyWindow(eventHandlerPropertyWindow);
}
//-----------------------------------------------------------------------
void EventHandlerPropertyWindow::copyAttributesFromPropertyWindow(EventHandlerPropertyWindow* eventHandlerPropertyWindow)
{
	// Name: String
	doSetString(PRNL_NAME, eventHandlerPropertyWindow->doGetString(PRNL_NAME));

	// Type: List of types
	wxPGProperty* propTo = GetPropertyPtr(PRNL_HANDLER_TYPE);
	wxPGProperty* propFrom = eventHandlerPropertyWindow->GetPropertyPtr(PRNL_HANDLER_TYPE);
	propTo->DoSetValue(propFrom->DoGetValue());
}
//-----------------------------------------------------------------------
void EventHandlerPropertyWindow::copyAttributeToEventHandler(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::ParticleEventHandler* handler = static_cast<ParticleUniverse::ParticleEventHandler*>(mOwner->getPUElement());
	if (!handler)
		return;

	if (propertyName == PRNL_NAME)
	{
		// Name: String
		Ogre::String name = wx2ogre(prop->GetValueAsString());
		handler->setName(name);
	}
	else if (propertyName == PRNL_HANDLER_TYPE)
	{
		// Type: List of types
		// This requires the handler to be replaced.
		replaceHandlerType(prop);
	}
	else
	{
		PropertyWindow::copyAttributeToComponent(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void EventHandlerPropertyWindow::copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler* eventHandler)
{
	// Name: Ogre::String
	doSetString(PRNL_NAME, ogre2wx(eventHandler->getName()));

	// Type: List of types
	wxPGProperty* propTo = GetPropertyPtr(PRNL_HANDLER_TYPE);
	wxString type = ogre2wxTranslate(eventHandler->getEventHandlerType());
	propTo->SetValueFromString(type);
}
//-----------------------------------------------------------------------
void EventHandlerPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	CST_HANDLER_DO_AFFECTOR = ogre2wxTranslate(HANDLER_DO_AFFECTOR);
	CST_HANDLER_DO_ENABLE_COMPONENT = ogre2wxTranslate(HANDLER_DO_ENABLE_COMPONENT);
	CST_HANDLER_DO_EXPIRE = ogre2wxTranslate(HANDLER_DO_EXPIRE);
	CST_HANDLER_DO_FREEZE = ogre2wxTranslate(HANDLER_DO_FREEZE);
	CST_HANDLER_DO_PLACEMENT_PARTICLE = ogre2wxTranslate(HANDLER_DO_PLACEMENT_PARTICLE);
	CST_HANDLER_DO_SCALE = ogre2wxTranslate(HANDLER_DO_SCALE);
	CST_HANDLER_DO_STOP_SYSTEM = ogre2wxTranslate(HANDLER_DO_STOP_SYSTEM);
	PRNL_HANDLER_TYPE = _("Event handler type");
	PRNL_NAME = _("Name");

	SetBoolChoices (_("True"), _("False")); // Forces Internationalization

	// Type: List of types (DO NOT CHANGE THE ORDER!
	// Internationalization means that the string values of this property are unusable to create a handler)
	mTypes.Add(CST_HANDLER_DO_AFFECTOR);
	mTypes.Add(CST_HANDLER_DO_ENABLE_COMPONENT);
	mTypes.Add(CST_HANDLER_DO_EXPIRE);
	mTypes.Add(CST_HANDLER_DO_FREEZE);
	mTypes.Add(CST_HANDLER_DO_PLACEMENT_PARTICLE);
	mTypes.Add(CST_HANDLER_DO_SCALE);
	mTypes.Add(CST_HANDLER_DO_STOP_SYSTEM);
	wxPGId pid = Append(wxEnumProperty(PRNL_HANDLER_TYPE, PRNL_HANDLER_TYPE, mTypes));
}
//-----------------------------------------------------------------------
void EventHandlerPropertyWindow::onPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();
	wxPGProperty* prop = event.GetPropertyPtr();
	onParentPropertyChanged(event);
	copyAttributeToEventHandler(prop, propertyName);
	notifyPropertyChanged();
}
//-----------------------------------------------------------------------
void EventHandlerPropertyWindow::onParentPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();
	PropertyWindow::onPropertyChanged(event);

	if (propertyName == PRNL_HANDLER_TYPE)
	{
		// Replace this window by another one
		notifyDestroyUnnecessaryConnections();
		wxString subType = event.GetPropertyValueAsString();
		mOwner->createPropertyWindow(subType, this);
		mOwner->setCaption();
		getOwner()->refreshCanvas();
	}
	notifyPropertyChanged();
}
//-----------------------------------------------------------------------
void EventHandlerPropertyWindow::notifyDestroyUnnecessaryConnections(void)
{
	// Delete unnecessary connections
	getOwner()->deleteConnection(CR_ENABLE, CRDIR_PRIMARY);
	getOwner()->deleteConnection(CR_FORCE, CRDIR_PRIMARY);
	getOwner()->deleteConnection(CR_PLACE, CRDIR_PRIMARY);
}
//-----------------------------------------------------------------------
void EventHandlerPropertyWindow::replaceHandlerType(wxPGProperty* prop)
{
	// Type: List of types
	Ogre::String type = getHandlerTypeByProperty(prop);
	if (type == Ogre::StringUtil::BLANK)
		return;

	ParticleUniverse::ParticleEventHandler* oldHandler = static_cast<ParticleUniverse::ParticleEventHandler*>(mOwner->getPUElement());
	if (oldHandler)
	{
		ParticleUniverse::ParticleObserver* observer = oldHandler->getParentObserver();
		if (observer)
		{
			ParticleUniverse::ParticleEventHandler* newHandler = observer->createEventHandler(type);
			oldHandler->copyParentAttributesTo(newHandler);
			bool wasStarted = false;
			ParticleUniverse::ParticleTechnique* technique = observer->getParentTechnique();
			ParticleUniverse::ParticleSystem* system = 0;
			if (technique)
			{
				system = technique->getParentSystem();
				if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED)
				{
					wasStarted = true;
					system->stop();
				}
			}
			observer->destroyEventHandler(oldHandler);
			mOwner->setPUElement(newHandler);
			if (wasStarted)
			{
				system->start();
			}
		}
		else
		{
			/** The old handler didn't have an observer, so create a new handler by means of the ParticleSystemManager itself and also 
				delete the old handler by means of the ParticleSystemManager
			*/
			ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			ParticleUniverse::ParticleEventHandler* newHandler = particleSystemManager->createEventHandler(type);
			oldHandler->copyParentAttributesTo(newHandler);
			particleSystemManager->destroyEventHandler(oldHandler);
			mOwner->setPUElement(newHandler);
		}
	}
	else
	{
		// There is no old handler. Create a new handler by means of the ParticleSystemManager
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleEventHandler* newHandler = particleSystemManager->createEventHandler(type);
		mOwner->setPUElement(newHandler);
	}
}
//-----------------------------------------------------------------------
const Ogre::String& EventHandlerPropertyWindow::getHandlerTypeByProperty(wxPGProperty* prop)
{
	int type = prop->DoGetValue().GetLong(); // The property must be a list (PRNL_EVENT_HANDLER_TYPE)
	switch (type)
	{
		case 0:
			return HANDLER_DO_AFFECTOR;
		break;
		case 1:
			return HANDLER_DO_ENABLE_COMPONENT;
		break;
		case 2:
			return HANDLER_DO_EXPIRE;
		break;
		case 3:
			return HANDLER_DO_FREEZE;
		break;
		case 4:
			return HANDLER_DO_PLACEMENT_PARTICLE;
		break;
		case 5:
			return HANDLER_DO_SCALE;
		break;
		case 6:
			return HANDLER_DO_STOP_SYSTEM;
		break;
	}

	return Ogre::StringUtil::BLANK;
}
