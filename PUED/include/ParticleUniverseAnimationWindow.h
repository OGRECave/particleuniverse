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

#ifndef __PUED_ANIMATION_WINDOW_H__
#define __PUED_ANIMATION_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include <wx/treectrl.h>
#include <wx/listctrl.h>

/**	Class that represents the animation details of the loaded mesh
*/
class ParticleUniverseEditorFrame;
class AnimationWindow : public wxNotebook
{
	public:
		enum AnimationPlayPauseStop {ANIM_PLAY, ANIM_PAUSE, ANIM_STOP, ANIM_NONE} mAnimationPlayPauseStop;
		enum AttachDetachOptions {ATTACH, DETACH, NO_SKELETON, SELECT_BONE};
		AnimationWindow(ParticleUniverseEditorFrame* parent, Ogre::SceneManager* sceneManager);
		virtual ~AnimationWindow(void){};
		void updateAnimation(void);
		void onPlayAnimation(wxCommandEvent& event);
		void onPauseAnimation(wxCommandEvent& event);
		void onStopAnimation(wxCommandEvent& event);
		void loadAnimationNames(Ogre::Entity* entity);
		void loadBoneNames(Ogre::Entity* entity);
		void displayMeshInfo(Ogre::Entity* entity);
		void loadBoneBranche(wxTreeItemId boneNode, Ogre::Node::ChildNodeIterator boneIterator);
		void notifyTimeRescaled(Ogre::Real scale);
		void doStopAnimation(void);
		void notifyMeshDeleted(Ogre::Entity* entity);
		void notifyParticleSystemDeleted(Ogre::Entity* entity);
		void onRestoreFocus(wxCommandEvent& event);
		void onSelectAnimation(wxCommandEvent& event);
		void onAttachDetachParticleSystem(wxCommandEvent& event);
		void onBoneTreeSelected(wxTreeEvent& event);
		bool attachParticleSystemToEntity(Ogre::String boneName, Ogre::Entity* entity);
		bool detachParticleSystemFromEntity(Ogre::Entity* entity);
		bool isParticleSystemAttachedToEntity() const;
		const Ogre::String& getBoneNameToWhichParticleSystemIsAttached(void) const;
		const Ogre::Vector3& _getDerivedPositionTagPoint(void) const;
		const Ogre::Quaternion& _getDerivedOrientationTagPointParent(void) const;
		const Ogre::Quaternion& _getDerivedOrientationTagPoint(void) const;
		void setOrientationTagPoint(const Ogre::Quaternion& orientation);
		void _setDerivedOrientationTagPoint(const Ogre::Quaternion& orientation);
		const Ogre::Quaternion& _getDerivedBaseOrientation(void) const;
		bool isAnimationRunning(void) const;
		bool isOrientationInheritedFromBone(void) const;

	protected:
		Ogre::Real mTimeScale;
		wxPanel* mAnimationPanel;
		wxPanel* mMeshPanel;
		wxButton* mAttachDetachButton;
		wxTreeCtrl* mListOfBones;
		wxListCtrl* mMeshInfo;
		ParticleUniverseEditorFrame* mFrame;
		Ogre::SceneManager* mSceneManager;
		Ogre::Timer mTimer;
		wxStaticText* mTxtAnimation;
		wxComboBox* mListAnimations;
		wxCheckBox* mCheckboxLoopAnimation;
		wxCheckBox* mCheckboxInheritOrientationFromBone;
		wxButton* mPauseAnimationButton;
		wxButton* mPlayAnimationButton;
		wxButton* mStopAnimationButton;
		Ogre::AnimationState* mAnimationState;
		Ogre::Real mTimeSinceLastUpdate;
		Ogre::Entity* mBoneMarker;
		Ogre::String mBoneToWhichParticleSystemAttached;
		Ogre::TagPoint* mTagPointToWhichParticleSystemAttached;
		Ogre::Quaternion mBaseOrientation;
		void setAttachDetachLabel(AttachDetachOptions options);
		void setEnableAnimationControls(AnimationPlayPauseStop animationState);
		void onInheritOrientationFromBone(wxCommandEvent& event);
};

#endif
