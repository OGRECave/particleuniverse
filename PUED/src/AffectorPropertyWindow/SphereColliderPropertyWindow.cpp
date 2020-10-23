/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/SphereColliderPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
SphereColliderPropertyWindow::SphereColliderPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
SphereColliderPropertyWindow::SphereColliderPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
SphereColliderPropertyWindow::SphereColliderPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::SphereCollider* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void SphereColliderPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::SphereCollider* affector = static_cast<ParticleUniverse::SphereCollider*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_SPHERE_COLLIDER_RADIUS)
	{
		// Radius: ParticleUniverse::Real
		affector->setRadius(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_SPHERE_COLLIDER_INNER)
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
void SphereColliderPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::SphereCollider* sphereCollider = static_cast<ParticleUniverse::SphereCollider*>(affector);

	// Radius: ParticleUniverse::Real
	doSetDouble(PRNL_SPHERE_COLLIDER_RADIUS, sphereCollider->getRadius());

	// Inner Collision: bool
	doSetBool(PRNL_SPHERE_COLLIDER_INNER, sphereCollider->isInnerCollision());

	// Friction: ParticleUniverse::Real
	doSetDouble(PRNL_COLLIDER_FRICTION, sphereCollider->getFriction());

	// Bouncyness: ParticleUniverse::Real
	doSetDouble(PRNL_COLLIDER_BOUNCYNESS, sphereCollider->getBouncyness());

	// Intersection type: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_INTERSECTION_TYPE);
	ParticleUniverse::BaseCollider::IntersectionType intersectionType = sphereCollider->getIntersectionType();
	wxString intersectionTypeString = IST_POINT;
	if (intersectionType == ParticleUniverse::BaseCollider::IT_BOX)
	{
		intersectionTypeString = IST_BOX;
	}
	propTo->SetValueFromString(intersectionTypeString);

	// Collision type: List
	propTo = GetPropertyPtr(PRNL_COLLISION_TYPE);
	ParticleUniverse::BaseCollider::CollisionType collisionType = sphereCollider->getCollisionType();
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
void SphereColliderPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_SPHERE_COLLIDER_RADIUS = _("Sphere radius");
	PRNL_SPHERE_COLLIDER_INNER = _("Inner Collision");
	PRNL_COLLIDER_FRICTION = _("Friction");
	PRNL_COLLIDER_BOUNCYNESS = _("Bouncyness");
	PRNL_INTERSECTION_TYPE = _("Intersection type");
	PRNL_COLLISION_TYPE = _("Collision type");
	IST_POINT = _("Point");
	IST_BOX = _("Box");
	COLLT_NONE = _("None");
	COLLT_BOUNCE = _("Bounce");
	COLLT_FLOW = _("Flow");

	mHelpHtml = wxT("ColliderSphere.html");

	// Radius: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_SPHERE_COLLIDER_RADIUS, PRNL_SPHERE_COLLIDER_RADIUS, ParticleUniverse::SphereCollider::DEFAULT_RADIUS));
	SetPropertyEditor(PRNL_SPHERE_COLLIDER_RADIUS, wxPG_EDITOR(SpinCtrl));

	// Inner Collision: bool
	Append(wxBoolProperty(PRNL_SPHERE_COLLIDER_INNER, PRNL_SPHERE_COLLIDER_INNER, false));

	// Friction: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_COLLIDER_FRICTION, PRNL_COLLIDER_FRICTION, ParticleUniverse::SphereCollider::DEFAULT_FRICTION));
	SetPropertyEditor(PRNL_COLLIDER_FRICTION, wxPG_EDITOR(SpinCtrl));

	// Bouncyness: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_COLLIDER_BOUNCYNESS, PRNL_COLLIDER_BOUNCYNESS, ParticleUniverse::SphereCollider::DEFAULT_BOUNCYNESS));
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