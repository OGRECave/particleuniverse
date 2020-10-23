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
#include "ParticleUniverseMeshSelector.h"
#include "wx/ogre/utils.h"

//-----------------------------------------------------------------------
MeshProperty::MeshProperty(
	const wxString& label, 
	const wxString& name) :
	wxStringPropertyClass(label, name, wxT("")),
	mMeshName(Ogre::StringUtil::BLANK)
{
}
//-----------------------------------------------------------------------
bool MeshProperty::OnEvent (wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
	if (event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED)
	{
		// Open the dialog
		size_t count = 0;
		wxString choices[1000];
		Ogre::StringVector listOfResourceGroups = Ogre::ResourceGroupManager::getSingletonPtr()->getResourceGroups();
		size_t numGroups = listOfResourceGroups.size();
		Ogre::StringVector::iterator itListOfResourceGroups = listOfResourceGroups.begin();

		for (unsigned int i = 0; i < numGroups; ++i)
		{          
			Ogre::StringVectorPtr listOfMeshes = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceNames((*itListOfResourceGroups), "*.mesh");
			Ogre::StringVector::iterator it;
			for (it = listOfMeshes->begin(); it != listOfMeshes->end(); ++it)
			{
				Ogre::String name = *it;
				choices[count] = ogre2wx(name);
				count++;
			}
			itListOfResourceGroups++;
		}

//		Ogre::ResourceManager::ResourceMapIterator meshIterator = Ogre::MeshManager::getSingleton().getResourceIterator();
//		while (meshIterator.hasMoreElements())
//		{
//			choices[count] = (static_cast<Ogre::MeshPtr>(meshIterator.peekNextValue()))->getName();
//			meshIterator.moveNext();
//			count++;
//		}

		if (count > 0)
		{
			MeshSelector meshSelector(propgrid, _("Mesh Selector"), _("Select a mesh"), count, choices);
			if (meshSelector.ShowModal() == wxID_OK)
			{
				wxString mesh = choices[meshSelector.GetSelection()];
				mMeshName = wx2ogre(mesh);
				SetValueFromString(mesh);
	
				// Force changing the value on screen. Doesn't work probably if this is the last property in the propgrid
				propgrid->SelectProperty(propgrid->GetNextProperty(GetId()));
			}
		}
	}
	return true;
}
//-----------------------------------------------------------------------
const Ogre::String& MeshProperty::getMeshName(void) const
{
	return mMeshName;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
const Ogre::String& MeshDialog::openDialog(wxWindow* parent)
{
	// Open the dialog
	size_t count = 0;
	wxString choices[1000];
	Ogre::StringVector listOfResourceGroups = Ogre::ResourceGroupManager::getSingletonPtr()->getResourceGroups();
	size_t numGroups = listOfResourceGroups.size();
	Ogre::StringVector::iterator itListOfResourceGroups = listOfResourceGroups.begin();
	numGroups--;

	for (unsigned int i = 0; i < numGroups; ++i)
	{          
		Ogre::StringVectorPtr listOfMeshes = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceNames((*itListOfResourceGroups), "*.mesh");
		Ogre::StringVector::iterator it;
		for (it = listOfMeshes->begin(); it != listOfMeshes->end(); ++it)
		{
			Ogre::String name = *it;
			choices[count] = ogre2wx(name);
			count++;
		}
		itListOfResourceGroups++;
	}

	if (count > 0)
	{
		MeshSelector meshSelector(parent, _("Mesh Selector"), _("Select a mesh"), count, choices);
		if (meshSelector.ShowModal() == wxID_OK)
		{	
			wxString mesh = choices[meshSelector.GetSelection()];
			mMeshName = wx2ogre(mesh);
			return mMeshName;
		}
	}

	return Ogre::StringUtil::BLANK;
}
//-----------------------------------------------------------------------
const Ogre::String& MeshDialog::getMeshName(void) const
{
	return mMeshName;
}

