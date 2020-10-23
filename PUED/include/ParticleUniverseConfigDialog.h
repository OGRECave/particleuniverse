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

#ifndef __PUED_CONFIG_DIALOG_H__
#define __PUED_CONFIG_DIALOG_H__

#include "wx/ogre/prerequisites.h"
#include "wx/propdlg.h"
#include "wx/generic/propdlg.h"
#include "ParticleUniverseRecorder.h"
#include "ParticleUniverseMediaTypeComboBox.h"
#include "wx/spinctrl.h"

/**	Configuration dialog
*/
class ParticleUniverseEditorFrame;
class ConfigDialog : public wxPropertySheetDialog
{
	public:
		// Constructor / Destructor
		ConfigDialog(wxWindow* parent);
		~ConfigDialog(void){};

		/**	Initialise the dialog window
		*/
		void initWindow(void);

		/**	Load the language separately
		*/
		void loadLanguage(void);
		wxString getLanguageAsString(const wxLanguage& language);

		/**	Create notebook panels
		*/
		wxPanel* createGeneralPanel(wxWindow* parent);
		wxPanel* createDirectoriesPanel(wxWindow* parent);
		wxPanel* createRecordPanel(wxWindow* parent);
#ifdef PU_PHYSICS_PHYSX
		wxPanel* createPhysXPanel(wxWindow* parent);
#endif //PU_PHYSICS_PHYSX

		/**	Get/Set Automatic start of the renderer tab
		*/
		bool isAutoStart(void) const;
		void setAutoStart(bool autoStart);

		/**	Get/Set Highlighting the keywords in the script
		*/
		bool isHighlight(void) const;
		void setHighlight(bool highlight);

		/**	Get/Set Statistics overlay
		*/
		bool isStatistics(void) const;
		void setStatistics(bool statistics);

		/**	Get/Set LightCheck
		*/
		bool isLightCheck(void) const;
		void setLightCheck(bool lightCheck);

		/**	Get/Set GridPlane
		*/
		bool isGridPlane(void) const;
		void setGridPlane(bool gridplane);

		/**	Gridplane distance functions
		*/
		ParticleUniverse::Real getGridPlaneDistance(void) const;
		void setGridPlaneDistance(ParticleUniverse::Real distance);

		/**	Gridplane scale functions
		*/
		ParticleUniverse::Real getGridPlaneScale(void) const;
		void setGridPlaneScale(ParticleUniverse::Real distance);

		/**	Get/Set position and orientation
		*/
		const Ogre::Vector3& getMainNodePosition(void) const;
		void setMainNodePosition(const Ogre::Vector3& position);
		const Ogre::Quaternion& getMainNodeOrientation(void) const;
		void setMainNodeOrientation(const Ogre::Quaternion& orientation);

		/**	Load/Save configuration
		*/
		void loadConfig(void);
		void saveConfig(void);

		/**	Resourcelocation functions
		*/
		void addToDirList(const Ogre::String& dir);
		const Ogre::StringVector& getResourceLocations(void);
		const Ogre::StringVector getExcludeDirList(void);
		void rebuildDirectoriesPanel(void);

		/**	Language functions
		*/
		const wxLanguage getLanguage(void) const;

		/**	Video directory functions
		*/
		wxString getVideoPath(void) const;
		void setVideoPath(wxString& videoPath);

		/**	Video (image) width functions
		*/
		ParticleUniverse::uint getImageWidth(void) const;
		void setImageWidth(ParticleUniverse::uint imageWidth);

		/**	Video (image) heigth functions
		*/
		ParticleUniverse::uint getImageHeight(void) const;
		void setImageHeight(ParticleUniverse::uint imageHeight);

		/**	Image type functions
		*/
		wxString getFileNameSuffix(void) const;
		void setFileNameSuffix(wxString& suffix);

		/**	Frames per second functions
		*/
		ParticleUniverse::uint getFPS(void) const;
		void setFPS(ParticleUniverse::uint fps);

		/**	Pausetime functions
		*/
		ParticleUniverse::Real getPauseTime(void) const;
		void setPauseTime(ParticleUniverse::Real pauseTime);

		/**	Video function
		*/
		bool isVideoSelected(void) const;

		/**	Video size functions
		*/
		wxString getVideoSize(bool propagate = true);
		void setVideoSize(wxString& videoSize, bool propagate = true);

