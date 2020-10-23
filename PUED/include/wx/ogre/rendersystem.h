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

#ifndef _WX_OGRE_RENDERSYSTEM_H_
#define _WX_OGRE_RENDERSYSTEM_H_

#include <Ogre.h>

#include "wx/ogre/utils.h"

/** Simplifies the ogre initialisation process.
 *
 * @author Martin Pieuchot
 */
class wxOgreRenderSystem : public Ogre::Singleton<wxOgreRenderSystem>
{
public:

    /** Creates the Ogre's root. */
    wxOgreRenderSystem();

    /** Creates the Ogre's root.
     *
     * @see Ogre::Root
     */
    wxOgreRenderSystem(const Ogre::String& plugins, const Ogre::String& config,
                       const Ogre::String& log = "boot.log");

    /** Clean Ogre instance. */
    virtual ~wxOgreRenderSystem();

    /** Manually load an Ogre plugin contained in a DLL / DSO. 
     *
     * @see Ogre::Root::loadPlugin
     */
    void LoadPlugin(const Ogre::String& plugin);

    /** Check through all available renderers if, the one the string 
     * is "render" exist, if so, it is selected.
     *
     * @param render that contains the name of the Render System 
     *        selected (e.g. Direct3D, OpenGL)
     */
    void SelectOgreRenderSystem(const Ogre::String& render);


    /** Checks for a capability.
     *
     * @see Ogre::RenderSystemCapabilities::hasCapability
     */
    bool HasCapability(const Ogre::Capabilities& c);

    /** Initialises the Ogre's renderer. */
    void Initialise();

private:

    Ogre::Root* m_root;

    DECLARE_OGRE_SINGLETON(wxOgreRenderSystem)
};

#endif
