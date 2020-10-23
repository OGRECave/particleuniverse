/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ExternPropertyWindow/BoxColliderExternPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
BoxColliderExternPropertyWindow::BoxColliderExternPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ExternPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
BoxColliderExternPropertyWindow::BoxColliderExternPropertyWindow(ExternPropertyWindow* externPropertyWindow) :
	ExternPropertyWindow(externPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
BoxColliderExternPropertyWindow::BoxColliderExternPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::BoxColliderExtern* externObject) :
ExternPropertyWindow(parent, owner, (static_cast<ParticleUniverse::Extern*>(externObject))->getName())
{
	copyAttributesFromExtern(externObject);
}
//-----------------------------------------------------------------------
void BoxColliderExternPropertyWindow::copyAttributeToExtern(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::Extern* ext = static_cast<ParticleUniverse::Extern*>(mOwner->getPUElement());
	ParticleUniverse::BoxColliderExtern* externObject = static_cast<ParticleUniverse::BoxColliderExtern*>(ext);

	if (propertyName == PRNL_EXTERN_THRESHOLD)
	{
		// Distance Threshold: ParticleUniverse::Real
		ParticleUniverse::Attachable* attachable = static_cast<ParticleUniverse::Attachable*>(mOwner->getPUElement());
		attachable->setDistanceThreshold(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_BOX_COLLIDER_WIDTH)
	{
		// Width: ParticleUniverse::Real
		externObject->setWidth(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_BOX_COLLIDER_HEIGHT)
	{
		// Height: ParticleUniverse::Real
		externObject->setHeight(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_BOX_COLLIDER_DEPTH)
	{
		// Depth: ParticleUniverse::Real
		externObject->setDepth(prop->DoGetValue().GetDouble());
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
void BoxColliderExternPropertyWindow::copyAttributesFromExtern(ParticleUniverse::Extern* externObject)
{
	ExternPropertyWindow::copyAttributesFromExtern(externObject);

	// Copy properties from Extern to property window
	ParticleUniverse::BoxColliderExtern* boxColliderExtern = static_cast<ParticleUniverse::BoxColliderExtern*>(externObject);

	// Distance Threshold: ParticleUniverse::Real
	doSetDouble(PRNL_EXTERN_THRESHOLD, boxColliderExtern->getDistanceThreshold());

	// Width: ParticleUniverse::Real
	doSetDouble(PRNL_BOX_COLLIDER_WIDTH, boxColliderExtern->getWidth());

	// Height: ParticleUniverse::Real
	doSetDouble(PRNL_BOX_COLLIDER_HEIGHT, boxColliderExtern->getHeight());

	// Depth: ParticleUniverse::Real
	doSetDouble(PRNL_BOX_COLLIDER_DEPTH, boxColliderExtern->getDepth());

	// Friction: ParticleUniverse::Real
	doSetDouble(PRNL_COLLIDER_FRICTION, boxColliderExtern->getFriction());

	// Bouncyness: ParticleUniverse::Real
	doSetDouble(PRNL_COLLIDER_BOUNCYNESS, boxColliderExtern->getBouncyness());

	// Intersection type: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_INTERSECTION_TYPE);
	ParticleUniverse::BaseCollider::IntersectionType intersectionType = boxColliderExtern->getIntersectionType();
	wxString intersectionTypeString = IST_POINT;
	if (intersectionType == ParticleUniverse::BaseCollider::IT_BOX)
	{
		intersectionTypeString = IST_BOX;
	}
	propTo->SetValueFromString(intersectionTypeString);

	// Collision type: List
	propTo = GetPropertyPtr(PRNL_COLLISION_TYPE);
	ParticleUniverse::BaseCollider::CollisionType collisionType = boxColliderExtern->getCollisionType();
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
void BoxColliderExternPropertyWindow::_initProperties(void)
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

	mHelpHtml = wxT("ExternBoxCollider.html");

	// Distance Threshold: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_EXTERN_THRESHOLD, PRNL_EXTERN_THRESHOLD, std::numeric_limits<float>::max()));
	SetPropertyEditor(PRNL_EXTERN_THRESHOLD, wxPG_EDITOR(SpinCtrl));

	// Friction: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_COLLIDER_FRICTION, PRNL_COLLIDER_FRICTION, ParticleUniverse::BoxCollider::DEFAULT_FRICTION));
	SetPropertyEditor(PRNL_COLLIDER_FRICTION, wxPG_EDITOR(SpinCtrl));

	// Bouncyness: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_COLLIDER_BOUNCYNESS, PRNL_COLLIDER_BOUNCYNESS, ParticleUniverse::BoxCollider::DEFAULT_BOUNCYNESS));
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

	// Box Width: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_BOX_COLLIDER_WIDTH, PRNL_BOX_COLLIDER_WIDTH, ParticleUniverse::BoxCollider::DEFAULT_WIDTH));
	SetPropertyEditor(PRNL_BOX_COLLIDER_WIDTH, wxPG_EDITOR(SpinCtrl));

	// Box Height: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_BOX_COLLIDER_HEIGHT, PRNL_BOX_COLLIDER_HEIGHT, ParticleUniverse::BoxCollider::DEFAULT_HEIGHT));
	SetPropertyEditor(PRNL_BOX_COLLIDER_HEIGHT, wxPG_EDITOR(SpinCtrl));

	// Box Depth: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_BOX_COLLIDER_DEPTH, PRNL_BOX_COLLIDER_DEPTH, ParticleUniverse::BoxCollider::DEFAULT_DEPTH));
	SetPropertyEditor(PRNL_BOX_COLLIDER_DEPTH, wxPG_EDITOR(SpinCtrl));
}