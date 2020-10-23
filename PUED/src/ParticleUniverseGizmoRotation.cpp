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
#include "ParticleUniverseGizmoRotation.h"
#include "ParticleUniverseGizmoManager.h"

const Ogre::Real GizmoRotation::SCALE_NODE_ROTATION_OUTER = 28.0f;
const Ogre::Real GizmoRotation::SCALE_NODE_ROTATION = 25.0f;
//-----------------------------------------------------------------------
GizmoRotation::GizmoRotation(GizmoManager* gizmoManager, Ogre::SceneManager* sceneManager) : 
	Gizmo(gizmoManager),
	mRotationNodeOuter(0),
	mRotationNodeX(0),
	mRotationNodeY(0),
	mRotationNodeZ(0)
{
	mLastPosition = Ogre::Vector2(0.5, 0.5);
	mRotationEntityOuter = sceneManager->createEntity("pu_rotationEntityOuter", "pu_rotation_outer.mesh");
	mRotationEntityX = sceneManager->createEntity("pu_rotationEntityX", "pu_rotation.mesh");
	mRotationEntityY = sceneManager->createEntity("pu_rotationEntityY", "pu_rotation.mesh");
	mRotationEntityZ = sceneManager->createEntity("pu_rotationEntityZ", "pu_rotation.mesh");
	mRotationEntityOuter->setMaterialName("pu_white"); // White
	mRotationEntityX->setMaterialName("pu_rotation_x"); // Red
	mRotationEntityY->setMaterialName("pu_rotation_y"); // Green
	mRotationEntityZ->setMaterialName("pu_rotation_z"); // Blue
	mRotationEntityOuter->setQueryFlags(GizmoManager::GIZMO_FLAG);
	mRotationEntityX->setQueryFlags(GizmoManager::GIZMO_FLAG);
	mRotationEntityY->setQueryFlags(GizmoManager::GIZMO_FLAG);
	mRotationEntityZ->setQueryFlags(GizmoManager::GIZMO_FLAG);
	mRotationEntityOuter->setRenderQueueGroup(100);
	mRotationEntityX->setRenderQueueGroup(100);
	mRotationEntityY->setRenderQueueGroup(100);
	mRotationEntityZ->setRenderQueueGroup(100);

	Ogre::Real sqrt0dot5 = sqrt(0.5);
	xQ = Ogre::Quaternion(sqrt0dot5, 0, 0, sqrt0dot5);
	yQ = Ogre::Quaternion(1, 0, 0, 0);
	zQ = Ogre::Quaternion(sqrt0dot5, sqrt0dot5, 0, 0);
	outerQ = Ogre::Quaternion(sqrt0dot5, sqrt0dot5, 0, 0);
}
//-----------------------------------------------------------------------
GizmoRotation::~GizmoRotation(void)
{
	detachFromNode();
}
//-----------------------------------------------------------------------
void GizmoRotation::attachToNode(Ogre::SceneNode* node)
{
	if (!node)
		return;

	if (isIgnoredWhenSelected(node))
		return;

	// First remove all nodes from the old node	
	detachFromNode();

	mRotationNodeOuter = node->createChildSceneNode();
	mRotationNodeX = node->createChildSceneNode();
	mRotationNodeY = node->createChildSceneNode();
	mRotationNodeZ = node->createChildSceneNode();

	mRotationNodeOuter->setInheritScale(false);
	mRotationNodeX->setInheritScale(false);
	mRotationNodeY->setInheritScale(false);
	mRotationNodeZ->setInheritScale(false);

	mRotationNodeOuter->setInheritOrientation(false);
	mRotationNodeX->setInheritOrientation(false);
	mRotationNodeY->setInheritOrientation(false);
	mRotationNodeZ->setInheritOrientation(false);

	mRotationNodeOuter->attachObject(mRotationEntityOuter);
	mRotationNodeX->attachObject(mRotationEntityX);
	mRotationNodeY->attachObject(mRotationEntityY);
	mRotationNodeZ->attachObject(mRotationEntityZ);

	// Set scale
	mRotationNodeOuter->setScale(SCALE_NODE_ROTATION_OUTER, SCALE_NODE_ROTATION_OUTER, SCALE_NODE_ROTATION_OUTER);
	mRotationNodeX->setScale(SCALE_NODE_ROTATION, SCALE_NODE_ROTATION, SCALE_NODE_ROTATION);
	mRotationNodeY->setScale(SCALE_NODE_ROTATION, SCALE_NODE_ROTATION, SCALE_NODE_ROTATION);
	mRotationNodeZ->setScale(SCALE_NODE_ROTATION, SCALE_NODE_ROTATION, SCALE_NODE_ROTATION);

	// Take over the orientation of the node
	setOrientation(node->getOrientation());

	// Set material
	mRotationEntityOuter->setMaterialName("pu_white");
	mRotationEntityX->setMaterialName("pu_rotation_x");
	mRotationEntityY->setMaterialName("pu_rotation_y");
	mRotationEntityZ->setMaterialName("pu_rotation_z");
}
//-----------------------------------------------------------------------
Ogre::SceneNode* GizmoRotation::detachFromNode(void)
{
	Ogre::SceneNode* parent = 0;

	if (mRotationNodeOuter)
	{
		parent = mRotationNodeOuter->getParentSceneNode();
		parent->removeAndDestroyChild(mRotationNodeOuter->getName());
		mRotationNodeOuter = 0;
	}
	if (mRotationNodeX)
	{
		parent = mRotationNodeX->getParentSceneNode();
		parent->removeAndDestroyChild(mRotationNodeX->getName());
		mRotationNodeX = 0;
	}
	if (mRotationNodeY)
	{
		parent = mRotationNodeY->getParentSceneNode();
		parent->removeAndDestroyChild(mRotationNodeY->getName());
		mRotationNodeY = 0;
	}
	if (mRotationNodeZ)
	{
		parent = mRotationNodeZ->getParentSceneNode();
		parent->removeAndDestroyChild(mRotationNodeZ->getName());
		mRotationNodeZ = 0;
	}

	return parent;
}
//-----------------------------------------------------------------------
void GizmoRotation::startSelect(const Ogre::Vector2& screenPosition)
{
	mLastPosition = screenPosition;
}
//-----------------------------------------------------------------------
bool GizmoRotation::select(const Ogre::RaySceneQueryResult& queryResult, const Ogre::ColourValue& pixelColour)
{
	// Check whether one of the objects of which this Gizmo exists is is the RaySceneQueryResult
	Ogre::RaySceneQueryResult::const_iterator it;
	Ogre::RaySceneQueryResult::const_iterator itEnd = queryResult.end();
	bool exists = false;
	Ogre::MovableObject* movableObject = 0;
	for (it = queryResult.begin(); it != itEnd; ++it)
	{
		movableObject = (*it).movable;
		if (movableObject == mRotationEntityOuter || 
			movableObject == mRotationEntityX ||
			movableObject == mRotationEntityY ||
			movableObject == mRotationEntityZ)
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
		mRotationEntityOuter->setMaterialName("pu_gizmo_select");
		mRotationEntityX->setMaterialName("pu_rotation_x");
		mRotationEntityY->setMaterialName("pu_rotation_y");
		mRotationEntityZ->setMaterialName("pu_rotation_z");
		mAxis = AXIS_OUTER;
	}
	else if (pixelColour.r > 0.6 && pixelColour.g < 0.05 && pixelColour.b < 0.05)
	{
		mRotationEntityOuter->setMaterialName("pu_white");
		mRotationEntityX->setMaterialName("pu_rotation_select");
		mRotationEntityY->setMaterialName("pu_rotation_y");
		mRotationEntityZ->setMaterialName("pu_rotation_z");
		mAxis = AXIS_X;
	}
	else if (pixelColour.r < 0.05 && pixelColour.g > 0.6 && pixelColour.b < 0.05)
	{
		mRotationEntityOuter->setMaterialName("pu_white");
		mRotationEntityX->setMaterialName("pu_rotation_x");
		mRotationEntityY->setMaterialName("pu_rotation_select");
		mRotationEntityZ->setMaterialName("pu_rotation_z");
		mAxis = AXIS_Y;
	}
	else if (pixelColour.r < 0.05 && pixelColour.g < 0.05 && pixelColour.b > 0.6)
	{
		mRotationEntityOuter->setMaterialName("pu_white");
		mRotationEntityX->setMaterialName("pu_rotation_x");
		mRotationEntityY->setMaterialName("pu_rotation_y");
		mRotationEntityZ->setMaterialName("pu_rotation_select");
		mAxis = AXIS_Z;
	}

	// Return true, because the gizmo WAS picked, only the user missed it (prevents that the gizmo attached to another node)
	return true;
}
//-----------------------------------------------------------------------
void GizmoRotation::endSelect(void)
{
	// No implementation (yet)
}
//-----------------------------------------------------------------------
const Ogre::Vector3& GizmoRotation::getPosition (void) const
{
	//return mRotationNodeX->getPosition();
	return mRotationNodeX->_getDerivedPosition();
}
//-----------------------------------------------------------------------
void GizmoRotation::setPosition (const Ogre::Vector3& position)
{
	mRotationNodeZ->getParentSceneNode()->setPosition(position);
}
//-----------------------------------------------------------------------
const Ogre::Quaternion& GizmoRotation::getOrientation (void) const
{
	// Return the orientation of the parent node, which should be in sync with the Gizmo
	return mRotationNodeX->getParentSceneNode()->getOrientation();
}
//-----------------------------------------------------------------------
void GizmoRotation::setOrientation (const Ogre::Quaternion& orientation)
{
	// Set torus orientations + orientation of its parent node
	mRotationNodeZ->getParentSceneNode()->setOrientation(orientation);
	applyWorldLocalSpaceSetting();
}
//-----------------------------------------------------------------------
void GizmoRotation::resetOrientation (void)
{
	setOrientation(Ogre::Quaternion::IDENTITY);
}
//-----------------------------------------------------------------------
void GizmoRotation::rotate (Ogre::Vector2 screenPosition, Ogre::Camera* camera)
{
	Ogre::Real diffX = screenPosition.x - mLastPosition.x;
	Ogre::Real diffY = screenPosition.y - mLastPosition.y;
	Ogre::Real l = diffX + diffY;
	Ogre::Radian angle = Ogre::Radian(Ogre::Degree(l));
	switch(mAxis)
	{
		case AXIS_X:
		{
			Ogre::Quaternion q = mRotationNodeX->getOrientation() * xQ;
			mRotationNodeY->rotate(q.xAxis(), angle, Ogre::Node::TS_WORLD);
			mRotationNodeZ->rotate(q.xAxis(), angle, Ogre::Node::TS_WORLD);
			mRotationNodeZ->getParentSceneNode()->rotate(q.xAxis(), angle, Ogre::Node::TS_WORLD);
		}
		break;

		case AXIS_Y:
		{
			Ogre::Quaternion q = mRotationNodeY->getOrientation() * yQ;
			mRotationNodeX->rotate(q.yAxis(), angle, Ogre::Node::TS_WORLD);
			mRotationNodeZ->rotate(q.yAxis(), angle, Ogre::Node::TS_WORLD);
			mRotationNodeZ->getParentSceneNode()->rotate(q.yAxis(), angle, Ogre::Node::TS_WORLD);
		}
		break;

		case AXIS_Z:
		{
			Ogre::Quaternion q = mRotationNodeZ->getOrientation() * zQ;
			mRotationNodeX->rotate(q.zAxis(), angle, Ogre::Node::TS_WORLD);
			mRotationNodeY->rotate(q.zAxis(), angle, Ogre::Node::TS_WORLD);
			mRotationNodeY->getParentSceneNode()->rotate(q.zAxis(), angle, Ogre::Node::TS_WORLD);
		}
		break;

		case AXIS_OUTER:
		{
			Ogre::Vector3 rotationVector = (mRotationNodeOuter->getOrientation() * outerQ).zAxis();
			mRotationNodeX->rotate(rotationVector, angle, Ogre::Node::TS_WORLD);
			mRotationNodeY->rotate(rotationVector, angle, Ogre::Node::TS_WORLD);
			mRotationNodeZ->rotate(rotationVector, angle, Ogre::Node::TS_WORLD);
			mRotationNodeZ->getParentSceneNode()->rotate(rotationVector, angle, Ogre::Node::TS_WORLD);
		}
		break;
	}

	mLastPosition = screenPosition;
}
//-----------------------------------------------------------------------
bool GizmoRotation::isVisible(void) const
{
	return mRotationEntityOuter->isVisible();
}
//-----------------------------------------------------------------------
void GizmoRotation::setVisible(bool visible)
{
	mRotationEntityOuter->setVisible(visible);
	mRotationEntityX->setVisible(visible);
	mRotationEntityY->setVisible(visible);
	mRotationEntityZ->setVisible(visible);
}
//-----------------------------------------------------------------------
Ogre::SceneNode* GizmoRotation::getParentNode(void)
{
	if (mRotationNodeZ)
	{
		return mRotationNodeZ->getParentSceneNode();
	}
	
	return 0;
}
//-----------------------------------------------------------------------
const Gizmo::GizmoType GizmoRotation::getType(void) const
{
	return GIZMO_ROTATE;
}
//-----------------------------------------------------------------------
void GizmoRotation::cameraPreRenderScene (Ogre::Camera *camera)
{
	 // Align the outer ring, so that it always faces the camera
	mRotationNodeOuter->lookAt(camera->getPosition(), Ogre::Node::TS_WORLD);
	mRotationNodeOuter->setOrientation(mRotationNodeOuter->getOrientation() * outerQ);

	// Scale the gizmo
	if (mRotationNodeOuter && mRotationNodeX && mRotationNodeY && mRotationNodeZ)
	{
		//calculateScaleFactors(camera, mRotationNodeZ->getParentSceneNode()->getPosition());
		calculateScaleFactors(camera, mRotationNodeZ->getParentSceneNode()->_getDerivedPosition());
		Ogre::Real outerScale = 64 * mScaleFactor;
		Ogre::Real axisScale = 60 * mScaleFactor;
		mRotationNodeOuter->setScale(outerScale, outerScale, outerScale);
		mRotationNodeX->setScale(axisScale, axisScale, axisScale);
		mRotationNodeY->setScale(axisScale, axisScale, axisScale);
		mRotationNodeZ->setScale(axisScale, axisScale, axisScale);
	}
}
//-----------------------------------------------------------------------
void GizmoRotation::setWorldspace(bool worldspace)
{
	Gizmo::setWorldspace(worldspace);

	if (!mRotationNodeOuter || !mRotationNodeX || !mRotationNodeX || !mRotationNodeZ)
		return;

	if (mWorldSpace)
	{
		mRotationNodeOuter->setOrientation(outerQ);
		mRotationNodeX->setOrientation(xQ);
		mRotationNodeY->setOrientation(yQ);
		mRotationNodeZ->setOrientation(zQ);
	}
	else
	{
		Ogre::Node* node = mRotationNodeOuter->getParent();
		if (node)
		{
			mRotationNodeOuter->setOrientation(node->getOrientation() * outerQ);
			mRotationNodeX->setOrientation(node->getOrientation() * xQ);
			mRotationNodeY->setOrientation(node->getOrientation() * yQ);
			mRotationNodeZ->setOrientation(node->getOrientation() * zQ);
		}
	}
}
//-----------------------------------------------------------------------
void GizmoRotation::applyWorldLocalSpaceSetting(void)
{
	setWorldspace(mWorldSpace);
}
