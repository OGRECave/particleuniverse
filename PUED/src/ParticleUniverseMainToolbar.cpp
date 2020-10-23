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
#include "ParticleUniverseMainToolbar.h"
#include "wx/file.h"

//-----------------------------------------------------------------------
UIMainToolbar::UIMainToolbar(ParticleUniverseEditorFrame* parent)
{
	mParent = parent;
	mWxToolBar = new wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxSize(MAX_WIDTH, 20), wxTB_FLAT);
	mWxToolBar->SetMargins(4, 4);
	mWxToolBar->SetToolBitmapSize(wxSize(16, 16));

	mUIFileIcons = new UIFileIcons(mWxToolBar);
	mUICompileIcons = new UICompileIcons(mWxToolBar);
	mUIPlayIcons = new UIPlayIcons(mWxToolBar);
	mUIMiscIcons = new UIMiscIcons(parent, mWxToolBar);
	mWxToolBar->Realize();
}
//-----------------------------------------------------------------------
UIMainToolbar::~UIMainToolbar(void)
{
	delete mUIFileIcons;
	delete mUICompileIcons;
	delete mUIPlayIcons;
	delete mUIMiscIcons;
}
//-----------------------------------------------------------------------
void UIMainToolbar::resetTimeScale(void)
{
	mUIMiscIcons->resetTimeScale();
}
//-----------------------------------------------------------------------
wxToolBar* UIMainToolbar::getWxToolbar(void) const
{
	return mWxToolBar;
}
//-----------------------------------------------------------------------
UIFileIcons* UIMainToolbar::getFileIcons(void) const
{
	return mUIFileIcons;
}
//-----------------------------------------------------------------------
UICompileIcons* UIMainToolbar::getCompileIcons(void) const
{
	return mUICompileIcons;
}
//-----------------------------------------------------------------------
UIPlayIcons* UIMainToolbar::getPlayIcons(void) const
{
	return mUIPlayIcons;
}
//-----------------------------------------------------------------------
UIMiscIcons* UIMainToolbar::getMiscIcons(void) const
{
	return mUIMiscIcons;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
UIPlayIcons::UIPlayIcons(wxToolBar* toolbar)
{
	// Create the bitmaps of the main toolbar
	mToolbar = toolbar;
	toolbar->AddTool(ID_REWIND, _("Rewind"), 
		wxBitmap(ICONS_DIR + wxT("control_rewind_blue.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Rewind"), _("Get the previous particle system from the list"));
	toolbar->AddTool(ID_PAUSE, _("Pause"), 
		wxBitmap(ICONS_DIR + wxT("control_pause_blue.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Pause"), _("Pause the current particle system"));
	toolbar->AddTool(ID_PLAY, _("Play"), 
		wxBitmap(ICONS_DIR + wxT("control_play_blue.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Play"), _("Run the current particle system"));
	toolbar->AddTool(ID_STOP, _("Stop"), 
		wxBitmap(ICONS_DIR + wxT("control_stop_blue.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Stop"), _("Stop the current particle system"));
	toolbar->AddTool(ID_RECORD, _("Record"), 
		wxBitmap(ICONS_DIR + wxT("control_record_blue.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Record"), _("Record the current particle system"));
	toolbar->AddTool(ID_WIND, _("Wind"), 
		wxBitmap(ICONS_DIR + wxT("control_fastforward_blue.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Wind"), _("Get the next particle system from the list"));
	toolbar->AddSeparator();
	disableAll();
}
//-----------------------------------------------------------------------
void UIPlayIcons::reset(unsigned int numberOfListItems)
{
	if (numberOfListItems > 0)
	{
		resetListFilled();
	}
	else
	{
		disableAll();
	}
}
//-----------------------------------------------------------------------
void UIPlayIcons::disable(unsigned int numberOfListItems)
{
	if (numberOfListItems > 0)
	{
		disableListFilled();
	}
	else
	{
		disableAll();
	}
}
//-----------------------------------------------------------------------
void UIPlayIcons::disableAll(void)
{
	// Set the icons on the toolbar to their defaults
	mToolbar->EnableTool(ID_REWIND, false);
	mToolbar->EnableTool(ID_PLAY, false);
	mToolbar->EnableTool(ID_RECORD, false);
	mToolbar->EnableTool(ID_PAUSE, false);
	mToolbar->EnableTool(ID_STOP, false);
	mToolbar->EnableTool(ID_WIND, false);
}
//-----------------------------------------------------------------------
void UIPlayIcons::disableListFilled(void)
{
	// Set the icons on the toolbar to their defaults
	mToolbar->EnableTool(ID_REWIND, true);
	mToolbar->EnableTool(ID_PLAY, false);
	mToolbar->EnableTool(ID_RECORD, false);
	mToolbar->EnableTool(ID_PAUSE, false);
	mToolbar->EnableTool(ID_STOP, false);
	mToolbar->EnableTool(ID_WIND, true);
}
//-----------------------------------------------------------------------
void UIPlayIcons::resetListFilled(void)
{
	// Set the icons on the toolbar to their defaults
	mToolbar->EnableTool(ID_REWIND, true);
	mToolbar->EnableTool(ID_PLAY, true);

#ifndef PU_FULL_VERSION
	mToolbar->EnableTool(ID_RECORD, false);
#else
	mToolbar->EnableTool(ID_RECORD, true);
#endif // PU_FULL_VERSION

	mToolbar->EnableTool(ID_PAUSE, false);
	mToolbar->EnableTool(ID_STOP, false);
	mToolbar->EnableTool(ID_WIND, true);
}
//-----------------------------------------------------------------------
void UIPlayIcons::play(void)
{
	mToolbar->EnableTool(ID_REWIND, false);
	mToolbar->EnableTool(ID_PLAY, false);
	mToolbar->EnableTool(ID_RECORD, false);
	mToolbar->EnableTool(ID_PAUSE, true);
	mToolbar->EnableTool(ID_STOP, true);
	mToolbar->EnableTool(ID_WIND, false);
}
//-----------------------------------------------------------------------
void UIPlayIcons::pause(void)
{
	mToolbar->EnableTool(ID_REWIND, false);
	mToolbar->EnableTool(ID_PLAY, true);
	mToolbar->EnableTool(ID_RECORD, false);
	mToolbar->EnableTool(ID_PAUSE, false);
	mToolbar->EnableTool(ID_STOP, true);
	mToolbar->EnableTool(ID_WIND, false);
}
//-----------------------------------------------------------------------
void UIPlayIcons::record(void)
{
	mToolbar->EnableTool(ID_REWIND, false);
	mToolbar->EnableTool(ID_PLAY, false);
	mToolbar->EnableTool(ID_RECORD, false);
	mToolbar->EnableTool(ID_PAUSE, false);
	mToolbar->EnableTool(ID_STOP, true);
	mToolbar->EnableTool(ID_WIND, false);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
UIFileIcons::UIFileIcons(wxToolBar* toolbar)
{
	// Create the bitmaps of the main toolbar
	mToolbar = toolbar;
	toolbar->AddTool(wxID_NEW, _("New"), 
		wxBitmap(ICONS_DIR + wxT("new.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("New"), _("Create a new particle system"));
	toolbar->AddTool(wxID_FILE, _("Load"), 
		wxBitmap(ICONS_DIR + wxT("load.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Load"), _("Load a *.pu or *.pua file"));
	toolbar->AddTool(wxID_OPEN, _("Add resource location"), 
		wxBitmap(ICONS_DIR + wxT("folder_add.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Add resource location"), _("Add a resource location"));
	toolbar->AddTool(wxID_SAVE, _("Save"), 
		wxBitmap(ICONS_DIR + wxT("save.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Save"), _("Save a *.pu or *.pua file"));
	toolbar->AddSeparator();
}
//-----------------------------------------------------------------------
void UIFileIcons::reset(unsigned int numberOfListItems)
{
	mToolbar->EnableTool(wxID_NEW, true);
	mToolbar->EnableTool(wxID_FILE, true);
	mToolbar->EnableTool(wxID_OPEN, true);
	if (numberOfListItems > 0)
	{
		mToolbar->EnableTool(wxID_SAVE, true);
	}
	else
	{
		mToolbar->EnableTool(wxID_SAVE, false);
	}
}
//-----------------------------------------------------------------------
void UIFileIcons::disableAll(void)
{
	mToolbar->EnableTool(wxID_NEW, false);
	mToolbar->EnableTool(wxID_FILE, false);
	mToolbar->EnableTool(wxID_OPEN, false);
	mToolbar->EnableTool(wxID_SAVE, false);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
UICompileIcons::UICompileIcons(wxToolBar* toolbar)
{
	// Create the bitmaps of the main toolbar
	mToolbar = toolbar;
	toolbar->AddTool(ID_COMPILE, _("Compile"), 
		wxBitmap(ICONS_DIR + wxT("compile.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Compile"), _("Compile a particle script"));
	toolbar->AddSeparator();
}
//-----------------------------------------------------------------------
void UICompileIcons::reset(unsigned int numberOfListItems)
{
	if (numberOfListItems > 0)
	{
		mToolbar->EnableTool(ID_COMPILE, true);
	}
	else
	{
		mToolbar->EnableTool(ID_COMPILE, false);
	}
}
//-----------------------------------------------------------------------
void UICompileIcons::disableAll(void)
{
	mToolbar->EnableTool(ID_COMPILE, false);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
UIMiscIcons::UIMiscIcons(ParticleUniverseEditorFrame* frame, wxToolBar* toolbar) : 
	mFrame(frame),
	mToolbar(toolbar),
	mWorldSpace(true),
	mCameraPerspective(true),
	mCamera45Angle(true)
{
	// Create the bitmaps of the main toolbar
	mToolbar = toolbar;

	// Gizmo's
	toolbar->AddTool(ID_GIZMO_MOVE, _("Move"), 
		wxBitmap(ICONS_DIR + wxT("gizmo_move.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Move"), _("Set the gizmo to move"));
	toolbar->AddTool(ID_GIZMO_ROTATE, _("Rotate"), 
		wxBitmap(ICONS_DIR + wxT("gizmo_rotate.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Rotate"), _("Set the gizmo to rotate"));
	toolbar->AddTool(ID_GIZMO_SCALE, _("Scale"), 
		wxBitmap(ICONS_DIR + wxT("gizmo_scale.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Scale"), _("Set the gizmo to scale"));
	toolbar->AddTool(ID_TOGGLE_GIZMO, _("Toggle gizmo"), 
		wxBitmap(ICONS_DIR + wxT("toggle_gizmo.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Toggle gizmo"), _("Enable/disable the gizmo"));
	mWorldSpaceBitmap = wxBitmap(ICONS_DIR + wxT("worldspace.png"), wxBITMAP_TYPE_PNG);
	mLocalSpaceBitmap = wxBitmap(ICONS_DIR + wxT("localspace.png"), wxBITMAP_TYPE_PNG);
	toolbar->AddTool(ID_WORLD_LOCAL_SPACE, _("Gizmo manipulation is in global (world) space"), 
		mWorldSpaceBitmap,
		wxNullBitmap, wxITEM_NORMAL,
		_("Gizmo manipulation is in global (world) space"), _("Gizmo manipulation is in global (world) space"));
	toolbar->AddSeparator();

	// Camera icons
	toolbar->AddTool(ID_CAMERA_RESET, _("Camera Reset"), 
		wxBitmap(ICONS_DIR + wxT("camera_reset.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Camera Reset"), _("Reset the camera position"));
	toolbar->AddTool(ID_CAMERA_SAVE, _("Save camera position"), 
		wxBitmap(ICONS_DIR + wxT("camera_save.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Save camera position"), _("Save the position of the camera"));
	mPerspectiveBitmap = wxBitmap(ICONS_DIR + wxT("camera_perspective.png"), wxBITMAP_TYPE_PNG);
	mOrthographicBitmap = wxBitmap(ICONS_DIR + wxT("camera_orthographic.png"), wxBITMAP_TYPE_PNG);
	toolbar->AddTool(ID_TOGGLE_CAMERA_PROJECTION, _("Set orthographic camera"), 
		mPerspectiveBitmap,
		wxNullBitmap, wxITEM_NORMAL,
		_("Set orthographic camera"), _("Set orthographic camera"));
	mOrtho45AngleBitmap = wxBitmap(ICONS_DIR + wxT("ortho_45.png"), wxBITMAP_TYPE_PNG);
	mOrtho90AngleBitmap = wxBitmap(ICONS_DIR + wxT("ortho_90.png"), wxBITMAP_TYPE_PNG);
	toolbar->AddTool(ID_TOGGLE_CAMERA_ANGLE, _("Set angle perpendicular"), 
		mOrtho45AngleBitmap,
		wxNullBitmap, wxITEM_NORMAL,
		_("Set angle perpendicular"), _("Set angle perpendicular"));
	mToolbar->EnableTool(ID_TOGGLE_CAMERA_ANGLE, false); // Set it disabled by default
	toolbar->AddTool(ID_CENTRE_OBJECT, _("Move object to centre"), 
		wxBitmap(ICONS_DIR + wxT("centre.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Move selected object to the centre of the scene"), _("Move selected object to the centre of the scene"));

	toolbar->AddSeparator();

	// Miscellanious
	toolbar->AddTool(ID_BACKGROUND_COLOUR, _("Background colour"), 
		wxBitmap(ICONS_DIR + wxT("change_colour.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Background colour"), _("Change the backgroundcolour of the renderwindow"));
	toolbar->AddTool(ID_TOGGLE_STATS, _("Toggle statistics"), 
		wxBitmap(ICONS_DIR + wxT("stats.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Toggle statistics"), _("Enable/disable the statistics overlay"));
	toolbar->AddTool(ID_TOGGLE_GRID_PLANE, _("Toggle grid plane"), 
		wxBitmap(ICONS_DIR + wxT("grid_icon.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Toggle grid plane"), _("Enable/disable the grid plane"));
	toolbar->AddTool(ID_ADD_MESH_TO_SCENE, _("Add mesh to scene"), 
		wxBitmap(ICONS_DIR + wxT("tux.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Add mesh to scene"), _("Add a mesh to the scene"));
	toolbar->AddTool(ID_REMOVE_MESH_FROM_SCENE, _("Remove mesh from scene"), 
		wxBitmap(ICONS_DIR + wxT("tux_off.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Remove mesh from scene"), _("Remove a mesh from the scene"));
	toolbar->EnableTool(ID_REMOVE_MESH_FROM_SCENE, false); // Disable this feature by default, because there is nothing to remove
	toolbar->AddTool(ID_TOGGLE_LIGHT, _("Add/remove light to/from scene"), 
		wxBitmap(ICONS_DIR + wxT("light_bulb.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Add/remove light to/from scene"), _("Add or remove a light to/from the scene"));
//	toolbar->AddTool(ID_SKELETON, _("Select a bone to attach the particle system"), 
//		wxBitmap(ICONS_DIR + wxT("skeleton.png"), wxBITMAP_TYPE_PNG),
//		wxNullBitmap, wxITEM_NORMAL,
//		_("Select a bone to attach the particle system"), _("Select a bone to attach the particle system"));


	// X, Y and Z axis: Create spincontrols (event handling is done in the SpinCtrlXYZ class)
	toolbar->AddSeparator();
	mSpinZ = new SpinCtrlXYZ (this, toolbar, wxID_SPIN_Z, wxT("0"), wxDefaultPosition, wxSize(64, wxDefaultCoord), wxTAB_TRAVERSAL, -359, 359, 0, wxT("mSpinZ"));
	mSpinY = new SpinCtrlXYZ (this, toolbar, wxID_SPIN_Y, wxT("0"), wxDefaultPosition, wxSize(64, wxDefaultCoord), wxTAB_TRAVERSAL, -359, 359, 0, wxT("mSpinY"), mSpinZ);
	mSpinX = new SpinCtrlXYZ (this, toolbar, wxID_SPIN_X, wxT("0"), wxDefaultPosition, wxSize(64, wxDefaultCoord), wxTAB_TRAVERSAL, -359, 359, 0, wxT("mSpinX"), mSpinY);
	mSpinZ->setNextSpinCtrl(mSpinX);

	// X, Y and Z axis: Use tools with images as a label with transparent background
	toolbar->AddTool(ID_X_LABEL, _(""), 
		wxBitmap(ICONS_DIR + wxT("x_label.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, 
		wxITEM_NORMAL,
		wxT(""), wxT(""));
	toolbar->EnableTool(ID_X_LABEL, false);
	toolbar->AddControl(mSpinX);

	toolbar->AddTool(ID_Y_LABEL, _(""), 
		wxBitmap(ICONS_DIR + wxT("y_label.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, 
		wxITEM_NORMAL,
		wxT(""), wxT(""));
	toolbar->EnableTool(ID_Y_LABEL, false);
	toolbar->AddControl(mSpinY);

	toolbar->AddTool(ID_Z_LABEL, _(""), 
		wxBitmap(ICONS_DIR + wxT("z_label.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, 
		wxITEM_NORMAL,
		wxT(""), wxT(""));
	toolbar->EnableTool(ID_Z_LABEL, false);
	toolbar->AddControl(mSpinZ);

	// Icon + slider
	toolbar->AddSeparator();
	toolbar->AddTool(ID_RESET_TIME, _("Reset time"), 
		wxBitmap(ICONS_DIR + wxT("time.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL,
		_("Reset time"), _("Reset the time settings"));
	toolbar->AddSeparator();

	wxControl* sliderControl = new wxControl(toolbar, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	mTimeScaleSlider = new TimeScaleSlider(frame, sliderControl, 100, 0, 200);
	toolbar->AddControl(sliderControl);
	sliderControl->Fit();
	sliderControl->Layout();
}
//-----------------------------------------------------------------------
bool UIMiscIcons::doToggleWorldLocalSpace(void)
{
	if (mWorldSpace)
	{
		// Make local space
		mToolbar->SetToolNormalBitmap(ID_WORLD_LOCAL_SPACE, mLocalSpaceBitmap);
		mToolbar->SetToolLongHelp(ID_WORLD_LOCAL_SPACE, _("Gizmo manipulation is in global (object) space"));
		mToolbar->SetToolShortHelp(ID_WORLD_LOCAL_SPACE, _("Gizmo manipulation is in local (object) space"));
		mWorldSpace = false;
	}
	else
	{
		// Make world space
		mToolbar->SetToolNormalBitmap(ID_WORLD_LOCAL_SPACE, mWorldSpaceBitmap);
		mToolbar->SetToolLongHelp(ID_WORLD_LOCAL_SPACE, _("Gizmo manipulation is in global (world) space"));
		mToolbar->SetToolShortHelp(ID_WORLD_LOCAL_SPACE, _("Gizmo manipulation is in global (world) space"));
		mWorldSpace = true;
	}
	return mWorldSpace;
}
//-----------------------------------------------------------------------
bool UIMiscIcons::doToggleCameraProjectionType(void)
{
	if (mCameraPerspective)
	{
		// Make orthographic
		mToolbar->SetToolNormalBitmap(ID_TOGGLE_CAMERA_PROJECTION, mOrthographicBitmap);
		mToolbar->SetToolLongHelp(ID_TOGGLE_CAMERA_PROJECTION, _("Set perspective camera"));
		mToolbar->SetToolShortHelp(ID_TOGGLE_CAMERA_PROJECTION, _("Set perspective camera"));
		mToolbar->EnableTool(ID_TOGGLE_CAMERA_ANGLE, true);
		mCameraPerspective = false;
	}
	else
	{
		// Make perspective
		mToolbar->SetToolNormalBitmap(ID_TOGGLE_CAMERA_PROJECTION, mPerspectiveBitmap);
		mToolbar->SetToolLongHelp(ID_TOGGLE_CAMERA_PROJECTION, _("Set orthographic camera"));
		mToolbar->SetToolShortHelp(ID_TOGGLE_CAMERA_PROJECTION, _("Set orthographic camera"));
		mToolbar->EnableTool(ID_TOGGLE_CAMERA_ANGLE, false);
		mCameraPerspective = true;
	}

	return mCameraPerspective;
}
//-----------------------------------------------------------------------
bool UIMiscIcons::doToggleCameraAngle(void)
{
	if (mCamera45Angle)
	{
		// Make 90 angle
		mToolbar->SetToolNormalBitmap(ID_TOGGLE_CAMERA_ANGLE, mOrtho90AngleBitmap);
		mToolbar->SetToolLongHelp(ID_TOGGLE_CAMERA_ANGLE, _("Set angle 45 degrees"));
		mToolbar->SetToolShortHelp(ID_TOGGLE_CAMERA_ANGLE, _("Set angle 45 degrees"));
		mCamera45Angle = false;
	}
	else
	{
		// Make 45 angle
		mToolbar->SetToolNormalBitmap(ID_TOGGLE_CAMERA_ANGLE, mOrtho45AngleBitmap);
		mToolbar->SetToolLongHelp(ID_TOGGLE_CAMERA_ANGLE, _("Set angle perpendicular"));
		mToolbar->SetToolShortHelp(ID_TOGGLE_CAMERA_ANGLE, _("Set angle perpendicular"));
		mCamera45Angle = true;
	}

	return mCamera45Angle;
}
//-----------------------------------------------------------------------
bool UIMiscIcons::isCameraPerspective(void) const
{
	return mCameraPerspective;
}
//-----------------------------------------------------------------------
bool UIMiscIcons::isCamera45Angle(void) const
{
	return mCamera45Angle;
}
//-----------------------------------------------------------------------
void UIMiscIcons::resetTimeScale(void)
{
	if (mTimeScaleSlider)
	{
		mTimeScaleSlider->resetTimeScale();
	}
}
//-----------------------------------------------------------------------
void UIMiscIcons::reset(void)
{
	enable(true);
}
//-----------------------------------------------------------------------
void UIMiscIcons::disableAll(void)
{
	enable(false);
}
//-----------------------------------------------------------------------
void UIMiscIcons::enable(bool enabled)
{
	mToolbar->EnableTool(ID_GIZMO_MOVE, enabled);
	mToolbar->EnableTool(ID_GIZMO_ROTATE, enabled);
	mToolbar->EnableTool(ID_GIZMO_SCALE, enabled);
	mToolbar->EnableTool(ID_TOGGLE_GIZMO, enabled);
	mToolbar->EnableTool(ID_WORLD_LOCAL_SPACE, enabled);
	mToolbar->EnableTool(ID_CAMERA_RESET, enabled);
	mToolbar->EnableTool(ID_CAMERA_SAVE, enabled);
	mToolbar->EnableTool(ID_TOGGLE_CAMERA_PROJECTION, enabled);
	
	if (!mCameraPerspective && enabled)
	{
		mToolbar->EnableTool(ID_TOGGLE_CAMERA_ANGLE, true);
	}
	else
	{
		mToolbar->EnableTool(ID_TOGGLE_CAMERA_ANGLE, false);
	}
	
	mToolbar->EnableTool(ID_CENTRE_OBJECT, enabled);
	mToolbar->EnableTool(ID_BACKGROUND_COLOUR, enabled);
	mToolbar->EnableTool(ID_TOGGLE_STATS, enabled);
	mToolbar->EnableTool(ID_TOGGLE_GRID_PLANE, enabled);
	mToolbar->EnableTool(ID_ADD_MESH_TO_SCENE, enabled);
	mToolbar->EnableTool(ID_REMOVE_MESH_FROM_SCENE, enabled);
	mToolbar->EnableTool(ID_TOGGLE_LIGHT, enabled);
	//mToolbar->EnableTool(ID_SKELETON, enabled);
	mToolbar->EnableTool(ID_X_LABEL, enabled);
	mToolbar->EnableTool(ID_Y_LABEL, enabled);
	mToolbar->EnableTool(ID_Z_LABEL, enabled);
	mSpinX->Enable(enabled);
	mSpinY->Enable(enabled);
	mSpinZ->Enable(enabled);
}
//-----------------------------------------------------------------------
void UIMiscIcons::enableRemoveEntity(bool enable)
{
	mToolbar->EnableTool(ID_REMOVE_MESH_FROM_SCENE, enable);
}
//-----------------------------------------------------------------------
void UIMiscIcons::enableOrthoCameraSelected(bool enabled)
{
	mToolbar->EnableTool(ID_CAMERA_RESET, enabled);
	mToolbar->EnableTool(ID_CAMERA_SAVE, enabled);
}
//-----------------------------------------------------------------------
void UIMiscIcons::callbackSpinCtrlXYZ(SpinCtrlXYZ* spinCtrl)
{
	if (spinCtrl == mSpinX)
	{
		mFrame->callbackSpinCtrlXYZ(mSpinX->GetValue(), mSpinY->GetValue(), mSpinZ->GetValue(), "XYZ");
	}
	else if (spinCtrl == mSpinY)
	{
		mFrame->callbackSpinCtrlXYZ(mSpinX->GetValue(), mSpinY->GetValue(), mSpinZ->GetValue(), "YXZ");
	}
	else if (spinCtrl == mSpinZ)
	{
		mFrame->callbackSpinCtrlXYZ(mSpinX->GetValue(), mSpinY->GetValue(), mSpinZ->GetValue(), "ZXY");
	}
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TimeScaleSlider::TimeScaleSlider(ParticleUniverseEditorFrame* frame, wxControl* control, int value , int minValue, int maxValue) : 
	wxSlider(control, wxID_ANY, value, minValue, maxValue)
{
	mFrame = frame;
	Connect(wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(TimeScaleSlider::OnSliderUpdate));
}
//-----------------------------------------------------------------------
ParticleUniverse::Real TimeScaleSlider::getTimeScale(void) const
{
	return static_cast<ParticleUniverse::Real>(0.01 * GetValue());
}
//-----------------------------------------------------------------------
void TimeScaleSlider::setTimeScale(ParticleUniverse::Real scale)
{
	int scaleInt = static_cast<int>(100 * scale);
	SetValue(scaleInt);
}
//-----------------------------------------------------------------------
void TimeScaleSlider::resetTimeScale(void)
{
	setTimeScale(1.0);
	mFrame->notifyTimeRescaled(1.0);
}
//-----------------------------------------------------------------------
void TimeScaleSlider::OnSliderUpdate(wxCommandEvent& event)
{
	int scaleInt = GetValue();
	mFrame->notifyTimeRescaled(0.01 * (static_cast<ParticleUniverse::Real>(scaleInt)));
}
