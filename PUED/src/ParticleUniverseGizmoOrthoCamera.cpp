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
#include "ParticleUniverseGizmoOrthoCamera.h"
#include "ParticleUniverseGizmoManager.h"

const Ogre::Real OrthoCameraGizmo::SCALE_NODE_ORTHO_CAMERA = 0.7f;
//-----------------------------------------------------------------------
OrthoCameraGizmo::OrthoCameraGizmo(GizmoManager* gizmoManager, Ogre::SceneManager* sceneManager) : 
	m45DegreesOrtho(true),
	mOrthoCameraNodeBox(0),
	mOrthoCameraNodeX(0),
	mOrthoCameraNodeMinX(0),
	mOrthoCameraNodeY(0),
	mOrthoCameraNodeZ(0),
	mOrthoCameraNodeMinZ(0),
	mAxis(Gizmo::AXIS_NONE)
{
	// Create entities
	mOrthoCameraEntityBox = sceneManager->createEntity("pu_OrthoCameraEntityBox", "pu_box_scale.mesh");
	mOrthoCameraEntityX = sceneManager->createEntity("pu_OrthoCameraEntityX", "pu_cone.mesh");
	mOrthoCameraEntityMinX = sceneManager->createEntity("pu_OrthoCameraEntityMinX", "pu_cone.mesh");
	mOrthoCameraEntityY = sceneManager->createEntity("pu_OrthoCameraEntityY", "pu_cone.mesh");
	mOrthoCameraEntityZ = sceneManager->createEntity("pu_OrthoCameraEntityZ", "pu_cone.mesh");
	mOrthoCameraEntityMinZ = sceneManager->createEntity("pu_OrthoCameraEntityMinZ", "pu_cone.mesh");

	// Set material
	mOrthoCameraEntityBox->setMaterialName("pu_box_ortho");
	mOrthoCameraEntityX->setMaterialName("pu_x_ortho");
	mOrthoCameraEntityMinX->setMaterialName("pu_x_ortho");
	mOrthoCameraEntityY->setMaterialName("pu_y_ortho");
	mOrthoCameraEntityZ->setMaterialName("pu_z_ortho");
	mOrthoCameraEntityMinZ->setMaterialName("pu_z_ortho");

	mOrthoCameraEntityBox->setQueryFlags(GizmoManager::GIZMO_FLAG);
	mOrthoCameraEntityX->setQueryFlags(GizmoManager::GIZMO_FLAG);
	mOrthoCameraEntityMinX->setQueryFlags(GizmoManager::GIZMO_FLAG);
	mOrthoCameraEntityY->setQueryFlags(GizmoManager::GIZMO_FLAG);
	mOrthoCameraEntityZ->setQueryFlags(GizmoManager::GIZMO_FLAG);
	mOrthoCameraEntityMinZ->setQueryFlags(GizmoManager::GIZMO_FLAG);

	mOrthoCameraEntityBox->setRenderQueueGroup(100);
	mOrthoCameraEntityX->setRenderQueueGroup(100);
	mOrthoCameraEntityMinX->setRenderQueueGroup(100);
	mOrthoCameraEntityY->setRenderQueueGroup(100);
	mOrthoCameraEntityZ->setRenderQueueGroup(100);
	mOrthoCameraEntityMinZ->setRenderQueueGroup(100);
}
//-----------------------------------------------------------------------
void OrthoCameraGizmo::cameraPreRenderScene (Ogre::Camera *camera)
{
	// Adjust the ortho camera gizmo
	Ogre::SceneNode* node = mOrthoCameraNodeBox->getParentSceneNode();
	if (node)
	{
		// Reposition according to the actual viewport
		Ogre::Ray ray = camera->getCameraToViewportRay(0.9f, 0.1f);
		Ogre::Vector3 position = ray.getPoint(80.0f);
		//position = camera->getDerivedOrientation().Inverse() * (position - camera->getDerivedPosition()); // Transform from world to local (camera) position
		node->setPosition(position);

		// Adjust the scale
		Ogre::Real scaleFactor = Gizmo::SCALE_NODE_ORTHOZOOM_FACTOR * camera->getOrthoWindowWidth() / camera->getViewport()->getActualWidth();
		setScale(scaleFactor);
	}
}
//-----------------------------------------------------------------------
void OrthoCameraGizmo::attachToNode(Ogre::SceneNode* node)
{
	if (!node)
		return;

	// First remove the old node
	detachFromNode();

	// Recreate the nodes
	mOrthoCameraNodeBox = node->createChildSceneNode();
	mOrthoCameraNodeX = node->createChildSceneNode();
	mOrthoCameraNodeMinX = node->createChildSceneNode();
	mOrthoCameraNodeY = node->createChildSceneNode();
	mOrthoCameraNodeZ = node->createChildSceneNode();
	mOrthoCameraNodeMinZ = node->createChildSceneNode();

	mOrthoCameraNodeBox->setInheritScale(false);
	mOrthoCameraNodeX->setInheritScale(false);
	mOrthoCameraNodeMinX->setInheritScale(false);
	mOrthoCameraNodeY->setInheritScale(false);
	mOrthoCameraNodeZ->setInheritScale(false);
	mOrthoCameraNodeMinZ->setInheritScale(false);

	mOrthoCameraNodeBox->setInheritOrientation(false);
	mOrthoCameraNodeX->setInheritOrientation(false);
	mOrthoCameraNodeMinX->setInheritOrientation(false);
	mOrthoCameraNodeY->setInheritOrientation(false);
	mOrthoCameraNodeZ->setInheritOrientation(false);
	mOrthoCameraNodeMinZ->setInheritOrientation(false);

	mOrthoCameraNodeBox->attachObject(mOrthoCameraEntityBox);
	mOrthoCameraNodeX->attachObject(mOrthoCameraEntityX);
	mOrthoCameraNodeMinX->attachObject(mOrthoCameraEntityMinX);
	mOrthoCameraNodeY->attachObject(mOrthoCameraEntityY);
	mOrthoCameraNodeZ->attachObject(mOrthoCameraEntityZ);
	mOrthoCameraNodeMinZ->attachObject(mOrthoCameraEntityMinZ);

	// Set scale
	setScale(SCALE_NODE_ORTHO_CAMERA);
//	mOrthoCameraNodeBox->setScale(0.5, 0.5, 0.5); // TEST
//	mOrthoCameraNodeX->setScale(0.5, 0.5, 0.5);// TEST
//	mOrthoCameraNodeMinX->setScale(0.5, 0.5, 0.5);// TEST
//	mOrthoCameraNodeY->setScale(0.5, 0.5, 0.5);// TEST
//	mOrthoCameraNodeZ->setScale(0.5, 0.5, 0.5);// TEST
//	mOrthoCameraNodeMinZ->setScale(0.5, 0.5, 0.5);// TEST

	// Set orientation
	Ogre::Real sqrt0dot5 = sqrt(0.5);
	Ogre::Quaternion xQ(sqrt0dot5, 0, 0, sqrt0dot5);
	Ogre::Quaternion minXQ(-sqrt0dot5, 0, 0, sqrt0dot5);
	Ogre::Quaternion yQ(1, 0, 0, 0);
	Ogre::Quaternion zQ(-sqrt0dot5, sqrt0dot5, 0, 0);
	Ogre::Quaternion minZQ(sqrt0dot5, sqrt0dot5, 0, 0);
	mOrthoCameraNodeX->setOrientation(xQ);
	mOrthoCameraNodeMinX->setOrientation(minXQ);
	mOrthoCameraNodeY->setOrientation(yQ);
	mOrthoCameraNodeZ->setOrientation(zQ);
	mOrthoCameraNodeMinZ->setOrientation(minZQ);

	// Set visible
	setVisible(true);
}
//-----------------------------------------------------------------------
void OrthoCameraGizmo::setScale(Ogre::Real scaleFactor)
{
	mOrthoCameraNodeBox->setScale(scaleFactor, scaleFactor, scaleFactor);
	mOrthoCameraNodeX->setScale(scaleFactor, scaleFactor, scaleFactor);
	mOrthoCameraNodeMinX->setScale(scaleFactor, scaleFactor, scaleFactor);
	mOrthoCameraNodeY->setScale(scaleFactor, scaleFactor, scaleFactor);
	mOrthoCameraNodeZ->setScale(scaleFactor, scaleFactor, scaleFactor);
	mOrthoCameraNodeMinZ->setScale(scaleFactor, scaleFactor, scaleFactor);
}
//-----------------------------------------------------------------------
Ogre::SceneNode* OrthoCameraGizmo::detachFromNode(void)
{
	Ogre::SceneNode* parent = 0;

	if (mOrthoCameraNodeBox)
	{
		parent = mOrthoCameraNodeBox->getParentSceneNode();
		parent->removeAndDestroyChild(mOrthoCameraNodeBox->getName());
		mOrthoCameraNodeBox = 0;
	}
	if (mOrthoCameraNodeX)
	{
		parent = mOrthoCameraNodeX->getParentSceneNode();
		parent->removeAndDestroyChild(mOrthoCameraNodeX->getName());
		mOrthoCameraNodeX = 0;
	}
	if (mOrthoCameraNodeMinX)
	{
		parent = mOrthoCameraNodeMinX->getParentSceneNode();
		parent->removeAndDestroyChild(mOrthoCameraNodeMinX->getName());
		mOrthoCameraNodeMinX = 0;
	}
	if (mOrthoCameraNodeY)
	{
		parent = mOrthoCameraNodeY->getParentSceneNode();
		parent->removeAndDestroyChild(mOrthoCameraNodeY->getName());
		mOrthoCameraNodeY = 0;
	}
	if (mOrthoCameraNodeZ)
	{
		parent = mOrthoCameraNodeZ->getParentSceneNode();
		parent->removeAndDestroyChild(mOrthoCameraNodeZ->getName());
		mOrthoCameraNodeZ = 0;
	}
	if (mOrthoCameraNodeMinZ)
	{
		parent = mOrthoCameraNodeMinZ->getParentSceneNode();
		parent->removeAndDestroyChild(mOrthoCameraNodeMinZ->getName());
		mOrthoCameraNodeMinZ = 0;
	}
	
	return parent;
}
//-----------------------------------------------------------------------
bool OrthoCameraGizmo::select(const Ogre::RaySceneQueryResult& queryResult, const Ogre::ColourValue& pixelColour)
{
	// Check whether one of the objects of which this Gizmo exists is is the RaySceneQueryResult
	Ogre::RaySceneQueryResult::const_iterator it;
	Ogre::RaySceneQueryResult::const_iterator itEnd = queryResult.end();
	bool exists = false;
	Ogre::MovableObject* movableObject = 0;
	for (it = queryResult.begin(); it != itEnd; ++it)
	{
		movableObject = (*it).movable;
		if (movableObject == mOrthoCameraEntityBox ||
			movableObject == mOrthoCameraEntityX ||
			movableObject == mOrthoCameraEntityMinX ||
			movableObject == mOrthoCameraEntityY ||
			movableObject == mOrthoCameraEntityZ ||
			movableObject == mOrthoCameraEntityMinZ)
		{
			exists = true;
			break;
		}
	}

	// If none of the objects was it, forget it
	if (!exists)
	{
		return false;
	}

	// Validate the pixelcolour (at the x,y location)
	mAxis = Gizmo::AXIS_NONE;
	if (pixelColour.r > 0.6 && pixelColour.g < 0.05 && pixelColour.b < 0.05 && movableObject == mOrthoCameraEntityX)
	{
		mAxis = Gizmo::AXIS_X;
	}
	else if (pixelColour.r > 0.6 && pixelColour.g < 0.05 && pixelColour.b < 0.05 && movableObject == mOrthoCameraEntityMinX)
	{
		mAxis = Gizmo::AXIS_MIN_X;
	}
	else if (pixelColour.r < 0.05 && pixelColour.g > 0.6 && pixelColour.b < 0.05 && !m45DegreesOrtho)
	{
		mAxis = Gizmo::AXIS_Y;
	}
	else if (pixelColour.r < 0.05 && pixelColour.g < 0.05 && pixelColour.b > 0.6 && movableObject == mOrthoCameraEntityZ)
	{
		mAxis = Gizmo::AXIS_Z;
	}
	else if (pixelColour.r < 0.05 && pixelColour.g < 0.05 && pixelColour.b > 0.6 && movableObject == mOrthoCameraEntityMinZ)
	{
		mAxis = Gizmo::AXIS_MIN_Z;
	}

	return true;
}
//-----------------------------------------------------------------------
Gizmo::Axis OrthoCameraGizmo::getOrthoCameraAxisSelection(void)
{
	return mAxis;
}
//-----------------------------------------------------------------------
bool OrthoCameraGizmo::isVisible(void) const
{
	return mOrthoCameraEntityBox->isVisible();
}
//-----------------------------------------------------------------------
void OrthoCameraGizmo::setVisible(bool visible)
{
	mOrthoCameraEntityBox->setVisible(visible);
	mOrthoCameraEntityX->setVisible(visible);
	mOrthoCameraEntityMinX->setVisible(visible);
	mOrthoCameraEntityY->setVisible(visible);
	mOrthoCameraEntityZ->setVisible(visible);
	mOrthoCameraEntityMinZ->setVisible(visible);
}
//-----------------------------------------------------------------------
void OrthoCameraGizmo::set45DegreesOrtho(bool is45DegreesOrtho)
{
	m45DegreesOrtho = is45DegreesOrtho;
}