		/**	Camera position functions
		*/
		const Ogre::Vector3& getCameraPosition(void) const;
		void setCameraPosition(const Ogre::Vector3& position);

		/**	Filter functions
		*/
		const Recorder::ImageFilter getFilter(void) const;
		wxString getFilterAsString(void) const;
		void setFilter(const Recorder::ImageFilter& filter);
		void setFilter(const wxString& filter);

		/**	PhysX plane normal functions
		*/
		Ogre::Vector3& getPhysXPlaneNormal(void);
		void setPhysXPlaneNormal(Ogre::Vector3& normal);

		/**	PhysX plane distance functions
		*/
		ParticleUniverse::Real getPhysXPlaneDistance(void) const;
		void setPhysXPlaneDistance(ParticleUniverse::Real distance);

		/**	Indication whether the PhysX dialog is displayed during startup.
		*/
		bool isDisplayPhysXStartupDialog(void) const;
		void setDisplayPhysXStartupDialog(bool displayPhysXStartupDialog);

		/**	Indication whether PhysX is used in case the dialog is not displayed.
		*/
		bool isUsePhysXIfNoStartupDialog(void) const;
		void setUsePhysXIfNoStartupDialog(bool usePhysXIfNoStartupDialog);

		/**	PhysX gravity functions
		*/
		Ogre::Vector3& getPhysXGravity(void);
		void setPhysXGravity(Ogre::Vector3& gravity);

		/**	Event handlers
		*/
		void OnFileNameSuffixChanged(wxCommandEvent& event);

		/**	Get/set edit proportions
		*/
		ParticleUniverse::Real getEditProportion(void);
		void setEditProportion(ParticleUniverse::Real editProportion);

	protected:
		/**	Resourcelocation functions
		*/
		void _removeFromDirList(const Ogre::String& dir);

		/**	Execute the handler
		*/
		void doFileNameSuffixChanged(void);

		/**	Propagate the values of the video size to the image width and height
		*/
		void _propagateVideoSize(void);

		ParticleUniverseEditorFrame* mParentFrame;

		// Autostart
		wxCheckBox* mCheckAutoStart;

		// Highlight keywords
		wxCheckBox* mHighlight;

		// Statistics
		wxCheckBox* mStatistics;

		// Light
		wxCheckBox* mLightCheck;

		// Gridplane
		wxCheckBox* mGridPlane;
		wxSpinCtrl* mGridPlaneDistance;
		wxTextCtrl* mGridPlaneScale;

		// Resource Locations
		Ogre::StringVector mDirlist;
		Ogre::StringVector mExcludeDirlist;
		wxBoxSizer* mDirListSizer;
		wxPanel* mDirListPanel;
		std::vector<wxCheckBox*> mCheckboxes;
		std::vector<wxTextCtrl*> mTexts;

		// Record
		wxTextCtrl* mVideoPath;
		wxSpinCtrl* mImageWidth;
		wxSpinCtrl* mImageHeight;
		wxSpinCtrl* mFramesPerSecond;
		MediaTypeComboBox* mFileNameSuffix;
		wxTextCtrl* mPauseTime;
		wxComboBox* mFilter;
		wxComboBox* mVideoSize;
		bool mVideoSelected;

		// PhysX
		wxTextCtrl* mPhysXPlaneNormalX;
		wxTextCtrl* mPhysXPlaneNormalY;
		wxTextCtrl* mPhysXPlaneNormalZ;
		wxTextCtrl* mPhysXGravityX;
		wxTextCtrl* mPhysXGravityY;
		wxTextCtrl* mPhysXGravityZ;
		Ogre::Vector3 mPhysXPlaneNormal;
		Ogre::Vector3 mPhysXGravity;
		wxSpinCtrl* mPhysXPlaneDistance;
		wxCheckBox* mCheckPhysXDialog; // Determines whether the dialog must be displayed when the application starts.
		bool mUsePhysXIfNoStartupDialog; // Determines PhysX is used if the dialog is not displayed (default = false).
										 // The value is set based on the first time the Physx dialog is displayed.

		// Camera position
		Ogre::Vector3 mCameraPosition;

		// Edit Proportions
		Ogre::Real mEditProportion;

		// Not used
		Ogre::Vector3 mMainNodePosition;
		Ogre::Quaternion mMainNodeOrientation;

		// Language
		wxLanguage mLanguage;
};

#endif
