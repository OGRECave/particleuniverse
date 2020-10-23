/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseBehaviourPropertyWindow.h"
#include "ParticleUniverseEditComponent.h"
#include "ParticleUniverseSystemManager.h"
#include "wx/ogre/utils.h"

//-----------------------------------------------------------------------
BehaviourPropertyWindow::BehaviourPropertyWindow(wxWindow* parent, EditComponent* owner) : PropertyWindow(parent, owner, Ogre::StringUtil::BLANK)
{
	_initProperties();
}
//-----------------------------------------------------------------------
BehaviourPropertyWindow::BehaviourPropertyWindow(BehaviourPropertyWindow* behaviourPropertyWindow) : PropertyWindow(
	behaviourPropertyWindow->GetParent(), 
	behaviourPropertyWindow->getOwner(), 
	Ogre::StringUtil::BLANK)
{
	_initProperties();
	copyAttributesFromPropertyWindow(behaviourPropertyWindow);
}
//-----------------------------------------------------------------------
void BehaviourPropertyWindow::copyAttributesFromPropertyWindow(BehaviourPropertyWindow* behaviourPropertyWindow)
{
	// Name: String
	//doSetString(PRNL_NAME, behaviourPropertyWindow->doGetString(PRNL_NAME));

	// Type: List of types
	wxPGProperty* propTo = GetPropertyPtr(PRNL_BEHAVIOUR_TYPE);
	wxPGProperty* propFrom = behaviourPropertyWindow->GetPropertyPtr(PRNL_BEHAVIOUR_TYPE);
	propTo->DoSetValue(propFrom->DoGetValue());
}
//-----------------------------------------------------------------------
void BehaviourPropertyWindow::copyAttributeToBehaviour(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::ParticleBehaviour* behaviour = static_cast<ParticleUniverse::ParticleBehaviour*>(mOwner->getPUElement());
	if (!behaviour)
		return;

	if (propertyName == PRNL_BEHAVIOUR_TYPE)
	{
		// Type: List of types
		// This requires the behaviour to be replaced.
		replaceBehaviourType(prop);
	}
	else
	{
		PropertyWindow::copyAttributeToComponent(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void BehaviourPropertyWindow::copyAttributesFromBehaviour(const ParticleUniverse::ParticleBehaviour* behaviour)
{
	// Type: List of types
	wxPGProperty* propTo = GetPropertyPtr(PRNL_BEHAVIOUR_TYPE);
	wxString type = ogre2wxTranslate(behaviour->getBehaviourType());
	propTo->SetValueFromString(type);
}
//-----------------------------------------------------------------------
void BehaviourPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	CST_BEHAVIOUR_SLAVE = ogre2wxTranslate(BEHAVIOUR_SLAVE);
	PRNL_BEHAVIOUR_TYPE = _("Behaviour type");
	PRNL_NAME = _("Name");

	SetBoolChoices (_("True"), _("False")); // Forces Internationalization

	// Type
	mTypes.Add(CST_BEHAVIOUR_SLAVE);
	wxPGId pid = Append(wxEnumProperty(PRNL_BEHAVIOUR_TYPE, PRNL_BEHAVIOUR_TYPE, mTypes));
}
//-----------------------------------------------------------------------
void BehaviourPropertyWindow::onPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();
	wxPGProperty* prop = event.GetPropertyPtr();
	onParentPropertyChanged(event);
	copyAttributeToBehaviour(prop, propertyName);
	notifyPropertyChanged();
}
//-----------------------------------------------------------------------
void BehaviourPropertyWindow::onParentPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();
	PropertyWindow::onPropertyChanged(event);

	if (propertyName == PRNL_BEHAVIOUR_TYPE)
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
void BehaviourPropertyWindow::replaceBehaviourType(wxPGProperty* prop)
{
	// Type: List of types
	Ogre::String type = getBehaviourTypeByProperty(prop);
	if (type == Ogre::StringUtil::BLANK)
		return;

	ParticleUniverse::ParticleBehaviour* oldBehaviour = static_cast<ParticleUniverse::ParticleBehaviour*>(mOwner->getPUElement());
	if (oldBehaviour)
	{
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleBehaviour* newBehaviour = particleSystemManager->createBehaviour(type);
		oldBehaviour->copyParentAttributesTo(newBehaviour);
		ParticleUniverse::ParticleTechnique* technique = oldBehaviour->getParentTechnique();
		if (technique)
		{
			bool wasStarted = false;
			ParticleUniverse::ParticleSystem* system = technique->getParentSystem();
			if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED)
			{
				wasStarted = true;
				system->stop();
			}
			technique->_destroyBehaviourTemplate(oldBehaviour);
			technique->_addBehaviourTemplate(newBehaviour);
			mOwner->setPUElement(newBehaviour);
			technique->_unprepareBehaviours();
			if (wasStarted)
			{
				system->start();
			}
		}
		else
		{
			/** The old behaviour didn't have a technique.
			*/
			particleSystemManager->destroyBehaviour(oldBehaviour);
			mOwner->setPUElement(newBehaviour);
		}
	}
	else
	{
		// There is no old behaviour. Create a new behaviour by means of the ParticleSystemManager
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleBehaviour* newBehaviour = particleSystemManager->createBehaviour(type);
		mOwner->setPUElement(newBehaviour);
	}
}
//-----------------------------------------------------------------------
const Ogre::String& BehaviourPropertyWindow::getBehaviourTypeByProperty(wxPGProperty* prop)
{
	int type = prop->DoGetValue().GetLong(); // The property must be a list (PRNL_BEHAVIOUR_TYPE)
	switch (type)
	{
		case 0:
			return BEHAVIOUR_SLAVE;
		break;
	}

	return Ogre::StringUtil::BLANK;
}
