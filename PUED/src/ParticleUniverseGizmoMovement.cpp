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
#include "ParticleUniverseGizmoMovement.h"
#include "ParticleUniverseGizmoManager.h"
#include "ParticleUniverseUtils.h"

const Ogre::Real GizmoMovement::SCALE_NODE_MOVEMENT = 0.5f;
//-----------------------------------------------------------------------
GizmoMovement::GizmoMovement(GizmoManager* gizmoManager, Ogre::SceneManager* sceneManager) : 
	Gizmo(gizmoManager),
	mMovementNodeX(0),
	mMovementNodeY(0),
	mMovementNodeZ(0)
{
	mLastPosition = Ogre::Vector2(0.5, 0.5);
	mMovementEntityX = sceneManager->createEntity("pu_movementEntityX", "pu_axis.mesh");
	mMovementEntityY = sceneManager->createEntity("pu_movementEntityY", "pu_axis.mesh");
	mMovementEntityZ = sceneManager->createEntity("pu_movementEntityZ", "pu_axis.mesh");

	mMovementEntityX->setMaterialName("pu_x_axis"); // Red
	mMovementEntityY->setMaterialName("pu_y_axis"); // Green
	mMovementEntityZ->setMaterialName("pu_z_axis"); // Blue

	mMovementEntityX->setQueryFlags(GizmoManager::GIZMO_FLAG);
	mMovementEntityY->setQueryFlags(GizmoManager::GIZMO_FLAG);
	mMovementEntityZ->setQueryFlags(GizmoManager::GIZMO_FLAG);

	mMovementEntityX->setRenderQueueGroup(100);
	mMovementEntityY->setRenderQueueGroup(100);
	mMovementEntityZ->setRenderQueueGroup(100);
}
//-----------------------------------------------------------------------
GizmoMovement::~GizmoMovement(void)
{
	detachFromNode();
}
//-----------------------------------------------------------------------
void GizmoMovement::attachToNode(Ogre::SceneNode* node)
{
	if (!node)
		return;

	// First remove the old nodes
	detachFromNode();

	// Recreate the movement nodes
	mMovementNodeX = node->createChildSceneNode();
	mMovementNodeY = node->createChildSceneNode();
	mMovementNodeZ = node->createChildSceneNode();

	// For testing purposes
	//mMovementNodeX->showBoundingBox(true);
	//mMovementNodeY->showBoundingBox(true);
	//mMovementNodeZ->showBoundingBox(true);

	mMovementNodeX->setInheritScale(false);
	mMovementNodeY->setInheritScale(false);
	mMovementNodeZ->setInheritScale(false);

	mMovementNodeX->setInheritOrientation(false);
	mMovementNodeY->setInheritOrientation(false);
	mMovementNodeZ->setInheritOrientation(false);

	mMovementNodeX->attachObject(mMovementEntityX);
	mMovementNodeY->attachObject(mMovementEntityY);
	mMovementNodeZ->attachObject(mMovementEntityZ);

	// Set scale
	mMovementNodeX->setScale(SCALE_NODE_MOVEMENT, SCALE_NODE_MOVEMENT, SCALE_NODE_MOVEMENT);
	mMovementNodeY->setScale(SCALE_NODE_MOVEMENT, SCALE_NODE_MOVEMENT, SCALE_NODE_MOVEMENT);
	mMovementNodeZ->setScale(SCALE_NODE_MOVEMENT, SCALE_NODE_MOVEMENT, SCALE_NODE_MOVEMENT);

	// Set orientation
	applyWorldLocalSpaceSetting();

	// Set material
	mMovementEntityX->setMaterialName("pu_x_axis");
	mMovementEntityY->setMaterialName("pu_y_axis");
	mMovementEntityZ->setMaterialName("pu_z_axis");
}
//-----------------------------------------------------------------------
Ogre::SceneNode* GizmoMovement::detachFromNode(void)
{
	Ogre::SceneNode* parent = 0;

	if (mMovementNodeX)
	{
		parent = mMovementNodeX->getParentSceneNode();
		parent->removeAndDestroyChild(mMovementNodeX->getName());
		mMovementNodeX = 0;
	}
	if (mMovementNodeY)
	{
		parent = mMovementNodeY->getParentSceneNode();
		parent->removeAndDestroyChild(mMovementNodeY->getName());
		mMovementNodeY = 0;
	}
	if (mMovementNodeZ)
	{
		parent = mMovementNodeZ->getParentSceneNode();
		parent->removeAndDestroyChild(mMovementNodeZ->getName());
		mMovementNodeZ = 0;
	}
	
	return parent;
}
//-----------------------------------------------------------------------
void GizmoMovement::startSelect(const Ogre::Vector2& screenPosition)
{
	mLastPosition = screenPosition;
}
//-----------------------------------------------------------------------
bool GizmoMovement::select(const Ogre::RaySceneQueryResult& queryResult, const Ogre::ColourValue& pixelColour)
{
	// Check whether one of the objects of which this Gizmo exists is is the RaySceneQueryResult
	Ogre::RaySceneQueryResult::const_iterator it;
	Ogre::RaySceneQueryResult::const_iterator itEnd = queryResult.end();
	bool exists = false;
	Ogre::MovableObject* movableObject = 0;
	for (it = queryResult.begin(); it != itEnd; ++it)
	{
		movableObject = (*it).movable;
		if (movableObject == mMovementEntityX ||
			movableObject == mMovementEntityY ||
			movableObject == mMovementEntityZ)
		{
			exists = true;
		}
	}

	// If none of the objects was it, forget it
	if (!exists)
	{
		return false;
	}

	// Validate the pixelcolour (at the x,y location)
	if (pixelColour.r > 0.6 && pixelColour.g < 0.05 && pixelColour.b < 0.05)
	{
		mMovementEntityX->setMaterialName("pu_gizmo_select");
		mMovementEntityY->setMaterialName("pu_y_axis");
		mMovementEntityZ->setMaterialName("pu_z_axis");
		mAxis = AXIS_X;
	}
	else if (pixelColour.r < 0.05 && pixelColour.g > 0.6 && pixelColour.b < 0.05)
	{
		mMovementEntityX->setMaterialName("pu_x_axis");
		mMovementEntityY->setMaterialName("pu_gizmo_select");
		mMovementEntityZ->setMaterialName("pu_z_axis");
		mAxis = AXIS_Y;
	}
	else if (pixelColour.r < 0.05 && pixelColour.g < 0.05 && pixelColour.b > 0.6)
	{
		mMovementEntityX->setMaterialName("pu_x_axis");
		mMovementEntityY->setMaterialName("pu_y_axis");
		mMovementEntityZ->setMaterialName("pu_gizmo_select");
		mAxis = AXIS_Z;
	}

	// Return true, because the gizmo WAS picked, only the user missed it (prevents that the gizmo attached to another node)
	return true;
}
//-----------------------------------------------------------------------
void GizmoMovement::endSelect(void)
{
	// No implementation (yet)
}
//-----------------------------------------------------------------------
const Ogre::Vector3& GizmoMovement::getPosition (void) const
{
	//return mMovementNodeX->getPosition();
	return mMovementNodeX->_getDerivedPosition();
}
//-----------------------------------------------------------------------
void GizmoMovement::setPosition (const Ogre::Vector3& position)
{
	mMovementNodeZ->getParentSceneNode()->setPosition(position);
}
//-----------------------------------------------------------------------
void GizmoMovement::setOrientation (const Ogre::Quaternion& orientation)
{
	if (mMovementNodeX)
	{
		mMovementNodeX->getParentSceneNode()->setOrientation(orientation);
	}
}
//-----------------------------------------------------------------------
const Ogre::Quaternion& GizmoMovement::getOrientation (void) const
{
	if (mMovementNodeX)
	{
		return mMovementNodeX->getParentSceneNode()->getOrientation();
	}
	return Ogre::Quaternion::IDENTITY;
}
//-----------------------------------------------------------------------
void GizmoMovement::move (Ogre::Vector2 screenPosition, Ogre::Camera* camera)
{
	if (!mMovementNodeZ || !mMovementNodeZ->getParentSceneNode())
		return;

	Ogre::Real diffX = screenPosition.x - mLastPosition.x;
	Ogre::Real diffY = screenPosition.y - mLastPosition.y;
	Ogre::Real l = 0;
	Ogre::Vector2 sign;
	switch(mAxis)
	{
		case AXIS_X:
		{
			getAxisDirectionOnScreen(AXIS_X, camera, sign);
			diffX *= sign.x;
			diffY *= sign.y;
			l = mLogScaleFactor * (diffX + diffY);

			if (mWorldSpace)
			{
				// Translation over world x-axis.
				mMovementNodeZ->getParentSceneNode()->translate(l, 0, 0);
			}
			else
			{
				// Translation over x-axis of the node to which the Gizmo is attached
				Ogre::Vector3 vectorX = mMovementNodeX->getParentSceneNode()->getOrientation().xAxis();
				vectorX.normalise();
				mMovementNodeX->getParentSceneNode()->translate(l * vectorX);
			}
		}
		break;

		case AXIS_Y:
		{
			getAxisDirectionOnScreen(AXIS_Y, camera, sign);
			diffX *= sign.x;
			diffY *= sign.y;
			l = mLogScaleFactor * (diffX + diffY);

			if (mWorldSpace)
			{
				// Translation over world y-axis
				mMovementNodeZ->getParentSceneNode()->translate(0, -l, 0);
			}
			else
			{
				// Translation over y-axis of the node to which the Gizmo is attached
				Ogre::Vector3 vectorY = mMovementNodeY->getParentSceneNode()->getOrientation().yAxis();
				vectorY.normalise();
				mMovementNodeY->getParentSceneNode()->translate(-l * vectorY);
			}
		}
		break;

		case AXIS_Z:
		{
			getAxisDirectionOnScreen(AXIS_Z, camera, sign);
			diffX *= sign.x;
			diffY *= sign.y;
			l = mLogScaleFactor * (diffX + diffY);

			if (mWorldSpace)
			{
				// Translation over world z-axis
				mMovementNodeZ->getParentSceneNode()->translate(0, 0, l);
			}
			else
			{
				// Translation over z-axis of the node to which the Gizmo is attached
				Ogre::Vector3 vectorZ = mMovementNodeZ->getParentSceneNode()->getOrientation().zAxis();
				vectorZ.normalise();
				mMovementNodeZ->getParentSceneNode()->translate(l * vectorZ);
			}
		}
		break;
	}

	mLastPosition = screenPosition;
}
//-----------------------------------------------------------------------
bool GizmoMovement::isVisible(void) const
{
	return mMovementEntityX->isVisible();
}
//-----------------------------------------------------------------------
void GizmoMovement::setVisible(bool visible)
{
	mMovementEntityX->setVisible(visible);
	mMovementEntityY->setVisible(visible);
	mMovementEntityZ->setVisible(visible);
}
//-----------------------------------------------------------------------
Ogre::SceneNode* GizmoMovement::getParentNode(void)
{
	if (mMovementNodeZ)
	{
		return mMovementNodeZ->getParentSceneNode();
	}

	return 0;
}
//-----------------------------------------------------------------------
const Gizmo::GizmoType GizmoMovement::getType(void) const
{
	return GIZMO_MOVE;
}
//-----------------------------------------------------------------------
void GizmoMovement::cameraPreRenderScene (Ogre::Camera *camera)
{
	// Scale the gizmo
	if (mMovementNodeX && mMovementNodeY && mMovementNodeZ)
	{
		//calculateScaleFactors(camera, mMovementNodeZ->getParentSceneNode()->getPosition());
		calculateScaleFactors(camera, mMovementNodeZ->getParentSceneNode()->_getDerivedPosition());
		mMovementNodeX->setScale(mScaleFactor, mScaleFactor, mScaleFactor);
		mMovementNodeY->setScale(mScaleFactor, mScaleFactor, mScaleFactor);
		mMovementNodeZ->setScale(mScaleFactor, mScaleFactor, mScaleFactor);
	}
}
//-----------------------------------------------------------------------
void GizmoMovement::setWorldspace(bool worldspace)
{
	Gizmo::setWorldspace(worldspace);

	if (!mMovementNodeX || !mMovementNodeY || !mMovementNodeZ)
		return;

	// Set orientation
	Ogre::Real sqrt0dot5 = sqrt(0.5);
	Ogre::Quaternion xQ(sqrt0dot5, 0, 0, sqrt0dot5);
	Ogre::Quaternion yQ(1, 0, 0, 0);
	Ogre::Quaternion zQ(-sqrt0dot5, sqrt0dot5, 0, 0);

	if (mWorldSpace)
	{
		mMovementNodeX->setOrientation(xQ);
		mMovementNodeY->setOrientation(yQ);
		mMovementNodeZ->setOrientation(zQ);
	}
	else
	{
		Ogre::Node* node = mMovementNodeX->getParent();
		if (node)
		{
			mMovementNodeX->setOrientation(node->getOrientation() * xQ);
			mMovementNodeY->setOrientation(node->getOrientation() * yQ);
			mMovementNodeZ->setOrientation(node->getOrientation() * zQ);
		}
	}
}
//-----------------------------------------------------------------------
void GizmoMovement::applyWorldLocalSpaceSetting(void)
{
	setWorldspace(mWorldSpace);
}
//-----------------------------------------------------------------------
void GizmoMovement::getAxisDirectionOnScreen(Gizmo::Axis axis, Ogre::Camera* camera, Ogre::Vector2& sign)
{
	// Returns 1 or -1
	switch (axis)
	{
		case AXIS_X:
			{
				/** Take the position of the node and convert to screen position. Do the same with the x-axis (= position of the node +
					direction vector). Note the use of .yAxis(). The x-axis of the gimzo is in fact the local y-axis, but rotated 90 degrees.
				*/
				Ogre::Vector3 xAxis = mMovementNodeX->_getDerivedOrientation().yAxis() + mMovementNodeX->_getDerivedPosition();
				Ogre::Vector2 center = Utils::convertWorldToScreenPosition(mMovementNodeX->_getDerivedPosition(), camera);
				Ogre::Vector2 direction = Utils::convertWorldToScreenPosition(xAxis, camera);
				direction.x += 0.0001f;
				sign.x = center.x < direction.x ? -1 : 1;
				sign.y = center.y < direction.y ? -1 : 1;
				return;
			}
			break;
		case AXIS_Y:
			{
				if (mMovementNodeY->_getDerivedOrientation().yAxis() == Ogre::Vector3::UNIT_Y)
				{
					sign.x = 1;
					sign.y = 1;
					return;
				}

				if (mMovementNodeY->_getDerivedOrientation().yAxis() == Ogre::Vector3::NEGATIVE_UNIT_Y)
				{
					sign.x = -1;
					sign.y = -1;
					return;
				}

				Ogre::Vector3 yAxis = mMovementNodeY->_getDerivedOrientation().yAxis() + mMovementNodeY->_getDerivedPosition();
				Ogre::Vector2 center = Utils::convertWorldToScreenPosition(mMovementNodeY->_getDerivedPosition(), camera);
				Ogre::Vector2 direction = Utils::convertWorldToScreenPosition(yAxis, camera);
				direction.x += 0.0001f;
				sign.x = center.x < direction.x ? -1 : 1;
				sign.y = center.y < direction.y ? -1 : 1;
				return;
			}
			break;
		case AXIS_Z:
			{
				Ogre::Vector3 zAxis = mMovementNodeZ->_getDerivedOrientation().yAxis() + mMovementNodeZ->_getDerivedPosition();
				Ogre::Vector2 center = Utils::convertWorldToScreenPosition(mMovementNodeZ->_getDerivedPosition(), camera);
				Ogre::Vector2 direction = Utils::convertWorldToScreenPosition(zAxis, camera);
				direction.x += 0.0001f;
				sign.x = center.x < direction.x ? -1 : 1;
				sign.y = center.y < direction.y ? -1 : 1;
				return;
			}
			break;
	}

	return;
}
