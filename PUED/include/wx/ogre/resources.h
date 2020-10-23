/*
 * Copyright (C) 2007-2008 Martin Pieuchot 
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#ifndef _WX_OGRE_RESOURCES_H_ 
#define _WX_OGRE_RESOURCES_H_ 

#include <Ogre.h>

#include "wx/ogre/utils.h"

/** Wrapper around the Ogre resources system.
 *
 * @author Martin Pieuchot
 */
class wxOgreResources : public Ogre::Singleton<wxOgreResources> 
{
// Is it possible to have longer names? :p
#define DEFAULT_GROUP Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME

public:

    wxOgreResources();

    /** Initialise all resource groups which are yet to be initialised. */
    void InitialiseAllResources();

    /** Add a resource location to for a given resource group. */
    void AddResource(const Ogre::String& path,
                     const Ogre::String& type = "FileSystem",
                     const Ogre::String& group = DEFAULT_GROUP);

    /** Load a resource config file like the one used by Ogre's demos.
     *
     * @param file the comple path to the resource file. 
     */
    bool LoadResourceFile(const Ogre::String& file);

    /** Find all file names matching a given pattern in a group. */
    Ogre::StringVectorPtr GetResourcesList(const Ogre::String& group,
                                           const Ogre::String& pattern);

    /** List all file names in a group. */ 
    Ogre::StringVectorPtr GetResourcesList(const Ogre::String& group);

    /** Get a list of the currently defined resource groups. */
    Ogre::StringVector GetResourcesGroups();

private:

    Ogre::ResourceGroupManager* m_rmgr;

    DECLARE_OGRE_SINGLETON(wxOgreResources)
};

#endif
