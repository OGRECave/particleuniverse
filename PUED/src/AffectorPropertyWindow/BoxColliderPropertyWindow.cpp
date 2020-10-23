/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/BoxColliderPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
BoxColliderPropertyWindow::BoxColliderPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
BoxColliderPropertyWindow::BoxColliderPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
BoxColliderPropertyWindow::BoxColliderPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::BoxCollider* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void BoxColliderPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::BoxCollider* affector = static_cast<ParticleUniverse::BoxCollider*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_BOX_COLLIDER_WIDTH)
	{
		// Width: ParticleUniverse::Real
		affector->setWidth(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_BOX_COLLIDER_HEIGHT)
	{
		// Height: ParticleUniverse::Real
		affector->setHeight(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_BOX_COLLIDER_DEPTH)
	{
		// Depth: ParticleUniverse::Real
		affector->setDepth(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_BOX_COLLIDER_INNER)
	{
		// Inner Collision: bool
		affector->setInnerCollision(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_COLLIDER_FRICTION)
	{
		// Friction: ParticleUniverse::Real
		affector->setFriction(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_COLLIDER_BOUNCYNESS)
	{
		// Bouncyness: ParticleUniverse::Real
		affector->setBouncyness(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_INTERSECTION_TYPE)
	{
		// Intersection type: List
		wxString intersection = prop->GetValueAsString();
		if (intersection == IST_POINT)
		{
			affector->setIntersectionType(ParticleUniverse::BaseCollider::IT_POINT);
		}
		else if (intersection == IST_BOX)
		{
			affector->setIntersectionType(ParticleUniverse::BaseCollider::IT_BOX);
		}
	}
	else if (propertyName == PRNL_COLLISION_TYPE)
	{
		// Collision type: List
		wxString collision = prop->GetValueAsString();
		if (collision == COLLT_BOUNCE)
		{
			affector->setCollisionType(ParticleUniverse::BaseCollider::CT_BOUNCE);
		}
		else if (collision == COLLT_FLOW)
		{
			affector->setCollisionType(ParticleUniverse::BaseCollider::CT_FLOW);
		}
		else if (collision == COLLT_NONE)
		{
			affector->setCollisionType(ParticleUniverse::BaseCollider::CT_NONE);
		}
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void BoxColliderPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::BoxCollider* boxCollider = static_cast<ParticleUniverse::BoxCollider*>(affector);

	// Width: ParticleUniverse::Real
	doSetDouble(PRNL_BOX_COLLIDER_WIDTH, boxCollider->getWidth());

	// Height: ParticleUniverse::Real
	doSetDouble(PRNL_BOX_COLLIDER_HEIGHT, boxCollider->getHeight());

	// Depth: ParticleUniverse::Real
	doSetDouble(PRNL_BOX_COLLIDER_DEPTH, boxCollider->getDepth());

	// Inner Collision: bool
	doSetBool(PRNL_BOX_COLLIDER_INNER, boxCollider->isInnerCollision());

	// Friction: ParticleUniverse::Real
	doSetDouble(PRNL_COLLIDER_FRICTION, boxCollider->getFriction());

	// Bouncyness: ParticleUniverse::Real
	doSetDouble(PRNL_COLLIDER_BOUNCYNESS, boxCollider->getBouncyness());

	// Intersection type: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_INTERSECTION_TYPE);
	ParticleUniverse::BaseCollider::IntersectionType intersectionType = boxCollider->getIntersectionType();
	wxString intersectionTypeString = IST_POINT;
	if (intersectionType == ParticleUniverse::BaseCollider::IT_BOX)
	{
		intersectionTypeString = IST_BOX;
	}
	propTo->SetValueFromString(intersectionTypeString);

	// Collision type: List
	propTo = GetPropertyPtr(PRNL_COLLISION_TYPE);
	ParticleUniverse::BaseCollider::CollisionType collisionType = boxCollider->getCollisionType();
	wxString collisionTypeString = COLLT_NONE;
	if (collisionType == ParticleUniverse::BaseCollider::CT_BOUNCE)
	{
		collisionTypeString = COLLT_BOUNCE;
	}
	else if (collisionType == ParticleUniverse::BaseCollider::CT_FLOW)
	{
		collisionTypeString = COLLT_FLOW;
	}
	propTo->SetValueFromString(collisionTypeString);
}
//-----------------------------------------------------------------------
void BoxColliderPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_COLLIDER_FRICTION = _("Friction");
	PRNL_COLLIDER_BOUNCYNESS = _("Bouncyness");
	PRNL_INTERSECTION_TYPE = _("Intersection type");
	PRNL_COLLISION_TYPE = _("Collision type");
	IST_POINT = _("Point");
	IST_BOX = _("Box");
	COLLT_NONE = _("None");
	COLLT_BOUNCE = _("Bounce");
	COLLT_FLOW = _("Flow");
	PRNL_BOX_COLLIDER_WIDTH = _("Box width");
	PRNL_BOX_COLLIDER_HEIGHT = _("Box height");
	PRNL_BOX_COLLIDER_DEPTH = _("Box depth");
	PRNL_BOX_COLLIDER_INNER = _("Inner Collision");

	mHelpHtml = wxT("ColliderBox.html");

	// Width: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_BOX_COLLIDER_WIDTH, PRNL_BOX_COLLIDER_WIDTH, ParticleUniverse::BoxCollider::DEFAULT_WIDTH));
	SetPropertyEditor(PRNL_BOX_COLLIDER_WIDTH, wxPG_EDITOR(SpinCtrl));

	// Height: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_BOX_COLLIDER_HEIGHT, PRNL_BOX_COLLIDER_HEIGHT, ParticleUniverse::BoxCollider::DEFAULT_HEIGHT));
	SetPropertyEditor(PRNL_BOX_COLLIDER_HEIGHT, wxPG_EDITOR(SpinCtrl));

	// Depth: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_BOX_COLLIDER_DEPTH, PRNL_BOX_COLLIDER_DEPTH, ParticleUniverse::BoxCollider::DEFAULT_DEPTH));
	SetPropertyEditor(PRNL_BOX_COLLIDER_DEPTH, wxPG_EDITOR(SpinCtrl));

	// Inner Collision: bool
	Append(wxBoolProperty(PRNL_BOX_COLLIDER_INNER, PRNL_BOX_COLLIDER_INNER, false));

	// Friction: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_COLLIDER_FRICTION, PRNL_COLLIDER_FRICTION, ParticleUniverse::BoxCollider::DEFAULT_FRICTION));
	SetPropertyEditor(PRNL_COLLIDER_FRICTION, wxPG_EDITOR(SpinCtrl));

	// Bouncyness: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_COLLIDER_BOUNCYNESS, PRNL_COLLIDER_BOUNCYNESS, ParticleUniverse::BoxCollider::DEFAULT_BOUNCYNESS));
	SetPropertyEditor(PRNL_COLLIDER_BOUNCYNESS, wxPG_EDITOR(SpinCtrl));

	// Intersection type: List
	mIntersectionTypes.Add(IST_POINT);
	mIntersectionTypes.Add(IST_BOX);
	Append(wxEnumProperty(PRNL_INTERSECTION_TYPE, PRNL_INTERSECTION_TYPE, mIntersectionTypes));

	// Collision type: List
	mCollisionTypes.Add(COLLT_NONE);
	mCollisionTypes.Add(COLLT_BOUNCE);
	mCollisionTypes.Add(COLLT_FLOW);
	Append(wxEnumProperty(PRNL_COLLISION_TYPE, PRNL_COLLISION_TYPE, mCollisionTypes));
}