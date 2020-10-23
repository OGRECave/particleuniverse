/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseContextMenu.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
ContextMenuElement::ContextMenuElement(ContextMenu* parent,
                 wxWindowID id,
                 const wxString& label,
				 const wxString& bitmapNameEnabled,
				 const wxString& bitmapNameDisabled,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style,
                 const wxString& name) :
	wxStaticText(parent, id, label, pos, size, style, name),
	mParent(parent),
	mEnabled(true)
{
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(ContextMenuElement::OnWindowEnter));
	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(ContextMenuElement::OnWindowLeave));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ContextMenuElement::OnMouseLButtonPressed));

	mBitmapEnabled = new wxStaticBitmap(this, wxID_ANY, wxBitmap(ICONS_DIR + bitmapNameEnabled, wxBITMAP_TYPE_ANY), wxDefaultPosition, wxDefaultSize, 0); 
	mBitmapDisabled = new wxStaticBitmap(this, wxID_ANY, wxBitmap(ICONS_DIR + bitmapNameDisabled, wxBITMAP_TYPE_ANY), wxDefaultPosition, wxDefaultSize, 0); 
	mBitmapDisabled->Hide();
}
//------------------------------------------------------------------------------
wxStaticBitmap* ContextMenuElement::getEnabledIcon(void)
{
	return mBitmapEnabled;
}
//------------------------------------------------------------------------------
void ContextMenuElement::OnWindowEnter(wxMouseEvent& event)
{
	if (mEnabled)
	{
		SetBackgroundColour(wxColor(51, 153, 255));
		SetForegroundColour(wxColor(255, 255, 255));
		Refresh();
	}
}
//------------------------------------------------------------------------------
void ContextMenuElement::OnWindowLeave(wxMouseEvent& event)
{
	if (mEnabled)
	{
		SetBackgroundColour(wxColor(240, 240, 240));
		SetForegroundColour(wxColor(0, 0, 0));
		Refresh();
	}
}
//------------------------------------------------------------------------------
void ContextMenuElement::OnMouseLButtonPressed(wxMouseEvent& event)
{
	if (mEnabled)
	{
		// Perform callback
		mParent->callbackContextMenu(GetId(), this);
		GetParent()->Hide();
	}
}
//-----------------------------------------------------------------------
void ContextMenuElement::setEnabled(bool enabled)
{
	mEnabled = enabled;
	if (mEnabled)
	{
		SetBackgroundColour(wxColor(240, 240, 240));
		SetForegroundColour(wxColor(0, 0, 0));
		mBitmapEnabled->Show();
		mBitmapDisabled->Hide();
	}
	else
	{
		SetBackgroundColour(wxColor(240, 240, 240));
		SetForegroundColour(wxColor(160, 160, 160));
		mBitmapEnabled->Hide();
		mBitmapDisabled->Show();
	}
	
	Refresh();
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ContextMenuCallbackObject::ContextMenuCallbackObject(void)
{
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ContextMenu::ContextMenu(wxWindow* parent, ContextMenuCallbackObject* callbackObject, const wxPoint& pos, const wxSize& size) :
	wxFrame(parent, wxID_ANY, wxT(""), pos, size, wxFRAME_NO_TASKBAR | wxFRAME_FLOAT_ON_PARENT | wxBORDER_DOUBLE | wxBORDER_RAISED),
	mParent(parent),
	mInitialised(false),
	mCallbackObject(callbackObject)
{
}
//-----------------------------------------------------------------------
void ContextMenu::addMenuElement(wxWindowID id,
    const wxString& label,
	const wxString& bitmapNameEnabled,
	const wxString& bitmapNameDisabled,
	const wxSize& size)
{
	mMenuElements.push_back(new ContextMenuElement(this,
		id,
        label,
		bitmapNameEnabled,
		bitmapNameDisabled,
        wxDefaultPosition,
        size,
        wxALIGN_LEFT | wxST_NO_AUTORESIZE));
}
//-----------------------------------------------------------------------
void ContextMenu::initialise(void)
{
	if (mInitialised)
		return;

	// Build up context menu
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* item = new wxBoxSizer(wxVERTICAL);

	std::vector<ContextMenuElement*>::iterator it;
	std::vector<ContextMenuElement*>::iterator itEnd = mMenuElements.end();
	for (it = mMenuElements.begin(); it != itEnd; ++it)
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		//sizer->Add((*it)->getEnabledIcon(), 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
		sizer->Add(*it, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
		item->Add(sizer, 0, wxGROW|wxALL, 0);
	}

	// Add to panel
	topSizer->Add(item, 1, wxGROW|wxALIGN_CENTRE|wxALL, 0);
	SetSizer(topSizer);
	topSizer->Fit(this);
	SetBackgroundColour(wxColor(240, 240, 240));
	mInitialised = true;
}
//-----------------------------------------------------------------------
void ContextMenu::hideIfNeeded(void)
{
	if (!IsMouseInWindow())
	{
		// Hide the context menu if the mouse is in another window
		Hide();
	}
}
//-----------------------------------------------------------------------
void ContextMenu::hideContextMenu(void)
{
	Hide();
}
//------------------------------------------------------------------------------
void ContextMenu::callbackContextMenu(wxWindowID id, wxWindow* window)
{
	if (mCallbackObject)
	{
		mCallbackObject->callbackContextMenu(id, window);
	}
}
//------------------------------------------------------------------------------
void ContextMenu::showContextMenu(wxPoint position)
{
	// Open context menu
	position.y += GetSize().y - 6;
	position.x += 10;
	SetPosition(position);
	Show();
}
//------------------------------------------------------------------------------
void ContextMenu::enableMenuElement(wxWindowID id, bool enabled)
{
	std::vector<ContextMenuElement*>::iterator it;
	std::vector<ContextMenuElement*>::iterator itEnd = mMenuElements.end();
	for (it = mMenuElements.begin(); it != itEnd; ++it)
	{
		if ((*it)->GetId() == id)
		{
			(*it)->setEnabled(enabled);
		}
	}
}
