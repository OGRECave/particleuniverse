/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_ORTHO_CAMERA_GIZMO_H__
#define __PUED_ORTHO_CAMERA_GIZMO_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseGizmo.h"

class OrthoCameraGizmo
{
	public:
		OrthoCameraGizmo(GizmoManager* gizmoManager, Ogre::SceneManager* sceneManager);
		virtual ~OrthoCameraGizmo(void){};

		static const Ogre::Real SCALE_NODE_ORTHO_CAMERA;

		// Attach and detach
		virtual void attachToNode(Ogre::SceneNode* node);
		virtual Ogre::SceneNode* detachFromNode(void);

		// Selection
		bool select(const Ogre::RaySceneQueryResult& queryResult, const Ogre::ColourValue& pixelColour = Ogre::ColourValue::Black);
		Gizmo::Axis getOrthoCameraAxisSelection(void);

		// Visible
		void set45DegreesOrtho(bool is45DegreesOrtho);
		virtual void setVisible(bool visible);
		bool isVisible(void) const;
		void setScale(Ogre::Real scaleFactor);

		// Camera events
		virtual void cameraPreRenderScene (Ogre::Camera* camera);

	protected:
		bool m45DegreesOrtho;
		Gizmo::Axis mAxis;
		Ogre::Entity* mOrthoCameraEntityBox;
		Ogre::Entity* mOrthoCameraEntityX;
		Ogre::Entity* mOrthoCameraEntityMinX;
		Ogre::Entity* mOrthoCameraEntityY;
		Ogre::Entity* mOrthoCameraEntityZ;
		Ogre::Entity* mOrthoCameraEntityMinZ;
		Ogre::SceneNode* mOrthoCameraNodeBox;
		Ogre::SceneNode* mOrthoCameraNodeX;
		Ogre::SceneNode* mOrthoCameraNodeMinX;
		Ogre::SceneNode* mOrthoCameraNodeY;
		Ogre::SceneNode* mOrthoCameraNodeZ;
		Ogre::SceneNode* mOrthoCameraNodeMinZ;

};

#endif
