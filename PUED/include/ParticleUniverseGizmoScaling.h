/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_GIZMO_SCALING_H__
#define __PUED_GIZMO_SCALING_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseGizmo.h"

class GizmoScaling : public Gizmo
{
	public:
		GizmoScaling(GizmoManager* gizmoManager, Ogre::SceneManager* sceneManager);
		virtual ~GizmoScaling(void);

		static const Ogre::Real SCALE_NODE_SCALING;

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

		// Scale: This is the scaling of the object to which the gizmo is attached; not the scaling og the Gizmo itself
		void scale (Ogre::Vector2 screenPosition, Ogre::Camera* camera);

		// Scale: Scale the parentnode
		const Ogre::Vector3& getScale(void) const;
		void setScale (const Ogre::Vector3& scale);

		// Visible
		virtual bool isVisible(void) const;
		virtual void setVisible(bool visible);

		// Miscellanious
		virtual const GizmoType getType(void) const;

		// Camera events
		virtual void cameraPreRenderScene (Ogre::Camera* camera);

	protected:
		Ogre::Vector2 mLastPosition;
		Ogre::Entity* mScalingEntityBox;
		Ogre::Entity* mScalingEntityX;
		Ogre::Entity* mScalingEntityY;
		Ogre::Entity* mScalingEntityZ;
		Ogre::SceneNode* mScalingNodeBox;
		Ogre::SceneNode* mScalingNodeX;
		Ogre::SceneNode* mScalingNodeY;
		Ogre::SceneNode* mScalingNodeZ;
		Ogre::Real getAxisDirectionOnScreen(Gizmo::Axis axis, Ogre::Camera* camera);

};

#endif
