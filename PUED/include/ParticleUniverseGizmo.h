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

#ifndef __PUED_GIZMO_H__
#define __PUED_GIZMO_H__

#include "wx/ogre/prerequisites.h"

/**	Gizmo class
*/
class GizmoManager;
class Gizmo
{
	public:
		enum Axis {AXIS_X, AXIS_MIN_X, AXIS_Y, AXIS_Z, AXIS_MIN_Z, AXIS_OUTER, AXIS_NONE, AXIS_ALL} mAxis;
		enum GizmoType {GIZMO_MOVE, GIZMO_ROTATE, GIZMO_SCALE, GIZMO_NONE};
		static const Ogre::Real SCALE_NODE_ZOOM_FACTOR;
		static const Ogre::Real SCALE_ACTION_FACTOR;
		static const Ogre::Real SCALE_NODE_ORTHOZOOM_FACTOR;

		Gizmo(GizmoManager* gizmoManager);
		virtual ~Gizmo(void){};

		// virtual functions
		virtual const GizmoType getType(void) const = 0;
		virtual void attachToNode(Ogre::SceneNode* node) = 0;
		virtual Ogre::SceneNode* detachFromNode(void) = 0;
		virtual void startSelect(const Ogre::Vector2& screenPosition) = 0;
		virtual bool select(const Ogre::RaySceneQueryResult& queryResult, const Ogre::ColourValue& pixelColour = Ogre::ColourValue::Black) = 0;
		virtual void endSelect(void) = 0;
		virtual bool isVisible(void) const = 0;
		virtual void setVisible(bool visible) = 0;
		virtual Ogre::SceneNode* getParentNode(void) = 0;
		virtual void cameraPreRenderScene (Ogre::Camera *camera) = 0;
		virtual void setWorldspace(bool worldspace);
		virtual void applyWorldLocalSpaceSetting(void){}; // No implementation
		void registerIgnoreWhenSelected(Ogre::Node* node) {mIgnoreList.push_back(node);};
		bool isIgnoredWhenSelected(Ogre::Node* node);
		void calculateScaleFactors(Ogre::Camera *camera, Ogre::Vector3 position);
		Ogre::Real getScaleFactor(void) const;
		Ogre::Real getLogScaleFactor(void) const;

		// Get/set position
		virtual const Ogre::Vector3& getPosition (void) const = 0;
		virtual void setPosition (const Ogre::Vector3& position) = 0;

		// Get/set orientation
		virtual const Ogre::Quaternion& getOrientation (void) const = 0;
		virtual void setOrientation (const Ogre::Quaternion& orientation) = 0;

	protected:
		GizmoManager* mGizmoManager;
		bool mWorldSpace;
		std::vector<Ogre::Node*> mIgnoreList;
		Ogre::Real mScaleFactor;
		Ogre::Real mLogScaleFactor;

};

#endif
