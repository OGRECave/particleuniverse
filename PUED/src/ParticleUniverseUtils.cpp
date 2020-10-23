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
#include "ParticleUniverseUtils.h"

wxString Utils::CURRENT_DIR_ATTR = wxT("");
wxString Utils::SCRIPT_DIR_ATTR = wxT("/manual/script/");
//-----------------------------------------------------------------------
const wxString& Utils::getCurrentDirectory(void)
{
	if (CURRENT_DIR_ATTR.empty())
	{
	    //size_t size = 1024;
		//char* path = new char[size];
	    //path = getcwd(path, size);
		//CURRENT_DIR_ATTR = wxString(path);
		//delete [] path;

		size_t size = 2048;
		wchar_t* path = new wchar_t[size];
	    path = _wgetcwd(path, size);
		CURRENT_DIR_ATTR = wxString(path);
		delete [] path;
	}
	return CURRENT_DIR_ATTR;
}
//-----------------------------------------------------------------------
const wxString& Utils::getManualScriptDirectory(void)
{
	return SCRIPT_DIR_ATTR;
}
//-----------------------------------------------------------------------
Ogre::Vector3 Utils::convertLocalToWorldPosition (const Ogre::Quaternion& worldOrientation, 
												  const Ogre::Vector3& localPosition, 
												  const Ogre::Vector3& worldPosition,
												  const Ogre::Vector3& worldScale)
{
	return (worldOrientation * localPosition * worldScale) + worldPosition;
}
//-----------------------------------------------------------------------
Ogre::Vector2 Utils::convertWorldToScreenPosition(const Ogre::Vector3& worldPosition, Ogre::Camera* camera)
{
    Ogre::Vector3 screenPosition = camera->getProjectionMatrix() * camera->getViewMatrix() * worldPosition;
    return Ogre::Vector2(0.5f + 0.5f * screenPosition.x, 0.5f - 0.5f * screenPosition.y);
}
