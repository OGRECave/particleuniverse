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
#include "ParticleUniverseGizmoManager.h"

const Ogre::uint32 GizmoManager::GIZMO_FLAG = 1<<1;
const Ogre::uint32 GizmoManager::OBJECT_FLAG = 1<<2;
//-----------------------------------------------------------------------
GizmoManager::GizmoManager (Ogre::SceneManager* sceneManager) : 
	mGizmo(0)
{
	mObjectFlags = 0;
	addObjectFlag(GIZMO_FLAG);
	addObjectFlag(OBJECT_FLAG);
}
//-----------------------------------------------------------------------
GizmoManager::~GizmoManager (void)
{
	mGizmo = mGizmoMap.find("GizmoMovement")->second;
	OGRE_DELETE_T(static_cast<GizmoMovement*>(mGizmo), GizmoMovement, Ogre::MEMCATEGORY_SCENE_OBJECTS);
	mGizmo = mGizmoMap.find("GizmoRotation")->second;
	OGRE_DELETE_T(static_cast<GizmoRotation*>(mGizmo), GizmoRotation, Ogre::MEMCATEGORY_SCENE_OBJECTS);
	mGizmo = mGizmoMap.find("GizmoScaling")->second;
	OGRE_DELETE_T(static_cast<GizmoScaling*>(mGizmo), GizmoScaling, Ogre::MEMCATEGORY_SCENE_OBJECTS);
	mGizmoMap.clear();
	mGizmo = 0;
	OGRE_DELETE_T(static_cast<OrthoCameraGizmo*>(mOrthoCameraGizmo), OrthoCameraGizmo, Ogre::MEMCATEGORY_SCENE_OBJECTS);
	mOrthoCameraGizmo = 0;
}
//-----------------------------------------------------------------------
void GizmoManager::registerIgnoreGizmoWhenSelected(Ogre::Node* node, Gizmo::GizmoType gizmoType)
{
	switch (gizmoType)
	{
		case Gizmo::GIZMO_MOVE:
		{
			Gizmo* gizmo = mGizmoMap.find("GizmoMovement")->second;
			gizmo->registerIgnoreWhenSelected(node);
		}
		break;
		case Gizmo::GIZMO_ROTATE:
		{
			Gizmo* gizmo = mGizmoMap.find("GizmoRotation")->second;
			gizmo->registerIgnoreWhenSelected(node);
		}
		break;
		case Gizmo::GIZMO_SCALE:
		{
			Gizmo* gizmo = mGizmoMap.find("GizmoScaling")->second;
			gizmo->registerIgnoreWhenSelected(node);
		}
		break;
	}
}
//-----------------------------------------------------------------------
void GizmoManager::startSelect(Ogre::SceneManager* sceneManager, Ogre::Camera* camera, Ogre::Vector2 screenPosition)
{
	// Determine which object in the scene was selected
	Ogre::Vector3 normalizedScreenPosition;
	normalizedScreenPosition.x = (float)screenPosition.x / camera->getViewport()->getActualWidth();
	normalizedScreenPosition.y = (float)screenPosition.y / camera->getViewport()->getActualHeight();
	mRay = camera->getCameraToViewportRay(normalizedScreenPosition.x, normalizedScreenPosition.y);
	mRaySceneQuery = sceneManager->createRayQuery(mRay, Ogre::SceneManager::ENTITY_TYPE_MASK);
	mRaySceneQuery->setQueryMask(mObjectFlags);
	mRaySceneQuery->setRay(mRay);
	mRaySceneQuery->setSortByDistance(true);
	mQueryResult = mRaySceneQuery->execute();

	// Start selecting the active gizmo (move, rotate, scale)
	mGizmo->startSelect(screenPosition);
}
//-----------------------------------------------------------------------
bool GizmoManager::isOrthoCameraGizmoSelected(void)
{
	// Fast rejection; return false if the gizmo is not displayed
	if (!mOrthoCameraGizmo->isVisible())
		return false;

	Ogre::ColourValue pixelColour = getColourFromRay(mRay.getOrigin(), mRay.getDirection());

	// Perform select with ortho camera gizmo
	return mOrthoCameraGizmo->select(mQueryResult, pixelColour);
}
//-----------------------------------------------------------------------
bool GizmoManager::isGizmoSelected(void)
{
	// Fast rejection; return false if the gizmo is not displayed
	if (!mGizmo->isVisible())
		return false;

	Ogre::ColourValue pixelColour = getColourFromRay(mRay.getOrigin(), mRay.getDirection());

	// Perform select with active gizmo (move, rotate, scale)
	return mGizmo->select(mQueryResult, pixelColour);
}
//-----------------------------------------------------------------------
bool GizmoManager::isMovableObjectSelected(Ogre::MovableObject* movableObject)
{
	Ogre::RaySceneQueryResult::iterator it;
	Ogre::RaySceneQueryResult::iterator itEnd = mQueryResult.end();
	for (it = mQueryResult.begin(); it != itEnd; ++it)
	{
		if ((*it).movable == movableObject)
		{
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------
void GizmoManager::endSelect(Ogre::SceneManager* sceneManager)
{
	sceneManager->destroyQuery(mRaySceneQuery);
}
//-----------------------------------------------------------------------
void GizmoManager::initialize(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* renderWindow, Ogre::Viewport* mainViewport)
{
	// Create Gizmo´s.
	mOrthoCameraGizmo = OGRE_NEW_T(OrthoCameraGizmo, Ogre::MEMCATEGORY_SCENE_OBJECTS)(this, sceneManager);
	mGizmo = OGRE_NEW_T(GizmoRotation, Ogre::MEMCATEGORY_SCENE_OBJECTS)(this, sceneManager);
	mGizmoMap["GizmoRotation"] = mGizmo;
	mGizmo = OGRE_NEW_T(GizmoScaling, Ogre::MEMCATEGORY_SCENE_OBJECTS)(this, sceneManager);
	mGizmoMap["GizmoScaling"] = mGizmo;
	mGizmo = OGRE_NEW_T(GizmoMovement, Ogre::MEMCATEGORY_SCENE_OBJECTS)(this, sceneManager);
	mGizmoMap["GizmoMovement"] = mGizmo; // Default Gizmo type is GIZMO_MOVE

	// Create pixel camera
	mOnePixelCamera = sceneManager->createCamera("onePixelCamera");
	mOnePixelCamera->setFarClipDistance(99999999);
	mOnePixelCamera->setNearClipDistance(0.1f);
	mOnePixelCamera->setFOVy(Ogre::Radian(1.0f));
	mOnePixelCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
	mOnePixelCamera->setOrthoWindow(0.01f, 0.01f);
	Ogre::Real width = 1.0f / mainViewport->getActualWidth();
	Ogre::Real height = 1.0f / mainViewport->getActualHeight();

	renderWindow->addViewport(mOnePixelCamera, 1, 0, 0, width, height);
//	renderWindow->addViewport(mOnePixelCamera, 1, 0.0, 0.0, 0.1, 0.1);
	mOnePixelCamera->getViewport()->setOverlaysEnabled(false);
	mOnePixelCamera->getViewport()->setAutoUpdated(true);
	mOnePixelCamera->getViewport()->setClearEveryFrame(true);
}
//-----------------------------------------------------------------------
void GizmoManager::registerNode(Ogre::Node* node)
{
	mRegisteredNodes.push_back(node);
}
//-----------------------------------------------------------------------
void GizmoManager::registerNodeForUpdatingScale(Ogre::Node* node, Ogre::Vector3 additionalScale)
{
	mRegisteredNodesForUpdatingScale.insert(std::make_pair(node, additionalScale));
}
//-----------------------------------------------------------------------
void GizmoManager::unregisterNodeForUpdatingScale(Ogre::Node* node)
{
	RegisteredNodesForUpdatingScale::iterator it;
	RegisteredNodesForUpdatingScale::iterator itEnd = mRegisteredNodesForUpdatingScale.end();
	for (it = mRegisteredNodesForUpdatingScale.begin(); it != itEnd; ++it)
	{
		if (it->first == node)
		{
			mRegisteredNodesForUpdatingScale.erase(it);
			return;
		}
	}
}
//-----------------------------------------------------------------------
void GizmoManager::registerNodeForFacingCamera(Ogre::Node* node)
{
	mRegisteredNodesForFacingCamera.push_back(node);
}
//-----------------------------------------------------------------------
void GizmoManager::registerNodeForMakingVisibleAfterDetach(Ogre::Node* node)
{
	mRegisteredNodesMakingVisibleAfterDetach.push_back(node);
}
//-----------------------------------------------------------------------
bool GizmoManager::attachToNode(Ogre::SceneNode* node)
{
	if (!node)
		return false;

	// If the node is not registered, none of the gizmo's can be attached
	RegisteredNodes::iterator it;
	RegisteredNodes::iterator itEnd = mRegisteredNodes.end();
	bool registered = false;
	for (it = mRegisteredNodes.begin(); it != itEnd; ++it)
	{
		if (*it == node)
		{
			registered = true;
			break;
		}
	}
	if (!registered)
		return false;

	// If the node is on the ignore list for that specific gizmo, it cannot be attached
	if (mGizmo->isIgnoredWhenSelected(node))
		return false;

	
	// If all attached objects of the node or its childnodes are invisible, it cannot be attached
	bool visible = hasVisibleObjects(node);
	if (!visible)
	{
		unsigned short numChildren = node->numChildren();
		for (unsigned short i = 0; i < numChildren; ++i)
		{
			Ogre::SceneNode* child = static_cast<Ogre::SceneNode*>(node->getChild(i));
			if (hasVisibleObjects(child))
			{
				visible = true;
				break;
			}
		}
	}

	if (!visible)
		return false;

	// Set visibility of the 'old' node
	setAllWhenAttachedVisible(getAttachedNode(), true);

	// Attach the active gizmo (move, rotate, scale) to the new node
	mGizmo->attachToNode(node);
	setVisible(true);
	node->needUpdate(true);
	return true;
}
//-----------------------------------------------------------------------
bool GizmoManager::hasVisibleObjects(Ogre::SceneNode* node)
{
	if (!node)
		return false;

	unsigned short numAttachedObjects = node->numAttachedObjects();
	for (unsigned short j = 0; j < numAttachedObjects; ++j)
	{
		if (node->getAttachedObject(j)->isVisible())
		{
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------
Ogre::SceneNode* GizmoManager::detachFromNode(void)
{
	// Set visibility of the 'old' node
	setAllWhenAttachedVisible(getAttachedNode(), true);

	// Detach the active gizmo (move, rotate, scale)
	Ogre::SceneNode* node = mGizmo->detachFromNode();
	return node;
}
//-----------------------------------------------------------------------
Gizmo* GizmoManager::getActiveGizmo(void)
{
	// Return the active gizmo (move, rotate, scale)
	return mGizmo;
}
//-----------------------------------------------------------------------
const Ogre::ColourValue GizmoManager::getColourFromRay(const Ogre::Vector3 & cameraPosition, const Ogre::Vector3 & cameraDirection)
{
	Ogre::ColourValue colour = Ogre::ColourValue::Black;

	if (!mOnePixelCamera)
		return colour; // return Black colour

	// Set the pixel camera with the position and looking at the direction.
	mOnePixelCamera->setPosition(cameraPosition);
	mOnePixelCamera->setDirection(cameraDirection);

	Ogre::RenderTarget* renderTarget = mOnePixelCamera->getViewport()->getTarget();
	renderTarget->update();
	//mOnePixelCamera->getViewport()->getActualDimensions(left, top, width, height);
	int left = 0, top = 0, width = 2, height = 2; // Do not use the actual dimensions, because when the frame size is decreased, width/height become 0.
	Ogre::PixelFormat pixelFormat = Ogre::PF_BYTE_RGBA;
	Ogre::uchar* viewportData = OGRE_ALLOC_T(Ogre::uchar, width * height * Ogre::PixelUtil::getNumElemBytes(pixelFormat), Ogre::MEMCATEGORY_RENDERSYS);
	Ogre::Box dimensions(left, top, left + width, top + height);
	Ogre::PixelBox pixelBox(dimensions, pixelFormat, viewportData);
	renderTarget->copyContentsToMemory(pixelBox);
	Ogre::PixelUtil::unpackColour(&colour, pixelFormat, pixelBox.data);
	OGRE_FREE(viewportData, Ogre::MEMCATEGORY_RENDERSYS);
	return colour;
}
//-----------------------------------------------------------------------
const Ogre::Quaternion& GizmoManager::getOrientation (void) const
{
	if (mGizmo)
	{
		return mGizmo->getOrientation();
	}
	return Ogre::Quaternion::IDENTITY;
}
//-----------------------------------------------------------------------
void GizmoManager::setOrientation (const Ogre::Quaternion& orientation)
{
	if (mGizmo)
	{
		mGizmo->setOrientation(orientation);
	}
}
//-----------------------------------------------------------------------
void GizmoManager::resetOrientation (void)
{
	if (mGizmo && mGizmo && mGizmo->getType() == Gizmo::GIZMO_ROTATE)
	{
		(static_cast<GizmoRotation*>(mGizmo))->resetOrientation();
	}
}
//-----------------------------------------------------------------------
const Ogre::Vector3& GizmoManager::getPosition (void) const
{
	if (mGizmo)
	{
		return mGizmo->getPosition();
	}
	return Ogre::Vector3::ZERO;
}
//-----------------------------------------------------------------------
void GizmoManager::setPosition (const Ogre::Vector3& position)
{
	if (mGizmo)
	{
		mGizmo->setPosition(position);
	}
}
//-----------------------------------------------------------------------
const Ogre::Vector3& GizmoManager::getScale(void) const
{
	if (mGizmo && mGizmo->getType() == Gizmo::GIZMO_SCALE)
	{
		return (static_cast<GizmoScaling*>(mGizmo))->getScale();
	}
	return Ogre::Vector3::UNIT_SCALE;
}
//-----------------------------------------------------------------------
void GizmoManager::setScale (const Ogre::Vector3& scale)
{
	if (mGizmo && mGizmo->getType() == Gizmo::GIZMO_SCALE)
	{
		(static_cast<GizmoScaling*>(mGizmo))->setScale(scale);
	}
}
//-----------------------------------------------------------------------
void GizmoManager::action(Ogre::Vector2 position, Ogre::Camera* camera)
{
	// Perform action on active gizmo (move, rotate, scale)
	switch (mGizmo->getType())
	{
		case Gizmo::GIZMO_MOVE:
			(static_cast<GizmoMovement*>(mGizmo))->move(position, camera);
		break;
		case Gizmo::GIZMO_ROTATE:
		{
			(static_cast<GizmoRotation*>(mGizmo))->rotate(position, camera);
		}
		break;
		case Gizmo::GIZMO_SCALE:
			(static_cast<GizmoScaling*>(mGizmo))->scale(position, camera);
		break;
	}
}
//-----------------------------------------------------------------------
const Gizmo::GizmoType GizmoManager::getGizmoType(void) const
{
	if (mGizmo)
	{
		return mGizmo->getType();
	}
	return Gizmo::GIZMO_NONE;
}
//-----------------------------------------------------------------------
void GizmoManager::setGizmoType(const Gizmo::GizmoType& gizmoType)
{
	// First check if the new gizmo is allowed for the current node, otherwise ignore the request
	Gizmo* newGizmo = 0;
	switch (gizmoType)
	{
		case Gizmo::GIZMO_MOVE:
		{
			newGizmo = mGizmoMap.find("GizmoMovement")->second;
		}
		break;
		case Gizmo::GIZMO_ROTATE:
		{
			newGizmo = mGizmoMap.find("GizmoRotation")->second;
		}
		break;
		case Gizmo::GIZMO_SCALE:
		{
			newGizmo = mGizmoMap.find("GizmoScaling")->second;
		}
		break;
	}

	// Validate if it is allowed
	if (newGizmo->isIgnoredWhenSelected(getAttachedNode()))
		return; // Cannot do this

	// First remove from the node
	Ogre::SceneNode* node = detachFromNode();

	// Set the new gizmo and attach to the node
	mGizmo = newGizmo;
	attachToNode(node);
}
//-----------------------------------------------------------------------
bool GizmoManager::isVisible(void) const
{
	if (mGizmo)
	{
		return mGizmo->isVisible();
	}
	return false;
}
//-----------------------------------------------------------------------
void GizmoManager::setVisible(bool visible)
{
	if (mGizmo)
	{
		mGizmo->setVisible(visible);
		setRegisteredHideWhenAttachedVisible(getAttachedNode(), !visible);
	}
}
//-----------------------------------------------------------------------
void GizmoManager::registerHideWhenAttached(const Gizmo::GizmoType& gizmoType, Ogre::MovableObject* movableObject)
{
	mHiddenMovableObjectWhenAttached.insert(std::make_pair(gizmoType, movableObject));
}
//-----------------------------------------------------------------------
bool GizmoManager::isAttachedToNode(Ogre::Node* node)
{
	if (mGizmo)
	{
		return (node == mGizmo->getParentNode());
	}
	return false;
}
//-----------------------------------------------------------------------
Ogre::SceneNode* GizmoManager::getAttachedNode(void)
{
	if (mGizmo)
	{
		return mGizmo->getParentNode();
	}
	return 0;
}
//-----------------------------------------------------------------------
void GizmoManager::setRegisteredHideWhenAttachedVisible(Ogre::Node* node, bool visible)
{
	if (!node)
		return;

	HiddenMovableObjectWhenAttached::iterator it;
	HiddenMovableObjectWhenAttached::iterator itEnd;
	it = mHiddenMovableObjectWhenAttached.find(mGizmo->getType());
	if (it == mHiddenMovableObjectWhenAttached.end())
	{
		return;
	}

	// Run through all nodes that are registered to this type
	itEnd = mHiddenMovableObjectWhenAttached.upper_bound(mGizmo->getType());
	for ( ; it != itEnd; ++it)
	{
		Ogre::MovableObject* object = it->second;

		// Determine whether the object is attached to the node: Run through all childnodes to determine the movable object
		unsigned short numChildren = node->numChildren();
		for (unsigned short i = 0; i < numChildren; ++i)
		{
			Ogre::SceneNode* child = static_cast<Ogre::SceneNode*>(node->getChild(i)); // Assume they all are SceneNodes
			unsigned short numAttachedObjects = child->numAttachedObjects();
			for (unsigned short j = 0; j < numAttachedObjects; ++j)
			{
				if (child->getAttachedObject(j) == object)
				{
					object->setVisible(visible);
				}
			}
		}
	}
}
//-----------------------------------------------------------------------
void GizmoManager::setAllWhenAttachedVisible(Ogre::Node* node, bool visible)
{
	if (!node)
		return;

	// Determine the objects attached to the node: Run through all childnodes to determine the movable object
	RegisteredNodesMakingVisibleAfterDetach::iterator it;
	RegisteredNodesMakingVisibleAfterDetach::iterator itEnd = mRegisteredNodesMakingVisibleAfterDetach.end();
	Ogre::SceneNode* child = 0;
	unsigned short numChildren = node->numChildren();
	for (unsigned short i = 0; i < numChildren; ++i)
	{
		// Run through all childnodes
		child = static_cast<Ogre::SceneNode*>(node->getChild(i)); // Assume they all are SceneNodes
		itEnd = mRegisteredNodesMakingVisibleAfterDetach.end();
		for (it = mRegisteredNodesMakingVisibleAfterDetach.begin(); it != itEnd; ++it)
		{
			// Run through all registered nodes
			if (child == *it)
			{
				child->setVisible(visible);
//				unsigned short numAttachedObjects = child->numAttachedObjects();
//				for (unsigned short j = 0; j < numAttachedObjects; ++j)
//				{
//					child->getAttachedObject(j)->setVisible(visible);
//				}
				break;
			}
		}
	}
}
//-----------------------------------------------------------------------
void GizmoManager::initialiseCameraListeners(Ogre::Camera* camera)
{
	camera->addListener(this);
}
//-----------------------------------------------------------------------
void GizmoManager::unInitialiseCameraListeners(Ogre::Camera* camera)
{
	camera->removeListener(this);
}
//-----------------------------------------------------------------------
void GizmoManager::cameraPreRenderScene(Ogre::Camera* camera)
{
	// Adjust the ortho camera gizmo
	if (mOrthoCameraGizmo)
	{
		mOrthoCameraGizmo->cameraPreRenderScene(camera);
	}

	if (!mGizmo)
		return;

	// Adjust the other gizmo
	mGizmo->cameraPreRenderScene(camera);

	// Scale all registered nodes
	RegisteredNodesForUpdatingScale::iterator it;
	RegisteredNodesForUpdatingScale::iterator itEnd = mRegisteredNodesForUpdatingScale.end();
	for (it = mRegisteredNodesForUpdatingScale.begin(); it != itEnd; ++it)
	{
		// Scale the node
		mGizmo->calculateScaleFactors(camera, ((*it).first)->_getDerivedPosition()); // Calculate based on position of the node
		Ogre::Vector3 additionalScaleFactor = (*it).second * mGizmo->getScaleFactor();
		((*it).first)->setScale(additionalScaleFactor);
	}

	// Align all registered nodes to face the camera
	RegisteredNodesForFacingCamera::iterator itAlign;
	RegisteredNodesForFacingCamera::iterator itAlignEnd = mRegisteredNodesForFacingCamera.end();
	for (itAlign = mRegisteredNodesForFacingCamera.begin(); itAlign != itAlignEnd; ++itAlign)
	{
		// Align the node
		Ogre::Vector3 dir = (*itAlign)->_getDerivedPosition() - camera->getPosition();
		dir.normalise();
		Ogre::Vector3 right(dir.z,0,-dir.x);
		right.normalise();
		Ogre::Vector3 up = dir.crossProduct(right);
		(*itAlign)->setOrientation(Ogre::Quaternion(right,up,dir));
	}
}
//-----------------------------------------------------------------------
void GizmoManager::setWorldspace(bool worldspace)
{
	Gizmo* gizmo = mGizmoMap.find("GizmoMovement")->second;
	gizmo->setWorldspace(worldspace);
	gizmo = mGizmoMap.find("GizmoRotation")->second;
	gizmo->setWorldspace(worldspace);
}
//-----------------------------------------------------------------------
void GizmoManager::applyWorldLocalSpaceSetting(void)
{
	Gizmo* gizmo = mGizmoMap.find("GizmoMovement")->second;
	gizmo->applyWorldLocalSpaceSetting();
	gizmo = mGizmoMap.find("GizmoRotation")->second;
	gizmo->applyWorldLocalSpaceSetting();
}
//-----------------------------------------------------------------------
void GizmoManager::attachOrthoCameraGizmo(Ogre::SceneNode* node)
{
	mOrthoCameraGizmo->attachToNode(node);
}
//-----------------------------------------------------------------------
Ogre::SceneNode* GizmoManager::detachOrthoCameraGizmo(void)
{
	return mOrthoCameraGizmo->detachFromNode();
}
//-----------------------------------------------------------------------
void GizmoManager::setVisibleOrthoCameraGizmo(bool visible)
{
	mOrthoCameraGizmo->setVisible(visible);
}
//-----------------------------------------------------------------------
Gizmo::Axis GizmoManager::getOrthoCameraAxisSelection(void)
{
	return mOrthoCameraGizmo->getOrthoCameraAxisSelection();
}
//-----------------------------------------------------------------------
void GizmoManager::set45DegreesOrtho(bool is45DegreesOrtho)
{
	mOrthoCameraGizmo->set45DegreesOrtho(is45DegreesOrtho);
}