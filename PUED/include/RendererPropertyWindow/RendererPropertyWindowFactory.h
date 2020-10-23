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

#ifndef __PUED_RENDERER_PROPERTY_FACTORY_H__
#define __PUED_RENDERER_PROPERTY_FACTORY_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseRendererPropertyWindow.h"
#include "RendererPropertyWindow/BeamRendererPropertyWindow.h"
#include "RendererPropertyWindow/BillboardRendererPropertyWindow.h"
#include "RendererPropertyWindow/BoxRendererPropertyWindow.h"
#include "RendererPropertyWindow/EntityRendererPropertyWindow.h"
#include "RendererPropertyWindow/LightRendererPropertyWindow.h"
#include "RendererPropertyWindow/RibbonTrailRendererPropertyWindow.h"
#include "RendererPropertyWindow/SphereRendererPropertyWindow.h"

/**	
*/
class RendererPropertyWindowFactory
{
	public:
		/**
			Constructor / Destructor
		*/
		RendererPropertyWindowFactory(void)
		{
			CST_RENDERER_BEAM = ogre2wxTranslate(RENDERER_BEAM);
			CST_RENDERER_BILLBOARD = ogre2wxTranslate(RENDERER_BILLBOARD);
			CST_RENDERER_SPHERE = ogre2wxTranslate(RENDERER_SPHERE);
			CST_RENDERER_BOX = ogre2wxTranslate(RENDERER_BOX);
			CST_RENDERER_ENTITY = ogre2wxTranslate(RENDERER_ENTITY);
			CST_RENDERER_LIGHT = ogre2wxTranslate(RENDERER_LIGHT);
			CST_RENDERER_RIBBONTRAIL = ogre2wxTranslate(RENDERER_RIBBONTRAIL);
		};
		~RendererPropertyWindowFactory(void){};

		/**
			Create a specific Renderer Property Window
		*/
		RendererPropertyWindow* createRendererPropertyWindow(
			wxWindow* parent, 
			EditComponent* owner, 
			const Ogre::String& name,
			ComponentSubType subType)
		{
			if (subType == CST_UNDEFINED)
			{
				return new RendererPropertyWindow(parent, owner);
			}
			else if (subType == CST_RENDERER_BILLBOARD)
			{
				return new BillboardRendererPropertyWindow(parent, owner);
			}
			else if (subType == CST_RENDERER_BEAM)
			{
				return new BeamRendererPropertyWindow(parent, owner);
			}
			else if (subType == CST_RENDERER_BOX)
			{
				return new BoxRendererPropertyWindow(parent, owner);
			}
			else if (subType == CST_RENDERER_ENTITY)
			{
				return new EntityRendererPropertyWindow(parent, owner);
			}
			else if (subType == CST_RENDERER_LIGHT)
			{
				return new LightRendererPropertyWindow(parent, owner);
			}
			else if (subType == CST_RENDERER_RIBBONTRAIL)
			{
				return new RibbonTrailRendererPropertyWindow(parent, owner);
			}
			else if (subType == CST_RENDERER_SPHERE)
			{
				return new SphereRendererPropertyWindow(parent, owner);
			}
			else
			{
				return new RendererPropertyWindow(parent, owner);
			}
			return 0;
		};

		/**
			Create a specific Renderer Property Window and propagate the attributes from the RendererPropertyWindow
		*/
		RendererPropertyWindow* createRendererPropertyWindow(ComponentSubType subType, RendererPropertyWindow* rendererPropertyWindow)
		{
			if (subType == CST_UNDEFINED)
			{
				return new RendererPropertyWindow(rendererPropertyWindow);
			}
			else if (subType == CST_RENDERER_BILLBOARD)
			{
				return new BillboardRendererPropertyWindow(rendererPropertyWindow);
			}
			else if (subType == CST_RENDERER_BEAM)
			{
				return new BeamRendererPropertyWindow(rendererPropertyWindow);
			}
			else if (subType == CST_RENDERER_BOX)
			{
				return new BoxRendererPropertyWindow(rendererPropertyWindow);
			}
			else if (subType == CST_RENDERER_ENTITY)
			{
				return new EntityRendererPropertyWindow(rendererPropertyWindow);
			}
			else if (subType == CST_RENDERER_LIGHT)
			{
				return new LightRendererPropertyWindow(rendererPropertyWindow);
			}
			else if (subType == CST_RENDERER_RIBBONTRAIL)
			{
				return new RibbonTrailRendererPropertyWindow(rendererPropertyWindow);
			}
			else if (subType == CST_RENDERER_SPHERE)
			{
				return new SphereRendererPropertyWindow(rendererPropertyWindow);
			}
			else
			{
				return new RendererPropertyWindow(rendererPropertyWindow);
			}
			return 0;
		};
};

#endif

