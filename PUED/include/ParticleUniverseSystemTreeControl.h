/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_TREE_CTRL_H__
#define __PUED_TREE_CTRL_H__

#include "wx/ogre/prerequisites.h"
#include <wx/treectrl.h>
#include "ParticleUniverseContextMenu.h"

class ParticleUniverseEditorFrame;

static wxString ROOT_NODE_NAME = wxT("");
static wxString DEFAULT_CATEGORY_NAME = wxT("");

/**	Extended wxTreeCtrl class
*/
class SystemTreeControl : public wxTreeCtrl, ContextMenuCallbackObject
{
	public:
		// Constructor / Destructor
		SystemTreeControl(ParticleUniverseEditorFrame* parentFrame, 
			wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize);
		~SystemTreeControl(void){};

		/**	Handle mouse event
		*/
		void OnMouseEvent(wxMouseEvent& event);
		void OnMouseRButtonPressed(wxMouseEvent& event);
		void OnWindowLeave(wxMouseEvent& event);

		/**	Handle context menu
		*/
		void resetContextMenu(void);
		virtual void callbackContextMenu(wxWindowID id, wxWindow* window);

		/**	Context menu functions
		*/
		void setPlayInContextMenuEnabled(bool enabled);

		/**	Expand only the root and collapse the categories
		*/
		void expandRootOnly(void);

		/**	Useful functions
		*/
		bool isCategoryName(const wxString& name);
		bool isSelectedItemCategory(void);
		wxTreeItemId findCategory(const wxString& name);
		wxTreeItemId findParticleSystem(const wxString& name);
		wxTreeItemId getCurrentCategory(void);
		bool isSelectedItemLast(void);
		bool isSelectedItemFirst(void);
		wxTreeItemId getNextItem(void);
		wxTreeItemId getPreviousItem(void);
		wxTreeItemId addItem(wxString& categoryName, wxString& particleSystemName, bool sortParent = true);
		void sortAll(void);
		wxTreeItemId setToFirstParticleSystem(void);

	protected:
		ParticleUniverseEditorFrame* mParentFrame;
		ContextMenu* mContextMenu;
		bool mSuppressSelectEvent;

		virtual bool Destroy(void);
};

#endif
