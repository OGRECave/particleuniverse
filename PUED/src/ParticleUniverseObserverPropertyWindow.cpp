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
#include "ParticleUniverseObserverPropertyWindow.h"
#include "ParticleUniverseEditComponent.h"
#include "ParticleUniverseSystemManager.h"
#include "wx/ogre/utils.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
ObserverPropertyWindow::ObserverPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) : PropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
ObserverPropertyWindow::ObserverPropertyWindow(ObserverPropertyWindow* observerPropertyWindow) : PropertyWindow(
	observerPropertyWindow->GetParent(), 
	observerPropertyWindow->getOwner(), 
	observerPropertyWindow->getComponentName())
{
	_initProperties();
	copyAttributesFromPropertyWindow(observerPropertyWindow);
}
//-----------------------------------------------------------------------
void ObserverPropertyWindow::copyAttributesFromPropertyWindow(ObserverPropertyWindow* observerPropertyWindow)
{
	Ogre::Vector3 v;

	// Name: String
	doSetString(PRNL_NAME, observerPropertyWindow->doGetString(PRNL_NAME));

	// Type: List of types
	wxPGProperty* propTo = GetPropertyPtr(PRNL_OBSERVER_TYPE);
	wxPGProperty* propFrom = observerPropertyWindow->GetPropertyPtr(PRNL_OBSERVER_TYPE);
	propTo->DoSetValue(propFrom->DoGetValue());

	// Enabled: bool
	SetBoolChoices (_("True"), _("False")); // Forces Internationalization
	doSetBool(PRNL_OBSERVER_ENABLED, observerPropertyWindow->doGetBool(PRNL_OBSERVER_ENABLED));

	// Observe Particle Type: List
	propTo = GetPropertyPtr(PRNL_PARTICLE_TYPE);
	propFrom = observerPropertyWindow->GetPropertyPtr(PRNL_PARTICLE_TYPE);
	propTo->DoSetValue(propFrom->DoGetValue());

	// Observe Interval: ParticleUniverse::Real
	doSetDouble(PRNL_OBSERVE_INTERVAL, observerPropertyWindow->doGetDouble(PRNL_OBSERVE_INTERVAL));

	// Observe Until Event: bool
	doSetBool(PRNL_UNTIL_EVENT, observerPropertyWindow->doGetBool(PRNL_UNTIL_EVENT));
}
//-----------------------------------------------------------------------
void ObserverPropertyWindow::copyAttributeToObserver(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::ParticleObserver* observer = static_cast<ParticleUniverse::ParticleObserver*>(mOwner->getPUElement());
	if (!observer)
		return;

	if (propertyName == PRNL_NAME)
	{
		// Name: String
		Ogre::String name = wx2ogre(prop->GetValueAsString());
		observer->setName(name);
	}
	else if (propertyName == PRNL_OBSERVER_TYPE)
	{
		// Type: List of types
		// This requires the observer to be replaced.
		replaceObserverType(prop);
	}
	else if (propertyName == PRNL_PARTICLE_TYPE)
	{
		// Observe Particle Type: List
		wxString observeType = prop->GetValueAsString();
		if (observeType == PT_VISUAL)
		{
			observer->setParticleTypeToObserve(ParticleUniverse::Particle::PT_VISUAL);
		}
		else if (observeType == PT_EMITTER)
		{
			observer->setParticleTypeToObserve(ParticleUniverse::Particle::PT_EMITTER);
		}
		else if (observeType == PT_AFFECTOR)
		{
			observer->setParticleTypeToObserve(ParticleUniverse::Particle::PT_AFFECTOR);
		}
		else if (observeType == PT_TECHNIQUE)
		{
			observer->setParticleTypeToObserve(ParticleUniverse::Particle::PT_TECHNIQUE);
		}
		else if (observeType == PT_SYSTEM)
		{
			observer->setParticleTypeToObserve(ParticleUniverse::Particle::PT_SYSTEM);
		}
	}
	else if (propertyName == PRNL_OBSERVER_ENABLED)
	{
		// Enabled: Bool
		observer->_resetEnabled();
		observer->setEnabled(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_OBSERVE_INTERVAL)
	{
		// Observe Interval: ParticleUniverse::Real
		observer->setObserverInterval(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_UNTIL_EVENT)
	{
		// Observe Until Event: bool
		observer->setObserveUntilEvent(prop->DoGetValue().GetBool());
	}
	else
	{
		PropertyWindow::copyAttributeToComponent(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void ObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver* observer)
{
	// Name: Ogre::String
	doSetString(PRNL_NAME, ogre2wx(observer->getName()));

	// Type: List of types
	wxPGProperty* propTo = GetPropertyPtr(PRNL_OBSERVER_TYPE);
	wxString type = ogre2wxTranslate(observer->getObserverType());
	propTo->SetValueFromString(type);

	// Enabled: bool
	doSetBool(PRNL_OBSERVER_ENABLED, observer->isEnabled());

	// Observe Particle Type: List
	propTo = GetPropertyPtr(PRNL_PARTICLE_TYPE);
	wxString pType = wxT("");
	if (observer->isParticleTypeToObserveSet())
	{
		ParticleUniverse::Particle::ParticleType particleType = observer->getParticleTypeToObserve();
		switch(particleType)
		{
			case ParticleUniverse::Particle::PT_VISUAL:
				pType = PT_VISUAL;
			break;
			case ParticleUniverse::Particle::PT_EMITTER:
				pType = PT_EMITTER;
			break;
			case ParticleUniverse::Particle::PT_AFFECTOR:
				pType = PT_AFFECTOR;
			break;
			case ParticleUniverse::Particle::PT_TECHNIQUE:
				pType = PT_TECHNIQUE;
			break;
			case ParticleUniverse::Particle::PT_SYSTEM:
				pType = PT_SYSTEM;
			break;
		}
	}
	else
	{
		pType = PT_ALL;
	}
	propTo->SetValueFromString(pType);

	// Observe Interval: ParticleUniverse::Real
	doSetDouble(PRNL_OBSERVE_INTERVAL, observer->getObserverInterval());

	// Observe Until Event: bool
	doSetBool(PRNL_UNTIL_EVENT, observer->getObserveUntilEvent());
}
//-----------------------------------------------------------------------
void ObserverPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	CST_OBSERVER_ON_CLEAR = ogre2wxTranslate(OBSERVER_ON_CLEAR);
	CST_OBSERVER_ON_COLLISION = ogre2wxTranslate(OBSERVER_ON_COLLISION);
	CST_OBSERVER_ON_COUNT = ogre2wxTranslate(OBSERVER_ON_COUNT);
	CST_OBSERVER_ON_EMISSION = ogre2wxTranslate(OBSERVER_ON_EMISSION);
	CST_OBSERVER_ON_EVENTFLAG = ogre2wxTranslate(OBSERVER_ON_EVENTFLAG);
	CST_OBSERVER_ON_EXPIRE = ogre2wxTranslate(OBSERVER_ON_EXPIRE);
	CST_OBSERVER_ON_POSITION = ogre2wxTranslate(OBSERVER_ON_POSITION);
	CST_OBSERVER_ON_QUOTA = ogre2wxTranslate(OBSERVER_ON_QUOTA);
	CST_OBSERVER_ON_RANDOM = ogre2wxTranslate(OBSERVER_ON_RANDOM);
	CST_OBSERVER_ON_TIME = ogre2wxTranslate(OBSERVER_ON_TIME);
	CST_OBSERVER_ON_VELOCITY = ogre2wxTranslate(OBSERVER_ON_VELOCITY);
	PRNL_OBSERVER_TYPE = _("Observer type");
	PRNL_OBSERVER_ENABLED = _("Enabled");
	PRNL_UNTIL_EVENT = _("Observer until event");
	PRNL_OBSERVE_INTERVAL = _("Observe interval");
	PRNL_NAME = _("Name");
	PRNL_PARTICLE_TYPE = _("Particle type");
	PT_ALL = _("All types");
	PT_VISUAL = _("Visual");
	PT_EMITTER = _("Emitter");
	PT_AFFECTOR = _("Affector");
	PT_SYSTEM = _("System");
	PT_TECHNIQUE = _("Technique");

	// Type: List of types (DO NOT CHANGE THE ORDER!
	// Internationalization means that the string values of this property are unusable to create an observer)
	mTypes.Add(CST_OBSERVER_ON_CLEAR);
	mTypes.Add(CST_OBSERVER_ON_COLLISION);
	mTypes.Add(CST_OBSERVER_ON_COUNT);
	mTypes.Add(CST_OBSERVER_ON_EMISSION);
	mTypes.Add(CST_OBSERVER_ON_EVENTFLAG);
	mTypes.Add(CST_OBSERVER_ON_EXPIRE);
	mTypes.Add(CST_OBSERVER_ON_POSITION);
	mTypes.Add(CST_OBSERVER_ON_QUOTA);
	mTypes.Add(CST_OBSERVER_ON_RANDOM);
	mTypes.Add(CST_OBSERVER_ON_TIME);
	mTypes.Add(CST_OBSERVER_ON_VELOCITY);
	wxPGId pid = Append(wxEnumProperty(PRNL_OBSERVER_TYPE, PRNL_OBSERVER_TYPE, mTypes));

	// Enabled: bool
	SetBoolChoices (_("True"), _("False")); // Forces Internationalization
	Append(wxBoolProperty(PRNL_OBSERVER_ENABLED, PRNL_OBSERVER_ENABLED, ParticleUniverse::ParticleObserver::DEFAULT_ENABLED));

	// Observe Particle Type: List
	mParticleTypes.Add(PT_ALL);
	mParticleTypes.Add(PT_VISUAL);
	mParticleTypes.Add(PT_EMITTER);
	mParticleTypes.Add(PT_AFFECTOR);
	mParticleTypes.Add(PT_SYSTEM);
	mParticleTypes.Add(PT_TECHNIQUE);
	pid = Append(wxEnumProperty(PRNL_PARTICLE_TYPE, PRNL_PARTICLE_TYPE, mParticleTypes));

	// Observe Interval: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_OBSERVE_INTERVAL, PRNL_OBSERVE_INTERVAL, 0.0f));
	SetPropertyEditor(PRNL_OBSERVE_INTERVAL, wxPG_EDITOR(SpinCtrl));

	// Observe Until Event: bool
	Append(wxBoolProperty(PRNL_UNTIL_EVENT, PRNL_UNTIL_EVENT, ParticleUniverse::ParticleObserver::DEFAULT_UNTIL_EVENT));
}
//-----------------------------------------------------------------------
void ObserverPropertyWindow::onPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();
	wxPGProperty* prop = event.GetPropertyPtr();
	onParentPropertyChanged(event);
	copyAttributeToObserver(prop, propertyName);
	notifyPropertyChanged();
}
//-----------------------------------------------------------------------
void ObserverPropertyWindow::onParentPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();
	PropertyWindow::onPropertyChanged(event);

	if (propertyName == PRNL_OBSERVER_TYPE)
	{
		// Replace this window by another one
		wxString subType = event.GetPropertyValueAsString();
		mOwner->createPropertyWindow(subType, this);
		mOwner->setCaption();
		getOwner()->refreshCanvas();
	}
	notifyPropertyChanged();
}
//-----------------------------------------------------------------------
void ObserverPropertyWindow::replaceObserverType(wxPGProperty* prop)
{
	// Type: List of types
	Ogre::String type = getObserverTypeByProperty(prop);
	if (type == Ogre::StringUtil::BLANK)
		return;

	ParticleUniverse::ParticleObserver* oldObserver = static_cast<ParticleUniverse::ParticleObserver*>(mOwner->getPUElement());
	if (oldObserver)
	{
		ParticleUniverse::ParticleTechnique* technique = oldObserver->getParentTechnique();
		if (technique)
		{
			ParticleUniverse::ParticleObserver* newObserver = technique->createObserver(type);
			oldObserver->copyParentAttributesTo(newObserver);
			copyHandlersTo(oldObserver, newObserver);
			bool wasStarted = false;
			ParticleUniverse::ParticleSystem* system = technique->getParentSystem();
			if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED)
			{
				wasStarted = true;
				system->stop();
			}
			technique->destroyObserver(oldObserver);
			mOwner->setPUElement(newObserver);
			if (wasStarted)
			{
				system->start();
			}
		}
		else
		{
			/** The old observer didn't have a technique, so create a new observer by means of the ParticleSystemManager itself and also delete
				the old observer by means of the ParticleSystemManager
			*/
			ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			ParticleUniverse::ParticleObserver* newObserver = particleSystemManager->createObserver(type);
			oldObserver->copyParentAttributesTo(newObserver);
			copyHandlersTo(oldObserver, newObserver);
			particleSystemManager->destroyObserver(oldObserver);
			mOwner->setPUElement(newObserver);
		}
	}
	else
	{
		// There is no old observer. Create a new observer by means of the ParticleSystemManager
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleObserver* newObserver = particleSystemManager->createObserver(type);
		mOwner->setPUElement(newObserver);
	}
}
//-----------------------------------------------------------------------
void ObserverPropertyWindow::copyHandlersTo(ParticleUniverse::ParticleObserver* oldObserver, ParticleUniverse::ParticleObserver* newObserver)
{
	// First delete all handlers that where created during the copy action
	newObserver->destroyAllEventHandlers();

	// Move the handlers from the old observer to the new one
	size_t size = oldObserver->getNumEventHandlers();
	ParticleUniverse::ParticleEventHandler* handler;
	while(oldObserver->getNumEventHandlers() > 0)
	{
		handler = oldObserver->getEventHandler(0);
		oldObserver->removeEventHandler(handler);
		newObserver->addEventHandler(handler);
	}
}
//-----------------------------------------------------------------------
const Ogre::String& ObserverPropertyWindow::getObserverTypeByProperty(wxPGProperty* prop)
{
	int type = prop->DoGetValue().GetLong(); // The property must be a list (PRNL_OBSERVER_TYPE)
	switch (type)
	{
		case 0:
			return OBSERVER_ON_CLEAR;
		break;
		case 1:
			return OBSERVER_ON_COLLISION;
		break;
		case 2:
			return OBSERVER_ON_COUNT;
		break;
		case 3:
			return OBSERVER_ON_EMISSION;
		break;
		case 4:
			return OBSERVER_ON_EVENTFLAG;
		break;
		case 5:
			return OBSERVER_ON_EXPIRE;
		break;
		case 6:
			return OBSERVER_ON_POSITION;
		break;
		case 7:
			return OBSERVER_ON_QUOTA;
		break;
		case 8:
			return OBSERVER_ON_RANDOM;
		break;
		case 9:
			return OBSERVER_ON_TIME;
		break;
		case 10:
			return OBSERVER_ON_VELOCITY;
		break;
	}

	return Ogre::StringUtil::BLANK;
}
