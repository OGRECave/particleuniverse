/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseAffectorPropertyWindow.h"
#include "ParticleUniverseEditComponent.h"
#include "ParticleUniverseSystemManager.h"
#include "wx/ogre/utils.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
AffectorPropertyWindow::AffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) : PropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
AffectorPropertyWindow::AffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) : PropertyWindow(
	affectorPropertyWindow->GetParent(), 
	affectorPropertyWindow->getOwner(), 
	affectorPropertyWindow->getComponentName())
{
	_initProperties();
	copyAttributesFromPropertyWindow(affectorPropertyWindow);
}
//-----------------------------------------------------------------------
void AffectorPropertyWindow::copyAttributesFromPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow)
{
	Ogre::Vector3 v;

	// Name: String
	doSetString(PRNL_NAME, affectorPropertyWindow->doGetString(PRNL_NAME));

	// Type: List of types
	wxPGProperty* propTo = GetPropertyPtr(PRNL_AFFECTOR_TYPE);
	wxPGProperty* propFrom = affectorPropertyWindow->GetPropertyPtr(PRNL_AFFECTOR_TYPE);
	propTo->DoSetValue(propFrom->DoGetValue());

	// Enabled: Bool
	doSetBool(PRNL_AFFECTOR_ENABLED, affectorPropertyWindow->doGetBool(PRNL_AFFECTOR_ENABLED));

	// Position: Ogre::Vector3
	doSetVector3(PRNL_AFFECTOR_POSITION, affectorPropertyWindow->doGetVector3(PRNL_AFFECTOR_POSITION, v));

	// Mass: ParticleUniverse::Real
	doSetDouble(PRNL_AFFECTOR_MASS, affectorPropertyWindow->doGetDouble(PRNL_AFFECTOR_MASS));

	// Specialisation: List of specialisations
	// Todo: Since there are no good cases, the option is disable for now
	//propTo = GetPropertyPtr(PRNL_AFFECTOR_SPECIALISATION);
	//propFrom = affectorPropertyWindow->GetPropertyPtr(PRNL_AFFECTOR_SPECIALISATION);
	//propTo->DoSetValue(propFrom->DoGetValue());
}
//-----------------------------------------------------------------------
void AffectorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::ParticleAffector* affector = static_cast<ParticleUniverse::ParticleAffector*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_NAME)
	{
		// Name: String
		Ogre::String name = wx2ogre(prop->GetValueAsString());
		affector->setName(name);
	}
	else if (propertyName == PRNL_AFFECTOR_TYPE)
	{
		// Type: List of types
		// This requires the affector to be replaced.
		replaceAffectorType(prop);
	}
	else if (propertyName == PRNL_AFFECTOR_ENABLED)
	{
		// Enabled: Bool
		affector->_setOriginalEnabled(prop->DoGetValue().GetBool());
		affector->setEnabled(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_AFFECTOR_POSITION + PRNL_X ||
		propertyName == PRNL_AFFECTOR_POSITION + PRNL_Y ||
		propertyName == PRNL_AFFECTOR_POSITION + PRNL_Z)
	{
		// Position: Ogre::Vector3
		Ogre::Vector3 v3;
		v3 = doGetVector3(PRNL_AFFECTOR_POSITION, v3);
		affector->position = v3;
		affector->originalPosition = v3;
	}
	else if (propertyName == PRNL_AFFECTOR_MASS)
	{
		// Mass: ParticleUniverse::Real
		affector->mass = prop->DoGetValue().GetDouble();
	}
//	else if (propertyName == PRNL_AFFECTOR_SPECIALISATION)
//	{
		// Affect specialisation: List
//		wxString specialisation = prop->GetValueAsString();
//		if (specialisation == PRNL_AFFECTOR_SPEC_DEFAULT)
//		{
//			affector->setAffectSpecialisation(ParticleUniverse::ParticleAffector::AFSP_DEFAULT);
//		}
//		else if (specialisation == PRNL_AFFECTOR_SPEC_TT_DECREASE)
//		{
//			affector->setAffectSpecialisation(ParticleUniverse::ParticleAffector::AFSP_TTL_DECREASE);
//		}
//		else if (specialisation == PRNL_AFFECTOR_SPEC_TT_INCREASE)
//		{
//			affector->setAffectSpecialisation(ParticleUniverse::ParticleAffector::AFSP_TTL_INCREASE);
//		}
//	}
	else
	{
		PropertyWindow::copyAttributeToComponent(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void AffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	// Name: Ogre::String
	doSetString(PRNL_NAME, ogre2wx(affector->getName()));

	// Type: List of types
	wxPGProperty* propTo = GetPropertyPtr(PRNL_AFFECTOR_TYPE);
	wxString type = ogre2wxTranslate(affector->getAffectorType());
	propTo->SetValueFromString(type);

	// Enabled: Bool
	doSetBool(PRNL_AFFECTOR_ENABLED, affector->isEnabled());

	// Position: Ogre::Vector3
	doSetVector3(PRNL_AFFECTOR_POSITION, affector->position);

	// Mass: ParticleUniverse::Real
	doSetDouble(PRNL_AFFECTOR_MASS, affector->mass);

	// Specialisation: List of specialisations
//	propTo = GetPropertyPtr(PRNL_AFFECTOR_SPECIALISATION);
//	ParticleUniverse::ParticleAffector::AffectSpecialisation specialisation = affector->getAffectSpecialisation();
//	wxString specialisationString = PRNL_AFFECTOR_SPEC_DEFAULT;
//	if (specialisation == ParticleUniverse::ParticleAffector::AFSP_TTL_DECREASE)
//	{
//		specialisationString = PRNL_AFFECTOR_SPEC_TT_DECREASE;
//	}
//	else if (specialisation == ParticleUniverse::ParticleAffector::AFSP_TTL_INCREASE)
//	{
//		specialisationString = PRNL_AFFECTOR_SPEC_TT_INCREASE;
//	}
//	propTo->SetValueFromString(specialisationString);
}
//-----------------------------------------------------------------------
void AffectorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	CST_AFFECTOR_ALIGN = ogre2wxTranslate(AFFECTOR_ALIGN);
	CST_AFFECTOR_BOX_COLLIDER = ogre2wxTranslate(AFFECTOR_BOX_COLLIDER);
	CST_AFFECTOR_COLLISION_AVOIDANCE = ogre2wxTranslate(AFFECTOR_COLLISION_AVOIDANCE);
	CST_AFFECTOR_COLOUR = ogre2wxTranslate(AFFECTOR_COLOUR);
	CST_AFFECTOR_FLOCK_CENTERING = ogre2wxTranslate(AFFECTOR_FLOCK_CENTERING);
	CST_AFFECTOR_FORCEFIELD = ogre2wxTranslate(AFFECTOR_FORCEFIELD);
	CST_AFFECTOR_GEOMETRY_ROTATOR = ogre2wxTranslate(AFFECTOR_GEOMETRY_ROTATOR);
	CST_AFFECTOR_GRAVITY = ogre2wxTranslate(AFFECTOR_GRAVITY);
	CST_AFFECTOR_INTER_PARTICLE_COLLIDER = ogre2wxTranslate(AFFECTOR_INTER_PARTICLE_COLLIDER);
	CST_AFFECTOR_JET = ogre2wxTranslate(AFFECTOR_JET);
	CST_AFFECTOR_LINE = ogre2wxTranslate(AFFECTOR_LINE);
	CST_AFFECTOR_LINEAR_FORCE = ogre2wxTranslate(AFFECTOR_LINEAR_FORCE);
	CST_AFFECTOR_PARTICLE_FOLLOWER = ogre2wxTranslate(AFFECTOR_PARTICLE_FOLLOWER);
	CST_AFFECTOR_PATH_FOLLOWER = ogre2wxTranslate(AFFECTOR_PATH_FOLLOWER);
	CST_AFFECTOR_PLANE_COLLIDER = ogre2wxTranslate(AFFECTOR_PLANE_COLLIDER);
	CST_AFFECTOR_RANDOMISER = ogre2wxTranslate(AFFECTOR_RANDOMISER);
	CST_AFFECTOR_SCALE = ogre2wxTranslate(AFFECTOR_SCALE);
	CST_AFFECTOR_SCALE_VELOCITY = ogre2wxTranslate(AFFECTOR_SCALE_VELOCITY);
	CST_AFFECTOR_SINE_FORCE = ogre2wxTranslate(AFFECTOR_SINE_FORCE);
	CST_AFFECTOR_SPHERE_COLLIDER = ogre2wxTranslate(AFFECTOR_SPHERE_COLLIDER);
	CST_AFFECTOR_TEXTURE_ANIMATOR = ogre2wxTranslate(AFFECTOR_TEXTURE_ANIMATOR);
	CST_AFFECTOR_TEXTURE_ROTATOR = ogre2wxTranslate(AFFECTOR_TEXTURE_ROTATOR);
	CST_AFFECTOR_VELOCITY_MATCHING = ogre2wxTranslate(AFFECTOR_VELOCITY_MATCHING);
	CST_AFFECTOR_VORTEX = ogre2wxTranslate(AFFECTOR_VORTEX);
	PRNL_NAME = _("Name");
	PRNL_AFFECTOR_TYPE = _("Affector type");
	PRNL_AFFECTOR_ENABLED = _("Enabled");
	PRNL_AFFECTOR_POSITION = _("Position");
	PRNL_AFFECTOR_MASS = _("Mass");
	PRNL_AFFECTOR_SPECIALISATION = _("Specialisation");
	PRNL_AFFECTOR_SPEC_DEFAULT = _("Default");
	PRNL_AFFECTOR_SPEC_TT_INCREASE = _("Increment by time to live");
	PRNL_AFFECTOR_SPEC_TT_DECREASE = _("Decrement by time to live");
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");

	// Type: List of types (DO NOT CHANGE THE ORDER!
	// Internationalization means that the string values of this property are unusable to create an affector)
	mTypes.Add(CST_AFFECTOR_ALIGN);
	mTypes.Add(CST_AFFECTOR_BOX_COLLIDER);
	mTypes.Add(CST_AFFECTOR_COLLISION_AVOIDANCE);
	mTypes.Add(CST_AFFECTOR_COLOUR);
	mTypes.Add(CST_AFFECTOR_FLOCK_CENTERING);
	mTypes.Add(CST_AFFECTOR_FORCEFIELD);
	mTypes.Add(CST_AFFECTOR_GEOMETRY_ROTATOR);
	mTypes.Add(CST_AFFECTOR_GRAVITY);
	mTypes.Add(CST_AFFECTOR_INTER_PARTICLE_COLLIDER);
	mTypes.Add(CST_AFFECTOR_JET);
	mTypes.Add(CST_AFFECTOR_LINE);
	mTypes.Add(CST_AFFECTOR_LINEAR_FORCE);
	mTypes.Add(CST_AFFECTOR_PARTICLE_FOLLOWER);
	mTypes.Add(CST_AFFECTOR_PATH_FOLLOWER);
	mTypes.Add(CST_AFFECTOR_PLANE_COLLIDER);
	mTypes.Add(CST_AFFECTOR_RANDOMISER);
	mTypes.Add(CST_AFFECTOR_SCALE);
	mTypes.Add(CST_AFFECTOR_SCALE_VELOCITY);
	mTypes.Add(CST_AFFECTOR_SINE_FORCE);
	mTypes.Add(CST_AFFECTOR_SPHERE_COLLIDER);
	mTypes.Add(CST_AFFECTOR_TEXTURE_ANIMATOR);
	mTypes.Add(CST_AFFECTOR_TEXTURE_ROTATOR);
	mTypes.Add(CST_AFFECTOR_VELOCITY_MATCHING);
	mTypes.Add(CST_AFFECTOR_VORTEX);
	wxPGId pid = Append(wxEnumProperty(PRNL_AFFECTOR_TYPE, PRNL_AFFECTOR_TYPE, mTypes));

	// Enabled: Bool
	SetBoolChoices (_("True"), _("False")); // Forces Internationalization
	Append(wxBoolProperty(PRNL_AFFECTOR_ENABLED, PRNL_AFFECTOR_ENABLED, ParticleUniverse::ParticleAffector::DEFAULT_ENABLED));

	// Position: Ogre::Vector3
	appendVector3(PRNL_AFFECTOR_POSITION, PRNL_AFFECTOR_POSITION, ParticleUniverse::ParticleAffector::DEFAULT_POSITION);

	// Mass: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_AFFECTOR_MASS, PRNL_AFFECTOR_MASS, ParticleUniverse::ParticleAffector::DEFAULT_MASS));
	SetPropertyEditor(PRNL_AFFECTOR_MASS, wxPG_EDITOR(SpinCtrl));

	// Exclude Emitter: This is not a property, but done by means of a connection

	// Specialisation: List of specialisations
//	mSpecialisation.Add(PRNL_AFFECTOR_SPEC_DEFAULT);
//	mSpecialisation.Add(PRNL_AFFECTOR_SPEC_TT_INCREASE);
//	mSpecialisation.Add(PRNL_AFFECTOR_SPEC_TT_DECREASE);
//	pid = Append(wxEnumProperty(PRNL_AFFECTOR_SPECIALISATION, PRNL_AFFECTOR_SPECIALISATION, mSpecialisation));
}
//-----------------------------------------------------------------------
void AffectorPropertyWindow::onPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();
	wxPGProperty* prop = event.GetPropertyPtr();
	onParentPropertyChanged(event);
	copyAttributeToAffector(prop, propertyName);
	ParticleUniverse::ParticleAffector* affector = static_cast<ParticleUniverse::ParticleAffector*>(mOwner->getPUElement());
	if (affector && affector->_isMarkedForEmission() && affector->getParentTechnique())
	{
		// Unprepare, to change a property of an emitted affector
		_unprepare(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
	}
	notifyPropertyChanged();
}
//-----------------------------------------------------------------------
void AffectorPropertyWindow::onParentPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();
	PropertyWindow::onPropertyChanged(event);

	if (propertyName == PRNL_AFFECTOR_TYPE)
	{
		// Replace this window by another one
		//notifyDestroyUnnecessaryConnections();
		wxString subType = event.GetPropertyValueAsString();
		mOwner->createPropertyWindow(subType, this);
		mOwner->setCaption();
		getOwner()->refreshCanvas();
	}
	notifyPropertyChanged();
}
//-----------------------------------------------------------------------
void AffectorPropertyWindow::replaceAffectorType(wxPGProperty* prop)
{
	// Type: List of types
	Ogre::String type = getAffectorTypeByProperty(prop);
	if (type == Ogre::StringUtil::BLANK)
		return;

	ParticleUniverse::ParticleAffector* oldAffector = static_cast<ParticleUniverse::ParticleAffector*>(mOwner->getPUElement());
	if (oldAffector)
	{
		ParticleUniverse::ParticleTechnique* technique = oldAffector->getParentTechnique();
		if (technique)
		{
			ParticleUniverse::ParticleAffector* newAffector = technique->createAffector(type);
			oldAffector->copyParentAttributesTo(newAffector);
			bool wasStarted = false;
			ParticleUniverse::ParticleSystem* system = technique->getParentSystem();
			if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED)
			{
				wasStarted = true;
				system->stop();
			}
			technique->destroyAffector(oldAffector);
			mOwner->setPUElement(newAffector);
			technique->_unprepareAffectors();
			if (wasStarted)
			{
				system->start();
			}
		}
		else
		{
			/** The old affector didn't have a technique, so create a new affector by means of the ParticleSystemManager itself and also delete
				the old affector by means of the ParticleSystemManager
			*/
			ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			ParticleUniverse::ParticleAffector* newAffector = particleSystemManager->createAffector(type);
			oldAffector->copyParentAttributesTo(newAffector);
			particleSystemManager->destroyAffector(oldAffector);
			mOwner->setPUElement(newAffector);
		}
	}
	else
	{
		// There is no old affector. Create a new affector by means of the ParticleSystemManager
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleAffector* newAffector = particleSystemManager->createAffector(type);
		mOwner->setPUElement(newAffector);
	}
}
//-----------------------------------------------------------------------
bool AffectorPropertyWindow::validateTextureCoord(wxPGProperty* prop, ParticleUniverse::ParticleAffector* affector)
{
	if (!prop || !affector)
		return false;

	ParticleUniverse::ParticleTechnique* technique = affector->getParentTechnique();
	if (technique)
	{
		ParticleUniverse::ParticleRenderer* renderer = technique->getRenderer();
		if (renderer)
		{
			size_t max = renderer->getNumTextureCoords();
			size_t val = prop->DoGetValue().GetLong();
			if (val < 0 || val > max - 1)
			{
				Ogre::String ogreMax = Ogre::StringConverter::toString(max);
				wxString wxMax = ogre2wx(ogreMax);
				wxString message = _("Value must be less than ") + wxMax + _(" (but positive or 0)");
				return _setPropertyError(prop , message);
			}
		}
	}

	return true;
}
//-----------------------------------------------------------------------
const Ogre::String& AffectorPropertyWindow::getAffectorTypeByProperty(wxPGProperty* prop)
{
	int type = prop->DoGetValue().GetLong(); // The property must be a list (PRNL_AFFECTOR_TYPE)

	switch (type)
	{
		case 0:
			return AFFECTOR_ALIGN;
		break;

		case 1:
			return AFFECTOR_BOX_COLLIDER;
		break;

		case 2:
			return AFFECTOR_COLLISION_AVOIDANCE;
		break;

		case 3:
			return AFFECTOR_COLOUR;
		break;

		case 4:
			return AFFECTOR_FLOCK_CENTERING;
		break;

		case 5:
			return AFFECTOR_FORCEFIELD;
		break;

		case 6:
			return AFFECTOR_GEOMETRY_ROTATOR;
		break;

		case 7:
			return AFFECTOR_GRAVITY;
		break;

		case 8:
			return AFFECTOR_INTER_PARTICLE_COLLIDER;
		break;

		case 9:
			return AFFECTOR_JET;
		break;

		case 10:
			return AFFECTOR_LINE;
		break;

		case 11:
			return AFFECTOR_LINEAR_FORCE;
		break;

		case 12:
			return AFFECTOR_PARTICLE_FOLLOWER;
		break;

		case 13:
			return AFFECTOR_PATH_FOLLOWER;
		break;

		case 14:
			return AFFECTOR_PLANE_COLLIDER;
		break;

		case 15:
			return AFFECTOR_RANDOMISER;
		break;

		case 16:
			return AFFECTOR_SCALE;
		break;

		case 17:
			return AFFECTOR_SCALE_VELOCITY;
		break;

		case 18:
			return AFFECTOR_SINE_FORCE;
		break;

		case 19:
			return AFFECTOR_SPHERE_COLLIDER;
		break;

		case 20:
			return AFFECTOR_TEXTURE_ANIMATOR;
		break;

		case 21:
			return AFFECTOR_TEXTURE_ROTATOR;
		break;

		case 22:
			return AFFECTOR_VELOCITY_MATCHING;
		break;

		case 23:
			return AFFECTOR_VORTEX;			
		break;
	}

	return Ogre::StringUtil::BLANK;
}