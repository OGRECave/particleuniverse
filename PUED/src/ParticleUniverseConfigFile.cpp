/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseConfigFile.h"
#include <fstream>

//-----------------------------------------------------------------------
ConfigFile::ConfigFile(void) : 
	Ogre::ConfigFile()
{
}
//-----------------------------------------------------------------------
void ConfigFile::setSetting(const Ogre::String& key, const Ogre::String& value)
{
	mSaveBuffer[key]=value;
}
//-----------------------------------------------------------------------
void ConfigFile::setConcatSetting(const Ogre::String& key, const Ogre::StringVector& value)
{
	Ogre::String concat;
	Ogre::StringVector::const_iterator it = value.begin();
	Ogre::StringVector::const_iterator itEnd = value.end();
	if (!value.empty())
	{
		concat = *it;
		it++;
		while (it != itEnd)
		{
			concat = concat + "; " + *it;
			it++;
		}
		mSaveBuffer[key]=concat;
	}
}
//-----------------------------------------------------------------------
void ConfigFile::save(const Ogre::String& filename)
{
	// Run through the mSaveBuffer
	SaveBuffer::iterator it;
	SaveBuffer::iterator itEnd = mSaveBuffer.end();
	std::ofstream file;
	Ogre::String f = filename;
	file.open(f.c_str());
	for (it = mSaveBuffer.begin(); it != itEnd; ++it)
	{
		file << it->first << "=" << it->second << "\n";
	}
	file.close();
}
