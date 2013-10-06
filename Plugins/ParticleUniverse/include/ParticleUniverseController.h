/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2013 Henry van Merode. All rights reserved.

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

#ifndef __PU_CONTROLLER_H__
#define __PU_CONTROLLER_H__

// If the Ogre renderer is replaced by another renderer, the Controller class must be re-implemented
#include "OgreController.h"

namespace ParticleUniverse
{
	/** Using a subclass of public Ogre::Controller does not work, because of some conversion issues between Ogre::Controller and Controller in 
		the ControllerManager (i.e. in the createFrameTimePassthroughController() function).
	*/
	using Ogre::Controller;
//	template <typename T> class Controller : public Ogre::Controller <T>{};
	template <typename T> class ControllerValue : public Ogre::ControllerValue <T>{};
}

#endif
