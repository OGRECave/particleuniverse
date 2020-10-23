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
#include "ParticleUniverseEditor.h"
#include "ParticleUniverseSystemTreeControl.h"
#include "uxtheme.h"

//-----------------------------------------------------------------------
SystemTreeControl::SystemTreeControl(ParticleUniverseEditorFrame* parentFrame, 
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size)	: 
	wxTreeCtrl(parentFrame, id, pos, size, wxTR_HAS_BUTTONS | wxTR_SINGLE),
	mParentFrame(parentFrame),
	mContextMenu(0)
{
// To set the explorer look. See also http://weseetips.com/tag/setwindowtheme/
//#ifdef __WXMSW__ 
        //SetWindowTheme((HWND)GetHWND(), L"Explorer", NULL); 
//#endif 

	// Internationalize
	ROOT_NODE_NAME = _("Particle Systems");
	DEFAULT_CATEGORY_NAME = _("General");

	// Assign images to this tree	
	wxImageList* images = new wxImageList(16, 16, true);
	images->Add(wxBitmap(ICONS_DIR + wxT("pu_logo.png"), wxBITMAP_TYPE_PNG));
	images->Add(wxBitmap(ICONS_DIR + wxT("folder.png"), wxBITMAP_TYPE_PNG));
	AssignImageList(images);

	mContextMenu = new ContextMenu(this, this, wxDefaultPosition, wxSize(100, 100));
	mContextMenu->addMenuElement(ID_PLAY, 
		_("        Play"), 
		wxT("control_play_blue.png"),
		wxT("control_play.png"),
		wxSize(200, 16));
	mContextMenu->addMenuElement(ID_CLONE, 
		_("        Clone item"), 
		wxT("clone.png"),
		wxT("clone.png"),
		wxSize(200, 16));
	mContextMenu->addMenuElement(ID_REMOVE, 
		_("        Remove from list"), 
		wxT("remove.png"),
		wxT("remove.png"),
		wxSize(200, 16));
	mContextMenu->initialise();

	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(SystemTreeControl::OnMouseEvent));
	Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(SystemTreeControl::OnMouseRButtonPressed));
	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(SystemTreeControl::OnWindowLeave));
}
//-----------------------------------------------------------------------
bool SystemTreeControl::Destroy(void)
{
	delete mContextMenu;
	return wxTreeCtrl::Destroy();
}
//-----------------------------------------------------------------------
void SystemTreeControl::OnMouseEvent(wxMouseEvent& event)
{
	mParentFrame->OnMouseMoveCallback(event);
}
//------------------------------------------------------------------------------
void SystemTreeControl::OnMouseRButtonPressed(wxMouseEvent& event)
{
	// Open submenu
	wxPoint pos = GetParent()->GetPosition() + event.GetPosition();
	mContextMenu->showContextMenu(pos);
}
//------------------------------------------------------------------------------
void SystemTreeControl::resetContextMenu(void)
{
	mContextMenu->hideIfNeeded();
}
//------------------------------------------------------------------------------
void SystemTreeControl::OnWindowLeave(wxMouseEvent& event)
{
	resetContextMenu();
}
//------------------------------------------------------------------------------
void SystemTreeControl::setPlayInContextMenuEnabled(bool enabled)
{
	// This function is called from the parent frame
	mContextMenu->enableMenuElement(ID_PLAY, enabled);
}
//-----------------------------------------------------------------------
void SystemTreeControl::callbackContextMenu(wxWindowID id, wxWindow* window)
{
	switch (id)
	{
		case ID_PLAY:
			mParentFrame->doPlay();
		break;

		case ID_CLONE:
			mParentFrame->doClone();
		break;

		case ID_REMOVE:
			mParentFrame->doRemove();
		break;
	}
}
//-----------------------------------------------------------------------
void SystemTreeControl::expandRootOnly(void)
{
	wxTreeItemIdValue handle;
	wxTreeItemId item = GetFirstChild(GetRootItem(), handle);
	while(item.IsOk())
	{
		Collapse(item);
		item = GetNextChild(GetRootItem(), handle);
	}
	Expand(GetRootItem());
}
//-----------------------------------------------------------------------
bool SystemTreeControl::isCategoryName(const wxString& name)
{
	if (name == ROOT_NODE_NAME)
		return true;

	wxTreeItemIdValue handle;
	wxTreeItemId item = GetFirstChild(GetRootItem(), handle);
	while(item.IsOk())
	{
		if (name == GetItemText(item))
		{
			return true;
		}
		item = GetNextChild(GetRootItem(), handle);
	}
	return false;
}
//-----------------------------------------------------------------------
bool SystemTreeControl::isSelectedItemCategory(void)
{
	wxTreeItemId selected = GetSelection();
	wxString name = GetItemText(selected);

	if (name == ROOT_NODE_NAME)
		return true;

	wxTreeItemIdValue handle;
	wxTreeItemId item = GetFirstChild(GetRootItem(), handle);
	while(item.IsOk())
	{
		if (name == GetItemText(item))
		{
			return true;
		}
		item = GetNextChild(GetRootItem(), handle);
	}
	return false;
}
//-----------------------------------------------------------------------
wxTreeItemId SystemTreeControl::findCategory(const wxString& name)
{
	wxString category = name;
	if (name == ROOT_NODE_NAME)
	{
		// Change the category into the default category
		category = DEFAULT_CATEGORY_NAME;
	}

	wxTreeItemIdValue handle;
	wxTreeItemId item = GetFirstChild(GetRootItem(), handle);
	while(item.IsOk())
	{
		if (category == GetItemText(item))
		{
			return item;
		}
		item = GetNextChild(GetRootItem(), handle);
	}
	return 0;
}
//-----------------------------------------------------------------------
wxTreeItemId SystemTreeControl::findParticleSystem(const wxString& name)
{
	wxTreeItemIdValue handleRoot;
	wxTreeItemId category = GetFirstChild(GetRootItem(), handleRoot);
	while(category.IsOk())
	{
		wxTreeItemIdValue handleChild;
		wxTreeItemId item = GetFirstChild(category, handleChild);
		while(item.IsOk())
		{
			if (name == GetItemText(item))
			{
				return item;
			}
			item = GetNextChild(category, handleChild);
		}

		category = GetNextChild(GetRootItem(), handleRoot);
	}

	return 0;
}
//-----------------------------------------------------------------------
wxTreeItemId SystemTreeControl::getCurrentCategory(void)
{
	wxTreeItemId selected = GetSelection();
	wxString name = GetItemText(selected);
	if (name == ROOT_NODE_NAME)
	{
		name = DEFAULT_CATEGORY_NAME;
		selected = findCategory(DEFAULT_CATEGORY_NAME);
	}

	if (isCategoryName(name))
	{
		// It is a category
		return selected;
	}

	// It is a particle system, so get its parent
	return GetItemParent(selected);
}
//-----------------------------------------------------------------------
bool SystemTreeControl::isSelectedItemLast(void)
{
	// Get the current category (assuming the selected item is a Particle System)
	wxTreeItemId category = getCurrentCategory();
	if (category.IsOk())
	{
		wxTreeItemId lastItem = GetLastChild(category);
		return (lastItem == GetSelection());
	}
	return false;
}
//-----------------------------------------------------------------------
bool SystemTreeControl::isSelectedItemFirst(void)
{
	// Get the current category (assuming the selected item is a Particle System)
	wxTreeItemId category = getCurrentCategory();
	if (category.IsOk())
	{
		wxTreeItemIdValue cookie;
		wxTreeItemId firstItem = GetFirstChild(category, cookie);
		return (firstItem == GetSelection());
	}
	return false;
}
//-----------------------------------------------------------------------
wxTreeItemId SystemTreeControl::getNextItem(void)
{
	wxTreeItemId selected = GetSelection();
	wxString name = GetItemText(selected);
	wxTreeItemIdValue cookie;
	if (isCategoryName(name))
	{
		// It is a category, so set it to the first item.
		if (selected == GetRootItem())
		{
			wxTreeItemId category = GetFirstChild(selected, cookie);
			if (category.IsOk())
			{
				// Return the first child of the first category
				return GetFirstChild(category, cookie);
			}
		}
		else
		{
			// Return the first child of the selected category
			return GetFirstChild(selected, cookie);
		}
	}

	// It is no category, so it is a Particle System
	if (isSelectedItemLast())
	{
		// This is the last one in the category, so skip to the next category
		wxTreeItemId category = getCurrentCategory();
		wxTreeItemId sibling = GetNextSibling(category);
		if (sibling.IsOk())
		{
			return GetFirstChild(sibling, cookie);
		}
	}
	else
	{
		// Skip to the second one in the same category
		//return GetNextVisible(selected);
		return GetNextSibling(selected);
	}
	
	return 0;
}
//-----------------------------------------------------------------------
wxTreeItemId SystemTreeControl::getPreviousItem(void)
{
	wxTreeItemId selected = GetSelection();
	wxString name = GetItemText(selected);
	if (isCategoryName(name))
	{
		// It is a category, so set it to the first item.
		if (selected == GetRootItem() || name == DEFAULT_CATEGORY_NAME)
		{
			// Higher than this is not possible
			return 0;
		}
		else
		{
			// Go to the previous category and select the lastr child
			wxTreeItemId category = getCurrentCategory();
			wxTreeItemId sibling = GetPrevSibling(category);
			if (sibling.IsOk())
			{
				return GetLastChild(sibling);
			}
		}
	}

	// It is no category, so it is a Particle System
	if (isSelectedItemFirst())
	{
		// This is the first one in the category, so skip to the previous category
		wxTreeItemId category = getCurrentCategory();
		wxTreeItemId sibling = GetPrevSibling(category);
		if (sibling.IsOk())
		{
			return GetLastChild(sibling);
		}
	}
	else
	{
		// Skip to the second one in the same category
		//return GetPrevVisible(selected);
		return GetPrevSibling(selected);
	}
	
	return 0;
}
//-----------------------------------------------------------------------
wxTreeItemId SystemTreeControl::addItem(wxString& categoryName, wxString& particleSystemName, bool sortParent)
{
	wxTreeItemId newItem = 0;
	if (categoryName == wxT("") || categoryName == ROOT_NODE_NAME)
	{
		// Make it the default category
		categoryName = DEFAULT_CATEGORY_NAME;
	}

	// Insert the new name
	wxTreeItemId category = findCategory(categoryName);
	if (!category.IsOk())
	{
		// Category does not exist, create a new one
		category = AppendItem(GetRootItem(), categoryName, 1);
	}

	if (category.IsOk())
	{
		// Add the new name
		newItem = PrependItem(category, particleSystemName);
		if (newItem.IsOk())
		{
			SelectItem(newItem);
		}

		if (sortParent)
		{
			SortChildren(category);
		}
	}

	return newItem;
}
//-----------------------------------------------------------------------
void SystemTreeControl::sortAll(void)
{
	wxTreeItemIdValue handleRoot;
	wxTreeItemId category = GetFirstChild(GetRootItem(), handleRoot);
	while(category.IsOk())
	{
		SortChildren(category);
		category = GetNextChild(GetRootItem(), handleRoot);
	}
}
//-----------------------------------------------------------------------
wxTreeItemId SystemTreeControl::setToFirstParticleSystem(void)
{
	wxTreeItemId item;
	wxTreeItemIdValue handleRoot;
	wxTreeItemId category = GetFirstChild(GetRootItem(), handleRoot);
	while(category.IsOk())
	{
		if (GetChildrenCount(category) > 0)
		{
			wxTreeItemIdValue cookie;
			item = GetFirstChild(category, cookie);
			if (item.IsOk())
			{
				SelectItem(item);
				return item;
			}
		}
		category = GetNextChild(GetRootItem(), handleRoot);
	}

	return 0;
}