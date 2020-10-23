/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/InterParticleColliderPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
InterParticleColliderPropertyWindow::InterParticleColliderPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
InterParticleColliderPropertyWindow::InterParticleColliderPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
InterParticleColliderPropertyWindow::InterParticleColliderPropertyWindow(
	wxWindow* parent, 
	EditComponent* owner, 
	ParticleUniverse::InterParticleCollider* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void InterParticleColliderPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::InterParticleCollider* affector = static_cast<ParticleUniverse::InterParticleCollider*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_INTERPARTICLE_COLLIDER_ADJUSTMENT)
	{
		// Adjustment: ParticleUniverse::Real
		affector->setAdjustment(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_COLLISON_RESPONSE)
	{
		// Collision response: List
		wxString collision = prop->GetValueAsString();
		if (collision == CR_AVERAGE_VELOCITY)
		{
			affector->setInterParticleCollisionResponse(ParticleUniverse::InterParticleCollider::IPCR_AVERAGE_VELOCITY);
		}
		else if (collision == CR_ANGLE_BASED_VELOCITY)
		{
			affector->setInterParticleCollisionResponse(ParticleUniverse::InterParticleCollider::IPCR_ANGLE_BASED_VELOCITY);
		}
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void InterParticleColliderPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::InterParticleCollider* interParticleCollider = static_cast<ParticleUniverse::InterParticleCollider*>(affector);

	// Adjustment: ParticleUniverse::Real
	doSetDouble(PRNL_INTERPARTICLE_COLLIDER_ADJUSTMENT, interParticleCollider->getAdjustment());

	// Collision response: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_COLLISON_RESPONSE);
	ParticleUniverse::InterParticleCollider::InterParticleCollisionResponse collisionResponse = 
		interParticleCollider->getInterParticleCollisionResponse();
	wxString collisionResponseString = CR_AVERAGE_VELOCITY;
	if (collisionResponse == ParticleUniverse::InterParticleCollider::IPCR_ANGLE_BASED_VELOCITY)
	{
		collisionResponseString = CR_ANGLE_BASED_VELOCITY;
	}
	propTo->SetValueFromString(collisionResponseString);
}
//-----------------------------------------------------------------------
void InterParticleColliderPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_INTERPARTICLE_COLLIDER_ADJUSTMENT = _("Adjustment");
	PRNL_COLLISON_RESPONSE = _("Collision response");
	CR_AVERAGE_VELOCITY = _("Average velocity");
	CR_ANGLE_BASED_VELOCITY = _("Angle based velocity");

	mHelpHtml = wxT("ColliderInterParticle.html");

	// Adjustment: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_INTERPARTICLE_COLLIDER_ADJUSTMENT, 
		PRNL_INTERPARTICLE_COLLIDER_ADJUSTMENT, 
		ParticleUniverse::InterParticleCollider::DEFAULT_ADJUSTMENT));
	SetPropertyEditor(PRNL_INTERPARTICLE_COLLIDER_ADJUSTMENT, wxPG_EDITOR(SpinCtrl));

	// Collision response: List
	mCollisionResponse.Add(CR_AVERAGE_VELOCITY);
	mCollisionResponse.Add(CR_ANGLE_BASED_VELOCITY);
	Append(wxEnumProperty(PRNL_COLLISON_RESPONSE, PRNL_COLLISON_RESPONSE, mCollisionResponse));
}