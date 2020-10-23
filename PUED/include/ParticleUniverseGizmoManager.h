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
*/

#ifndef __PU_GIZMO_MANAGER_H__
#define __PU_GIZMO_MANAGER_H__

#include "wx/ogre/prerequisites.h"
#include "OgreSingleton.h"
#include "ParticleUniverseGizmoOrthoCamera.h"
#include "ParticleUniverseGizmoMovement.h"
#include "ParticleUniverseGizmoRotation.h"
#include "ParticleUniverseGizmoScaling.h"

/** GizmoManager */
class GizmoManager : public Ogre::Camera::Listener
{
	public:
		// Used for making entities invisible when the gizmo is attached
		typedef std::multimap<Gizmo::GizmoType, Ogre::MovableObject*> HiddenMovableObjectWhenAttached;
		typedef std::vector<Ogre::Node*> RegisteredNodes;
		typedef std::multimap<Ogre::Node*, Ogre::Vector3> RegisteredNodesForUpdatingScale;
		typedef std::vector<Ogre::Node*> RegisteredNodesForFacingCamera;
		typedef std::vector<Ogre::Node*> RegisteredNodesMakingVisibleAfterDetach;

		static const Ogre::uint32 GIZMO_FLAG; // Used for ray scene query
		static const Ogre::uint32 OBJECT_FLAG; // Used for ray scene query

		GizmoManager(Ogre::SceneManager* sceneManager);
		~GizmoManager(void);
		
		/** Initialize the camera needed to get detail query results; object picking with only a rayscenequery is not enough
		*/
		void initialize(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* renderWindow, Ogre::Viewport* mainViewport);
		void registerIgnoreGizmoWhenSelected(Ogre::Node* node, Gizmo::GizmoType gizmoType); // Even is an object is selected, the gizmp is not attached to the node

		/** Set/get the active gizmo
		*/
		Gizmo* getActiveGizmo(void);

		// Attach and detach

		// Gizmo's can only be attached to registered nodes 
		void registerNode(Ogre::Node* node);

		// Zooming in/out also scales the registered node(s) accordingly
		void registerNodeForUpdatingScale(Ogre::Node* node, Ogre::Vector3 additionalScale = Ogre::Vector3(1.0f, 1.0f, 1.0f));
		void unregisterNodeForUpdatingScale(Ogre::Node* node);

		// Moving the camera makes the registerd node always facing the camera
		void registerNodeForFacingCamera(Ogre::Node* node);

		// Makes all attached objects of a registered node visible if the Gizmo is detached
		void registerNodeForMakingVisibleAfterDetach(Ogre::Node* node);

		// Attach and detach to/from a node
		bool attachToNode(Ogre::SceneNode* node);
		Ogre::SceneNode* detachFromNode(void);

		// Validate if a node has visible objects attached
		bool hasVisibleObjects(Ogre::SceneNode* node);

		// Validate if the gizmo is attached to a given node / get the node to which the Gizmo is attached to
		bool isAttachedToNode(Ogre::Node* node);
		Ogre::SceneNode* getAttachedNode(void);

		// Selection
		void startSelect(Ogre::SceneManager* sceneManager, Ogre::Camera* camera, Ogre::Vector2 screenPosition);
		bool isOrthoCameraGizmoSelected(void);
		bool isGizmoSelected(void);
		bool isMovableObjectSelected(Ogre::MovableObject* movableObject);
		void endSelect(Ogre::SceneManager* sceneManager);

		// Get/set position, orientation and scale
		const Ogre::Vector3& getPosition (void) const;
		void setPosition (const Ogre::Vector3& position);
		const Ogre::Quaternion& getOrientation (void) const;
		void setOrientation (const Ogre::Quaternion& orientation);
		void resetOrientation (void);
		const Ogre::Vector3& getScale(void) const;
		void setScale (const Ogre::Vector3& scale);

		// Get/set the type of Gizmo
		const Gizmo::GizmoType getGizmoType(void) const;
		void setGizmoType(const Gizmo::GizmoType& gizmoType);

		// Perform action: Rotate, move, scale (todo)
		void action(Ogre::Vector2 position, Ogre::Camera* camera);

		// Visible
		void set45DegreesOrtho(bool is45DegreesOrtho);
		bool isVisible(void) const;
		void setVisible(bool visible);

		// Rayscene query flags
		void setObjectFlag(Ogre::uint32 flags) {mObjectFlags = flags;}
		void addObjectFlag(Ogre::uint32 flags) {mObjectFlags |= flags;}
		void removeObjectFlag(Ogre::uint32 flags) {mObjectFlags &= ~flags;}
		Ogre::uint32 getObjectFlags(void) const {return mObjectFlags;}
		bool hasObjectFlag(Ogre::uint32 flags) const {return mObjectFlags & flags;}

		// When set, the gizmo of that type disables the movable object when attached and enables it again when detachted
		void registerHideWhenAttached(const Gizmo::GizmoType& gizmoType, Ogre::MovableObject* movableObject);
		void setRegisteredHideWhenAttachedVisible(Ogre::Node* node, bool visible);
		void setAllWhenAttachedVisible(Ogre::Node* node, bool visible); // Enable/disable all movable objects. Even ones that are registered as hidden when attached

		// Register and unregister camera listeners
		void initialiseCameraListeners(Ogre::Camera* camera);
		void unInitialiseCameraListeners(Ogre::Camera* camera);
		virtual void cameraPreRenderScene (Ogre::Camera* camera);

		// Determine whether the gizmo acts on local or worldspace
		void setWorldspace(bool worldspace);
		void applyWorldLocalSpaceSetting(void); // Apply the worldspace orientation on the current setting of mWorldspace

		// Orthograpic camera gizmo
		void attachOrthoCameraGizmo(Ogre::SceneNode* node);
		Ogre::SceneNode* detachOrthoCameraGizmo(void);
		void setVisibleOrthoCameraGizmo(bool visible);
		Gizmo::Axis getOrthoCameraAxisSelection(void);

	protected:
		// Gizmo's
		Gizmo* mGizmo;
		RegisteredNodes mRegisteredNodes;
		RegisteredNodesForUpdatingScale mRegisteredNodesForUpdatingScale;
		RegisteredNodesForFacingCamera mRegisteredNodesForFacingCamera;
		RegisteredNodesMakingVisibleAfterDetach mRegisteredNodesMakingVisibleAfterDetach;
		std::map<Ogre::String, Gizmo*> mGizmoMap;

		// Camera for picking gizmo's
		Ogre::Camera* mOnePixelCamera;

		// Used for selection
		Ogre::Ray mRay;
		Ogre::RaySceneQuery* mRaySceneQuery;
		Ogre::RaySceneQueryResult mQueryResult;
		Ogre::uint32 mObjectFlags;

		// Ortho Camera
		OrthoCameraGizmo* mOrthoCameraGizmo;

		// Used for registering movable objects that are hidden or made visible, depending whether the gizmo is attached or not
		HiddenMovableObjectWhenAttached mHiddenMovableObjectWhenAttached;

		/** Get the colour of the first object the pixel camera sees.
		*/
		const Ogre::ColourValue getColourFromRay(const Ogre::Vector3& cameraPosition, const Ogre::Vector3& cameraDirection);

};

#endif

