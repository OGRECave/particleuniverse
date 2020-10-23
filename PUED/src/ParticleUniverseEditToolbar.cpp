/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseEditToolbar.h"
#include "wx/file.h"

UIEditToolbar::UIEditToolbar(wxWindow* parent)
{
	mWxToolBar1 = new wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxSize(TOOLBAR_EDIT_WIDTH, TOOLBAR_EDIT_HEIGHT), wxTB_VERTICAL | wxNO_BORDER);
	mWxToolBar1->SetMargins(8, 8);
	mWxToolBar1->SetToolBitmapSize(wxSize(16, 16));
	mUIEditIcons1 = new UIEditIcons1(mWxToolBar1);
	mWxToolBar1->Realize();

	mWxToolBar2 = new wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxSize(TOOLBAR_EDIT_WIDTH, TOOLBAR_EDIT_HEIGHT), wxTB_VERTICAL | wxNO_BORDER);
	mWxToolBar2->SetMargins(8, 8);
	mWxToolBar2->SetToolBitmapSize(wxSize(16, 16));
	mUIEditIcons2 = new UIEditIcons2(mWxToolBar2);
	mWxToolBar2->Realize();

	wxBoxSizer *vbox = new wxBoxSizer(wxHORIZONTAL);
	vbox->Add(mWxToolBar1, 0, wxEXPAND);
	vbox->Add(mWxToolBar2, 0, wxEXPAND);
	parent->SetSizer(vbox);
}
//-----------------------------------------------------------------------
UIEditToolbar::~UIEditToolbar(void)
{
	delete mUIEditIcons1;
	delete mUIEditIcons2;
}
//-----------------------------------------------------------------------
UIEditIcons1* UIEditToolbar::getEditIcons1(void) const
{
	return mUIEditIcons1;
}
//-----------------------------------------------------------------------
UIEditIcons2* UIEditToolbar::getEditIcons2(void) const
{
	return mUIEditIcons2;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
UIEditIcons1::UIEditIcons1(wxToolBar* toolbar)
{
	// Create the bitmaps of the edit toolbar
	mToolbar = toolbar;
//	toolbar->AddTool(ID_ADD_SYSTEM, _("System"), 
//		wxBitmap(ICONS_DIR + wxT("system.png"), wxBITMAP_TYPE_PNG),
//		wxNullBitmap, wxITEM_NORMAL,
//		wxT("System"), _("Add a new system"));
	toolbar->AddTool(ID_ADD_TECHNIQUE, _("Technique"), 
		wxBitmap(ICONS_DIR + wxT("technique.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Technique"), _("Add a new technique"));
	toolbar->AddTool(ID_ADD_RENDERER, _("Renderer"), 
		wxBitmap(ICONS_DIR + wxT("renderer.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Renderer"), _("Add a new renderer"));
	toolbar->AddTool(ID_ADD_EMITTER, _("Emitter"), 
		wxBitmap(ICONS_DIR + wxT("emitter.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Emitter"), _("Add a new emitter"));
	toolbar->AddTool(ID_ADD_AFFECTOR, _("Affector"), 
		wxBitmap(ICONS_DIR + wxT("affector.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Affector"), _("Add a new affector"));
	toolbar->AddTool(ID_ADD_OBSERVER, _("Observer"), 
		wxBitmap(ICONS_DIR + wxT("observer.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Observer"), _("Add a new observer"));
	toolbar->AddTool(ID_ADD_HANDLER, _("Handler"), 
		wxBitmap(ICONS_DIR + wxT("handler.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Handler"), _("Add a new handler"));
	toolbar->AddTool(ID_ADD_BEHAVIOUR, _("Behaviour"), 
		wxBitmap(ICONS_DIR + wxT("behaviour.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Behaviour"), _("Add a new behaviour"));
	toolbar->AddTool(ID_ADD_EXTERN, _("Extern"), 
		wxBitmap(ICONS_DIR + wxT("extern.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Extern"), _("Add a new extern"));
	reset();
}
//-----------------------------------------------------------------------
void UIEditIcons1::reset(void)
{
//	mToolbar->EnableTool(ID_ADD_SYSTEM, true);
	mToolbar->EnableTool(ID_ADD_TECHNIQUE, true);
	mToolbar->EnableTool(ID_ADD_RENDERER, true);
	mToolbar->EnableTool(ID_ADD_EMITTER, true);
	mToolbar->EnableTool(ID_ADD_AFFECTOR, true);
	mToolbar->EnableTool(ID_ADD_OBSERVER, true);
	mToolbar->EnableTool(ID_ADD_HANDLER, true);
	mToolbar->EnableTool(ID_ADD_BEHAVIOUR, true);
	mToolbar->EnableTool(ID_ADD_EXTERN, true);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
UIEditIcons2::UIEditIcons2(wxToolBar* toolbar)
{
	// Create the bitmaps of the edit toolbar
	mToolbar = toolbar;
	toolbar->AddTool(ID_CURSOR, _("Cursor"), 
		wxBitmap(ICONS_DIR + wxT("cursor.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Cursor"), _("Reset to default cursor"));
	toolbar->AddTool(ID_CONNECT, _("Connect"), 
		wxBitmap(ICONS_DIR + wxT("connect.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Connect"), _("Make connection between two components"));
	toolbar->AddTool(ID_DISCONNECT, _("Disconnect"), 
		wxBitmap(ICONS_DIR + wxT("disconnect.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Disconnect"), _("Delete a connection between two components"));
	toolbar->AddTool(ID_HELP_HTML, _("Help"), 
		wxBitmap(ICONS_DIR + wxT("help.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Help"), _("Display help of current component"));
	reset(0, 0); // Todo: Change when the loaded particle system is converted into graphical components (MDI childs and connections)
}
//-----------------------------------------------------------------------
void UIEditIcons2::reset(unsigned int numberOfListItems, unsigned int numberOfConnections)
{
	if (numberOfListItems > 1)
	{
		// Only 2 or more components can be connected
		mToolbar->EnableTool(ID_CONNECT, true);
	}
	else
	{
		mToolbar->EnableTool(ID_CONNECT, false);
	}
	if (numberOfConnections > 0)
	{
		mToolbar->EnableTool(ID_DISCONNECT, true);
	}
	else
	{
		mToolbar->EnableTool(ID_DISCONNECT, false);
	}
}
