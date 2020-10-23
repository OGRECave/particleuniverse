/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_MAIN_TOOLBAR_H__
#define __PUED_MAIN_TOOLBAR_H__

#include "wx/ogre/prerequisites.h"
#include "wx/toolbar.h"
#include "ParticleUniverseSpinCtrl.h"

class UIFileIcons;
class UICompileIcons;
class UIPlayIcons;
class UIMiscIcons;
class TimeScaleSlider;
class ParticleUniverseEditorFrame;

/**	Main toolbar with all icons
*/
class UIMainToolbar
{
	public:
		// Constructor / Destructor
		UIMainToolbar(ParticleUniverseEditorFrame* parent);
		~UIMainToolbar(void);
		wxToolBar* getWxToolbar(void) const;
		UIFileIcons* getFileIcons(void) const;
		UICompileIcons* getCompileIcons(void) const;
		UIPlayIcons* getPlayIcons(void) const;
		UIMiscIcons* getMiscIcons(void) const;
		void resetTimeScale(void);

	protected:
		ParticleUniverseEditorFrame* mParent;
		wxToolBar *mWxToolBar;
		UIFileIcons* mUIFileIcons;
		UICompileIcons* mUICompileIcons;
		UIPlayIcons* mUIPlayIcons;
		UIMiscIcons* mUIMiscIcons;

	private:
		UIMainToolbar(void){};
};


/**	Play, pause, stop, rewind, wind and record icons
*/
class UIPlayIcons
{
	public:
		// Constructor / Destructor
		UIPlayIcons(wxToolBar* toolbar);
		~UIPlayIcons(void){};

		// Actions
		void reset(unsigned int numberOfListItems);
		void disable(unsigned int numberOfListItems);
		void disableAll(void);
		void disableListFilled(void);
		void resetListFilled(void);
		void play(void);
		void pause(void);
		void record(void);

	protected:
		wxToolBar* mToolbar;
	
	private:
		UIPlayIcons(void){};
};

/**	New, Load, Save icons
*/
class UIFileIcons
{
	public:
		// Constructor / Destructor
		UIFileIcons(wxToolBar* toolbar);
		~UIFileIcons(void){};

		// Actions
		void reset(unsigned int numberOfListItems);
		void disableAll(void);

	protected:
		wxToolBar* mToolbar;
	
	private:
		UIFileIcons(void){};
};

/**	Compile icons
*/
class UICompileIcons
{
	public:
		// Constructor / Destructor
		UICompileIcons(wxToolBar* toolbar);
		~UICompileIcons(void){};

		// Actions
		void reset(unsigned int numberOfListItems);
		void disableAll(void);

	protected:
		wxToolBar* mToolbar;
	
	private:
		UICompileIcons(void){};
};

/**	Miscelanious icons
*/
class UIMiscIcons
{
	public:
		// Constructor / Destructor
		UIMiscIcons(ParticleUniverseEditorFrame* frame, wxToolBar* toolbar);
		~UIMiscIcons(void){};

		// Enable/Disable the icon
		void enableRemoveEntity(bool enable);
		void enableOrthoCameraSelected(bool enabled);

		// Handle spin controls
		void callbackSpinCtrlXYZ(SpinCtrlXYZ* spinCtrl);

		// Global/local event
		bool doToggleWorldLocalSpace(void);

		// Camera projection type event
		bool doToggleCameraProjectionType(void);
		bool isCameraPerspective(void) const;

		// Camera angle
		bool doToggleCameraAngle(void);
		bool isCamera45Angle(void) const;

		// Actions
		void resetTimeScale(void);
		void enable(bool enabled);
		void reset(void);
		void disableAll(void);

	protected:
		ParticleUniverseEditorFrame* mFrame;
		wxToolBar* mToolbar;
		SpinCtrlXYZ* mSpinX;
		SpinCtrlXYZ* mSpinY;
		SpinCtrlXYZ* mSpinZ;
		TimeScaleSlider* mTimeScaleSlider;
		bool mWorldSpace;
		bool mCameraPerspective;
		bool mCamera45Angle;
		wxBitmap mWorldSpaceBitmap;
		wxBitmap mLocalSpaceBitmap;
		wxBitmap mPerspectiveBitmap;
		wxBitmap mOrthographicBitmap;
		wxBitmap mOrtho45AngleBitmap;
		wxBitmap mOrtho90AngleBitmap;
	
	private:
		UIMiscIcons(void){};
};

/**	
*/
class TimeScaleSlider : public wxSlider
{
	public:
		// Constructor / Destructor
		TimeScaleSlider(ParticleUniverseEditorFrame* frame, wxControl* panel, int value , int minValue, int maxValue);
		virtual ~TimeScaleSlider(void){};
		ParticleUniverse::Real getTimeScale(void) const;
		void setTimeScale(ParticleUniverse::Real scale);
		void resetTimeScale(void);
		void OnSliderUpdate(wxCommandEvent& event);
	
	protected:
		ParticleUniverseEditorFrame* mFrame;
};

#endif
