/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseAnimationWindow.h"
#include "ParticleUniverseEditor.h"
#include "OgreTagPoint.h"
#include "iostream"

//-----------------------------------------------------------------------
AnimationWindow::AnimationWindow(ParticleUniverseEditorFrame* parent, Ogre::SceneManager* sceneManager) : 
	wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM),
	mFrame(parent),
	mSceneManager(sceneManager),
	mAnimationState(0),
	mTimeSinceLastUpdate(0.0f),
	mTimeScale(0.0f),
	mTagPointToWhichParticleSystemAttached(0),
	mAnimationPlayPauseStop(ANIM_NONE)
{
	mBaseOrientation = Ogre::Quaternion::IDENTITY;
	mBoneToWhichParticleSystemAttached = "";
	mAnimationPanel = new wxPanel(this);
	mMeshPanel = new wxPanel(this);
	AddPage(mAnimationPanel, _("Animations"), true);
	AddPage(mMeshPanel, _("Mesh"), false);

	// The main sizer
    wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);

	// Dropdown list with animations
	wxBoxSizer* animationSelectionSizer = new wxBoxSizer(wxHORIZONTAL);
	mTxtAnimation = new wxStaticText(mAnimationPanel, -1, _("Animation"));
	mListAnimations = new wxComboBox(mAnimationPanel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(0.5 * LB_TEMPLATES_WIDTH, wxDefaultSize.y), 0, wxCB_READONLY | wxCB_SORT);
	animationSelectionSizer->Add(mTxtAnimation, 0, wxALL|wxALIGN_LEFT, 10);
	animationSelectionSizer->Add(mListAnimations, 0, wxGROW|wxALL, 10);
	verticalSizer->Add(animationSelectionSizer, 0, wxGROW|wxALL, 0);
	mListAnimations->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AnimationWindow::onSelectAnimation), NULL, this);

	// The play buttons
	wxBoxSizer* playAnimationSizer = new wxBoxSizer(wxHORIZONTAL);
	mPauseAnimationButton = new wxButton(mAnimationPanel, ID_PLAY_ANIMATION, _("Pause"));
	mPlayAnimationButton = new wxButton(mAnimationPanel, ID_PLAY_ANIMATION, _("Play"));
	mStopAnimationButton = new wxButton(mAnimationPanel, ID_PLAY_ANIMATION, _("Stop"));
	playAnimationSizer->Add(mPauseAnimationButton, 0, wxALL|wxALIGN_RIGHT, 5);
	playAnimationSizer->Add(mPlayAnimationButton, 0, wxALL|wxALIGN_CENTER, 5);
	playAnimationSizer->Add(mStopAnimationButton, 0, wxALL|wxALIGN_LEFT, 5);
	verticalSizer->Add(playAnimationSizer, 0, wxGROW|wxALL, 0);

	wxBoxSizer* checkboxSizer = new wxBoxSizer(wxHORIZONTAL);
	mCheckboxLoopAnimation = new wxCheckBox(mAnimationPanel, ID_TOGGLE_LOOP, _("Loop Animation"));
	mCheckboxInheritOrientationFromBone = new wxCheckBox(mAnimationPanel, ID_TOGGLE_INHERIT_ORIENTATION_FROM_BONE, _("Inherit bone orientation"));
	checkboxSizer->Add(mCheckboxLoopAnimation, 0, wxALL|wxALIGN_LEFT, 5);
	checkboxSizer->Add(mCheckboxInheritOrientationFromBone, 0, wxALL|wxALIGN_LEFT, 5);
	verticalSizer->Add(checkboxSizer, 0, wxGROW|wxALL, 0);

	mPauseAnimationButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AnimationWindow::onPauseAnimation), NULL, this);
	mPlayAnimationButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AnimationWindow::onPlayAnimation), NULL, this);
	mStopAnimationButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AnimationWindow::onStopAnimation), NULL, this);
	mCheckboxLoopAnimation->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(AnimationWindow::onRestoreFocus), NULL, this);
	mCheckboxInheritOrientationFromBone->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(AnimationWindow::onInheritOrientationFromBone), NULL, this);
	setEnableAnimationControls(ANIM_NONE);

	// Bones tree
	mAttachDetachButton = new wxButton(mAnimationPanel, ID_ATTACH_DETACH);
	setAttachDetachLabel(NO_SKELETON);
	mListOfBones = new wxTreeCtrl(mAnimationPanel, ID_LISTOF_BONES, wxDefaultPosition, wxSize(0.8 * LB_TEMPLATES_WIDTH, 296), wxTR_HAS_BUTTONS | wxVSCROLL);
	verticalSizer->Add(mAttachDetachButton, 0, wxEXPAND|wxALIGN_LEFT, 5);
	verticalSizer->Add(mListOfBones, 0, wxEXPAND|wxALIGN_LEFT, 5);
	mAttachDetachButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AnimationWindow::onAttachDetachParticleSystem), NULL, this);
	mListOfBones->Connect(wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler(AnimationWindow::onBoneTreeSelected), NULL, this);
	mBoneMarker = mSceneManager->createEntity("pu_marker_bone", "pu_bold_marker.mesh");
	mAnimationPanel->SetSizer(verticalSizer);
	mAnimationPanel->Fit();
	mAnimationPanel->Layout();

	// Mesh info
	wxBoxSizer* meshInfoSizer = new wxBoxSizer(wxHORIZONTAL);
	mMeshInfo = new wxListCtrl(mMeshPanel, ID_MESH_INFO, wxDefaultPosition, wxSize(0.8 * LB_TEMPLATES_WIDTH, 280), wxLC_LIST);
	meshInfoSizer->Add(mMeshInfo, 0, wxEXPAND | wxALIGN_CENTER, 5);
	mMeshPanel->SetSizer(meshInfoSizer);
	mMeshPanel->Fit();
	mMeshPanel->Layout();
	
	Layout();
}
//-----------------------------------------------------------------------
void AnimationWindow::updateAnimation(void)
{
	if (IsBeingDeleted())
		return;

	Ogre::Real timer = mTimer.getMilliseconds();
	if (mAnimationPlayPauseStop == ANIM_PLAY)
	{
		// Check is there is an animation.
		if (!mAnimationState)
			return;
	
		if (!mCheckboxLoopAnimation->GetValue() && mAnimationState->hasEnded())
		{
			// If animation has ended, reset
			doStopAnimation();
		}

		Ogre::Real deltaTime = 0.001 * mTimeScale * (timer - mTimeSinceLastUpdate);
		mAnimationState->addTime(deltaTime);
	}
	mTimeSinceLastUpdate = timer;
}
//-----------------------------------------------------------------------
void AnimationWindow::onPlayAnimation(wxCommandEvent& event)
{
	// Set the selected animation
	if (IsBeingDeleted())
	{
		return;
	}

	mFrame->restoreFocus();

	Ogre::Entity* entity = mFrame->getAddEntity();
	if (!entity)
		return;

	Ogre::SkeletonInstance* skeleton = entity->getSkeleton();
	if (!skeleton)
		return;

	Ogre::String animationName = wx2ogre(mListAnimations->GetStringSelection());
	if (animationName == Ogre::StringUtil::BLANK)
		return;

	if (!skeleton->hasAnimation(animationName))
		return;

	if (mAnimationPlayPauseStop == ANIM_PAUSE)
	{
		// Resumed from pause
		mAnimationPlayPauseStop = ANIM_PLAY;
		setEnableAnimationControls(ANIM_PLAY);
		mTimeSinceLastUpdate = mTimer.getMilliseconds();
		return;
	}

	mBaseOrientation = _getDerivedOrientationTagPoint();
	entity->_updateAnimation();
	mAnimationState = entity->getAnimationState(animationName);
	mAnimationState->setEnabled(true);
	mAnimationState->setTimePosition(0);
	mAnimationState->setLoop(mCheckboxLoopAnimation->GetValue());
	mAnimationPlayPauseStop = ANIM_PLAY;
	setEnableAnimationControls(ANIM_PLAY);
	mTimer.reset();
	mTimeSinceLastUpdate = 0.0f;
	mFrame->notifyAnimationStarted(); // TEST
}
//-----------------------------------------------------------------------
void AnimationWindow::onPauseAnimation(wxCommandEvent& event)
{
	mAnimationPlayPauseStop = ANIM_PAUSE;
	setEnableAnimationControls(ANIM_PAUSE);
	mFrame->restoreFocus();
}
//-----------------------------------------------------------------------
void AnimationWindow::onStopAnimation(wxCommandEvent& event)
{
	doStopAnimation();
	mFrame->restoreFocus();
}
//-----------------------------------------------------------------------
void AnimationWindow::doStopAnimation(void)
{
	if (IsBeingDeleted())
	{
		return;
	}

	mAnimationPlayPauseStop = ANIM_STOP;
	setEnableAnimationControls(ANIM_STOP);
	if (mAnimationState)
	{
		mAnimationState->setEnabled(false);
		mAnimationState->setTimePosition(0);
	}
	mTimer.reset();
	mTimeSinceLastUpdate = 0.0f;
	mFrame->notifyAnimationStopped();
}
//-----------------------------------------------------------------------
void AnimationWindow::notifyMeshDeleted(Ogre::Entity* entity)
{
	mAnimationPlayPauseStop = ANIM_NONE;
	setEnableAnimationControls(ANIM_NONE);
	mAnimationState = 0;
	mListAnimations->Clear();
	mListOfBones->DeleteAllItems();
	mTimer.reset();
	mTimeSinceLastUpdate = 0.0f;

	// Remove registration
	mFrame->getGizmoManager()->unregisterNodeForUpdatingScale(mBoneMarker->getParentNode());

	// If the entity still has a particle system attached, detach it
	detachParticleSystemFromEntity(entity);

	// Set the label
	setAttachDetachLabel(NO_SKELETON);
}
//-----------------------------------------------------------------------
void AnimationWindow::notifyParticleSystemDeleted(Ogre::Entity* entity)
{
	// Detach from the entity
	detachParticleSystemFromEntity(entity);
}
//-----------------------------------------------------------------------
void AnimationWindow::loadAnimationNames(Ogre::Entity* entity)
{
	if (!entity)
		return;

	Ogre::SkeletonInstance* skeleton = entity->getSkeleton();
	if (!skeleton)
		return;

	Ogre::Animation *animation = 0;
	for (unsigned short i=0; i < skeleton->getNumAnimations(); ++i)
	{
		Ogre::Animation *animation = skeleton->getAnimation(i);
		wxString animationName = ogre2wx(animation->getName());
		mListAnimations->Append(animationName);
	}
	if (mListAnimations->GetCount() > 0)
	{
		mListAnimations->SetSelection(0);
	}
}
//-----------------------------------------------------------------------
void AnimationWindow::loadBoneNames(Ogre::Entity* entity)
{
	if (!entity)
		return;

	Ogre::SkeletonInstance* skeleton = entity->getSkeleton();
	if (!skeleton)
	{
		setAttachDetachLabel(NO_SKELETON);
		return;
	}

	mListOfBones->DeleteAllItems();
	Ogre::String rootName = entity->getMesh()->getName();
	wxTreeItemId rootNode = mListOfBones->AddRoot(ogre2wx(rootName), 0);
	Ogre::SkeletonInstance::BoneIterator boneIterator = skeleton->getRootBoneIterator();
	while(boneIterator.hasMoreElements())
	{
		Ogre::Bone* bone = boneIterator.peekNext();
		Ogre::String boneName = bone->getName();
		wxTreeItemId boneNode = mListOfBones->AppendItem(rootNode, ogre2wx(boneName), 0);
		Ogre::Node::ChildNodeIterator childNodeIterator = bone->getChildIterator();
		loadBoneBranche(boneNode, childNodeIterator);
		boneIterator.moveNext();
	}
	mListOfBones->ExpandAll();
	setAttachDetachLabel(SELECT_BONE);
	setEnableAnimationControls(ANIM_STOP);
	mFrame->restoreFocus();
}
//-----------------------------------------------------------------------
void AnimationWindow::loadBoneBranche(wxTreeItemId boneNode, Ogre::Node::ChildNodeIterator boneIterator)
{
	while(boneIterator.hasMoreElements())
	{
		Ogre::Node* bone = boneIterator.peekNextValue();
		Ogre::String boneName = bone->getName();
		wxTreeItemId subBoneNode = mListOfBones->AppendItem(boneNode, ogre2wx(boneName), 0);
		Ogre::Node::ChildNodeIterator childNodeIterator = bone->getChildIterator();
		loadBoneBranche(subBoneNode, childNodeIterator);
		boneIterator.moveNext();
	}
}
//-----------------------------------------------------------------------
void AnimationWindow::notifyTimeRescaled(Ogre::Real scale)
{
	mTimeScale = scale;
}
//-----------------------------------------------------------------------
void AnimationWindow::onRestoreFocus(wxCommandEvent& event)
{
	// To be able to manipulate the render window. Is the focus stays on the AnimationWindow, you cannot zoom in the renderwindow
	mFrame->restoreFocus();
}
//-----------------------------------------------------------------------
void AnimationWindow::onSelectAnimation(wxCommandEvent& event)
{
	doStopAnimation();
	mFrame->restoreFocus();
}
//-----------------------------------------------------------------------
void AnimationWindow::onAttachDetachParticleSystem(wxCommandEvent& event)
{
	Ogre::Entity* entity = mFrame->getAddEntity();
	if (!entity)
		return;

	Ogre::SkeletonInstance* skeleton = entity->getSkeleton();
	if (!skeleton)
		return;

	wxTreeItemId selection = mListOfBones->GetSelection();
	if (!selection.IsOk())
		return;

	wxString name = mListOfBones->GetItemText(selection);
	Ogre::String boneName = wx2ogre(name);
	
	if (mAttachDetachButton->GetLabel() == _("Attach particle system"))
	{
		// Attach the particle system
		attachParticleSystemToEntity(boneName, entity);
	}
	else
	{
		// Detach the particle system
		detachParticleSystemFromEntity(entity);
	}
	mFrame->restoreFocus();
}
//-----------------------------------------------------------------------
void AnimationWindow::onBoneTreeSelected(wxTreeEvent& event)
{
	wxTreeItemId selection = mListOfBones->GetSelection();
	if (selection.IsOk())
	{
		// Set the bone into the view
		mListOfBones->ScrollTo(selection);

		// Validate the selection
		wxString name = mListOfBones->GetItemText(selection);
		if (name == wxT(""))
			return;

		Ogre::String boneName = wx2ogre(name);
		Ogre::Entity* entity = mFrame->getAddEntity();
		if (!entity)
			return;

		Ogre::SkeletonInstance* skeleton = entity->getSkeleton();
		if (!skeleton)
			return;

		if (!skeleton->hasBone(boneName))
			return;

		// Make selection visible by means of the marker
		if (mFrame->getGizmoManager())
		{
			// First unregister for updating the scale with the gizmo manager
			mFrame->getGizmoManager()->unregisterNodeForUpdatingScale(mBoneMarker->getParentNode());
		}
		mBoneMarker->detachFromParent();
		Ogre::TagPoint* tag = entity->attachObjectToBone(boneName, mBoneMarker);
		tag->setInheritOrientation(isOrientationInheritedFromBone());
		tag->setInheritParentEntityScale(false);
		if (mFrame->getGizmoManager())
		{
			// Register again for updating the scale with the gizmo manager
			mFrame->getGizmoManager()->registerNodeForUpdatingScale(tag, Ogre::Vector3(1.0f, 1.0f, 1.0f));
		}
		mBoneMarker->setMaterialName("pu_marker_bone");

		if (!isParticleSystemAttachedToEntity())
		{
			// Only change the label if the particle system is not already attached
			setAttachDetachLabel(ATTACH);
		}
	}
	mFrame->restoreFocus();
}
//-----------------------------------------------------------------------
bool AnimationWindow::attachParticleSystemToEntity(Ogre::String boneName, Ogre::Entity* entity)
{
	if (!entity)
		return false;

	if (boneName == Ogre::StringUtil::BLANK)
		return false;

	ParticleUniverse::ParticleSystem* particleSystem = mFrame->getCurrentParticleSystemForRenderer();
	if (particleSystem && mBoneToWhichParticleSystemAttached == Ogre::StringUtil::BLANK)
	{
		Ogre::SkeletonInstance* skeleton = entity->getSkeleton();
		if (!skeleton)
			return false;

		Ogre::Quaternion orientationParticleSystem = mFrame->getOrientationMainSceneNode();
		mFrame->notifyParticleSystemAttachToEntity();
		orientationParticleSystem.Inverse();
		Ogre::Quaternion orientationOffset = entity->getParentNode()->getOrientation();
		mTagPointToWhichParticleSystemAttached = entity->attachObjectToBone(boneName, particleSystem, orientationOffset * orientationParticleSystem);
		mTagPointToWhichParticleSystemAttached->setInheritOrientation(isOrientationInheritedFromBone());
		mBoneToWhichParticleSystemAttached = boneName;
		setAttachDetachLabel(DETACH);
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------
bool AnimationWindow::detachParticleSystemFromEntity(Ogre::Entity* entity)
{
	if (!entity)
		return false;

	ParticleUniverse::ParticleSystem* particleSystem = mFrame->getCurrentParticleSystemForRenderer();
	if (particleSystem && mBoneToWhichParticleSystemAttached != Ogre::StringUtil::BLANK)
	{
		entity->detachObjectFromBone(particleSystem);
		mBoneToWhichParticleSystemAttached = Ogre::StringUtil::BLANK;
		mTagPointToWhichParticleSystemAttached = 0;
		mFrame->notifyParticleSystemDetachedFromEntity();
		setAttachDetachLabel(ATTACH);
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------
bool AnimationWindow::isParticleSystemAttachedToEntity(void) const
{
	return mBoneToWhichParticleSystemAttached != Ogre::StringUtil::BLANK;
}
//-----------------------------------------------------------------------
const Ogre::String& AnimationWindow::getBoneNameToWhichParticleSystemIsAttached(void) const
{
	return mBoneToWhichParticleSystemAttached;
}
//-----------------------------------------------------------------------
const Ogre::Vector3& AnimationWindow::_getDerivedPositionTagPoint(void) const
{
	if (mTagPointToWhichParticleSystemAttached)
	{
		return mTagPointToWhichParticleSystemAttached->_getDerivedPosition();
	}
	return Ogre::Vector3::ZERO;
}
//-----------------------------------------------------------------------
const Ogre::Quaternion& AnimationWindow::_getDerivedOrientationTagPointParent(void) const
{
	if (mTagPointToWhichParticleSystemAttached)
	{
		return mTagPointToWhichParticleSystemAttached->getParent()->_getDerivedOrientation();
	}
	return Ogre::Quaternion::IDENTITY;
}
//-----------------------------------------------------------------------
const Ogre::Quaternion& AnimationWindow::_getDerivedOrientationTagPoint(void) const
{
	if (mTagPointToWhichParticleSystemAttached)
	{
		return mTagPointToWhichParticleSystemAttached->_getDerivedOrientation();
	}
	return Ogre::Quaternion::IDENTITY;
}
//-----------------------------------------------------------------------
void AnimationWindow::setOrientationTagPoint(const Ogre::Quaternion& orientation)
{
	if (mTagPointToWhichParticleSystemAttached)
	{
		mTagPointToWhichParticleSystemAttached->setOrientation(orientation);
	}
}
//-----------------------------------------------------------------------
void AnimationWindow::_setDerivedOrientationTagPoint(const Ogre::Quaternion& orientation)
{
	if (mTagPointToWhichParticleSystemAttached)
	{
		mTagPointToWhichParticleSystemAttached->_setDerivedOrientation(orientation);
	}
}
//-----------------------------------------------------------------------
const Ogre::Quaternion& AnimationWindow::_getDerivedBaseOrientation(void) const
{
	return mBaseOrientation;
}
//-----------------------------------------------------------------------
bool AnimationWindow::isAnimationRunning(void) const
{
	return mAnimationPlayPauseStop == ANIM_PLAY;
}
//-----------------------------------------------------------------------
void AnimationWindow::setAttachDetachLabel(AttachDetachOptions options)
{
	switch(options)
	{
		case ATTACH:
			{
				mAttachDetachButton->SetLabel(_("Attach particle system"));
				mAttachDetachButton->SetForegroundColour(wxColour(255, 255, 255));
				mAttachDetachButton->SetBackgroundColour(wxColour(0, 164, 0));
				mAttachDetachButton->Enable(true);
			}
			break;

		case DETACH:
			{
				mAttachDetachButton->SetLabel(_("Detach particle system"));
				mAttachDetachButton->SetForegroundColour(wxColour(255, 255, 255));
				mAttachDetachButton->SetBackgroundColour(wxColour(164, 0, 0));
				mAttachDetachButton->Enable(true);
			}
			break;

		case NO_SKELETON:
			{
				mAttachDetachButton->SetLabel(_("Current mesh does not have a skeleton"));
				mAttachDetachButton->SetForegroundColour(wxColour(164, 164, 164));
				mAttachDetachButton->SetBackgroundColour(wxColour(255, 255, 255));
				mAttachDetachButton->Enable(false);
			}
			break;

		case SELECT_BONE:
			{
				mAttachDetachButton->SetLabel(_("Select bone to attach particle system"));
				mAttachDetachButton->SetForegroundColour(wxColour(0, 0, 0));
				mAttachDetachButton->SetBackgroundColour(wxColour(224, 224, 224));
				mAttachDetachButton->Enable(false);
			}
			break;
	}
}
//-----------------------------------------------------------------------
void AnimationWindow::setEnableAnimationControls(AnimationPlayPauseStop animationState)
{
	switch(animationState)
	{
		case ANIM_PLAY:
			{
				mTxtAnimation->Enable(true);
				mListAnimations->Enable(true);
				mPauseAnimationButton->Enable(true);
				mPlayAnimationButton->Enable(false);
				mStopAnimationButton->Enable(true);
				mCheckboxLoopAnimation->Enable(false);
				mCheckboxInheritOrientationFromBone->Enable(false);
			}
			break;

		case ANIM_PAUSE:
			{
				mTxtAnimation->Enable(true);
				mListAnimations->Enable(true);
				mPauseAnimationButton->Enable(false);
				mPlayAnimationButton->Enable(true);
				mStopAnimationButton->Enable(true);
				mCheckboxLoopAnimation->Enable(true);
				mCheckboxInheritOrientationFromBone->Enable(false);
			}
			break;

		case ANIM_STOP:
			{
				mTxtAnimation->Enable(true);
				mListAnimations->Enable(true);
				mPauseAnimationButton->Enable(false);
				mPlayAnimationButton->Enable(true);
				mStopAnimationButton->Enable(false);
				mCheckboxLoopAnimation->Enable(true);
				mCheckboxInheritOrientationFromBone->Enable(true);
			}
			break;

		case ANIM_NONE:
			{
				mTxtAnimation->Enable(false);
				mListAnimations->Enable(false);
				mPauseAnimationButton->Enable(false);
				mPlayAnimationButton->Enable(false);
				mStopAnimationButton->Enable(false);
				mCheckboxLoopAnimation->Enable(false);
				mCheckboxInheritOrientationFromBone->Enable(false);
			}
			break;
	}
}
//-----------------------------------------------------------------------
bool AnimationWindow::isOrientationInheritedFromBone(void) const 
{
	return mCheckboxInheritOrientationFromBone->GetValue();
}
//-----------------------------------------------------------------------
void AnimationWindow::onInheritOrientationFromBone(wxCommandEvent& event)
{
	// Detach and attach to activate the new setting
	Ogre::String boneName = mBoneToWhichParticleSystemAttached;
	Ogre::Entity* entity = mFrame->getAddEntity();
	detachParticleSystemFromEntity(entity);
	attachParticleSystemToEntity(boneName, entity);
}
//-----------------------------------------------------------------------
void AnimationWindow::displayMeshInfo(Ogre::Entity* entity)
{
	// Always clear
	mMeshInfo->ClearAll();

	// Faset rejection
	if (!entity)
		return;

	// Material(s)
	unsigned int index = 0;
	unsigned int numberOfSubEntities = entity->getNumSubEntities();
	Ogre::SubEntity* subEntity = 0;
	for (unsigned int i = 0; i < numberOfSubEntities; ++i)
	{
		subEntity = entity->getSubEntity(i);
		wxString materialName = ogre2wx(subEntity->getMaterialName());
		mMeshInfo->InsertItem(index, wxString::Format(_("Material submesh(%u): %s"), i, materialName));
		++index;
	}

	// Vertices
	size_t numberOfVertices = 0;
	size_t numberOfIndices = 0;
	for (unsigned int i = 0; i < numberOfSubEntities; ++i)
	{
		subEntity = entity->getSubEntity(i);
		Ogre::SubMesh* subMesh = subEntity->getSubMesh();
		if (subMesh->vertexData)
		{
			numberOfVertices += subMesh->vertexData->vertexCount;
		}
		if (subMesh->indexData)
		{
			numberOfIndices += subMesh->indexData->indexCount;
		}
	}
	mMeshInfo->InsertItem(index, wxString::Format(_("Total number of vertices: %u"), numberOfVertices));
	++index;
	mMeshInfo->InsertItem(index, wxString::Format(_("Total number of indices: %u"), numberOfIndices));
	++index;

	// Bones
	Ogre::SkeletonInstance* skeleton = entity->getSkeleton();
	if (skeleton)
	{
		mMeshInfo->InsertItem(index, wxString::Format(_("Number of bones: %d"), skeleton->getNumBones()));
		++index;
	}

	// Mesh data
	Ogre::MeshPtr mesh = entity->getMesh();
	if (!mesh.isNull())
	{
		mesh->isEdgeListBuilt() ? mMeshInfo->InsertItem(index, _("Edgelist build: Yes")) : mMeshInfo->InsertItem(index, _("Edgelist build: No"));
		++index;

		mMeshInfo->InsertItem(index, wxString::Format(_("Boundingsphere radius: %f"), mesh->getBoundingSphereRadius()));
		++index;

		mMeshInfo->InsertItem(index, wxString::Format(_("Number of LOD levels: %u"), mesh->getNumLodLevels()));
		++index;
	}

}