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

#ifndef __PUED_CONFIG_FILE_H__
#define __PUED_CONFIG_FILE_H__

#include "wx/ogre/prerequisites.h"

/**	This is an extension of the ConfigFile class, with additional functions to save a configfile
*/
class ConfigFile : public Ogre::ConfigFile
{
	// No sections are allowed.
	typedef std::map<Ogre::String, Ogre::String> SaveBuffer;

	public:
		// Constructor / Destructor
		ConfigFile(void);
		~ConfigFile(void){};

		/**	Write key/value pairs to an internal buffer.
		@remarks
			Don't use a section.
		*/
		void setSetting(const Ogre::String& key, const Ogre::String& value);

		/**	Write key with multiple values to an internal buffer.
		@remarks
			Don't use a section.
		*/
		void setConcatSetting(const Ogre::String& key, const Ogre::StringVector& value);
		
		/**	Save the internal buffer
		*/
		void save(const Ogre::String& filename);

	protected:
		SaveBuffer mSaveBuffer;
};

#endif
