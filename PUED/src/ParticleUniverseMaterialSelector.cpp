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
#include "ParticleUniverseMaterialSelector.h"
#include "wx/ogre/utils.h"

//-----------------------------------------------------------------------
MaterialProperty::MaterialProperty(
	const wxString& label, 
	const wxString& name) :
	wxStringPropertyClass(label, name, wxT("BaseWhite")),
	mMaterialName("BaseWhite")
{
}
//-----------------------------------------------------------------------
bool MaterialProperty::OnEvent (wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
	if (event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED)
	{
		// Open the dialog
		size_t count = 0;
		wxString choices[2000];

		Ogre::ResourceManager::ResourceMapIterator materialIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();
		while (materialIterator.hasMoreElements())
		{
			choices[count] = ogre2wx((static_cast<Ogre::MaterialPtr>(materialIterator.peekNextValue()))->getName());
			materialIterator.moveNext();
			count++;
		}

		MaterialSelector materialSelector(propgrid, _("Material Selector"), _("Select a material"), count, choices);
		if (materialSelector.ShowModal() == wxID_OK)
		{
			wxString mat = choices[materialSelector.GetSelection()];
			mMaterialName = wx2ogre(mat);
			SetValueFromString(mat);

			// Force changing the value on screen. Doesn't work probably if this is the last property in the propgrid
			propgrid->SelectProperty(propgrid->GetNextProperty(GetId()));
		}
	}
	return true;
}
//-----------------------------------------------------------------------
const Ogre::String& MaterialProperty::getMaterialName(void) const
{
	return mMaterialName;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
const Ogre::String& MaterialDialog::openDialog(wxWindow* parent)
{
	// Open the dialog
	size_t count = 0;
	wxString choices[2000];

	Ogre::ResourceManager::ResourceMapIterator materialIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();
	while (materialIterator.hasMoreElements())
	{
		choices[count] = ogre2wx((static_cast<Ogre::MaterialPtr>(materialIterator.peekNextValue()))->getName());
		materialIterator.moveNext();
		count++;
	}

	MaterialSelector materialSelector(parent, _("Material Selector"), _("Select a material"), count, choices);
	if (materialSelector.ShowModal() == wxID_OK)
	{
		wxString mat = choices[materialSelector.GetSelection()];
		mMaterialName = wx2ogre(mat);
		return mMaterialName;
	}
	return Ogre::StringUtil::BLANK;
}
//-----------------------------------------------------------------------
const Ogre::String& MaterialDialog::getMaterialName(void) const
{
	return mMaterialName;
}
