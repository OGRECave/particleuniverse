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

#ifndef __PUED_CONTEXTMENU_H__
#define __PUED_CONTEXTMENU_H__

#include "wx/ogre/prerequisites.h"

/**	Menu item of the context menu.
*/
class ContextMenu;
class ContextMenuElement : public wxStaticText
{
	public:
		// Constructor / Destructor
		ContextMenuElement(ContextMenu *parent,
                 wxWindowID id,
                 const wxString& label,
				 const wxString& bitmapNameEnabled,
				 const wxString& bitmapNameDisabled,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = wxStaticTextNameStr);
		~ContextMenuElement(void){};

		// Getters/Setters
		wxStaticBitmap* getEnabledIcon(void);

		// Event handlers
		void OnWindowEnter(wxMouseEvent& event);
		void OnWindowLeave(wxMouseEvent& event);
		void OnMouseLButtonPressed(wxMouseEvent& event);

		// Other functions
		void setEnabled(bool enabled);

	protected:
		bool mEnabled;
		ContextMenu* mParent;
		wxStaticBitmap* mBitmapEnabled;
		wxStaticBitmap* mBitmapDisabled;
};

/**	Used to perform a callback. Every class that uses a context menu must have this function, so it can handle the selected element.
*/
class ContextMenuCallbackObject
{
	public:
		// Constructor / Destructor
		ContextMenuCallbackObject(void);
		~ContextMenuCallbackObject(void){};

		// Callback function
		virtual void callbackContextMenu(wxWindowID id, wxWindow* window) = 0;
};


/**	Context menu, that can be selected with a right mouse click.
*/
class ContextMenu : public wxFrame
{
	public:
		// Constructor / Destructor
		ContextMenu(wxWindow* parent, ContextMenuCallbackObject* callbackObject, const wxPoint& pos, const wxSize& size);
		~ContextMenu(void){};

		// Add a menu element
		void addMenuElement(wxWindowID id,
			const wxString& label,
			const wxString& bitmapNameEnabled,
			const wxString& bitmapNameDisabled,
			const wxSize& size);


		// Enable or disable a menu element
		void enableMenuElement(wxWindowID id, bool enabled);

		// Generate the menu
		void initialise(void);

		// Hide the context menu if the mouse is in another window
		void hideIfNeeded(void);

		// Hide the menu
		void hideContextMenu(void);

		// Perform an action if the menu-item was selected
		void callbackContextMenu(wxWindowID id, wxWindow* window);

		/**	Handle mouse event
		*/
		virtual void showContextMenu(wxPoint position);

	protected:
		wxWindow* mParent;
		std::vector<ContextMenuElement*> mMenuElements;
		bool mInitialised;
		ContextMenuCallbackObject* mCallbackObject;
};

#endif
