/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ExternPropertyWindow/SphereColliderExternPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "AffectorPropertyWindow/SphereColliderPropertyWindow.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
SphereColliderExternPropertyWindow::SphereColliderExternPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ExternPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
SphereColliderExternPropertyWindow::SphereColliderExternPropertyWindow(ExternPropertyWindow* externPropertyWindow) :
	ExternPropertyWindow(externPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
SphereColliderExternPropertyWindow::SphereColliderExternPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::SphereColliderExtern* externObject) :
	ExternPropertyWindow(parent, owner, (static_cast<ParticleUniverse::Extern*>(externObject))->getName())
{
	copyAttributesFromExtern(externObject);
}
//-----------------------------------------------------------------------
void SphereColliderExternPropertyWindow::copyAttributeToExtern(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::Extern* ext = static_cast<ParticleUniverse::Extern*>(mOwner->getPUElement());
	ParticleUniverse::SphereColliderExtern* externObject = static_cast<ParticleUniverse::SphereColliderExtern*>(ext);

	if (propertyName == PRNL_EXTERN_THRESHOLD)
	{
		// Distance Threshold: ParticleUniverse::Real
		ParticleUniverse::Attachable* attachable = static_cast<ParticleUniverse::Attachable*>(mOwner->getPUElement());
		attachable->setDistanceThreshold(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_SPHERE_COLLIDER_RADIUS)
	{
		// Radius: ParticleUniverse::Real
		externObject->setRadius(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_COLLIDER_FRICTION)
	{
		// Friction: ParticleUniverse::Real
		externObject->setFriction(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_COLLIDER_BOUNCYNESS)
	{
		// Bouncyness: ParticleUniverse::Real
		externObject->setBouncyness(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_INTERSECTION_TYPE)
	{
		// Intersection type: List
		wxString intersection = prop->GetValueAsString();
		if (intersection == IST_POINT)
		{
			externObject->setIntersectionType(ParticleUniverse::BaseCollider::IT_POINT);
		}
		else if (intersection == IST_BOX)
		{
			externObject->setIntersectionType(ParticleUniverse::BaseCollider::IT_BOX);
		}
	}
	else if (propertyName == PRNL_COLLISION_TYPE)
	{
		// Collision type: List
		wxString collision = prop->GetValueAsString();
		if (collision == COLLT_BOUNCE)
		{
			externObject->setCollisionType(ParticleUniverse::BaseCollider::CT_BOUNCE);
		}
		else if (collision == COLLT_FLOW)
		{
			externObject->setCollisionType(ParticleUniverse::BaseCollider::CT_FLOW);
		}
		else if (collision == COLLT_NONE)
		{
			externObject->setCollisionType(ParticleUniverse::BaseCollider::CT_NONE);
		}
	}
	else
	{
		// Update extern with another attribute
		ExternPropertyWindow::copyAttributeToExtern(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void SphereColliderExternPropertyWindow::copyAttributesFromExtern(ParticleUniverse::Extern* externObject)
{
	ExternPropertyWindow::copyAttributesFromExtern(externObject);

	// Copy properties from Extern to property window
	ParticleUniverse::SphereColliderExtern* sphereColliderExtern = static_cast<ParticleUniverse::SphereColliderExtern*>(externObject);

	// Distance Threshold: ParticleUniverse::Real
	doSetDouble(PRNL_EXTERN_THRESHOLD, sphereColliderExtern->getDistanceThreshold());

	// Radius: ParticleUniverse::Real
	doSetDouble(PRNL_SPHERE_COLLIDER_RADIUS, sphereColliderExtern->getRadius());

	// Friction: ParticleUniverse::Real
	doSetDouble(PRNL_COLLIDER_FRICTION, sphereColliderExtern->getFriction());

	// Bouncyness: ParticleUniverse::Real
	doSetDouble(PRNL_COLLIDER_BOUNCYNESS, sphereColliderExtern->getBouncyness());

	// Intersection type: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_INTERSECTION_TYPE);
	ParticleUniverse::BaseCollider::IntersectionType intersectionType = sphereColliderExtern->getIntersectionType();
	wxString intersectionTypeString = IST_POINT;
	if (intersectionType == ParticleUniverse::BaseCollider::IT_BOX)
	{
		intersectionTypeString = IST_BOX;
	}
	propTo->SetValueFromString(intersectionTypeString);

	// Collision type: List
	propTo = GetPropertyPtr(PRNL_COLLISION_TYPE);
	ParticleUniverse::BaseCollider::CollisionType collisionType = sphereColliderExtern->getCollisionType();
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
void SphereColliderExternPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	// TODO
	PRNL_COLLIDER_FRICTION = _("Friction");
	PRNL_COLLIDER_BOUNCYNESS = _("Bouncyness");
	PRNL_INTERSECTION_TYPE = _("Intersection type");
	PRNL_COLLISION_TYPE = _("Collision type");
	IST_POINT = _("Point");
	IST_BOX = _("Box");
	COLLT_NONE = _("None");
	COLLT_BOUNCE = _("Bounce");
	COLLT_FLOW = _("Flow");
	PRNL_EXTERN_THRESHOLD = _("Distance threshold");

	mHelpHtml = wxT("ExternSphereCollider.html");

	// Distance Threshold: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_EXTERN_THRESHOLD, PRNL_EXTERN_THRESHOLD, std::numeric_limits<float>::max()));
	SetPropertyEditor(PRNL_EXTERN_THRESHOLD, wxPG_EDITOR(SpinCtrl));

	// Friction: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_COLLIDER_FRICTION, PRNL_COLLIDER_FRICTION, ParticleUniverse::BaseCollider::DEFAULT_FRICTION));
	SetPropertyEditor(PRNL_COLLIDER_FRICTION, wxPG_EDITOR(SpinCtrl));

	// Bouncyness: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_COLLIDER_BOUNCYNESS, PRNL_COLLIDER_BOUNCYNESS, ParticleUniverse::BaseCollider::DEFAULT_BOUNCYNESS));
	SetPropertyEditor(PRNL_COLLIDER_BOUNCYNESS, wxPG_EDITOR(SpinCtrl));

	// Intersection Type: List
	mIntersectionTypes.Add(IST_POINT);
	mIntersectionTypes.Add(IST_BOX);
	Append(wxEnumProperty(PRNL_INTERSECTION_TYPE, PRNL_INTERSECTION_TYPE, mIntersectionTypes));

	// Collision Type: List
	mCollisionTypes.Add(COLLT_NONE);
	mCollisionTypes.Add(COLLT_BOUNCE);
	mCollisionTypes.Add(COLLT_FLOW);
	Append(wxEnumProperty(PRNL_COLLISION_TYPE, PRNL_COLLISION_TYPE, mCollisionTypes));

	// Radius: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_SPHERE_COLLIDER_RADIUS, PRNL_SPHERE_COLLIDER_RADIUS, ParticleUniverse::SphereColliderExtern::DEFAULT_RADIUS));
	SetPropertyEditor(PRNL_SPHERE_COLLIDER_RADIUS, wxPG_EDITOR(SpinCtrl));
}