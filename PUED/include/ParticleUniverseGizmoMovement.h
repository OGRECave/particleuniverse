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

#ifndef __PUED_GIZMO_MOVEMENT_H__
#define __PUED_GIZMO_MOVEMENT_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseGizmo.h"

class GizmoMovement : public Gizmo
{
	public:
		GizmoMovement(GizmoManager* gizmoManager, Ogre::SceneManager* sceneManager);
		virtual ~GizmoMovement(void);

		static const Ogre::Real SCALE_NODE_MOVEMENT;

		// Attach and detach
		virtual void attachToNode(Ogre::SceneNode* node);
		virtual Ogre::SceneNode* detachFromNode(void);
		virtual Ogre::SceneNode* getParentNode(void);

		// Selection
		virtual void startSelect(const Ogre::Vector2& screenPosition);
		virtual bool select(const Ogre::RaySceneQueryResult& queryResult, const Ogre::ColourValue& pixelColour = Ogre::ColourValue::Black);
		virtual void endSelect(void);

		// Get/set position
		virtual const Ogre::Vector3& getPosition (void) const;
		virtual void setPosition (const Ogre::Vector3& position);

		// Get/set orientation
		virtual const Ogre::Quaternion& getOrientation (void) const;
		virtual void setOrientation (const Ogre::Quaternion& orientation);
		virtual void setWorldspace(bool worldspace);
		virtual void applyWorldLocalSpaceSetting(void);

		// Move
		void move (Ogre::Vector2 screenPosition, Ogre::Camera* camera);

		// Visible
		virtual bool isVisible(void) const;
		virtual void setVisible(bool visible);

		// Miscellanious
		virtual const GizmoType getType(void) const;

		// Camera events
		virtual void cameraPreRenderScene (Ogre::Camera* camera);

	protected:
		void getAxisDirectionOnScreen(Gizmo::Axis axis, Ogre::Camera* camera, Ogre::Vector2& sign);
		Ogre::Vector2 mLastPosition;
		Ogre::Entity* mMovementEntityX;
		Ogre::Entity* mMovementEntityY;
		Ogre::Entity* mMovementEntityZ;
		Ogre::SceneNode* mMovementNodeX;
		Ogre::SceneNode* mMovementNodeY;
		Ogre::SceneNode* mMovementNodeZ;

};

#endif
