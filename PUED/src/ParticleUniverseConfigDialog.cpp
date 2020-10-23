/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseEditor.h"
#include "ParticleUniverseConfigDialog.h"
#include "ParticleUniverseConfigFile.h"
#include "ParticleUniverseMediaTypeComboBox.h"
#include "wx/notebook.h"
#include "wx/checkbox.h"

static const Ogre::String CONFIG_FILENAME = "pued.cfg";

//-----------------------------------------------------------------------
ConfigDialog::ConfigDialog(wxWindow* parent) : 
	wxPropertySheetDialog(),
	mCheckAutoStart(0),
	mHighlight(0),
	mStatistics(0),
	mLightCheck(0),
	mGridPlane(0),
	mGridPlaneDistance(0),
	mGridPlaneScale(0),
	mVideoPath(0),
	mImageWidth(0),
	mImageHeight(0),
	mFramesPerSecond(0),
	mFileNameSuffix(0),
	mPauseTime(0),
	mFilter(0),
	mVideoSize(0),
	mEditProportion(0.5f),
	mMainNodePosition(Ogre::Vector3(0, 0, -1000)),
	mMainNodeOrientation(Ogre::Quaternion::IDENTITY),
	mVideoSelected(false),
	mDirListSizer(0),
	mDirListPanel(0),
	mCheckboxes(0),
	mTexts(0),
	mPhysXPlaneNormalX(0),
	mPhysXPlaneNormalY(0),
	mPhysXPlaneNormalZ(0),
	mPhysXPlaneNormal(Ogre::Vector3::UNIT_Y),
	mPhysXPlaneDistance(0),
	mPhysXGravity(Ogre::Vector3::ZERO),
	mCheckPhysXDialog(0),
	mPhysXGravityX(0),
	mPhysXGravityY(0),
	mPhysXGravityZ(0),
	mUsePhysXIfNoStartupDialog(false),
	mLanguage(wxLANGUAGE_DEFAULT),
	mCameraPosition(Ogre::Vector3::ZERO)
{
	mParentFrame = static_cast<ParticleUniverseEditorFrame*>(parent);
}
//-----------------------------------------------------------------------
void ConfigDialog::initWindow(void)
{
	mCameraPosition = Ogre::Vector3(-1100, 0, -1100);
	wxSize size = wxSize(0.7 * APP_WIDTH, 0.7 * APP_HEIGHT);
	SetSheetStyle(wxPROPSHEET_NOTEBOOK);
	Create(mParentFrame, wxID_ANY, _("Configuration"), wxDefaultPosition, size, wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP);
	CreateButtons(wxOK | wxCANCEL);
	wxBookCtrlBase* notebook = GetBookCtrl();
	notebook->SetSize(size);
	notebook->SetImageList(0);
	wxPanel* generalPanel = createGeneralPanel(notebook);
	wxPanel* directoriesPanel = createDirectoriesPanel(notebook);
	wxPanel* recordPanel = createRecordPanel(notebook);
	notebook->AddPage(generalPanel, _("General"), true, 0);
	notebook->AddPage(directoriesPanel, _("Directories"), false, 1);
	notebook->AddPage(recordPanel, _("Record"), false, 2);
#ifdef PU_PHYSICS_PHYSX
	wxPanel* physXPanel = createPhysXPanel(notebook);
	notebook->AddPage(physXPanel, _("PhysX"), false, 3);
#endif //PU_PHYSICS_PHYSX
	LayoutDialog();
	SetSize(size);
	Center();
}
//-----------------------------------------------------------------------
void ConfigDialog::loadLanguage(void)
{
	ConfigFile cfg;
	cfg.load(CONFIG_FILENAME);

	// Load language (no widget for language)
	wxString language = ogre2wx(cfg.getSetting("language"));
	wxLocale locale;
	const wxLanguageInfo* info = locale.FindLanguageInfo(language);
	mLanguage = (wxLanguage)info->Language; // Nasty cast
}
//-----------------------------------------------------------------------
wxString ConfigDialog::getLanguageAsString(const wxLanguage& language)
{
	wxLocale locale;
	const wxLanguageInfo* info = locale.GetLanguageInfo(language);
	return info->CanonicalName;
}
//-----------------------------------------------------------------------
wxPanel* ConfigDialog::createGeneralPanel(wxWindow* parent)
{
	// Fill in panel
	wxPanel* panel = new wxPanel(parent, wxID_ANY);
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* item = new wxBoxSizer(wxVERTICAL);

    // Autostart
    wxBoxSizer* itemSizer = new wxBoxSizer(wxHORIZONTAL);
    mCheckAutoStart = new wxCheckBox(panel, wxID_ANY, _("Auto rendering on/off"), wxDefaultPosition, wxDefaultSize);
    itemSizer->Add(mCheckAutoStart, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item->Add(itemSizer, 0, wxGROW|wxALL, 0);
	
    // Highlight keywords
    wxBoxSizer* highlightSizer = new wxBoxSizer(wxHORIZONTAL);
    mHighlight = new wxCheckBox(panel, wxID_ANY, _("Highlight keywords on/off"), wxDefaultPosition, wxDefaultSize);
    highlightSizer->Add(mHighlight, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item->Add(highlightSizer, 0, wxGROW|wxALL, 0);

    // Statistics
    wxBoxSizer* statisticsSizer = new wxBoxSizer(wxHORIZONTAL);
    mStatistics = new wxCheckBox(panel, wxID_ANY, _("Statistics overlay on/off"), wxDefaultPosition, wxDefaultSize);
    statisticsSizer->Add(mStatistics, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item->Add(statisticsSizer, 0, wxGROW|wxALL, 0);

	// Light
    wxBoxSizer* lightSizer = new wxBoxSizer(wxHORIZONTAL);
    mLightCheck = new wxCheckBox(panel, wxID_ANY, _("Point light on/off"), wxDefaultPosition, wxDefaultSize);
    lightSizer->Add(mLightCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item->Add(lightSizer, 0, wxGROW|wxALL, 0);

	// Gridplane
    wxBoxSizer* gridSizer = new wxBoxSizer(wxHORIZONTAL);
    mGridPlane = new wxCheckBox(panel, wxID_ANY, _("Gridplane on/off"), wxDefaultPosition, wxDefaultSize);
    gridSizer->Add(mGridPlane, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item->Add(gridSizer, 0, wxGROW|wxALL, 0);

	// Gridplane distance
	wxBoxSizer* distanceSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* txtDistance = new wxStaticText(panel, -1, _("Gridplane distance"));
	mGridPlaneDistance = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, 24));
	mGridPlaneDistance->SetRange(-99999, 99999);
	mGridPlaneDistance->SetValue(-200);
	distanceSizer->Add(txtDistance, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	distanceSizer->Add(mGridPlaneDistance, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	item->Add(distanceSizer, 0, wxGROW|wxALL, 0);

	// Gridplane scale
	wxBoxSizer* gridPlaneScaleSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* txtScale = new wxStaticText(panel, -1, _("Gridplane scale"));
	mGridPlaneScale = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, 24));
	gridPlaneScaleSizer->Add(txtScale, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	gridPlaneScaleSizer->Add(mGridPlaneScale, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	item->Add(gridPlaneScaleSizer, 0, wxGROW|wxALL, 0);

	topSizer->Add(item, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5);
	panel->SetSizer(topSizer);
	topSizer->Fit(panel);
	return panel;
}
//-----------------------------------------------------------------------
wxPanel* ConfigDialog::createDirectoriesPanel(wxWindow* parent)
{
	// Fill in panel
	mDirListPanel = new wxPanel(parent, wxID_ANY);
	mDirListSizer = new wxBoxSizer(wxVERTICAL);
	rebuildDirectoriesPanel();
	mDirListPanel->SetSizer(mDirListSizer);
	mDirListSizer->Fit(mDirListPanel);
	return mDirListPanel;
}
//-----------------------------------------------------------------------
void ConfigDialog::rebuildDirectoriesPanel(void)
{
	wxBoxSizer* item;
    wxBoxSizer* itemSizer;
	mDirListSizer->Clear(true);

	// Resource directories
	mExcludeDirlist.clear();
	mCheckboxes.clear();
	mTexts.clear();

	Ogre::StringVector::iterator it;
	Ogre::StringVector::iterator itEnd = mDirlist.end();
	for (it = mDirlist.begin(); it != itEnd; ++it)
	{
		itemSizer = new wxBoxSizer(wxHORIZONTAL);
		item = new wxBoxSizer(wxVERTICAL);
		wxCheckBox* checkbox = new wxCheckBox(mDirListPanel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
		checkbox->SetValue(true);
		mCheckboxes.push_back(checkbox);
		wxString dir = ogre2wx(*it);
		wxTextCtrl* text = new wxTextCtrl(mDirListPanel, wxID_ANY, dir, wxDefaultPosition, wxSize(0.4 * APP_WIDTH, 24));
		mTexts.push_back(text);
		itemSizer->Add(checkbox, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
		itemSizer->Add(text, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
		item->Add(itemSizer, 0, wxGROW|wxALL, 0);
		mDirListSizer->Add(item, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5);
	}
	mDirListSizer->Layout();
	mDirListSizer->Fit(mDirListPanel);
}
//-----------------------------------------------------------------------
wxPanel* ConfigDialog::createRecordPanel(wxWindow* parent)
{
	// Fill in panel
	wxPanel* panel = new wxPanel(parent, wxID_ANY);
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* item = new wxBoxSizer(wxVERTICAL);

    // Output directory
    wxBoxSizer* pathSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* txtOutputDir = new wxStaticText(panel, -1, _("Output directory"));
	mVideoPath = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(0.4 * APP_WIDTH, 24));
	pathSizer->Add(txtOutputDir, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    pathSizer->Add(mVideoPath, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	item->Add(pathSizer, 0, wxGROW|wxALL, 0);

	// Size
	wxBoxSizer* sizeSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* txtSize = new wxStaticText(panel, -1, _("Image size"));
	mImageWidth = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, 24));
	mImageWidth->SetRange(8, 4096);
	mImageWidth->SetValue(1024);
	mImageHeight = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, 24));
	mImageHeight->SetRange(8, 4096);
	mImageHeight->SetValue(768);
	sizeSizer->Add(txtSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	sizeSizer->Add(mImageWidth, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	sizeSizer->Add(mImageHeight, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	item->Add(sizeSizer, 0, wxGROW|wxALL, 0);
	
	// Type
	wxBoxSizer* typeSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* txtType = new wxStaticText(panel, -1, _("Media type"));
	mFileNameSuffix = new MediaTypeComboBox(panel, this, -1); // Use the derived one
	mVideoSize = new wxComboBox(panel, -1, wxT(""));
	mFileNameSuffix->Append(_(".png"));
	mFileNameSuffix->Append(_(".jpg"));
	mFileNameSuffix->Append(_(".tiff"));
	mFileNameSuffix->Append(_(".avi"));
	mFileNameSuffix->Append(_(".swf"));
	mFileNameSuffix->Append(_(".wmv"));
#ifdef PU_USE_MP4
	mFileNameSuffix->Append(_(".mp4"));
#endif
	mVideoSize->Append(_("352x240"));
	mVideoSize->Append(_("352x288"));
	mVideoSize->Append(_("720x480"));
	mVideoSize->Append(_("720x576"));
	mVideoSize->Append(_("1024x768"));
	mVideoSize->Append(_("1280x720"));
	mVideoSize->Append(_("1440x1080"));
	mVideoSize->Append(_("1920x1080"));
	typeSizer->Add(txtType, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	typeSizer->Add(mFileNameSuffix, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	typeSizer->Add(mVideoSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	item->Add(typeSizer, 0, wxGROW|wxALL, 0);

	// Frames per second
	wxBoxSizer* fpsSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* txtFPS = new wxStaticText(panel, -1, _("Frames per second"));
	mFramesPerSecond = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, 24));
	mFramesPerSecond->SetRange(1, 256);
	mFramesPerSecond->SetValue(27);
	fpsSizer->Add(txtFPS, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	fpsSizer->Add(mFramesPerSecond, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	item->Add(fpsSizer, 0, wxGROW|wxALL, 0);

	// Filter
	wxBoxSizer* filterSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* txtFilter = new wxStaticText(panel, -1, _("Filter"));
	mFilter = new wxComboBox(panel, -1, wxT(""));
	mFilter->Append(_("No filtering")); // Use this order. No filtering = 1
	mFilter->Append(_("Alpha from luminance")); // Use this order. Alpha from luminance = 2
	mFilter->Append(_("Alpha from background colour")); // Use this order. Alpha from background colour = 3
	filterSizer->Add(txtFilter, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	filterSizer->Add(mFilter, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	item->Add(filterSizer, 0, wxGROW|wxALL, 0);

	// Pause time
	wxBoxSizer* pauseSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* txtPause = new wxStaticText(panel, -1, _("Calibration pause"));
	mPauseTime = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, 24));
	pauseSizer->Add(txtPause, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	pauseSizer->Add(mPauseTime, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	item->Add(pauseSizer, 0, wxGROW|wxALL, 0);

	// Set in panel
	topSizer->Add(item, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5);
	panel->SetSizer(topSizer);
	topSizer->Fit(panel);
	return panel;
}
//-----------------------------------------------------------------------
#ifdef PU_PHYSICS_PHYSX
wxPanel* ConfigDialog::createPhysXPanel(wxWindow* parent)
{
	// Fill in panel
	wxPanel* panel = new wxPanel(parent, wxID_ANY);
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* item = new wxBoxSizer(wxVERTICAL);

	// Display dialog at startup
	mCheckPhysXDialog = new wxCheckBox(panel, wxID_ANY, _("Display PhysX detection dialog at startup? (Restart the application to apply the settings!)"));
	mCheckPhysXDialog->SetValue(false);
	item->Add(mCheckPhysXDialog, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	item->AddSpacer(16);

	// Plane normal
	wxBoxSizer* normalSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* txtNormal = new wxStaticText(panel, -1, _("Plane normal"));
	mPhysXPlaneNormalX = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, 24));
	mPhysXPlaneNormalX->SetValue(ogre2wx("0.0"));
	mPhysXPlaneNormalY = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, 24));
	mPhysXPlaneNormalY->SetValue(ogre2wx("0.0"));
	mPhysXPlaneNormalZ = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, 24));
	mPhysXPlaneNormalZ->SetValue(ogre2wx("0.0"));
	normalSizer->Add(txtNormal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	normalSizer->Add(mPhysXPlaneNormalX, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	normalSizer->Add(mPhysXPlaneNormalY, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	normalSizer->Add(mPhysXPlaneNormalZ, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	item->Add(normalSizer, 0, wxGROW|wxALL, 0);

	// Plane distance
	wxBoxSizer* distanceSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* txtDistance = new wxStaticText(panel, -1, _("Plane distance"));
	mPhysXPlaneDistance = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, 24));
	mPhysXPlaneDistance->SetRange(-99999, 99999);
	mPhysXPlaneDistance->SetValue(-200);
	distanceSizer->Add(txtDistance, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	distanceSizer->Add(mPhysXPlaneDistance, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	item->Add(distanceSizer, 0, wxGROW|wxALL, 0);

	// Gravity
	wxBoxSizer* gravitySizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* txtGravity = new wxStaticText(panel, -1, _("Gravity"));
	mPhysXGravityX = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, 24));
	mPhysXGravityX->SetValue(ogre2wx("0.0"));
	mPhysXGravityY = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, 24));
	mPhysXGravityY->SetValue(ogre2wx("0.0"));
	mPhysXGravityZ = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(80, 24));
	mPhysXGravityZ->SetValue(ogre2wx("0.0"));
	gravitySizer->Add(txtGravity, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	gravitySizer->Add(mPhysXGravityX, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	gravitySizer->Add(mPhysXGravityY, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	gravitySizer->Add(mPhysXGravityZ, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	item->Add(gravitySizer, 0, wxGROW|wxALL, 0);

	// Set in panel
	topSizer->Add(item, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5);
	panel->SetSizer(topSizer);
	topSizer->Fit(panel);
	return panel;
}
#endif //PU_PHYSICS_PHYSX
//-----------------------------------------------------------------------
bool ConfigDialog::isAutoStart(void) const
{
	if (mCheckAutoStart)
	{
		return mCheckAutoStart->GetValue();
	}

	return false;
}
//-----------------------------------------------------------------------
void ConfigDialog::setAutoStart(bool autoStart)
{
	if (mCheckAutoStart)
	{
		mCheckAutoStart->SetValue(autoStart);
	}
}
//-----------------------------------------------------------------------
bool ConfigDialog::isHighlight(void) const
{
	if (mHighlight)
	{
		return mHighlight->GetValue();
	}

	return false;
}
//-----------------------------------------------------------------------
void ConfigDialog::setHighlight(bool highlight)
{
	if (mHighlight)
	{
		mHighlight->SetValue(highlight);
	}
}
//-----------------------------------------------------------------------
bool ConfigDialog::isStatistics(void) const
{
	if (mStatistics)
	{
		return mStatistics->GetValue();
	}

	return false;
}
//-----------------------------------------------------------------------
void ConfigDialog::setStatistics(bool statistics)
{
	if (mStatistics)
	{
		mStatistics->SetValue(statistics);
	}
}
//-----------------------------------------------------------------------
bool ConfigDialog::isLightCheck(void) const
{
	if (mLightCheck)
	{
		return mLightCheck->GetValue();
	}

	return false;
}
//-----------------------------------------------------------------------
void ConfigDialog::setLightCheck(bool lightCheck)
{
	if (mLightCheck)
	{
		mLightCheck->SetValue(lightCheck);
	}
}
//-----------------------------------------------------------------------
bool ConfigDialog::isGridPlane(void) const
{
	if (mGridPlane)
	{
		return mGridPlane->GetValue();
	}

	return false;
}
//-----------------------------------------------------------------------
void ConfigDialog::setGridPlane(bool gridplane)
{
	if (mGridPlane)
	{
		mGridPlane->SetValue(gridplane);
	}
}
//-----------------------------------------------------------------------
ParticleUniverse::Real ConfigDialog::getGridPlaneDistance(void) const
{
	return static_cast<ParticleUniverse::Real>(mGridPlaneDistance->GetValue());
}
//-----------------------------------------------------------------------
void ConfigDialog::setGridPlaneDistance(ParticleUniverse::Real distance)
{
	mGridPlaneDistance->SetValue(distance);
}
//-----------------------------------------------------------------------
ParticleUniverse::Real ConfigDialog::getGridPlaneScale(void) const
{
	Ogre::String strVal = wx2ogre(mGridPlaneScale->GetValue());
	return Ogre::StringConverter::parseReal(strVal);
}
//-----------------------------------------------------------------------
void ConfigDialog::setGridPlaneScale(ParticleUniverse::Real scale)
{
	Ogre::String str = Ogre::StringConverter::toString(scale);
	mGridPlaneScale->SetValue(ogre2wx(str));
}
//-----------------------------------------------------------------------
const wxLanguage ConfigDialog::getLanguage(void) const
{
	return mLanguage;
}
//-----------------------------------------------------------------------
wxString ConfigDialog::getVideoPath(void) const
{
	return mVideoPath->GetValue();
}
//-----------------------------------------------------------------------
void ConfigDialog::setVideoPath(wxString& videoPath)
{
	mVideoPath->SetValue(videoPath);
}
//-----------------------------------------------------------------------
ParticleUniverse::uint ConfigDialog::getImageWidth(void) const
{
	return mImageWidth->GetValue();
}
//-----------------------------------------------------------------------
void ConfigDialog::setImageWidth(ParticleUniverse::uint imageWidth)
{
	mImageWidth->SetValue(imageWidth);
}
//-----------------------------------------------------------------------
ParticleUniverse::uint ConfigDialog::getImageHeight(void) const
{
	return mImageHeight->GetValue();
}
//-----------------------------------------------------------------------
void ConfigDialog::setImageHeight(ParticleUniverse::uint imageHeight)
{
	mImageHeight->SetValue(imageHeight);
}
//-----------------------------------------------------------------------
wxString ConfigDialog::getFileNameSuffix(void) const
{
	return mFileNameSuffix->GetValue();
}
//-----------------------------------------------------------------------
void ConfigDialog::setFileNameSuffix(wxString& suffix)
{
	mFileNameSuffix->SetValue(suffix);
}
//-----------------------------------------------------------------------
ParticleUniverse::uint ConfigDialog::getFPS(void) const
{
	return mFramesPerSecond->GetValue();
}
//-----------------------------------------------------------------------
void ConfigDialog::setFPS(ParticleUniverse::uint fps)
{
	mFramesPerSecond->SetValue(fps);
}
//-----------------------------------------------------------------------
ParticleUniverse::Real ConfigDialog::getPauseTime(void) const
{
	Ogre::String strVal = wx2ogre(mPauseTime->GetValue());
	return Ogre::StringConverter::parseReal(strVal);
}
//-----------------------------------------------------------------------
void ConfigDialog::setPauseTime(ParticleUniverse::Real pauseTime)
{
	Ogre::String str = Ogre::StringConverter::toString(pauseTime);
	mPauseTime->SetValue(ogre2wx(str));
}
//-----------------------------------------------------------------------
bool ConfigDialog::isVideoSelected(void) const
{
	return mVideoSelected;
}
//-----------------------------------------------------------------------
wxString ConfigDialog::getVideoSize(bool propagate)
{
	if (propagate)
	{
		_propagateVideoSize();
	}
	return mVideoSize->GetValue();
}
//-----------------------------------------------------------------------
void ConfigDialog::setVideoSize(wxString& videoSize, bool propagate)
{
	if (propagate)
	{
		_propagateVideoSize();
	}
	mVideoSize->SetValue(videoSize);
}
//-----------------------------------------------------------------------
const Ogre::Vector3& ConfigDialog::getCameraPosition(void) const
{
	return mCameraPosition;
}
//-----------------------------------------------------------------------
void ConfigDialog::setCameraPosition(const Ogre::Vector3& position)
{
	mCameraPosition = position;
}
//-----------------------------------------------------------------------
void ConfigDialog::_propagateVideoSize(void)
{
	wxString videoSize = mVideoSize->GetValue();
	if (videoSize == wxT(""))
	{
		mVideoSize->SetValue(_("720x576"));
		videoSize = mVideoSize->GetValue();
	}

	if (videoSize == _("352x288"))
	{
		setImageWidth(352);
		setImageHeight(288);
	}
	else if (videoSize == _("352x240"))
	{
		setImageWidth(352);
		setImageHeight(240);
	}
	else if (videoSize == _("720x480"))
	{
		setImageWidth(720);
		setImageHeight(440);
	}
	else if (videoSize == _("720x576"))
	{
		setImageWidth(720);
		setImageHeight(576);
	}
	else if (videoSize == _("1024x768"))
	{
		setImageWidth(1024);
		setImageHeight(768);
	}
	else if (videoSize == _("1280x720"))
	{
		setImageWidth(1280);
		setImageHeight(720);
	}
	else if (videoSize == _("1440x1080"))
	{
		setImageWidth(1440);
		setImageHeight(1080);
	}
	else if (videoSize == _("1920x1080"))
	{
		setImageWidth(1920);
		setImageHeight(1080);
	}
}
//-----------------------------------------------------------------------
ParticleUniverse::Real ConfigDialog::getEditProportion(void)
{
	return mEditProportion;
}
//-----------------------------------------------------------------------
void ConfigDialog::setEditProportion(ParticleUniverse::Real editProportion)
{
	mEditProportion = editProportion;
}
//-----------------------------------------------------------------------
const Recorder::ImageFilter ConfigDialog::getFilter(void) const
{
	int test = mFilter->GetCurrentSelection();
	if (mFilter->GetCurrentSelection() == 0)
	{
		return Recorder::IF_NONE;
	}
	else if (mFilter->GetCurrentSelection() == 1)
	{
		return Recorder::IF_ALPHA_FROM_LUMINANCE;
	}
	else if (mFilter->GetCurrentSelection() == 2)
	{
		return Recorder::IF_ALPHA_FROM_BACKGROUND_COLOUR;
	}

	return Recorder::IF_NONE;
}
//-----------------------------------------------------------------------
wxString ConfigDialog::getFilterAsString(void) const
{
	return mFilter->GetValue();
}
//-----------------------------------------------------------------------
void ConfigDialog::setFilter(const Recorder::ImageFilter& filter)
{
	switch(filter)
	{
		case Recorder::IF_NONE:
			mFilter->SetValue(_("No filtering"));
		break;
		case Recorder::IF_ALPHA_FROM_LUMINANCE:
			mFilter->SetValue(_("Alpha from luminance"));
		break;
		case Recorder::IF_ALPHA_FROM_BACKGROUND_COLOUR:
			mFilter->SetValue(_("Alpha from background colour"));
		break;
	}
}
//-----------------------------------------------------------------------
void ConfigDialog::setFilter(const wxString& filter)
{
	mFilter->SetValue(filter);
}
//-----------------------------------------------------------------------
Ogre::Vector3& ConfigDialog::getPhysXPlaneNormal(void)
{
	Ogre::String strVal = wx2ogre(mPhysXPlaneNormalX->GetValue());
	mPhysXPlaneNormal.x = Ogre::StringConverter::parseReal(strVal);
	strVal = wx2ogre(mPhysXPlaneNormalY->GetValue());
	mPhysXPlaneNormal.y = Ogre::StringConverter::parseReal(strVal);
	strVal = wx2ogre(mPhysXPlaneNormalZ->GetValue());
	mPhysXPlaneNormal.z = Ogre::StringConverter::parseReal(strVal);
	return mPhysXPlaneNormal;
}
//-----------------------------------------------------------------------
void ConfigDialog::setPhysXPlaneNormal(Ogre::Vector3& normal)
{
	mPhysXPlaneNormal = normal;
	Ogre::String str = Ogre::StringConverter::toString(mPhysXPlaneNormal.x);
	mPhysXPlaneNormalX->SetValue(ogre2wx(str));
	str = Ogre::StringConverter::toString(mPhysXPlaneNormal.y);
	mPhysXPlaneNormalY->SetValue(ogre2wx(str));
	str = Ogre::StringConverter::toString(mPhysXPlaneNormal.z);
	mPhysXPlaneNormalZ->SetValue(ogre2wx(str));
}
//-----------------------------------------------------------------------
ParticleUniverse::Real ConfigDialog::getPhysXPlaneDistance(void) const
{
	return static_cast<ParticleUniverse::Real>(mPhysXPlaneDistance->GetValue());
}
//-----------------------------------------------------------------------
void ConfigDialog::setPhysXPlaneDistance(ParticleUniverse::Real distance)
{
	mPhysXPlaneDistance->SetValue(distance);
}
//-----------------------------------------------------------------------
bool ConfigDialog::isDisplayPhysXStartupDialog(void) const
{
	return mCheckPhysXDialog->GetValue();
}
//-----------------------------------------------------------------------
void ConfigDialog::setDisplayPhysXStartupDialog(bool displayPhysXStartupDialog)
{
	mCheckPhysXDialog->SetValue(displayPhysXStartupDialog);
}
//-----------------------------------------------------------------------
bool ConfigDialog::isUsePhysXIfNoStartupDialog(void) const
{
	return mUsePhysXIfNoStartupDialog;
}
//-----------------------------------------------------------------------
void ConfigDialog::setUsePhysXIfNoStartupDialog(bool usePhysXIfNoStartupDialog)
{
	mUsePhysXIfNoStartupDialog = usePhysXIfNoStartupDialog;
}
//-----------------------------------------------------------------------
Ogre::Vector3& ConfigDialog::getPhysXGravity(void)
{
	Ogre::String strVal = wx2ogre(mPhysXGravityX->GetValue());
	mPhysXGravity.x = Ogre::StringConverter::parseReal(strVal);
	strVal = wx2ogre(mPhysXGravityY->GetValue());
	mPhysXGravity.y = Ogre::StringConverter::parseReal(strVal);
	strVal = wx2ogre(mPhysXGravityZ->GetValue());
	mPhysXGravity.z = Ogre::StringConverter::parseReal(strVal);
	return mPhysXGravity;
}
//-----------------------------------------------------------------------
void ConfigDialog::setPhysXGravity(Ogre::Vector3& gravity)
{
	mPhysXGravity = gravity;
	Ogre::String str = Ogre::StringConverter::toString(mPhysXGravity.x);
	mPhysXGravityX->SetValue(ogre2wx(str));
	str = Ogre::StringConverter::toString(mPhysXGravity.y);
	mPhysXGravityY->SetValue(ogre2wx(str));
	str = Ogre::StringConverter::toString(mPhysXGravity.z);
	mPhysXGravityZ->SetValue(ogre2wx(str));
}
//-----------------------------------------------------------------------
const Ogre::Vector3& ConfigDialog::getMainNodePosition(void) const
{
	return mMainNodePosition;
}
//-----------------------------------------------------------------------
void ConfigDialog::setMainNodePosition(const Ogre::Vector3& position)
{
	mMainNodePosition = position;
}
//-----------------------------------------------------------------------
const Ogre::Quaternion& ConfigDialog::getMainNodeOrientation(void) const
{
	return mMainNodeOrientation;
}
//-----------------------------------------------------------------------
void ConfigDialog::setMainNodeOrientation(const Ogre::Quaternion& orientation)
{
	mMainNodeOrientation = orientation;
}
//-----------------------------------------------------------------------
void ConfigDialog::addToDirList(const Ogre::String& dir)
{
	// Add to mDirlist and remove duplicates
	mDirlist.push_back(dir);
	Ogre::StringVector::iterator r , w;
	std::set<Ogre::String> tmpset;
	for(r = mDirlist.begin() , w = mDirlist.begin() ; r != mDirlist.end() ; ++r)
	{
		if(tmpset.insert(*r).second)
		{
			*w++ = *r;
		}
	}
	mDirlist.erase(w , mDirlist.end());
}
//-----------------------------------------------------------------------
const Ogre::StringVector& ConfigDialog::getResourceLocations(void)
{
	getExcludeDirList(); // For force removal of the ones not used.
	return mDirlist;
}
//-----------------------------------------------------------------------
const Ogre::StringVector ConfigDialog::getExcludeDirList(void)
{
	std::vector<wxCheckBox*>::iterator it;
	std::vector<wxCheckBox*>::iterator itEnd = mCheckboxes.end();
	std::vector<wxTextCtrl*>::iterator itTexts = mTexts.begin();
	for (it = mCheckboxes.begin(); it != itEnd; ++it)
	{
		if (!(*it)->GetValue())
		{
			Ogre::String dir = wx2ogre((*itTexts)->GetValue());
			mExcludeDirlist.push_back(dir);
			_removeFromDirList(dir);
		}
		itTexts++;
	}


	return mExcludeDirlist;
}
//-----------------------------------------------------------------------
void ConfigDialog::_removeFromDirList(const Ogre::String& dir)
{
	if (mDirlist.empty())
		return;

	Ogre::StringVector::iterator it;
	Ogre::StringVector::iterator itEnd = mDirlist.end();
	for (it = mDirlist.begin(); it != itEnd; ++it)
	{
		if (*it == dir)
		{
			mDirlist.erase(it);
			return;
		}
	}
}
//-----------------------------------------------------------------------
void ConfigDialog::loadConfig(void)
{
	ConfigFile cfg;
	cfg.load(CONFIG_FILENAME);

	// Load autostart
	setAutoStart(Ogre::StringConverter::parseBool(cfg.getSetting("autostart")));

	// Load highlight
	setHighlight(Ogre::StringConverter::parseBool(cfg.getSetting("highlight")));

	// Load statistics
	setStatistics(Ogre::StringConverter::parseBool(cfg.getSetting("statistics")));

	// Load light
	setLightCheck(Ogre::StringConverter::parseBool(cfg.getSetting("light")));

	// Load gridplane
	setGridPlane(Ogre::StringConverter::parseBool(cfg.getSetting("gridplane")));

	// Load gridplane distance
	ParticleUniverse::Real gridplaneDistance = Ogre::StringConverter::parseReal(cfg.getSetting("gridplane_distance"));
	if (gridplaneDistance == 0.0f)
	{
		gridplaneDistance = -200.0f;
	}
	setGridPlaneDistance(gridplaneDistance);

	// Load gridplane scale
	ParticleUniverse::Real gridplaneScale = Ogre::StringConverter::parseReal(cfg.getSetting("gridplane_scale"));
	if (gridplaneScale == 0.0f)
	{
		gridplaneScale = 0.25f;
	}
	setGridPlaneScale(gridplaneScale);

	// Load position and orientation
	//mMainNodePosition = Ogre::StringConverter::parseVector3(cfg.getSetting("position"));
	//mMainNodeOrientation = Ogre::StringConverter::parseQuaternion(cfg.getSetting("orientation"));

	// Load resource locations
	Ogre::String list = cfg.getSetting("resource_locations");
	if (!list.empty())
	{
		mDirlist = Ogre::StringUtil::split(list, ";,");
	}
	else
	{
		mDirlist.clear();
	}

	// Load Video directory
	wxString videoPath = ogre2wx(cfg.getSetting("videopath"));
	if (videoPath == wxT(""))
	{
		videoPath = wxT("media/video");
	}
	setVideoPath(videoPath);

	// Load Video (image) width
	ParticleUniverse::uint width = Ogre::StringConverter::parseInt(cfg.getSetting("imagewidth"));
	if (width < 1)
	{
		width = 640;
	}
	setImageWidth(width);

	// Load Video (image) heigth
	ParticleUniverse::uint height = Ogre::StringConverter::parseInt(cfg.getSetting("imageheight"));
	if (height < 1)
	{
		height = 480;
	}
	setImageHeight(height);

	// Load Image type
	wxString type = ogre2wx(cfg.getSetting("mediatype"));
	if (type == wxT(""))
	{
		type = _(".png");
	}
	setFileNameSuffix(type);
	doFileNameSuffixChanged();

	// Load Video size
	wxString vsize = ogre2wx(cfg.getSetting("videosize"));
	if (vsize == wxT(""))
	{
		vsize = _("720x576");
	}
	if (mVideoSelected)
	{
		setVideoSize(vsize, true);
	}
	else
	{
		setVideoSize(vsize, false);
	}

	// Load Frames per second
	ParticleUniverse::uint fps = Ogre::StringConverter::parseInt(cfg.getSetting("fps"));
	if (fps < 1)
	{
		 fps = 27;
	}
	setFPS(fps);

	// Load Pausetime
	ParticleUniverse::Real pauseTime = Ogre::StringConverter::parseReal(cfg.getSetting("pausetime"));
	if (pauseTime == 0.0f)
	{
		pauseTime = 0.2f;
	}
	setPauseTime(pauseTime);

	// Load Filter
	wxString filter = wxGetTranslation(ogre2wx(cfg.getSetting("filter")));
	if (videoPath == wxT(""))
	{
		filter = _("No filtering");
	}
	setFilter(filter);

	// Load Edit Proportion
	ParticleUniverse::Real editProportion = Ogre::StringConverter::parseReal(cfg.getSetting("edit_window_proportion"));
	if (editProportion == 0.0f)
	{
		editProportion = 0.5f;
	}
	setEditProportion(editProportion);


#ifdef PU_PHYSICS_PHYSX
	// Load PhysX plane normal
	Ogre::Vector3 planeNormal = Ogre::StringConverter::parseVector3(cfg.getSetting("physx_plane_normal"));
	setPhysXPlaneNormal(planeNormal);

	// Load PhysX plane distance
	ParticleUniverse::Real planeDistance = Ogre::StringConverter::parseReal(cfg.getSetting("physx_plane_distance"));
	if (planeDistance == 0.0f)
	{
		planeDistance = -200.0f;
	}
	setPhysXPlaneDistance(planeDistance);

	// Load DisplayPhysXStartupDialog
	setDisplayPhysXStartupDialog(Ogre::StringConverter::parseBool(cfg.getSetting("physx_display_dialog")));

	// Load UsPhysXIfNoStartupDialog
	setUsePhysXIfNoStartupDialog(Ogre::StringConverter::parseBool(cfg.getSetting("use_physx_if_no_dialog")));

	// Load PhysX Gravity
	Ogre::Vector3 gravity = Ogre::StringConverter::parseVector3(cfg.getSetting("physx_gravity"));
	setPhysXGravity(gravity);

#endif //PU_PHYSICS_PHYSX

	// Load Camera position
	Ogre::Vector3 position = Ogre::StringConverter::parseVector3(cfg.getSetting("camera_position"));
	setCameraPosition(position);

	// Rebuild
	rebuildDirectoriesPanel();
}
//-----------------------------------------------------------------------
void ConfigDialog::saveConfig(void)
{
	ConfigFile cfg;

	// Save autostart
	cfg.setSetting("autostart", Ogre::StringConverter::toString(isAutoStart()));

	// Save hightlight
	cfg.setSetting("highlight", Ogre::StringConverter::toString(isHighlight()));

	// Save statistics
	cfg.setSetting("statistics", Ogre::StringConverter::toString(isStatistics()));

	// Save light
	cfg.setSetting("light", Ogre::StringConverter::toString(isLightCheck()));

	// Save gridplane
	cfg.setSetting("gridplane", Ogre::StringConverter::toString(isGridPlane()));

	// Save gridplane distance
	cfg.setSetting("gridplane_distance", Ogre::StringConverter::toString(getGridPlaneDistance()));

	// Save gridplane scale
	cfg.setSetting("gridplane_scale", Ogre::StringConverter::toString(getGridPlaneScale()));

	// Save resource locations (save mDirlist minus mExcludeDirlist)
	getExcludeDirList(); // Forces updating the mDirlist
	cfg.setConcatSetting("resource_locations", mDirlist);

	// Save language
	wxString language = getLanguageAsString(mLanguage);
	cfg.setSetting("language", wx2ogre(language));

	// Save Video directory
	cfg.setSetting("videopath", wx2ogre(getVideoPath()));

	// Save Video (image) width
	cfg.setSetting("imagewidth", Ogre::StringConverter::toString(getImageWidth()));

	// Save Video (image) heigth
	cfg.setSetting("imageheight", Ogre::StringConverter::toString(getImageHeight()));

	// Save Image type
	cfg.setSetting("mediatype", wx2ogre(getFileNameSuffix()));

	// Save Video size
	cfg.setSetting("videosize", wx2ogre(getVideoSize(false)));

	// Save Frames per second
	cfg.setSetting("fps", Ogre::StringConverter::toString(getFPS()));

	// Save Pause time
	cfg.setSetting("pausetime", Ogre::StringConverter::toString(getPauseTime()));

	// Save Filter
	int test = mFilter->GetCurrentSelection();
	if (mFilter->GetCurrentSelection() == 0)
	{
		cfg.setSetting("filter", "No filtering");
	}
	else if (mFilter->GetCurrentSelection() == 1)
	{
		cfg.setSetting("filter", "Alpha from luminance");
	}
	else if (mFilter->GetCurrentSelection() == 2)
	{
		cfg.setSetting("filter", "Alpha from background colour");
	}

#ifdef PU_PHYSICS_PHYSX
	// Save PhysX plane normal
	cfg.setSetting("physx_plane_normal", Ogre::StringConverter::toString(getPhysXPlaneNormal()));

	// Save PhysX plane distance
	cfg.setSetting("physx_plane_distance", Ogre::StringConverter::toString(getPhysXPlaneDistance()));

	// Save DisplayPhysXStartupDialog
	cfg.setSetting("physx_display_dialog", Ogre::StringConverter::toString(isDisplayPhysXStartupDialog()));

	// Save DisplayPhysXStartupDialog
	cfg.setSetting("use_physx_if_no_dialog", Ogre::StringConverter::toString(isUsePhysXIfNoStartupDialog()));

	// Save PhysX gravity
	cfg.setSetting("physx_gravity", Ogre::StringConverter::toString(getPhysXGravity()));

#endif //PU_PHYSICS_PHYSX

	// Save Camera position
	cfg.setSetting("camera_position", Ogre::StringConverter::toString(getCameraPosition()));

	// Save the config file
	cfg.save(CONFIG_FILENAME);
}
//-----------------------------------------------------------------------
void ConfigDialog::OnFileNameSuffixChanged(wxCommandEvent& event)
{
	doFileNameSuffixChanged();
}
//-----------------------------------------------------------------------
void ConfigDialog::doFileNameSuffixChanged(void)
{
	// Change some things if it concerns an AVI (video) file
	wxString type = mFileNameSuffix->GetValue();
	if (type == _(".avi") || type == _(".swf") || type == _(".wmv")
#ifdef PU_USE_MP4
	|| type == _(".mp4")
#endif
	)
	{
		mVideoSize->Show();
		mImageWidth->Disable();
		mImageHeight->Disable();
		mVideoSelected = true;
	}
	else
	{
		mVideoSize->Hide();
		mImageWidth->Enable();
		mImageHeight->Enable();
		mVideoSelected = false;
	}
}
