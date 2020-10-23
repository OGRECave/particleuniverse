/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/PlaneColliderPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
PlaneColliderPropertyWindow::PlaneColliderPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
PlaneColliderPropertyWindow::PlaneColliderPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
PlaneColliderPropertyWindow::PlaneColliderPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::PlaneCollider* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void PlaneColliderPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::PlaneCollider* affector = static_cast<ParticleUniverse::PlaneCollider*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_PLANE_COLLIDER_NORMAL + PRNL_X ||
		propertyName == PRNL_PLANE_COLLIDER_NORMAL + PRNL_Y ||
		propertyName == PRNL_PLANE_COLLIDER_NORMAL + PRNL_Z)
	{
		// Normal: Ogre::Vector3
		Ogre::Vector3 v3;
		v3 = doGetVector3(PRNL_PLANE_COLLIDER_NORMAL, v3);
		affector->setNormal(v3);
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

		// To activate the new position, reset the plane normal
		Ogre::Vector3 normal = affector->getNormal();
		affector->setNormal(normal);
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
void PlaneColliderPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::PlaneCollider* planeCollider = static_cast<ParticleUniverse::PlaneCollider*>(affector);

	// Normal: Ogre::Vector3
	doSetVector3(PRNL_PLANE_COLLIDER_NORMAL, planeCollider->getNormal());

	// Friction: ParticleUniverse::Real
	doSetDouble(PRNL_COLLIDER_FRICTION, planeCollider->getFriction());

	// Bouncyness: ParticleUniverse::Real
	doSetDouble(PRNL_COLLIDER_BOUNCYNESS, planeCollider->getBouncyness());

	// Intersection type: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_INTERSECTION_TYPE);
	ParticleUniverse::BaseCollider::IntersectionType intersectionType = planeCollider->getIntersectionType();
	wxString intersectionTypeString = IST_POINT;
	if (intersectionType == ParticleUniverse::BaseCollider::IT_BOX)
	{
		intersectionTypeString = IST_BOX;
	}
	propTo->SetValueFromString(intersectionTypeString);

	// Collision type: List
	propTo = GetPropertyPtr(PRNL_COLLISION_TYPE);
	ParticleUniverse::BaseCollider::CollisionType collisionType = planeCollider->getCollisionType();
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
void PlaneColliderPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_PLANE_COLLIDER_NORMAL = _("Plane normal");
	PRNL_COLLIDER_FRICTION = _("Friction");
	PRNL_COLLIDER_BOUNCYNESS = _("Bouncyness");
	PRNL_INTERSECTION_TYPE = _("Intersection type");
	PRNL_COLLISION_TYPE = _("Collision type");
	IST_POINT = _("Point");
	IST_BOX = _("Box");
	COLLT_NONE = _("None");
	COLLT_BOUNCE = _("Bounce");
	COLLT_FLOW = _("Flow");

	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");

	mHelpHtml = wxT("ColliderPlane.html");

	// Normal: Ogre::Vector3
	appendVector3(PRNL_PLANE_COLLIDER_NORMAL, PRNL_PLANE_COLLIDER_NORMAL, ParticleUniverse::PlaneCollider::DEFAULT_NORMAL);

	// Friction: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_COLLIDER_FRICTION, PRNL_COLLIDER_FRICTION, ParticleUniverse::PlaneCollider::DEFAULT_FRICTION));
	SetPropertyEditor(PRNL_COLLIDER_FRICTION, wxPG_EDITOR(SpinCtrl));

	// Bouncyness: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_COLLIDER_BOUNCYNESS, PRNL_COLLIDER_BOUNCYNESS, ParticleUniverse::PlaneCollider::DEFAULT_BOUNCYNESS));
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