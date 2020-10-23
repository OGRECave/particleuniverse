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
#include "ParticleUniverseMaterialListBox.h"
#include "ParticleUniverseMaterialTab.h"

//-----------------------------------------------------------------------
MaterialListBox::MaterialListBox(MaterialTab* materialTab,
	wxWindow* parent, 
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size)	: 
	wxListBox(parent, id, pos, size, 0, wxLB_SORT),
	mMaterialTab(materialTab)
{

	Connect(wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(MaterialListBox::OnMaterialsClick));
}
//-----------------------------------------------------------------------
MaterialListBox::~MaterialListBox(void)
{
}
//-----------------------------------------------------------------------
void MaterialListBox::loadMaterials(void)
{
	Clear();
	Ogre::ResourceManager::ResourceMapIterator materialIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();
	while (materialIterator.hasMoreElements())
	{
		Ogre::String s = (static_cast<Ogre::MaterialPtr>(materialIterator.peekNextValue()))->getName();
		Append(ogre2wx(s));
		materialIterator.moveNext();
	}
}
//-----------------------------------------------------------------------
void MaterialListBox::OnMaterialsClick(wxCommandEvent& event)
{
	mMaterialTab->selectMaterial(GetStringSelection());
}
//-----------------------------------------------------------------------
void MaterialListBox::addMaterialName(wxString& materialName)
{
	Insert(materialName, 0);
	Select(0);
}
