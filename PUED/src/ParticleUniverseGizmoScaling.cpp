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
#include "ParticleUniverseGizmoScaling.h"
#include "ParticleUniverseGizmoManager.h"
#include "ParticleUniverseUtils.h"

const Ogre::Real GizmoScaling::SCALE_NODE_SCALING = 0.5f;
//-----------------------------------------------------------------------
GizmoScaling::GizmoScaling(GizmoManager* gizmoManager, Ogre::SceneManager* sceneManager) : 
	Gizmo(gizmoManager),
	mScalingNodeBox(0),
	mScalingNodeX(0),
	mScalingNodeY(0),
	mScalingNodeZ(0)
{
	mLastPosition = Ogre::Vector2(0.5, 0.5);
	mScalingEntityBox = sceneManager->createEntity("pu_scalingEntityBox", "pu_box_scale.mesh");
	mScalingEntityX = sceneManager->createEntity("pu_scalingEntityX", "pu_arrow_scale.mesh");
	mScalingEntityY = sceneManager->createEntity("pu_scalingEntityY", "pu_arrow_scale.mesh");
	mScalingEntityZ = sceneManager->createEntity("pu_scalingEntityZ", "pu_arrow_scale.mesh");

	mScalingEntityBox->setMaterialName("pu_white");
	mScalingEntityX->setMaterialName("pu_x_axis"); // Red
	mScalingEntityY->setMaterialName("pu_y_axis"); // Green
	mScalingEntityZ->setMaterialName("pu_z_axis"); // Blue

	mScalingEntityBox->setQueryFlags(GizmoManager::GIZMO_FLAG);
	mScalingEntityX->setQueryFlags(GizmoManager::GIZMO_FLAG);
	mScalingEntityY->setQueryFlags(GizmoManager::GIZMO_FLAG);
	mScalingEntityZ->setQueryFlags(GizmoManager::GIZMO_FLAG);

	mScalingEntityBox->setRenderQueueGroup(100);
	mScalingEntityX->setRenderQueueGroup(100);
	mScalingEntityY->setRenderQueueGroup(100);
	mScalingEntityZ->setRenderQueueGroup(100);
}
//-----------------------------------------------------------------------
GizmoScaling::~GizmoScaling(void)
{
	detachFromNode();
}
//-----------------------------------------------------------------------
void GizmoScaling::attachToNode(Ogre::SceneNode* node)
{
	if (!node)
		return;

	// First remove the old nodes
	detachFromNode();

	// Recreate the scaling nodes
	mScalingNodeBox = node->createChildSceneNode();
	mScalingNodeX = node->createChildSceneNode();
	mScalingNodeY = node->createChildSceneNode();
	mScalingNodeZ = node->createChildSceneNode();

	mScalingNodeBox->setInheritScale(false);
	mScalingNodeX->setInheritScale(false);
	mScalingNodeY->setInheritScale(false);
	mScalingNodeZ->setInheritScale(false);

	mScalingNodeBox->setInheritOrientation(false);
	mScalingNodeX->setInheritOrientation(false);
	mScalingNodeY->setInheritOrientation(false);
	mScalingNodeZ->setInheritOrientation(false);

	mScalingNodeBox->attachObject(mScalingEntityBox);
	mScalingNodeX->attachObject(mScalingEntityX);
	mScalingNodeY->attachObject(mScalingEntityY);
	mScalingNodeZ->attachObject(mScalingEntityZ);

	// Set scale
	mScalingNodeBox->setScale(SCALE_NODE_SCALING, SCALE_NODE_SCALING, SCALE_NODE_SCALING);
	mScalingNodeX->setScale(SCALE_NODE_SCALING, SCALE_NODE_SCALING, SCALE_NODE_SCALING);
	mScalingNodeY->setScale(SCALE_NODE_SCALING, SCALE_NODE_SCALING, SCALE_NODE_SCALING);
	mScalingNodeZ->setScale(SCALE_NODE_SCALING, SCALE_NODE_SCALING, SCALE_NODE_SCALING);

	// Set orientation
	Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY;
	orientation.FromAngleAxis(Ogre::Radian(1.570796325), Ogre::Vector3::UNIT_Z);
	mScalingNodeX->setOrientation(orientation);

	orientation = Ogre::Quaternion::IDENTITY;
	orientation.FromAngleAxis(Ogre::Radian(-1.570796325), Ogre::Vector3::UNIT_X);
	mScalingNodeZ->setOrientation(orientation);

	// Set material
	mScalingEntityBox->setMaterialName("pu_white");
	mScalingEntityX->setMaterialName("pu_x_axis");
	mScalingEntityY->setMaterialName("pu_y_axis");
	mScalingEntityZ->setMaterialName("pu_z_axis");
}
//-----------------------------------------------------------------------
Ogre::SceneNode* GizmoScaling::detachFromNode(void)
{
	Ogre::SceneNode* parent = 0;

	if (mScalingNodeBox)
	{
		parent = mScalingNodeBox->getParentSceneNode();
		parent->removeAndDestroyChild(mScalingNodeBox->getName());
		mScalingNodeBox = 0;
	}
	if (mScalingNodeX)
	{
		parent = mScalingNodeX->getParentSceneNode();
		parent->removeAndDestroyChild(mScalingNodeX->getName());
		mScalingNodeX = 0;
	}
	if (mScalingNodeY)
	{
		parent = mScalingNodeY->getParentSceneNode();
		parent->removeAndDestroyChild(mScalingNodeY->getName());
		mScalingNodeY = 0;
	}
	if (mScalingNodeZ)
	{
		parent = mScalingNodeZ->getParentSceneNode();
		parent->removeAndDestroyChild(mScalingNodeZ->getName());
		mScalingNodeZ = 0;
	}
	
	return parent;
}
//-----------------------------------------------------------------------
void GizmoScaling::startSelect(const Ogre::Vector2& screenPosition)
{
	mLastPosition = screenPosition;
}
//-----------------------------------------------------------------------
bool GizmoScaling::select(const Ogre::RaySceneQueryResult& queryResult, const Ogre::ColourValue& pixelColour)
{
	// Check whether one of the objects of which this Gizmo exists is is the RaySceneQueryResult
	Ogre::RaySceneQueryResult::const_iterator it;
	Ogre::RaySceneQueryResult::const_iterator itEnd = queryResult.end();
	bool exists = false;
	Ogre::MovableObject* movableObject = 0;
	for (it = queryResult.begin(); it != itEnd; ++it)
	{
		movableObject = (*it).movable;
		if (movableObject == mScalingEntityBox ||
			movableObject == mScalingEntityX ||
			movableObject == mScalingEntityY ||
			movableObject == mScalingEntityZ)
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
	if (pixelColour.r > 0.6 && pixelColour.g > 0.6 && pixelColour.b > 0.6)
	{
		mScalingEntityBox->setMaterialName("pu_gizmo_select");
		mScalingEntityX->setMaterialName("pu_x_axis");
		mScalingEntityY->setMaterialName("pu_y_axis");
		mScalingEntityZ->setMaterialName("pu_z_axis");
		mAxis = AXIS_OUTER;
	}
	else if (pixelColour.r > 0.6 && pixelColour.g < 0.05 && pixelColour.b < 0.05)
	{
		mScalingEntityBox->setMaterialName("pu_white");
		mScalingEntityX->setMaterialName("pu_gizmo_select");
		mScalingEntityY->setMaterialName("pu_y_axis");
		mScalingEntityZ->setMaterialName("pu_z_axis");
		mAxis = AXIS_X;
	}
	else if (pixelColour.r < 0.05 && pixelColour.g > 0.6 && pixelColour.b < 0.05)
	{
		mScalingEntityBox->setMaterialName("pu_white");
		mScalingEntityX->setMaterialName("pu_x_axis");
		mScalingEntityY->setMaterialName("pu_gizmo_select");
		mScalingEntityZ->setMaterialName("pu_z_axis");
		mAxis = AXIS_Y;
	}
	else if (pixelColour.r < 0.05 && pixelColour.g < 0.05 && pixelColour.b > 0.6)
	{
		mScalingEntityBox->setMaterialName("pu_white");
		mScalingEntityX->setMaterialName("pu_x_axis");
		mScalingEntityY->setMaterialName("pu_y_axis");
		mScalingEntityZ->setMaterialName("pu_gizmo_select");
		mAxis = AXIS_Z;
	}

	// Return true, because the gizmo WAS picked, only the user missed it (prevents that the gizmo attached to another node)
	return true;
}
//-----------------------------------------------------------------------
void GizmoScaling::endSelect(void)
{
	// No implementation (yet)
}
//-----------------------------------------------------------------------
const Ogre::Vector3& GizmoScaling::getPosition (void) const
{
	//return mScalingNodeX->getPosition();
	return mScalingNodeX->_getDerivedPosition();
}
//-----------------------------------------------------------------------
void GizmoScaling::setPosition (const Ogre::Vector3& position)
{
	mScalingNodeBox->setPosition(position);
	mScalingNodeX->setPosition(position);
	mScalingNodeY->setPosition(position);
	mScalingNodeZ->setPosition(position);
}
//-----------------------------------------------------------------------
void GizmoScaling::setOrientation (const Ogre::Quaternion& orientation)
{
	if (mScalingNodeX)
	{
		mScalingNodeX->getParentSceneNode()->setOrientation(orientation);
	}
}
//-----------------------------------------------------------------------
const Ogre::Quaternion& GizmoScaling::getOrientation (void) const
{
	if (mScalingNodeX)
	{
		return mScalingNodeX->getParentSceneNode()->getOrientation();
	}
	return Ogre::Quaternion::IDENTITY;
}
//-----------------------------------------------------------------------
void GizmoScaling::scale (Ogre::Vector2 screenPosition, Ogre::Camera* camera)
{
	Ogre::Real diffX = screenPosition.x - mLastPosition.x;
	Ogre::Real diffY = mLastPosition.y - screenPosition.y;
	Ogre::Real l = mLogScaleFactor * 0.1 * (diffX + diffY);

	Ogre::SceneNode* node = mScalingNodeZ->getParentSceneNode();
	if (!node)
		return;

	Ogre::Vector3 scale = node->getScale();

	// Set the boundingbox
	node->showBoundingBox(true);

	switch(mAxis)
	{
		case AXIS_OUTER:
		{
			scale.x += l;
			scale.y += l;
			scale.z += l;
		}
		case AXIS_X:
		{
			scale.x -= (l * getAxisDirectionOnScreen(AXIS_X, camera));
		}
		break;

		case AXIS_Y:
		{
			scale.y -= (l * getAxisDirectionOnScreen(AXIS_Y, camera));
		}
		break;

		case AXIS_Z:
		{
			scale.z -= (l * getAxisDirectionOnScreen(AXIS_Z, camera));
		}
		break;
	}

	mScalingNodeZ->getParentSceneNode()->setScale(scale);
	mLastPosition = screenPosition;
}
//-----------------------------------------------------------------------
const Ogre::Vector3& GizmoScaling::getScale(void) const
{
	if (mScalingNodeBox)
	{
		return mScalingNodeBox->getParentSceneNode()->getScale();
	}
	return Ogre::Vector3::UNIT_SCALE;
}
//-----------------------------------------------------------------------
void GizmoScaling::setScale (const Ogre::Vector3& scale)
{
	if (mScalingNodeBox)
	{
		return mScalingNodeBox->getParentSceneNode()->setScale(scale);
	}
}
//-----------------------------------------------------------------------
bool GizmoScaling::isVisible(void) const
{
	return mScalingEntityX->isVisible();
}
//-----------------------------------------------------------------------
void GizmoScaling::setVisible(bool visible)
{
	mScalingEntityBox->setVisible(visible);
	mScalingEntityX->setVisible(visible);
	mScalingEntityY->setVisible(visible);
	mScalingEntityZ->setVisible(visible);
}
//-----------------------------------------------------------------------
Ogre::SceneNode* GizmoScaling::getParentNode(void)
{
	if (mScalingNodeZ)
	{
		return mScalingNodeZ->getParentSceneNode();
	}
	return 0;
}
//-----------------------------------------------------------------------
const Gizmo::GizmoType GizmoScaling::getType(void) const
{
	return GIZMO_SCALE;
}
//-----------------------------------------------------------------------
void GizmoScaling::cameraPreRenderScene (Ogre::Camera *camera)
{
	// Scale the gizmo
	if (mScalingNodeBox && mScalingNodeX && mScalingNodeY && mScalingNodeZ)
	{
		//calculateScaleFactors(camera, mScalingNodeBox->getParentSceneNode()->getPosition());
		calculateScaleFactors(camera, mScalingNodeBox->getParentSceneNode()->_getDerivedPosition());
		mScalingNodeBox->setScale(mScaleFactor, mScaleFactor, mScaleFactor);
		mScalingNodeX->setScale(mScaleFactor, mScaleFactor, mScaleFactor);
		mScalingNodeY->setScale(mScaleFactor, mScaleFactor, mScaleFactor);
		mScalingNodeZ->setScale(mScaleFactor, mScaleFactor, mScaleFactor);
	}
}
//-----------------------------------------------------------------------
Ogre::Real GizmoScaling::getAxisDirectionOnScreen(Gizmo::Axis axis, Ogre::Camera* camera)
{
	// Returns 1 or -1
	switch (axis)
	{
		case AXIS_X:
			{
				/** Take the position of the node and convert to screen position. Do the same with the x-axis (= position of the node +
					direction vector). Note the use of .yAxis(). The x-axis of the gimzo is in fact the local y-axis, but rotated 90 degrees.
				*/
				Ogre::Vector3 xAxis = mScalingNodeX->_getDerivedOrientation().yAxis() + mScalingNodeX->_getDerivedPosition();
				Ogre::Vector2 center = Utils::convertWorldToScreenPosition(mScalingNodeX->_getDerivedPosition(), camera);
				Ogre::Vector2 direction = Utils::convertWorldToScreenPosition(xAxis, camera);
				direction.x += 0.0001f;
				Ogre::Real sign = center.x < direction.x ? -1 : 1;
				return sign;
			}
			break;
		case AXIS_Y:
			{
				Ogre::Vector3 yAxis = mScalingNodeY->_getDerivedOrientation().yAxis() + mScalingNodeY->_getDerivedPosition();
				Ogre::Vector2 center = Utils::convertWorldToScreenPosition(mScalingNodeY->_getDerivedPosition(), camera);
				Ogre::Vector2 direction = Utils::convertWorldToScreenPosition(yAxis, camera);
				direction.x += 0.0001f;
				Ogre::Real sign = center.x < direction.x ? -1 : 1;
				return sign;
			}
			break;
		case AXIS_Z:
			{
				Ogre::Vector3 zAxis = mScalingNodeZ->_getDerivedOrientation().yAxis() + mScalingNodeZ->_getDerivedPosition();
				Ogre::Vector2 center = Utils::convertWorldToScreenPosition(mScalingNodeZ->_getDerivedPosition(), camera);
				Ogre::Vector2 direction = Utils::convertWorldToScreenPosition(zAxis, camera);
				Ogre::Real sign = center.x < direction.x ? -1 : 1;
				direction.x += 0.0001f;
				return sign;
			}
			break;
	}

	return 0.0f;
}
