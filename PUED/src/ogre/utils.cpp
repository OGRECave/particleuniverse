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

#include "ParticleUniverseEditorPCH.h"
#include "wx/ogre/utils.h"

#include <string>
#include <algorithm>

//------------------------------------------------------------------------------
void wxOgreExceptionBox(Ogre::Exception& e)
{
    wxMessageBox(std2wx(e.getFullDescription()),  _T("OGRE Exception"),
                 wxICON_ERROR);
}
//------------------------------------------------------------------------------
std::string wx2std(const wxString& s)
{
    return std::string(s.mb_str());
}
//------------------------------------------------------------------------------
wxString std2wx(const std::string& s)
{
    return wxString::FromAscii(s.c_str());
}
//------------------------------------------------------------------------------
wxString ogre2wx(const Ogre::String& s)
{
	return wxString::FromAscii(s.c_str());
}
//------------------------------------------------------------------------------
wxString ogre2wxTranslate(const Ogre::String& s)
{
	return wxGetTranslation(wxString::FromAscii(s.c_str()));
}
//------------------------------------------------------------------------------
Ogre::String wx2ogre(const wxString& s)
{
	//return Ogre::String(s.mb_str());
	return Ogre::String(s.mb_str(wxConvUTF8));
}
//------------------------------------------------------------------------------
std::string getExtension(const std::string& s)
{
    std::string ext;

    // Find the dot position.
    size_t pos = s.rfind('.');

    if (pos != std::string::npos)
        ext = s.substr(pos);

    return ext;
}
//------------------------------------------------------------------------------
