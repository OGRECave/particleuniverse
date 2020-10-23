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
#include "ParticleUniverseGizmo.h"
#include "ParticleUniverseGizmoManager.h"

const Ogre::Real Gizmo::SCALE_NODE_ZOOM_FACTOR = 0.000333f;
const Ogre::Real Gizmo::SCALE_ACTION_FACTOR = 0.001f;
const Ogre::Real Gizmo::SCALE_NODE_ORTHOZOOM_FACTOR = 0.384f;
//-----------------------------------------------------------------------
Gizmo::Gizmo(GizmoManager* gizmoManager) : 
	mAxis(AXIS_NONE),
	mScaleFactor(1.0f),
	mLogScaleFactor(1.0f),
	mGizmoManager(gizmoManager),
	mWorldSpace(true)
{
	mIgnoreList.clear();
}
//-----------------------------------------------------------------------
bool Gizmo::isIgnoredWhenSelected(Ogre::Node* node)
{
	// Ignore the node if it is 0
	if (!node)
		return true;

	// Ignore the node that is passed as argument, if it is in the ignorelist
	std::vector<Ogre::Node*>::iterator it;
	std::vector<Ogre::Node*>::iterator itEnd = mIgnoreList.end();
	for (it = mIgnoreList.begin(); it != mIgnoreList.end(); ++it)
	{
		if ((*it) == node)
		{
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------
void Gizmo::calculateScaleFactors(Ogre::Camera *camera, Ogre::Vector3 position)
{
	Ogre::Vector3 v = camera->getPosition() - position;
	Ogre::Real length = v.length();
	mScaleFactor = SCALE_NODE_ZOOM_FACTOR * length; // Used for scaling nodes

	if (camera->getProjectionType() == Ogre::PT_ORTHOGRAPHIC)
	{
		mScaleFactor = SCALE_NODE_ORTHOZOOM_FACTOR * camera->getOrthoWindowWidth() / camera->getViewport()->getActualWidth();
	}
	
	Ogre::Real sq = length / (length + 20);
	mLogScaleFactor = SCALE_ACTION_FACTOR * sq * sq * length; // Used for speed of rotation, translation
}
//-----------------------------------------------------------------------
Ogre::Real Gizmo::getScaleFactor(void) const
{
	return mScaleFactor;
}
//-----------------------------------------------------------------------
Ogre::Real Gizmo::getLogScaleFactor(void) const
{
	return mLogScaleFactor;
}
//-----------------------------------------------------------------------
void Gizmo::setWorldspace(bool worldspace)
{
	mWorldSpace = worldspace;
}