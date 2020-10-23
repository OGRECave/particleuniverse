/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"

#ifdef PU_PHYSICS_PHYSX

#include "ExternPropertyWindow/PhysXActorExternPropertyWindow.h"
#include "Externs/ParticleUniversePhysXExtern.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
PhysXActorExternPropertyWindow::PhysXActorExternPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ExternPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
PhysXActorExternPropertyWindow::PhysXActorExternPropertyWindow(ExternPropertyWindow* externPropertyWindow) :
	ExternPropertyWindow(externPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
PhysXActorExternPropertyWindow::PhysXActorExternPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::PhysXActorExtern* externObject) :
	ExternPropertyWindow(parent, owner, (static_cast<ParticleUniverse::Extern*>(externObject))->getName())
{
	copyAttributesFromExtern(externObject);
}
//-----------------------------------------------------------------------
void PhysXActorExternPropertyWindow::copyAttributeToExtern(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::Extern* ext = static_cast<ParticleUniverse::Extern*>(mOwner->getPUElement());
	ParticleUniverse::PhysXActorExtern* externObject = static_cast<ParticleUniverse::PhysXActorExtern*>(ext);

	if (propertyName == PRNL_PHYSX_COLLISION_GROUP)
	{
		// Actor Group: ParticleUniverse::uint16
		externObject->setCollisionGroup(prop->DoGetValue().GetLong());
	}
	else if (propertyName == PRNL_PHYSX_SHAPE_TYPE)
	{
		// Shape: list
		wxString shapeType = prop->GetValueAsString();
		if (shapeType == PHYSX_SHAPE_BOX)
		{
			externObject->mPhysicsShapeDesc->mPhysicsShapeType = ParticleUniverse::ST_BOX;
		}
		else if (shapeType == PHYSX_SHAPE_SPHERE)
		{
			externObject->mPhysicsShapeDesc->mPhysicsShapeType = ParticleUniverse::ST_SPHERE;
		}
		else if (shapeType == PHYSX_SHAPE_CAPSULE)
		{
			externObject->mPhysicsShapeDesc->mPhysicsShapeType = ParticleUniverse::ST_CAPSULE;
		}
	}
	else if (propertyName == PRNL_PHYSX_SHAPE_COLLISION_GROUP)
	{
		// Shape Group: ParticleUniverse::uint16
		externObject->mPhysicsShapeDesc->mCollisionGroup = prop->DoGetValue().GetLong();
	}
	else if (propertyName == PRNL_PHYSX_GROUP_MASK + PRNL_X ||
		propertyName == PRNL_PHYSX_GROUP_MASK + PRNL_Y ||
		propertyName == PRNL_PHYSX_GROUP_MASK + PRNL_Z ||
		propertyName == PRNL_PHYSX_GROUP_MASK + PRNL_W)
	{
		// Group Mask: ParticleUniverse::GroupMask
		Ogre::Vector4 v4;
		v4 = doGetVector4(PRNL_PHYSX_GROUP_MASK, v4);
		externObject->mPhysicsShapeDesc->mGroupMask.bits0 = v4.x;
		externObject->mPhysicsShapeDesc->mGroupMask.bits1 = v4.y;
		externObject->mPhysicsShapeDesc->mGroupMask.bits2 = v4.z;
		externObject->mPhysicsShapeDesc->mGroupMask.bits3 = v4.w;
	}
	else if (propertyName == PRNL_PHYSX_ANGULAR_VELOCITY + PRNL_X ||
		propertyName == PRNL_PHYSX_ANGULAR_VELOCITY + PRNL_Y ||
		propertyName == PRNL_PHYSX_ANGULAR_VELOCITY + PRNL_Z)
	{
		// Angular Velocity: Ogre::Vector3
		Ogre::Vector3 v3;
		v3 = doGetVector3(PRNL_PHYSX_ANGULAR_VELOCITY, v3);
		externObject->mPhysicsShapeDesc->mAngularVelocity = v3;
	}
	else if (propertyName == PRNL_PHYSX_ANGULAR_DAMPING)
	{
		// Angular Damping: ParticleUniverse::Real
		externObject->mPhysicsShapeDesc->mAngularDamping = prop->DoGetValue().GetDouble();
	}
	else if (propertyName == PRNL_PHYSX_MATERIAL_INDEX)
	{
		// Material Index: ParticleUniverse::uint16
		externObject->mPhysicsShapeDesc->mMaterialIndex = prop->DoGetValue().GetLong();
	}
	else
	{
		// Update extern with another attribute
		ExternPropertyWindow::copyAttributeToExtern(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void PhysXActorExternPropertyWindow::copyAttributesFromExtern(ParticleUniverse::Extern* externObject)
{
	ExternPropertyWindow::copyAttributesFromExtern(externObject);

	// Copy properties from Extern to property window
	ParticleUniverse::PhysXActorExtern* physXActorExtern = static_cast<ParticleUniverse::PhysXActorExtern*>(externObject);

	// Actor Group: ParticleUniverse::uint16
	doSetUint16(PRNL_PHYSX_COLLISION_GROUP, physXActorExtern->getCollisionGroup());

	// Shape: list
	wxPGProperty* propTo = GetPropertyPtr(PRNL_PHYSX_SHAPE_TYPE);
	ParticleUniverse::PhysicsShapeType shapeType = physXActorExtern->mPhysicsShapeDesc->mPhysicsShapeType;
	wxString shapeTypeString = PHYSX_SHAPE_BOX;
	if (shapeType == ParticleUniverse::ST_SPHERE)
	{
		shapeTypeString = PHYSX_SHAPE_SPHERE;
	}
	else if (shapeType == ParticleUniverse::ST_CAPSULE)
	{
		shapeTypeString = PHYSX_SHAPE_CAPSULE;
	}
	propTo->SetValueFromString(shapeTypeString);

	// Shape Group: ParticleUniverse::uint16
	doSetUint16(PRNL_PHYSX_SHAPE_COLLISION_GROUP, physXActorExtern->mPhysicsShapeDesc->mCollisionGroup);

	// Group Mask: ParticleUniverse::GroupMask
	Ogre::Vector4 v4;
	v4.x = physXActorExtern->mPhysicsShapeDesc->mGroupMask.bits0;
	v4.y = physXActorExtern->mPhysicsShapeDesc->mGroupMask.bits1;
	v4.z = physXActorExtern->mPhysicsShapeDesc->mGroupMask.bits2;
	v4.w = physXActorExtern->mPhysicsShapeDesc->mGroupMask.bits3;
	doSetVector4(PRNL_PHYSX_GROUP_MASK, v4);

	// Angular Velocity: Ogre::Vector3
	doSetVector3(PRNL_PHYSX_ANGULAR_VELOCITY, physXActorExtern->mPhysicsShapeDesc->mAngularVelocity);

	// Angular Damping: ParticleUniverse::Real
	doSetDouble(PRNL_PHYSX_ANGULAR_DAMPING, physXActorExtern->mPhysicsShapeDesc->mAngularDamping);

	// Material Index: ParticleUniverse::uint16
	doSetUint16(PRNL_PHYSX_MATERIAL_INDEX, physXActorExtern->mPhysicsShapeDesc->mMaterialIndex);
}
//-----------------------------------------------------------------------
void PhysXActorExternPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_PHYSX_COLLISION_GROUP = _("Collision group");
	PRNL_PHYSX_SHAPE_TYPE = _("Shape type");
	PRNL_PHYSX_SHAPE_COLLISION_GROUP = _("Shape collision group");
	PRNL_PHYSX_GROUP_MASK = _("Group mask");
	PRNL_PHYSX_ANGULAR_VELOCITY = _("Angular velocity");
	PRNL_PHYSX_ANGULAR_DAMPING = _("Angular damping");
	PRNL_PHYSX_MATERIAL_INDEX = _("Material index");
	PHYSX_SHAPE_BOX = _("Box");
	PHYSX_SHAPE_SPHERE = _("Sphere");
	PHYSX_SHAPE_CAPSULE = _("Capsule");
	PRNL_W = _(".w");
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");

	mHelpHtml = wxT("ExternPhysXActor.html");

	// Actor Group: ParticleUniverse::uint16
	Append(wxUIntProperty(PRNL_PHYSX_COLLISION_GROUP, PRNL_PHYSX_COLLISION_GROUP, 0));
	SetPropertyEditor(PRNL_PHYSX_COLLISION_GROUP, wxPG_EDITOR(SpinCtrl));

	// Shape: list
	wxArrayString shapeTypes;
	shapeTypes.Add(PHYSX_SHAPE_BOX);
	shapeTypes.Add(PHYSX_SHAPE_SPHERE);
	shapeTypes.Add(PHYSX_SHAPE_CAPSULE);
	wxPGId pid = Append(wxEnumProperty(PRNL_PHYSX_SHAPE_TYPE, PRNL_PHYSX_SHAPE_TYPE, shapeTypes));

	// Shape Group: ParticleUniverse::uint16
	Append(wxUIntProperty(PRNL_PHYSX_SHAPE_COLLISION_GROUP, PRNL_PHYSX_SHAPE_COLLISION_GROUP, 0));
	SetPropertyEditor(PRNL_PHYSX_SHAPE_COLLISION_GROUP, wxPG_EDITOR(SpinCtrl));

	// Group Mask: ParticleUniverse::GroupMask
	appendVector4(PRNL_PHYSX_GROUP_MASK, PRNL_PHYSX_GROUP_MASK, Ogre::Vector4::ZERO);

	// Angular Velocity: Ogre::Vector3
	appendVector3(PRNL_PHYSX_ANGULAR_VELOCITY, PRNL_PHYSX_ANGULAR_VELOCITY, Ogre::Vector3::ZERO);

	// Angular Damping: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_PHYSX_ANGULAR_DAMPING, PRNL_PHYSX_ANGULAR_DAMPING, 0));
	SetPropertyEditor(PRNL_PHYSX_ANGULAR_DAMPING, wxPG_EDITOR(SpinCtrl));

	// Material Index: ParticleUniverse::uint16
	Append(wxUIntProperty(PRNL_PHYSX_MATERIAL_INDEX, PRNL_PHYSX_MATERIAL_INDEX, 0));
	SetPropertyEditor(PRNL_PHYSX_MATERIAL_INDEX, wxPG_EDITOR(SpinCtrl));
}
//-----------------------------------------------------------------------
void PhysXActorExternPropertyWindow::onPropertyChanged(wxPropertyGridEvent& event)
{
	// Perform additional validations.
	if (!_validatePropertyFloatPositive(event.GetPropertyPtr(), PRNL_PHYSX_ANGULAR_DAMPING))
		return;

	ExternPropertyWindow::onPropertyChanged(event);
}

#endif //PU_PHYSICS_PHYSX	