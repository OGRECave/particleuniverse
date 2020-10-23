/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_GIZMO_ROTATION_H__
#define __PUED_GIZMO_ROTATION_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseGizmo.h"

class GizmoRotation : public Gizmo
{
	public:
		GizmoRotation(GizmoManager* gizmoManager, Ogre::SceneManager* sceneManager);
		virtual ~GizmoRotation(void);

		static const Ogre::Real SCALE_NODE_ROTATION_OUTER;
		static const Ogre::Real SCALE_NODE_ROTATION;

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
		void resetOrientation (void);
		virtual void setWorldspace(bool worldspace);
		virtual void applyWorldLocalSpaceSetting(void);

		// Rotate
		void rotate (Ogre::Vector2 screenPosition, Ogre::Camera* camera);

		// Visible
		virtual bool isVisible(void) const;
		virtual void setVisible(bool visible);

		// Miscellanious
		virtual const GizmoType getType(void) const;

		// Camera events
		virtual void cameraPreRenderScene (Ogre::Camera* camera);

	protected:
		Ogre::Quaternion xQ;
		Ogre::Quaternion yQ;
		Ogre::Quaternion zQ;
		Ogre::Quaternion outerQ;
		Ogre::Vector2 mLastPosition;
		Ogre::Entity* mRotationEntityOuter;
		Ogre::Entity* mRotationEntityX;
		Ogre::Entity* mRotationEntityY;
		Ogre::Entity* mRotationEntityZ;
		Ogre::SceneNode* mRotationNodeOuter;
		Ogre::SceneNode* mRotationNodeX;
		Ogre::SceneNode* mRotationNodeY;
		Ogre::SceneNode* mRotationNodeZ;

};

#endif
