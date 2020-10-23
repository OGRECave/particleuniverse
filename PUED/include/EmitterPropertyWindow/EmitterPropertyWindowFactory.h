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

#ifndef __PUED_EMITTER_PROPERTY_FACTORY_H__
#define __PUED_EMITTER_PROPERTY_FACTORY_H__

#include "wx/ogre/prerequisites.h"
#include "EmitterPropertyWindow/BoxEmitterPropertyWindow.h"
#include "EmitterPropertyWindow/CircleEmitterPropertyWindow.h"
#include "EmitterPropertyWindow/LineEmitterPropertyWindow.h"
#include "EmitterPropertyWindow/MeshSurfaceEmitterPropertyWindow.h"
#include "EmitterPropertyWindow/PointEmitterPropertyWindow.h"
#include "EmitterPropertyWindow/PositionEmitterPropertyWindow.h"
#include "EmitterPropertyWindow/SlaveEmitterPropertyWindow.h"
#include "EmitterPropertyWindow/SphereSurfaceEmitterPropertyWindow.h"
#include "EmitterPropertyWindow/VertexEmitterPropertyWindow.h"

/**	
*/
class EmitterPropertyWindowFactory
{
	public:
		/**
			Constructor / Destructor
		*/
		EmitterPropertyWindowFactory(void)
		{
			// Emitters
			CST_EMITTER_BOX = ogre2wxTranslate(EMITTER_BOX);
			CST_EMITTER_CIRCLE = ogre2wxTranslate(EMITTER_CIRCLE);
			CST_EMITTER_LINE = ogre2wxTranslate(EMITTER_LINE);
			CST_EMITTER_MESH_SURFACE = ogre2wxTranslate(EMITTER_MESH_SURFACE);
			CST_EMITTER_POINT = ogre2wxTranslate(EMITTER_POINT);
			CST_EMITTER_POSITION = ogre2wxTranslate(EMITTER_POSITION);
			CST_EMITTER_SLAVE = ogre2wxTranslate(EMITTER_SLAVE);
			CST_EMITTER_SPHERE = ogre2wxTranslate(EMITTER_SPHERE);
			CST_EMITTER_VERTEX = ogre2wxTranslate(EMITTER_VERTEX);
		};
		~EmitterPropertyWindowFactory(void){};

		/**
			Create a specific Emitter Property Window
		*/
		EmitterPropertyWindow* createEmitterPropertyWindow(
			wxWindow* parent, 
			EditComponent* owner, 
			const Ogre::String& name,
			ComponentSubType subType)
		{
			if (subType == CST_UNDEFINED)
			{
				return new EmitterPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_EMITTER_BOX)
			{
				return new BoxEmitterPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_EMITTER_CIRCLE)
			{
				return new CircleEmitterPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_EMITTER_LINE)
			{
				return new LineEmitterPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_EMITTER_MESH_SURFACE)
			{
				return new MeshSurfaceEmitterPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_EMITTER_POINT)
			{
				return new PointEmitterPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_EMITTER_POSITION)
			{
				return new PositionEmitterPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_EMITTER_SLAVE)
			{
				return new SlaveEmitterPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_EMITTER_SPHERE)
			{
				return new SphereSurfaceEmitterPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_EMITTER_VERTEX)
			{
				return new VertexEmitterPropertyWindow(parent, owner, name);
			}
			else
			{
				return new EmitterPropertyWindow(parent, owner, name);
			}
			return 0;
		};

		/**
			Create a specific Emitter Property Window and propagate the attributes from the EmitterPropertyWindow
		*/
		EmitterPropertyWindow* createEmitterPropertyWindow(ComponentSubType subType, EmitterPropertyWindow* emitterPropertyWindow)
		{
			if (subType == CST_UNDEFINED)
			{
				return new EmitterPropertyWindow(emitterPropertyWindow);
			}
			else if (subType == CST_EMITTER_BOX)
			{
				return new BoxEmitterPropertyWindow(emitterPropertyWindow);
			}
			else if (subType == CST_EMITTER_CIRCLE)
			{
				return new CircleEmitterPropertyWindow(emitterPropertyWindow);
			}
			else if (subType == CST_EMITTER_LINE)
			{
				return new LineEmitterPropertyWindow(emitterPropertyWindow);
			}
			else if (subType == CST_EMITTER_MESH_SURFACE)
			{
				return new MeshSurfaceEmitterPropertyWindow(emitterPropertyWindow);
			}
			else if (subType == CST_EMITTER_POINT)
			{
				return new PointEmitterPropertyWindow(emitterPropertyWindow);
			}
			else if (subType == CST_EMITTER_POSITION)
			{
				return new PositionEmitterPropertyWindow(emitterPropertyWindow);
			}
			else if (subType == CST_EMITTER_SLAVE)
			{
				return new SlaveEmitterPropertyWindow(emitterPropertyWindow);
			}
			else if (subType == CST_EMITTER_SPHERE)
			{
				return new SphereSurfaceEmitterPropertyWindow(emitterPropertyWindow);
			}
			else if (subType == CST_EMITTER_VERTEX)
			{
				return new VertexEmitterPropertyWindow(emitterPropertyWindow);
			}
			else
			{
				return new EmitterPropertyWindow(emitterPropertyWindow);
			}
			return 0;
		};
};

#endif

