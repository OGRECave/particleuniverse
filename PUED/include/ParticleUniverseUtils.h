/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_CUTILS_H__
#define __PUED_CUTILS_H__

#include "wx/ogre/prerequisites.h"

/**	Utils class
*/
class Utils
{
	public:
		static wxString CURRENT_DIR_ATTR;
		static wxString SCRIPT_DIR_ATTR;

		// Constructor / Destructor
		Utils(void){};
		~Utils(void){};

		/**	Return the current working directory
		*/
		static const wxString& getCurrentDirectory(void);

		/**	Return the directory where the html files are
		*/
		static const wxString& getManualScriptDirectory(void);

		/**	Calculate the world position from a local position
			worldOrientation = world orientation of the vector
			localPosition = the local position (in relation to its parent)
			worldPosition = world position of the parent
			worldScale = world scale of the parent (?)
		*/
		static Ogre::Vector3 convertLocalToWorldPosition (const Ogre::Quaternion& worldOrientation,
			const Ogre::Vector3& localPosition, 
			const Ogre::Vector3& worldPosition,
			const Ogre::Vector3& worldScale = Ogre::Vector3::UNIT_SCALE);

		/**	Calculate the screen position from a world position
			worldPosition = world position of the vector
			camera = camera associated with the screen
		*/
		static Ogre::Vector2 convertWorldToScreenPosition(const Ogre::Vector3& worldPosition, Ogre::Camera* camera);
};

#define CURRENT_DIR Utils::getCurrentDirectory()
#define SCRIPT_DIR Utils::getManualScriptDirectory()

#endif
