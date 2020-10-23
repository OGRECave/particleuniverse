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
#include "ParticleUniverseEditor.h"
#include "ParticleUniverseExternPropertyWindow.h"
#include "ParticleUniverseEditComponent.h"
#include "ParticleUniverseSystemManager.h"
#include "wx/ogre/utils.h"

//-----------------------------------------------------------------------
ExternPropertyWindow::ExternPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) : PropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
ExternPropertyWindow::ExternPropertyWindow(ExternPropertyWindow* externPropertyWindow) : PropertyWindow(
	externPropertyWindow->GetParent(), 
	externPropertyWindow->getOwner(), 
	externPropertyWindow->getComponentName())
{
	_initProperties();
	copyAttributesFromPropertyWindow(externPropertyWindow);
}
//-----------------------------------------------------------------------
void ExternPropertyWindow::copyAttributesFromPropertyWindow(ExternPropertyWindow* externPropertyWindow)
{
	// Name: String
	doSetString(PRNL_NAME, externPropertyWindow->doGetString(PRNL_NAME));

	// Type: List of types
	wxPGProperty* propTo = GetPropertyPtr(PRNL_EXTERN_TYPE);
	wxPGProperty* propFrom = externPropertyWindow->GetPropertyPtr(PRNL_EXTERN_TYPE);
	propTo->DoSetValue(propFrom->DoGetValue());
}
//-----------------------------------------------------------------------
void ExternPropertyWindow::copyAttributeToExtern(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::Extern* externObject = static_cast<ParticleUniverse::Extern*>(mOwner->getPUElement());
	if (!externObject)
		return;

	if (propertyName == PRNL_NAME)
	{
		// Name: String
		Ogre::String name = wx2ogre(prop->GetValueAsString());
		externObject->setName(name);
	}
	else if (propertyName == PRNL_EXTERN_TYPE)
	{
		// Type: List of types
		// This requires the extern to be replaced.
		replaceExternType(prop);
	}
	else
	{
		PropertyWindow::copyAttributeToComponent(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void ExternPropertyWindow::copyAttributesFromExtern(ParticleUniverse::Extern* externObject)
{
	// Name: Ogre::String
	doSetString(PRNL_NAME, ogre2wx(externObject->getName()));

	// Type: List of types
	wxPGProperty* propTo = GetPropertyPtr(PRNL_EXTERN_TYPE);
	wxString type = ogre2wxTranslate(externObject->getExternType());
	propTo->SetValueFromString(type);
}
//-----------------------------------------------------------------------
void ExternPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	CST_EXTERN_BOX_COLLIDER = ogre2wxTranslate(EXTERN_BOX_COLLIDER);
	CST_EXTERN_SPHERE_COLLIDER = ogre2wxTranslate(EXTERN_SPHERE_COLLIDER);
	CST_EXTERN_GRAVITY = ogre2wxTranslate(EXTERN_GRAVITY);
	CST_EXTERN_PHYSX_ACTOR = ogre2wxTranslate(EXTERN_PHYSX_ACTOR);
	CST_EXTERN_PHYSX_FLUID = ogre2wxTranslate(EXTERN_PHYSX_FLUID);
	CST_EXTERN_VORTEX = ogre2wxTranslate(EXTERN_VORTEX);
	PRNL_EXTERN_TYPE = _("Extern type");
	PRNL_PHYSX_COLLISION_GROUP = _("Collision group");
	PRNL_NAME = _("Name");

	SetBoolChoices (_("True"), _("False")); // Forces Internationalization

	// Type: List of types (DO NOT CHANGE THE ORDER!
	// Internationalization means that the string values of this property are unusable to create an extern)
	mTypes.Add(CST_EXTERN_BOX_COLLIDER);
	mTypes.Add(CST_EXTERN_GRAVITY);
	mTypes.Add(CST_EXTERN_SPHERE_COLLIDER);
	mTypes.Add(CST_EXTERN_VORTEX);
#ifdef PU_PHYSICS_PHYSX
	if (ParticleUniverseEditorFrame::isPhysXUsed())
	{
		mTypes.Add(CST_EXTERN_PHYSX_ACTOR);
		mTypes.Add(CST_EXTERN_PHYSX_FLUID);
	}
#endif //PU_PHYSICS_PHYSX
	wxPGId pid = Append(wxEnumProperty(PRNL_EXTERN_TYPE, PRNL_EXTERN_TYPE, mTypes));
}
//-----------------------------------------------------------------------
void ExternPropertyWindow::onPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();
	wxPGProperty* prop = event.GetPropertyPtr();
	onParentPropertyChanged(event);
	copyAttributeToExtern(prop, propertyName);
	notifyPropertyChanged();
}
//-----------------------------------------------------------------------
void ExternPropertyWindow::onParentPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();
	PropertyWindow::onPropertyChanged(event);

	if (propertyName == PRNL_EXTERN_TYPE)
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
void ExternPropertyWindow::replaceExternType(wxPGProperty* prop)
{
	// Type: List of types
	Ogre::String type = getExternTypeByProperty(prop);
	if (type == Ogre::StringUtil::BLANK)
		return;

	ParticleUniverse::Extern* oldExtern = static_cast<ParticleUniverse::Extern*>(mOwner->getPUElement());
	if (oldExtern)
	{
		ParticleUniverse::ParticleTechnique* technique = oldExtern->getParentTechnique();
		if (technique)
		{
			ParticleUniverse::Extern* newExtern = technique->createExtern(type);
			oldExtern->copyParentAttributesTo(newExtern);
			bool wasStarted = false;
			ParticleUniverse::ParticleSystem* system = technique->getParentSystem();
			if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED)
			{
				wasStarted = true;
				system->stop();
			}
			technique->destroyExtern(oldExtern);
			mOwner->setPUElement(newExtern);
			if (wasStarted)
			{
				system->start();
			}
		}
		else
		{
			/** The old extern didn't have a technique, so create a new extern by means of the ParticleSystemManager itself and also delete
				the old extern by means of the ParticleSystemManager
			*/
			ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			ParticleUniverse::Extern* newExtern = particleSystemManager->createExtern(type);
			oldExtern->copyParentAttributesTo(newExtern);
			particleSystemManager->destroyExtern(oldExtern);
			mOwner->setPUElement(newExtern);
		}
	}
	else
	{
		// There is no old extern. Create a new extern by means of the ParticleSystemManager
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::Extern* newExtern = particleSystemManager->createExtern(type);
		mOwner->setPUElement(newExtern);
	}
}
//-----------------------------------------------------------------------
const Ogre::String& ExternPropertyWindow::getExternTypeByProperty(wxPGProperty* prop)
{
	int type = prop->DoGetValue().GetLong(); // The property must be a list (PRNL_EXTERN_TYPE)
	switch (type)
	{
		case 0:
			return EXTERN_BOX_COLLIDER;
		break;
		case 1:
			return EXTERN_GRAVITY;
		break;
		case 2:
			return EXTERN_SPHERE_COLLIDER;
		break;
		case 3:
			return EXTERN_VORTEX;
		break;
		case 4:
			return EXTERN_PHYSX_ACTOR;
		break;
		case 5:
			return EXTERN_PHYSX_FLUID;
		break;
	}

	return Ogre::StringUtil::BLANK;
}
