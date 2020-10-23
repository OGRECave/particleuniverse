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
#include "ParticleUniversePropertyWindow.h"
#include "ParticleUniverseUtils.h"
#include "ParticleUniverseMeshSelector.h"
#include "ParticleUniverseMaterialSelector.h"
#include "OgreFontManager.h"
#include "wx/cmndata.h"
#include "wx/colordlg.h"

#include "OgreOverlay.h"

#ifdef PU_PHYSICS_PHYSX
	#include "Externs/ParticleUniversePhysXBridge.h"
#endif // PU_PHYSICS_PHYSX
//#include "Procedural.h" // TESTTESTTEST: Also remove compiler dirs + link dirs/lib

IMPLEMENT_APP(ParticleUniverseEditorApp)

//-----------------------------------------------------------------------
BEGIN_EVENT_TABLE(ParticleUniverseEditorFrame, wxFrame)
    EVT_MENU(wxID_NEW, ParticleUniverseEditorFrame::OnNewParticleSystem)
	EVT_MENU(wxID_FILE, ParticleUniverseEditorFrame::OnLoadFile)
	EVT_MENU(wxID_OPEN, ParticleUniverseEditorFrame::OnLoadResourcePath)
    EVT_MENU(wxID_SAVE, ParticleUniverseEditorFrame::OnSave)
    EVT_MENU(wxID_SAVEAS, ParticleUniverseEditorFrame::OnSaveAs)
    EVT_MENU(wxID_EXIT, ParticleUniverseEditorFrame::OnQuit)

	EVT_MENU(wxID_REFRESH, ParticleUniverseEditorFrame::OnCompile)
	EVT_MENU(ID_BACKGROUND_COLOUR, ParticleUniverseEditorFrame::OnBackgroundColour)
	EVT_MENU(ID_CAMERA_SAVE, ParticleUniverseEditorFrame::OnSaveCamera)
	EVT_MENU(ID_TOGGLE_CAMERA_PROJECTION, ParticleUniverseEditorFrame::OnCameraProjection)
	EVT_MENU(ID_TOGGLE_CAMERA_ANGLE, ParticleUniverseEditorFrame::OnCameraAngle)
	EVT_MENU(ID_RESET_TIME, ParticleUniverseEditorFrame::OnResetTime)
	EVT_MENU(ID_TOGGLE_STATS, ParticleUniverseEditorFrame::OnToggleStats)
	EVT_MENU(ID_TOGGLE_GRID_PLANE, ParticleUniverseEditorFrame::OnToggleGridPlane)
	EVT_MENU(ID_GIZMO_MOVE, ParticleUniverseEditorFrame::OnGizmoMoveSelect)
	EVT_MENU(ID_GIZMO_ROTATE, ParticleUniverseEditorFrame::OnGizmoRotateSelect)
	EVT_MENU(ID_GIZMO_SCALE, ParticleUniverseEditorFrame::OnGizmoScaleSelect)
	EVT_MENU(ID_TOGGLE_GIZMO, ParticleUniverseEditorFrame::OnToggleGizmo)
	EVT_MENU(ID_WORLD_LOCAL_SPACE, ParticleUniverseEditorFrame::OnToggleWorldLocalSpace)
	EVT_MENU(ID_CAMERA_RESET, ParticleUniverseEditorFrame::OnCameraReset)
	EVT_MENU(ID_CENTRE_OBJECT, ParticleUniverseEditorFrame::OnCentreObject)
	EVT_MENU(ID_ADD_MESH_TO_SCENE, ParticleUniverseEditorFrame::OnAddMesh)
	EVT_MENU(ID_TOGGLE_LIGHT, ParticleUniverseEditorFrame::OnToggleLight)
	EVT_MENU(ID_REMOVE_MESH_FROM_SCENE, ParticleUniverseEditorFrame::OnRemoveMesh)
	EVT_MENU(ID_OPTIONS, ParticleUniverseEditorFrame::OnOptions)
	EVT_MENU(ID_COMPILE, ParticleUniverseEditorFrame::OnCompile)

	EVT_MENU(wxID_ABOUT, ParticleUniverseEditorFrame::OnAbout)

	EVT_MENU(ID_REWIND, ParticleUniverseEditorFrame::OnRewind)
	EVT_MENU(ID_PLAY, ParticleUniverseEditorFrame::OnPlay)
	EVT_MENU(ID_RECORD, ParticleUniverseEditorFrame::OnRecord)
	EVT_MENU(ID_PAUSE, ParticleUniverseEditorFrame::OnPause)
	EVT_MENU(ID_STOP, ParticleUniverseEditorFrame::OnStop)
	EVT_MENU(ID_WIND, ParticleUniverseEditorFrame::OnWind)
	EVT_MENU(ID_CLONE, ParticleUniverseEditorFrame::OnClone)
	EVT_MENU(ID_REMOVE, ParticleUniverseEditorFrame::OnRemove)
END_EVENT_TABLE()

IMPLEMENT_CLASS(ParticleUniverseEditorFrame, wxFrame)

bool ParticleUniverseEditorFrame::mUsePhysX = false;
//-----------------------------------------------------------------------
// Use MAX_WIDTH to make it extremely wide. The width is corrected later, but it sizes the toolbar correctly.
ParticleUniverseEditorFrame::ParticleUniverseEditorFrame(wxWindow* parent, wxWindowID id)
: wxFrame(parent, 
		  id, 
		  wxT(""),
		  wxDefaultPosition, 
		  wxSize(MAX_WIDTH, MAX_HEIGHT)),
	mUIMainToolbar(0),
	mNotebook(0),
	mListOfTemplates(0),
	mLatestSelection(0),
	mEditNotebookPage(0),
	mMaterialNotebookPage(0),
	mTextCtrl(0),
	mCurrentParticleSystemForRenderer(0),
	mControl(0),
	mSceneManager(0),
	mMainSceneNode(0),
	mParticlerSystemSceneNode(0),
	mMarkerSceneNode(0),
	mMarker(0),
	mMarkerBold(0),
	mNewSystemCounter(1),
	mCurrentParticleScript(ParticleUniverse::StringUtil::BLANK),
	mAutoStartRender(true),
	mPropertyWindow(0),
	mMinMax(false),
	mScriptChanged(false),
	mTemplateSelected(false),
	mSavedFile(WX_STRING_BLANK),
	mConfigDialog(0),
	mLastPositionGizmo(wxPoint(0, 0)),
	mLastPositionAddEntity(wxPoint(0, 0)),
	mLastPositionLight(wxPoint(0, 0)),
	mRecorder(0),
	mVideoPath("media/video"),
	mImageWidth(640.0),
	mImageHeight(480.0),
	mFramesPerSecond(27),
	mFileNameSuffix(".png"),
	mStartTime(0.0f),
	mEndTime(7200.0f),
	mPauseTime(0.05f),
	mAddEntitySceneNode(0),
	mAddEntity(0),
	mGridPlaneSceneNodeBottom(0),
	mGridPlaneSceneNodeX(0),
	mGridPlaneSceneNodeMinX(0),
	mGridPlaneSceneNodeZ(0),
	mGridPlaneSceneNodeMinZ(0),
	mGridBottom(0),
	mGridX(0),
	mGridMinX(0),
	mGridZ(0),
	mGridMinZ(0),
	mLightSceneNode(0),
	mLightEntity(0),
	mLight(0),
	mPhysXNormal(Ogre::Vector3::ZERO),
	mPhysXGravity(Ogre::Vector3::ZERO),
	mPhysXDistance(-200.0f),
	mDebugOverlay(0),
	mOrthoCameraGizmoNode(0),
	mAverageFPS(0),
	mMaxNumberOfEmittedParticles(0),
	mMaxNumberOfVisualParticles(0),
	mTmenu(0),
	mOgreControlSmall(0),
	mControlPanelWithSmallRenderWindow(0),
	mCheckFocusListOfTemplates(false),
	mGizmoManager(0),
	mAnimationWindow(0),
	mStartCameraAnimation(false),
	mStartCameraAnimationDelta(0.05f),
	mTargetCameraAnimationPosition(Ogre::Vector3::ZERO),
	mStartCameraAnimationPosition(Ogre::Vector3::ZERO),
	mDiffCameraAnimationPosition(Ogre::Vector3::ZERO),
	mPreviousPositionAddEntitySceneNode(Ogre::Vector3::ZERO),
	m45DegreesOrtho(true),
	mLanguage(wxLANGUAGE_DEFAULT)
{
	mDefaultCameraPosition = Ogre::Vector3(-1100, 0, -1100);
	wxInitAllImageHandlers();
	Maximize();
	mStatusBar = CreateStatusBar();
	mMainVSizer = new wxBoxSizer(wxVERTICAL); // Main sizer
	mEditSizer = new wxBoxSizer(wxVERTICAL);
	mLBAndPropSizer = new wxBoxSizer(wxVERTICAL); // Sizer for template list + propertywindow
	mSubHSizer = new wxBoxSizer(wxHORIZONTAL);
	mSubHSizer->Add(mLBAndPropSizer, 0, wxGROW|wxALL, 0);
	mMainVSizer->Add(mSubHSizer, 1, wxTOP | wxEXPAND, 28);
	SetSizer(mMainVSizer);

	mSystemListener = new SystemListener(this);
	mLogManager = Ogre::LogManager::getSingletonPtr();
	mLog = mLogManager->createLog("pued.log", true, false, false);
	mLogListener = new LogListener(this);
	mLog->addListener(mLogListener);
	SetAutoLayout(true);
	mBackgroundColour = Ogre::ColourValue(0.3, 0.3, 0.3);

	// Set the language
	mConfigDialog = new ConfigDialog(this);
	mConfigDialog->loadLanguage();
	SetLanguage();

	// Setup the dialog window
	mConfigDialog->initWindow();
	mConfigDialog->loadConfig();

	// Set the name in the title
	SetTitle(
#ifdef PU_FULL_VERSION
		  _("Particle Universe Editor V1.5.1")
#else
		  _("Particle Universe Editor - DEMO V1.5.1")
#endif // PU_FULL_VERSION
	);

	// Create menu
	SetMenu();
}
//-----------------------------------------------------------------------
bool ParticleUniverseEditorFrame::Destroy()
{
	// Delete the droptarget
	delete GetDropTarget();
	SetDropTarget(0);

	// Remove the camera listeners
	mGizmoManager->unInitialiseCameraListeners(_getCamera());

	// Delete the gizmo's
	OGRE_DELETE_T (mGizmoManager, GizmoManager, Ogre::MEMCATEGORY_SCENE_OBJECTS);
	mGizmoManager = 0;

	// Delete the animation window
	mAnimationWindow->detachParticleSystemFromEntity(mAddEntity); // If mAddEntity is 0 or no particle system is attached doesn't mind
	delete mAnimationWindow;
	mAnimationWindow = 0;

	// Destroy overlay
	Ogre::OverlayManager::getSingleton().destroyAll();

	#ifdef PU_PHYSICS_PHYSX
	if (mUsePhysX)
	{
		ParticleUniverse::PhysXBridge::getSingletonPtr()->exitNx();
	}
	#endif //PU_PHYSICS_PHYSX

	ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyParticleSystem(CURRENT_PS_NAME, mSceneManager);
	ParticleUniverse::ParticleSystemManager::getSingletonPtr()->removeAndDestroyDanglingSceneNodes(mParticlerSystemSceneNode);

	delete mRecorder;
	mRecorder = 0;

	mLog->removeListener(mLogListener);
	delete mLogListener;
	mLogListener = 0;
	delete mSystemListener;
	mSystemListener = 0;

	if (mUIMainToolbar)
		delete mUIMainToolbar;

	mEditNotebookPage->destroyDanglingPUComponents();

	return wxFrame::Destroy();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::CreateScene(void)
{
	SetCursor(*wxHOURGLASS_CURSOR);

	// Setup context menu
    mSceneManager = mControl->CreateSceneManager(Ogre::ST_GENERIC);
	_getCamera()->setNearClipDistance(0.1f);
	_getCamera()->setFarClipDistance(99999.0f);

	// Initialise Gizmo's
	mGizmoManager = OGRE_NEW_T (GizmoManager, Ogre::MEMCATEGORY_SCENE_OBJECTS)(mSceneManager);
	mGizmoManager->initialize(mSceneManager, mControl->GetRenderWindow(), mControl->GetViewport());
	mGizmoManager->initialiseCameraListeners(_getCamera());

	// Create the ortho camera gizmo
	mGizmoManager->set45DegreesOrtho(m45DegreesOrtho);
	_setCameraPosition(mDefaultCameraPosition);
	initialiseOrthoCameraGizmo();

	// Create the background
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingletonPtr()->getByName("ParticleUniverseEditor/BackgroundMaterial");
	material->load();
	material->getTechnique(0)->getPass(0)->setAmbient(Ogre::ColourValue(0.3, 0.3, 0.3));
	material->getTechnique(0)->getPass(0)->setDiffuse(Ogre::ColourValue(0.3, 0.3, 0.3));
	material->getTechnique(0)->getPass(0)->setSpecular(Ogre::ColourValue(0.3, 0.3, 0.3));
	mSceneManager->setSkyBox(true, "ParticleUniverseEditor/BackgroundMaterial");
 
	// Create the nodes for the particle system and attach the marker
	mMainSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	mMainSceneNode->showBoundingBox(false); // For testing
	mParticlerSystemSceneNode = mMainSceneNode->createChildSceneNode();
	mParticlerSystemSceneNode->setInheritScale(false); // Don't scale the particle system

	mMarkerSceneNode = mMainSceneNode->createChildSceneNode();
	mMarker = mSceneManager->createEntity("pu_marker_system", "pu_marker.mesh");
	mMarkerBold = mSceneManager->createEntity("pu_bold_marker_system", "pu_bold_marker.mesh");
	mMarker->setMaterialName("pu_marker_system");
	mMarkerBold->setMaterialName("pu_marker_system");
	mMarker->setQueryFlags(GizmoManager::OBJECT_FLAG);
	mMarkerBold->setQueryFlags(GizmoManager::OBJECT_FLAG);
	mMarkerSceneNode->attachObject(mMarker);
	mMarkerSceneNode->attachObject(mMarkerBold);
	mMarkerSceneNode->setScale(0.4, 0.4, 0.4);
	Ogre::Vector3 mainSceneNodePosition = Ogre::Vector3::ZERO;
	mMainSceneNode->_setDerivedPosition(mainSceneNodePosition);

	// Create a scenenode for adding entity
	mAddEntitySceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	mAddEntitySceneNode->_setDerivedPosition(Ogre::Vector3::ZERO);

    mControl->Refresh();
	mNotebook->SetSelection(NOTEBOOK_RENDER);

	// Increase the depthscale for soft particles
	ParticleUniverse::ParticleSystemManager::getSingletonPtr()->setDepthScale(10.0);

	// Create Recorder
	mRecorder = new Recorder(_getCamera());
	mRecorder->setFileNameSuffix(mFileNameSuffix);
	mRecorder->setFramesPerSecond(mFramesPerSecond);
	mRecorder->setPauseTime(mPauseTime);
	mRecorder->setVideoDirectory(mVideoPath);
	mRecorder->setImageWidth(mImageWidth, false); // Don't allocate video memory already
	mRecorder->setImageHeight(mImageHeight); // Only allocate video memory once (setting w/h both reallocate memory by default)
	mRecorder->setBackgroundColour(mBackgroundColour);
	mRecorder->setFilter(Recorder::IF_NONE);

	// Check on PhysX installation (must be done before the config is read, so the PhysX tab can be created (or not))
#ifdef PU_PHYSICS_PHYSX
	if (mConfigDialog->isDisplayPhysXStartupDialog())
	{
		wxString physXmessage;
		if (isPhysXInstalled())
		{
			physXmessage = _("It seems that PhysX is installed on this computer. Do you want to make use of the PhysX features?");
		}
		else
		{
			physXmessage = _("It seems that PhysX is NOT installed on this computer. Do you still want to try to use PhysX?");
		}
		mUsePhysX = (showMessage(physXmessage, wxOK | wxCANCEL) == wxID_OK);
		mConfigDialog->setDisplayPhysXStartupDialog(false);
		mConfigDialog->setUsePhysXIfNoStartupDialog(mUsePhysX);
		mConfigDialog->saveConfig();
	}
	else
	{
		mUsePhysX = mConfigDialog->isUsePhysXIfNoStartupDialog();
	}
#endif //PU_PHYSICS_PHYSX

	// Get the default values from the config
	applyGeneralConfigOptions();

	// Set the camera position
	_setCameraPosition(mDefaultCameraPosition);
	_cameraLookAt(Ogre::Vector3(0, 0, 0));

	// Read resource locations
	ParticleUniverse::StringVector vec = mConfigDialog->getResourceLocations();
	if (!vec.empty())
	{
		ParticleUniverse::StringVector::iterator it;
		ParticleUniverse::StringVector::iterator itEnd = vec.end();
		for (it = vec.begin(); it != itEnd; ++it)
		{
			Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation((*it), "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		}
		Ogre::ResourceGroupManager::getSingletonPtr()->clearResourceGroup(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyAllParticleSystemTemplates();
	}

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(); // Initialise Ogre

#ifdef PU_PHYSICS_PHYSX
	// Start the PhysX engine ... wroooooom
	applyPhysXConfigOptions();
	if (mUsePhysX)
	{
		ParticleUniverse::PhysXBridge::getSingletonPtr()->initNx(-200); // Initialise with gravity y = -200
		ParticleUniverse::PhysXBridge::getSingletonPtr()->createPlane(mPhysXNormal, mPhysXDistance);
		ParticleUniverse::PhysXBridge::getSingletonPtr()->setGravity(mPhysXGravity); // Override gravity from config file
		ParticleUniverse::PhysXBridge::getSingletonPtr()->getScene()->setActorGroupPairFlags(100, 100, NX_NOTIFY_ON_START_TOUCH);
		ParticleUniverse::PhysXBridge::getSingletonPtr()->getScene()->setActorGroupPairFlags(100, 0, NX_NOTIFY_ON_START_TOUCH);
	}
	else
	{
		// Remove the PhysX factories to prevent new script to be parsed
		ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyParticleSystem(CURRENT_PS_NAME, mSceneManager);
	}
#endif //PU_PHYSICS_PHYSX

	// Get current dir
	CURRENT_DIR;

	// Add some stuff to the scene
	createGridPlane(mConfigDialog->getGridPlaneDistance());

	if (!mConfigDialog->isGridPlane())
	{
		showGridPlane(false);
	}
	createStatisticsOverlay();
	if (!mConfigDialog->isStatistics())
	{
		showOverlay(false);
	}
	createLight();
	if (!mConfigDialog->isLightCheck())
	{
		showLight(false);
	}

	// Register nodes at GizmoManager
	mGizmoManager->registerNode(mAddEntitySceneNode); // Gizmo's can be attached to mAddEntitySceneNode
	mGizmoManager->registerNode(mLightSceneNode); // Gizmo's can be attached to mLightSceneNode
	mGizmoManager->registerNode(mMainSceneNode); // Gizmo's can be attached to mMainSceneNode
	mGizmoManager->registerHideWhenAttached(Gizmo::GIZMO_MOVE, mMarker); // Hide mMarker entity when the Move gizmo is attached
	mGizmoManager->registerHideWhenAttached(Gizmo::GIZMO_SCALE, mMarker); // Hide mMarker entity when the Scale gizmo is attached (not used for now)
	mGizmoManager->registerIgnoreGizmoWhenSelected(mLightSceneNode, Gizmo::GIZMO_ROTATE); // Do not allow rotation of the light, because it doesn't make sense
	mGizmoManager->registerIgnoreGizmoWhenSelected(mLightSceneNode, Gizmo::GIZMO_SCALE); // Do not scaling of the light, because it doesn't make sense
	mGizmoManager->registerIgnoreGizmoWhenSelected(mMainSceneNode, Gizmo::GIZMO_SCALE); // Do not allow scaling of the particle system
	mGizmoManager->registerNodeForUpdatingScale(mMainSceneNode, Ogre::Vector3(2.0f, 2.0f, 2.0f)); // Scale the mMainSceneNode similar to the gizmo, while zooming and moving
	mGizmoManager->registerNodeForUpdatingScale(mLightSceneNode, Ogre::Vector3(2.0f, 2.0f, 0.0f)); // Scale the light
	mGizmoManager->registerNodeForFacingCamera(mLightSceneNode); // Align the light to the camera
	mGizmoManager->registerNodeForMakingVisibleAfterDetach(mMarkerSceneNode); // This scenenode is the only one with entities that are partly visible
	mGizmoManager->attachToNode(mMainSceneNode);

	// Create the animation window
	mAnimationWindow = new AnimationWindow(this, mSceneManager);

	// Check keypresses
	mControl->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(ParticleUniverseEditorFrame::OnKeyPressed), NULL, this);
	mListOfTemplates->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(ParticleUniverseEditorFrame::OnKeyPressed), NULL, this);
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(ParticleUniverseEditorFrame::OnKeyPressed));

	// Reset Gizmo
	SetCursor(wxNullCursor);

	// TESTTESTTESTTESTTEST
//	Procedural::Root::getInstance()->sceneManager = mSceneManager;
//	const Ogre::MeshPtr torusMesh = Procedural::TorusGenerator().setNumSegCircle(50).setRadius(3.f).setSectionRadius(0.04f).setUTile(1.).setVTile(1.).realizeMesh("torusMesh");
//	const Ogre::Mesh* torusMeshPointer = torusMesh.getPointer();
//	Ogre::MeshSerializer meshSerializer;
//	meshSerializer.exportMesh(torusMeshPointer, "torus.mesh");
	// TESTTESTTESTTESTTEST
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::SetLanguage(void)
{
	// Internationalization init
	mLanguage = mConfigDialog->getLanguage();

	// Check if the language is available
	if (mLanguage != wxLANGUAGE_DEFAULT)
	{
        // Use English as locale, because somehow setting the actual language gives a problem (particle system templates are not created
		// properly and the grid material is white). Therefore, the different directories with the .mo files are not used, but a 
		// catalog is added prefixes with the language code
		if (mLocale.Init(wxLANGUAGE_ENGLISH))
		{
			// To be sure that if the exe is not at the root path, the proper location is searched
			wxLocale::AddCatalogLookupPathPrefix(wxT("language"));
		
			// Initialize the catalogs
			wxString lang = mConfigDialog->getLanguageAsString(mLanguage);
			mLocale.AddCatalog(lang + wxT("_internat"));
			mLocale.AddCatalog(lang + wxT("_system"));
		}
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::CreateToolbar(void)
{
	// Create the toolbar with icons
	mUIMainToolbar = new UIMainToolbar(this);
	SetInitialSize(wxSize(APP_WIDTH, APP_HEIGHT)); // Hack to reset the Frame again (needed for toolbar).
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::CreateListOfTemplates(void)
{
	mListOfTemplates = new SystemTreeControl(this, ID_LISTOF_TEMPLATES, wxPoint(0, TAB_POS_Y), wxSize(LB_TEMPLATES_WIDTH, LB_TEMPLATES_HEIGHT));
	resetLeftSideWindow(); // Fills the listbox at the left side.
}
//------------------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnMouseMoveCallback(wxMouseEvent& event)
{
	if (mStartCameraAnimation) // Only check on the ortho camera gizmo if camera animation is stopped
		return;

	// A mouse event is retrieved from the Ogre component, which performs a callback to the ParticleUniverseEditorFrame
	if (event.GetEventType() == wxEVT_LEAVE_WINDOW)
	{
		// Remove the context menu and return (no further actions)
		doWindowLeave(event);
		return;
	}

	// V1.5
	wxPoint position = event.GetPosition();
	if (event.LeftIsDown())
	{
		if (event.Dragging())
		{
			// Drag with the mouse while pressing the left mouse button.
			// Depending on the Gizmo it is a rotation or movement
			Ogre::Vector2 pos(position.x, position.y);
			if (mGizmoManager)
			{
				mGizmoManager->action(pos, _getCamera());
			}
		}
		else
		{
			// Just pressing the left mouse button to select an object (gizmo, entity, light)
			doMouseButtonPressed(event);
		}
		mLastPositionGizmo = position;
		restoreFocus();
	}
	else if (event.RightIsDown())
	{
		if (event.Dragging())
		{
			if (_isCameraOrthographic())
			{
				// Don't allow dragging in ortho mode
				return;
			}
			Ogre::Vector3 pivot = getCurrentPivot();
			Ogre::Vector3 cameraPivotVector = _getCameraPosition() - pivot;
			Ogre::Real yawDiff = position.x - mLastPositionGizmo.x;
			Ogre::Real pitchDiff = mLastPositionGizmo.y - position.y;
			Ogre::Real scaleFactor = 0.3f;
			yawDiff *= scaleFactor; // Reduce speed yaw
			Ogre::Real length = cameraPivotVector.length();
			length = length < 0.0001f ? 0.0001f : length;
			scaleFactor = 0.5f / length;
			pitchDiff *= scaleFactor; // Reduce speed pitch
			Ogre::Radian yaw = Ogre::Radian(Ogre::Degree(yawDiff));
			Ogre::Radian pitch = Ogre::Radian(Ogre::Degree(pitchDiff));

			Ogre::Vector3 cameraPosition = _getCameraPosition();
			Ogre::Quaternion rotationY(yaw, Ogre::Vector3::UNIT_Y); // Rotation around the y-axis
			Ogre::Quaternion rotationX(pitch, cameraPivotVector.crossProduct(Ogre::Vector3::UNIT_Y)); // Rotation around axis perpendicular with the pivot-camera axis
			cameraPosition = pivot + rotationX * rotationY * cameraPivotVector; //  Calculate the new position
			_setCameraPosition(cameraPosition);
			_cameraLookAt(pivot);
		}
		mLastPositionGizmo = position;
		restoreFocus();
	}
	else if (event.LeftUp())
	{
		mGizmoManager->getAttachedNode()->showBoundingBox(false); // Reset after scaling
	}
	else if (event.GetWheelRotation() > 0)
	{
		// Zoom out
		zoom(1.05f);
	}
	else if (event.GetWheelRotation() < 0)
	{
		// Zoom in
		zoom(0.95f);
	}
	if (event.LeftDClick())
	{
		// Double click left. If an object is selected, select the material
		if (mGizmoManager && mGizmoManager->isMovableObjectSelected(mAddEntity))
		{
			selectMaterialForAddedMesh();
		}
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::zoom(Ogre::Real zoomFactor)
{
	Ogre::Vector3 cameraPosition = _getCameraPosition();
	Ogre::Vector3 pivot = getCurrentPivot();
	Ogre::Vector3 direction = cameraPosition - pivot;
	if (_isCameraOrthographic())
	{
		// Adjust the ortho window size
		_getCamera()->setOrthoWindowWidth(zoomFactor * cameraPosition.length());
	}
	_setCameraPosition(pivot + zoomFactor * direction);
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doMouseButtonPressed(wxMouseEvent& event)
{
	if (!mGizmoManager)
		return;

	// Determine which object in the scene was selected
	wxPoint pos = event.GetPosition();
	mGizmoManager->startSelect(mSceneManager, _getCamera(), Ogre::Vector2(pos.x + 0.002 * pos.x, pos.y + 0.002 * pos.y)); // Slightly offset

	// Determine whether the camera is set to ortho mode and that the ortho camera gizmo was selected
	if (mGizmoManager->isOrthoCameraGizmoSelected() && _isCameraOrthographic())
	{
		Ogre::Vector3 newCameraPosition = _getCameraPosition();
		Ogre::Real l = newCameraPosition.length();
		Ogre::Real sqrt0dot5 = sqrt(0.5);

		// The ortho camera gizmo has been selected
		switch (mGizmoManager->getOrthoCameraAxisSelection())
		{
			case Gizmo::AXIS_X:
				{
					if (m45DegreesOrtho)
					{
						setOrthoGridVisible(true, Gizmo::AXIS_Y);
						Ogre::Quaternion q;
						if (newCameraPosition.z > 0)
						{
							q = Ogre::Quaternion(0.9238795325112867f, 0.0f, 0.3826834323650897f, 0.0f);
						}
						else
						{
							q = Ogre::Quaternion(0.9238795325112867f, 0.0f, -0.3826834323650897f, 0.0f);
						}
						newCameraPosition = q * newCameraPosition;
						if (newCameraPosition.x > -2.0f && newCameraPosition.x < 2.0f)
							newCameraPosition.x = 0.0f;
						if (newCameraPosition.z > -2.0f && newCameraPosition.z < 2.0f)
							newCameraPosition.z = 0.0f;
					}
					else
					{
						// X-axis view
						setOrthoGridVisible(true, Gizmo::AXIS_X);
						newCameraPosition.x = -l;
						newCameraPosition.y = 0;
						newCameraPosition.z = 0;

					}
					startCameraAnimation(newCameraPosition);
				}
				break;

			case Gizmo::AXIS_MIN_X:
				{
					if (m45DegreesOrtho)
					{
						setOrthoGridVisible(true, Gizmo::AXIS_Y);
						Ogre::Quaternion q;
						if (newCameraPosition.z < 0)
						{
							q = Ogre::Quaternion(0.9238795325112867f, 0.0f, 0.3826834323650897f, 0.0f);
						}
						else
						{
							q = Ogre::Quaternion(0.9238795325112867f, 0.0f, -0.3826834323650897f, 0.0f);
						}
						newCameraPosition = q * newCameraPosition;
						if (newCameraPosition.x > -2.0f && newCameraPosition.x < 2.0f)
							newCameraPosition.x = 0.0f;
						if (newCameraPosition.z > -2.0f && newCameraPosition.z < 2.0f)
							newCameraPosition.z = 0.0f;
					}
					else
					{
						// -X-axis view
						setOrthoGridVisible(true, Gizmo::AXIS_MIN_X);
						newCameraPosition.x = l;
						newCameraPosition.y = 0;
						newCameraPosition.z = 0;
					}
					startCameraAnimation(newCameraPosition);
				}
				break;

			case Gizmo::AXIS_Y:
				{
					if (m45DegreesOrtho)
					{
						// Do nothing
					}
					else
					{
						// Y-axis view
						setOrthoGridVisible(true, Gizmo::AXIS_Y);
						newCameraPosition.x = 0;
						newCameraPosition.y = l;
						newCameraPosition.z = 0;
						startCameraAnimation(newCameraPosition);
					}
				}
				break;

			case Gizmo::AXIS_Z:
				{
					if (m45DegreesOrtho)
					{
						setOrthoGridVisible(true, Gizmo::AXIS_Y);
						Ogre::Quaternion q;
						if (newCameraPosition.x < 0)
						{
							q = Ogre::Quaternion(0.9238795325112867f, 0.0f, 0.3826834323650897f, 0.0f);
						}
						else
						{
							q = Ogre::Quaternion(0.9238795325112867f, 0.0f, -0.3826834323650897f, 0.0f);
						}
						newCameraPosition = q * newCameraPosition;
						if (newCameraPosition.x > -2.0f && newCameraPosition.x < 2.0f)
							newCameraPosition.x = 0.0f;
						if (newCameraPosition.z > -2.0f && newCameraPosition.z < 2.0f)
							newCameraPosition.z = 0.0f;
					}
					else
					{
						// Z-axis view
						setOrthoGridVisible(true, Gizmo::AXIS_Z);
						newCameraPosition.x = 0;
						newCameraPosition.y = 0;
						newCameraPosition.z = -l;
					}
					startCameraAnimation(newCameraPosition);
				}
				break;

			case Gizmo::AXIS_MIN_Z:
				{
					if (m45DegreesOrtho)
					{
						setOrthoGridVisible(true, Gizmo::AXIS_Y);
						Ogre::Quaternion q;
						if (newCameraPosition.x > 0)
						{
							q = Ogre::Quaternion(0.9238795325112867f, 0.0f, 0.3826834323650897f, 0.0f);
						}
						else
						{
							q = Ogre::Quaternion(0.9238795325112867f, 0.0f, -0.3826834323650897f, 0.0f);
						}
						newCameraPosition = q * newCameraPosition;
						if (newCameraPosition.x > -2.0f && newCameraPosition.x < 2.0f)
							newCameraPosition.x = 0.0f;
						if (newCameraPosition.z > -2.0f && newCameraPosition.z < 2.0f)
							newCameraPosition.z = 0.0f;
					}
					else
					{
						// -Z-axis view
						setOrthoGridVisible(true, Gizmo::AXIS_MIN_Z);
						newCameraPosition.x = 0;
						newCameraPosition.y = 0;
						newCameraPosition.z = l;
					}
					startCameraAnimation(newCameraPosition);
				}
				break;
		}
	}
	else if (!mGizmoManager->isGizmoSelected())
	{
		// No gizmo selected. Validate if it was something else
		if (mGizmoManager->isMovableObjectSelected(mMarker))
		{
			mGizmoManager->attachToNode(mMainSceneNode);
			_cameraLookAt(mMainSceneNode->_getDerivedPosition());
		}
		else if (mGizmoManager->isMovableObjectSelected(mMarkerBold))
		{
			mGizmoManager->attachToNode(mMainSceneNode);
			_cameraLookAt(mMainSceneNode->_getDerivedPosition());
		}
		else if (mGizmoManager->isMovableObjectSelected(mAddEntity))
		{
			mGizmoManager->attachToNode(mAddEntitySceneNode);
			_cameraLookAt(mAddEntitySceneNode->_getDerivedPosition());
		}
		else if (mGizmoManager->isMovableObjectSelected(mLightEntity))
		{
			mGizmoManager->attachToNode(mLightSceneNode);
			_cameraLookAt(mLightSceneNode->_getDerivedPosition());
		}
	}
	mGizmoManager->endSelect(mSceneManager);
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::startCameraAnimation(const Ogre::Vector3& targetPosition)
{
	mStartCameraAnimation = true;
	mStartCameraAnimationPosition = _getCameraPosition();
	mTargetCameraAnimationPosition = targetPosition;
	mDiffCameraAnimationPosition = mTargetCameraAnimationPosition - mStartCameraAnimationPosition;
	Ogre::Vector3 delta = 0.001 * mDiffCameraAnimationPosition;
	mStartCameraAnimationDelta = std::max(Ogre::Math::Abs(delta.x), Ogre::Math::Abs(delta.y));
	mStartCameraAnimationDelta = std::max(mStartCameraAnimationDelta, Ogre::Math::Abs(delta.z));
	mStartCameraAnimationDelta += 0.03f; // May never be 0
	mCameraAnimationTimer.reset();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnAddMesh(wxCommandEvent& event)
{
	doAddMesh();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doAddMesh(void)
{
	// Open dialog for mesh selection
	MeshDialog meshDialog;
	if (meshDialog.openDialog(this) != ParticleUniverse::StringUtil::BLANK)
	{
		doAddMesh(meshDialog.getMeshName());
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doAddMesh(const Ogre::String& meshName)
{
	if (mAddEntitySceneNode && mSceneManager)
	{
		if (mAddEntity)
		{
			// First delete the existing entity
			if (mAnimationWindow)
			{
				mAnimationWindow->notifyMeshDeleted(mAddEntity);
			}
			mAddEntitySceneNode->detachAllObjects();
			mSceneManager->destroyEntity(mAddEntity);
			mAddEntity = 0;
		}

		// Add the new entity
		mAddEntity = mSceneManager->createEntity("pu_add_entity", meshName);
		mAddEntity->setQueryFlags(GizmoManager::OBJECT_FLAG);
		if (mAnimationWindow)
		{
			mAnimationWindow->loadAnimationNames(mAddEntity); // Always call, because this also clears the list
			mAnimationWindow->loadBoneNames(mAddEntity); // Displays bone names, if available
			mAnimationWindow->displayMeshInfo(mAddEntity); // Displays additional info
		}
		mAddEntitySceneNode->attachObject(mAddEntity);
		mAddEntitySceneNode->_setDerivedOrientation(Ogre::Quaternion::IDENTITY);
		mAddEntitySceneNode->setScale(Ogre::Vector3::UNIT_SCALE);
		if (mUIMainToolbar)
		{
			mUIMainToolbar->getMiscIcons()->enableRemoveEntity(true);
		}
		if (mGizmoManager)
		{
			// Reset the Gizmo orientation to the orientation of the node to which the entity is attached
			mGizmoManager->setOrientation(mAddEntitySceneNode->getOrientation());
			mGizmoManager->setScale(Ogre::Vector3::UNIT_SCALE);
		}
		setLeftSideWindow(mAnimationWindow);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnToggleLight(wxCommandEvent& event)
{
	doToggleLight();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doToggleLight(void)
{
	if (mLightEntity)
	{
		if (mLightEntity->isVisible())
		{
			mLightEntity->setQueryFlags(0);
			if (mGizmoManager->isAttachedToNode(mLightSceneNode))
			{
				// Attach the gizmo to the particle system, because the light becomes invisible
				mGizmoManager->attachToNode(mMainSceneNode);
				_cameraLookAt(mMainSceneNode->_getDerivedPosition());
			}
			else
			{
				_cameraLookAt(mLightSceneNode->_getDerivedPosition());
			}
		}
		else
		{
			// The light is visible again, so set the flag
			mLightEntity->setQueryFlags(GizmoManager::OBJECT_FLAG);
		}
		showLight(!mLightEntity->isVisible());
	}
	else
	{
		createLight();
		showLight(true);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnRemoveMesh(wxCommandEvent& event)
{
	doRemoveMesh();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doRemoveMesh(void)
{
	if (mAddEntitySceneNode && mAddEntity && mSceneManager)
	{
		// Remove the animation window
		mAnimationWindow->notifyMeshDeleted(mAddEntity);
		resetLeftSideWindow();

		// Attach the gizmo to the particle system, because the mesh is removed
		if (mGizmoManager->isAttachedToNode(mAddEntitySceneNode))
		{
			if (mGizmoManager->getGizmoType() == Gizmo::GIZMO_SCALE)
			{
				// Set the type to move, because scaling the particle system is not possible
				mGizmoManager->setGizmoType(Gizmo::GIZMO_MOVE);
				mGizmoManager->attachToNode(mMainSceneNode);
				_cameraLookAt(mMainSceneNode->_getDerivedPosition());
			}
			else
			{
				// Just add to the main scenenode
				mGizmoManager->attachToNode(mMainSceneNode);
				_cameraLookAt(mMainSceneNode->_getDerivedPosition());
			}
		}

		// First delete the existing entity
		mAddEntitySceneNode->detachAllObjects();
		mSceneManager->destroyEntity(mAddEntity);
		mAddEntity = 0;
		if (mUIMainToolbar)
		{
			mUIMainToolbar->getMiscIcons()->enableRemoveEntity(false);
		}

		// Clear the animation info
		mAnimationWindow->displayMeshInfo(0);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::selectMaterialForAddedMesh(void)
{
	// Open dialog for material selection
	MaterialDialog materialDialog;
	if (materialDialog.openDialog(this) != ParticleUniverse::StringUtil::BLANK)
	{
		// Material selected
		if (mAddEntity)
		{
			// Set the material
			mAddEntity->setMaterialName(materialDialog.getMaterialName());
			mAnimationWindow->displayMeshInfo(mAddEntity);
		}
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::setPropertyWindow(PropertyWindow* propertyWindow)
{
	if (!propertyWindow)
		return;

	if (IsBeingDeleted())
	{
		return;
	}

	// Hide it by default
	propertyWindow->Hide();

	if (mPropertyWindow)
	{
		// Remove the old one
		mLBAndPropSizer->Clear();
		mLBAndPropSizer->Add(mListOfTemplates, 2, wxTOP);
		mLBAndPropSizer->AddSpacer(4);
		mPropertyWindow->Hide();
	}

	mLBAndPropSizer->Add(propertyWindow, 2, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	propertyWindow->ClearSelection(); // To force update of the last selected property
	mPropertyWindow = propertyWindow; // Assign the passed property window

	/** Only in case the edittab is displayed, show the property window and also only in case there is no check on the focus of mListOfTemplates.
		The check on mCheckFocusListOfTemplates is added, because after a particle system has been selected, new edit components are created.
		Every creation of an edit component results in creation of a property window, which is displayed, causing a flashing effect. As long as
		mCheckFocusListOfTemplates is true, the property windows should not be displayed.
	*/
	if (mNotebook && mNotebook->GetSelection() == NOTEBOOK_EDIT && !mCheckFocusListOfTemplates)
	{
		propertyWindow->Show();
	}
	mLBAndPropSizer->Layout();
	Layout(); // Aligns the property window correct
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::setPropertyWindow(void)
{
	if (mPropertyWindow)
	{
		mLBAndPropSizer->Clear();
		mLBAndPropSizer->Add(mListOfTemplates, 2, wxTOP);
		mLBAndPropSizer->AddSpacer(4);
		mLBAndPropSizer->Add(mPropertyWindow, 2, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
		mPropertyWindow->Show();
		mPropertyWindow->Fit();
		mLBAndPropSizer->Layout();
		Layout();
	}
}
//-----------------------------------------------------------------------
PropertyWindow* ParticleUniverseEditorFrame::getPropertyWindow(void)
{
	return mPropertyWindow;
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::hidePropertyWindow(void)
{
	if (mPropertyWindow)
	{
		mPropertyWindow->Hide();
		mLBAndPropSizer->Layout();
		Layout();
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::removePropertyWindow(wxPropertyGrid* propertyWindow)
{
	if (IsBeingDeleted())
		return;

	if (!propertyWindow)
		return;

	if (mPropertyWindow)
	{
		propertyWindow->Hide();
		mLBAndPropSizer->Clear();
		mLBAndPropSizer->Add(mListOfTemplates, 2, wxTOP);
		mLBAndPropSizer->AddSpacer(4);
		mPropertyWindow = 0;
		mLBAndPropSizer->Layout();
		Layout();
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::setLeftSideWindow(wxWindow* window)
{
	if (window)
	{
		mLBAndPropSizer->Clear();
		mLBAndPropSizer->Add(mListOfTemplates, 2, wxTOP);
		mLBAndPropSizer->AddSpacer(4);
		mLBAndPropSizer->Add(window, 2, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
		window->Show();
		mListOfTemplates->Fit();
		window->Fit();
		mLBAndPropSizer->Layout();
		Layout();
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::removeLeftSideWindow(wxWindow* window)
{
	if (window)
	{
		window->Hide();
		mLBAndPropSizer->Clear();
		mLBAndPropSizer->Add(mListOfTemplates, 2, wxTOP);
		mLBAndPropSizer->AddSpacer(4);
		mListOfTemplates->Fit();
		mLBAndPropSizer->Layout();
		Layout();
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::resetLeftSideWindow()
{
	if (mAnimationWindow)
	{
		mAnimationWindow->Hide();
	}
	if (mPropertyWindow)
	{
		mPropertyWindow->Hide();
	}
	mLBAndPropSizer->Clear();
	mLBAndPropSizer->Add(mListOfTemplates, 2, wxTOP);
	mLBAndPropSizer->AddSpacer(4);
	mLBAndPropSizer->AddStretchSpacer(2);
	mListOfTemplates->Fit();
	mLBAndPropSizer->Layout();
	Layout();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnHelp(void)
{
	if (mPropertyWindow)
	{
		// Start the browser
		wxString url = CURRENT_DIR + SCRIPT_DIR + mPropertyWindow->getHelpHtml();
		// ShellExecute(NULL, wxT("open"), url.c_str(), NULL, NULL, SW_SHOWNORMAL);
		mEditNotebookPage->getEditCanvas()->SetFocus(); // Needed to prevent exception
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::LoadListOfTemplates()
{
	// Fill the treestructure with particle system template names
	mListOfTemplates->Freeze();
	ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
	ParticleUniverse::vector<ParticleUniverse::String> names;
	particleSystemManager->particleSystemTemplateNames(names);
	ParticleUniverse::vector<ParticleUniverse::String>::iterator it;
	ParticleUniverse::vector<ParticleUniverse::String>::iterator itEnd = names.end();
	mListOfTemplates->DeleteAllItems(); // Clear the tree
	ROOT_NODE_NAME = _("Particle Systems");
	DEFAULT_CATEGORY_NAME = _("General");
	wxTreeItemId rootNode = mListOfTemplates->AddRoot(ROOT_NODE_NAME, 0);
	wxTreeItemId generalCategoryNode = mListOfTemplates->AppendItem(rootNode, DEFAULT_CATEGORY_NAME, 1); // Add the general node by default
	for (it = names.begin(); it != itEnd; ++it)
	{
		bool append = true;

		// Check on particle systems with a PhysX type and don't append them to the list
		// This must always be done, even if PU_PHYSICS_PHYSX is not defined.
		ParticleUniverse::ParticleSystem* pSys = particleSystemManager->getParticleSystemTemplate(*it);
		if (!mUsePhysX)
		{
			append = !(pSys && (pSys->hasExternType("PhysXActor") || pSys->hasExternType("PhysXFluid")));
		}

		if (append)
		{
			//	Get category from the template.
			wxString categoryName;
			if (pSys)
			{
				categoryName = ogre2wx(pSys->getCategory());
			}
			mListOfTemplates->addItem(categoryName, ogre2wx(*it), false);
		}
	}
	
	mListOfTemplates->sortAll();
	mUIMainToolbar->getFileIcons()->reset(mListOfTemplates->GetCount());
	mUIMainToolbar->getCompileIcons()->disableAll();
	resetPlayIcons();
	mListOfTemplates->expandRootOnly();
	wxTreeItemId item = mListOfTemplates->setToFirstParticleSystem();
	if (item.IsOk())
	{
		mLatestSelection = item;
	}
	else
	{
		// There is no item, so return to the root
		mLatestSelection = mListOfTemplates->GetRootItem();
		mListOfTemplates->SelectItem(mLatestSelection);
	}
	OnTemplatesClick((wxCommandEvent)0);
	applyTextConfigOptions();
	mListOfTemplates->Thaw();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::CreateTabs()
{
	// Set to max values to be able to size
	mNotebook = new wxNotebook(this, wxID_ANY, wxPoint(TAB_POS_X, TAB_POS_Y));

	// Set the drop target
	mNotebook->SetDropTarget(new FileDropTarget(this));

	// Render page (beware that the order of creating the tabs in the notebook in important)
    mControl = new wxOgreControl(mNotebook, ID_TAB_RENDER);
	mControl->setCallbackFrame(this);
    mNotebook->AddPage(mControl, _("Render"), false);

	// Edit page
	wxBoxSizer* split = new wxBoxSizer(wxHORIZONTAL); // Main sizer
	mEditNotebookPage = new EditTab(mNotebook, this);
	mNotebook->AddPage(mEditNotebookPage, _("Edit"), true);
	mControlPanelWithSmallRenderWindow = new wxPanel(this);
	wxPanel* ogreControlSmallPanel = new wxPanel(mControlPanelWithSmallRenderWindow);
	mOgreControlSmall = new OgreControlComponent(ogreControlSmallPanel);
	wxPanel* dividerPanel = new wxPanel(mControlPanelWithSmallRenderWindow);
	split->Add(ogreControlSmallPanel, 2, wxEXPAND);
	split->Add(dividerPanel, 1, wxEXPAND);
	mControlPanelWithSmallRenderWindow->SetSizer(split);

	Ogre::Real editProportion = 0.5f;
	if (mConfigDialog)
	{
		editProportion = mConfigDialog->getEditProportion();
	}
	editProportion *= 100;
	mEditSizer->Add(mNotebook, (int)editProportion, wxEXPAND);
	mEditSizer->Add(mControlPanelWithSmallRenderWindow, (int)(100 - editProportion), wxEXPAND);
	mEditSizer->Layout();
	mSubHSizer->Add(mEditSizer, 1, wxEXPAND);

	Connect(wxEVT_MOVING, wxMoveEventHandler(ParticleUniverseEditorFrame::OnMoveAndSize));
	Connect(wxEVT_MOVE, wxMoveEventHandler(ParticleUniverseEditorFrame::OnMoveAndSize));
	Connect(wxEVT_SIZING, wxMoveEventHandler(ParticleUniverseEditorFrame::OnMoveAndSize));
	Connect(wxEVT_MAXIMIZE, wxSizeEventHandler(ParticleUniverseEditorFrame::OnMinMax));
	Connect(wxEVT_ICONIZE, wxSizeEventHandler(ParticleUniverseEditorFrame::OnMinMax));
	Connect(wxEVT_IDLE, wxIdleEventHandler(ParticleUniverseEditorFrame::OnIdle));

	// Script page
//#ifdef PU_FULL_VERSION
	mTextCtrl = new TextControl(this, mNotebook, ID_TAB_SCRIPT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxHSCROLL | wxTE_RICH);
	mNotebook->AddPage(mTextCtrl, _("Script"), false);
	mTextCtrl->SetFont(wxFont(8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
//#endif // PU_FULL_VERSION

	// Implement Materials page
	mMaterialNotebookPage = new MaterialTab(mNotebook, this);
	mNotebook->AddPage(mMaterialNotebookPage, _("Material"), false);

	Connect(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING, wxCommandEventHandler(ParticleUniverseEditorFrame::OnTabChanging));
	Connect(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxCommandEventHandler(ParticleUniverseEditorFrame::OnTabChanged));
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::LoadTextControl(wxString text)
{
	if (mTextCtrl)
	{
		mTextCtrl->Clear();
		mTextCtrl->AppendText(text);
		mTextCtrl->SetInsertionPoint(0);
		mScriptChanged = false;
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnNewParticleSystem(wxCommandEvent& event)
{
	doNewParticleSystem();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doNewParticleSystem(void)
{
	wxString templateName = wxT("ParticleSystem") + ogre2wx(ParticleUniverse::StringConverter::toString(mNewSystemCounter));
	wxTreeItemId category = mListOfTemplates->getCurrentCategory();
	if (category.IsOk())
	{
		wxTreeItemId newItem = mListOfTemplates->PrependItem(category, templateName);
		if (newItem.IsOk())
		{
			mNewSystemCounter++;
	
			// Create new particle system template. If the name is changed in the (script) editor, the template name is updated.
			ParticleUniverse::ParticleSystem* pSys = ParticleUniverse::ParticleSystemManager::getSingletonPtr()->createParticleSystemTemplate(
				wx2ogre(templateName), 
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	
			// Set a default technique with renderer and emitter
			if (pSys)
			{
				ParticleUniverse::ParticleTechnique* technique = pSys->createTechnique();
				if (technique)
				{
					ParticleUniverse::ParticleEmitter* emitter = technique->createEmitter("Point");
					technique->setRenderer("Billboard");

					// Change some default settings (width, height, depth, velocity) based on the camera distance
					if (_getCamera())
					{
						Ogre::Real distance = (mMainSceneNode->_getDerivedPosition() - _getCameraPosition()).length();
						Ogre::Real dimension = 50.0f * distance / 1500.0f; // the camera distance in versions < 1.5 was about 1500; the default value was set accordingly
						technique->setDefaultWidth(dimension);
						technique->setDefaultHeight(dimension);
						technique->setDefaultDepth(dimension);

						if (emitter)
						{
							ParticleUniverse::DynamicAttributeFixed* dynamicAttributeFixed = PU_NEW_T(ParticleUniverse::DynamicAttributeFixed, ParticleUniverse::MEMCATEGORY_SCENE_OBJECTS)();
							Ogre::Real velocity = 100.0f * distance / 1500.0f; // the camera distance in versions < 1.5 was about 1500; the default value was set accordingly
							dynamicAttributeFixed->setValue(velocity);
							emitter->setDynVelocity(dynamicAttributeFixed);
						}
					}
				}
			}

			mListOfTemplates->SelectItem(newItem);
			OnTemplatesClick((wxCommandEvent)0);
			resetPlayIcons();
		}
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnLoadFile(wxCommandEvent& event)
{
	doLoadFile();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doLoadFile(void)
{
	// Add a new directory and parse all resources in that directory
	wxFileDialog loadDialog(this, 
		_("Load a scriptfile"), 
		wxT(""), 
		wxT(""),
		_("Particle and Alias scripts (*.pu;*.pua)|*.pu;*.pua"),
		wxFD_OPEN, 
		wxDefaultPosition, 
		wxDefaultSize, _("Load"));
	if (loadDialog.ShowModal() == wxID_OK)
	{
		// User has pressed ok, so load the content
		ParticleUniverse::String inputLine;
		ParticleUniverse::String script;
		std::ifstream fpIn;
		fpIn.open(loadDialog.GetPath());
		if (fpIn)
		{
			while (!fpIn.eof())
			{
				getline(fpIn, inputLine);
				script.append(inputLine + "\n");
			}
			fpIn.close();

			// Parse the script
			mLogListener->suppressLogging(false); // Make logging available
			parseScript(script);
			if (!mLogListener->errorsLogged())
			{
				showMessage(_("Compilation OK"), wxOK | wxICON_INFORMATION);
			}
			mLogListener->suppressLogging(true);

			// Update list with templates
			LoadListOfTemplates();
			ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			ParticleUniverse::String name = particleSystemManager->getLastCreatedParticleSystemTemplateName();
			wxTreeItemId item = mListOfTemplates->findParticleSystem(ogre2wx(name));
			if (item.IsOk())
			{
				mListOfTemplates->SelectItem(item);
			}
			OnTemplatesClick((wxCommandEvent)0);
		}
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnLoadResourcePath(wxCommandEvent& event)
{
	doLoadResourcePath();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doLoadResourcePath(void)
{
	// Add a resource location which contains particle universe scripts
	wxString dir = wxDirSelector(_("Select a directory that contains *.pu and .pua scripts"));
	if (!dir.empty())
	{
		SetCursor(*wxHOURGLASS_CURSOR);

		Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(wx2ogre(dir), "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		Ogre::ResourceGroupManager::getSingletonPtr()->clearResourceGroup(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyAllParticleSystemTemplates();
		Ogre::ResourceGroupManager::getSingletonPtr()->initialiseResourceGroup(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		LoadListOfTemplates();
		addResourcePathToConfig(dir);
		SetCursor(wxNullCursor);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::addResourcePathToConfig(wxString& resourcePath)
{
	if (mConfigDialog)
	{
		ParticleUniverse::String dir = wx2ogre(resourcePath);
		mConfigDialog->addToDirList(dir);
		mConfigDialog->saveConfig();
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnSave(wxCommandEvent& event)
{
	doSave();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doSave(void)
{
#ifdef PU_FULL_VERSION
	// Save the current particle system as a .pu file

	// First synchronise everything
	synchroniseScriptWithEditTab();

	if (mSavedFile == WX_STRING_BLANK)
	{
		wxString filename = mListOfTemplates->GetItemText(mListOfTemplates->GetSelection());
		wxFileDialog saveDialog(this, 
			_("Save the script"), 
			wxT(""), 
			filename,
			_("Particle and Alias scripts (*.pu;*.pua)|*.pu;*.pua"),
			wxFD_SAVE | wxFD_OVERWRITE_PROMPT, 
			wxDefaultPosition, 
			wxDefaultSize, _("Save"));
		if (saveDialog.ShowModal() == wxID_OK)
		{
			// User has pressed ok, so save the content of mTextCtrl
			mSavedFile = saveDialog.GetPath();
		}
	}

	mTextCtrl->SaveFile(mSavedFile);
	mScriptChanged = false;
	mEditNotebookPage->setSystemUpdatedByEditPage(false);

#endif // PU_FULL_VERSION
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnSaveAs(wxCommandEvent& event)
{
	doSaveAs();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doSaveAs(void)
{
#ifdef PU_FULL_VERSION

	// First synchronise everything
	synchroniseScriptWithEditTab();

	// Save the current particle system as a .pu file
	wxFileDialog saveDialog(this, 
		_("Save the script as"), 
		wxT(""), 
		wxT(""),
		_("Particle and Alias scripts (*.pu;*.pua)|*.pu;*.pua"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT, 
		wxDefaultPosition, 
		wxDefaultSize, _("Save As"));
	if (saveDialog.ShowModal() == wxID_OK)
	{
		// User has pressed ok, so save the content of mTextCtrl
		mTextCtrl->SaveFile(saveDialog.GetPath());
		mScriptChanged = false;
		mEditNotebookPage->setSystemUpdatedByEditPage(false);
	}
#endif // PU_FULL_VERSION
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnMoveAndSize(wxMoveEvent& event)
{
	if (IsBeingDeleted())
	{
		return;
	}

	// Also move the MDI parent
	if (mEditNotebookPage && mNotebook->GetSelection() == NOTEBOOK_EDIT)
	{
		Layout();
		mEditNotebookPage->Freeze(); // To prevent flickering
		mEditNotebookPage->adjustPosition();
		mEditNotebookPage->ActivateNext(); // Needed to set the focus on the MDI window and be able to select an MDI child
		mEditNotebookPage->Thaw();
		mMinMax = true; // Only for restoring the frame; this causes the MDI window to reposition after a restore
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnMinMax(wxSizeEvent& event)
{
	if (IsBeingDeleted())
	{
		/** This was done in case a wxEVT_ACTIVATE was generated, which is also generated if the window is closed. This would cause a runtime error. 
			The underlying windows were already deleted and mEditNotebookPage became an invalid (bad) pointer.
			Later on, the connection with wxEVT_ACTIVATE was removed, because setting focus on the property window was not possible.
			This code stays in just in case.
		*/
		return;
	}

	if (mEditNotebookPage && mNotebook->GetSelection() == NOTEBOOK_EDIT)
	{
		/** This is a two-step situation. The mEditNotebookPage position cannot be updated here, because of some strange ordering (positions are 0, 0).
			So the OnIdle function will reset the mEditNotebookPage after the application has been minimized / maximized.
		*/
		mMinMax = true;
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnIdle(wxIdleEvent& event)
{
	if (IsBeingDeleted())
	{
		return;
	}

	// Update the animation of the mesh
	if (mAnimationWindow)
	{
		// Update the animation (if running)
		mAnimationWindow->updateAnimation();

		if (mAnimationWindow->isParticleSystemAttachedToEntity())
		{
			/** Either the Gizmo follows the orientation of the tagpoint to which the particle system is attached, or the tagpoint follows
				the Gimzo's orientation if a user manipulated the Gizmo
			*/
			if (mGizmoManager)
			{
				mMainSceneNode->setPosition(mAnimationWindow->_getDerivedPositionTagPoint());
				if (mGizmoManager->getAttachedNode() == mMainSceneNode)
				{
					if (mAnimationWindow->isAnimationRunning())
					{
						// The Gizmo must follow the tagpoint
						mGizmoManager->setOrientation(mAnimationWindow->_getDerivedOrientationTagPoint());
					}
					else
					{
						/** If a user has manually taken the action to rotate the Gizmo, the Gizmo must not follow the tagPoint of the Particle
							System, but the other way around; the tagpoint must be re-oriented according to the rotation of the Gizmo
						*/
						if (mAnimationWindow->isOrientationInheritedFromBone())
						{
							// Particle System inherits orientation from the bone to which it is attached
							mAnimationWindow->_setDerivedOrientationTagPoint(mAddEntitySceneNode->getOrientation().Inverse() * mMainSceneNode->getOrientation());
						}
						else
						{
							// Particle System does not inherit orientation from the bone to which it is attached
//							mAnimationWindow->_setDerivedOrientationTagPoint((mAddEntitySceneNode->getOrientation() * 
//								mAnimationWindow->_getDerivedOrientationTagPointParent()).Inverse() * mMainSceneNode->getOrientation());
							mAnimationWindow->setOrientationTagPoint(mAddEntitySceneNode->getOrientation().Inverse() * mMainSceneNode->getOrientation());
						}
					}
				}
				else
				{
					// Set the orientation of the main scene node and not the gizmo (because the Gizmo is attached to something else)
					mMainSceneNode->setOrientation(mAnimationWindow->_getDerivedOrientationTagPoint());
				}
			}
		}
	}

	// Check whether another item from the tree control has been selected,
	// because no event can be catched after a change of selection.
	if (mLatestSelection != mListOfTemplates->GetSelection())
	{
		OnTemplatesClick(wxCommandEvent(0));
	}

	// If resized, update the position
	if (mMinMax)
	{
		mEditNotebookPage->adjustPosition();
		mMinMax = false;
	}

	// Update the overlay
	updateOverlay();

	// Determine whether the focus is on mListOfTemplates. If is isn't, set the focus.
	if (mCheckFocusListOfTemplates)
	{
		wxWindow* win = FindFocus();
		if (win == mListOfTemplates)
		{
			mCheckFocusListOfTemplates = false;
		}
		else
		{
			if (mListOfTemplates)
			{
				mListOfTemplates->SetFocus();
			}
		}
	}
	
	if (mStartCameraAnimation && _isCameraOrthographic())
	{
		Ogre::Real timeScale = 2.0f;
		Ogre::Real timer = mCameraAnimationTimer.getMilliseconds();
		Ogre::Real deltaTime = 0.001 * timeScale * timer; // In seconds
		if (deltaTime >= 1.0f)
		{
			mStartCameraAnimation = false;
		}
		else
		{
			Ogre::Quaternion fromOrientation;
			Ogre::Quaternion toOrientation = mStartCameraAnimationPosition.getRotationTo(mTargetCameraAnimationPosition);
			Ogre::Quaternion newOrientation = Ogre::Quaternion::Slerp(deltaTime, fromOrientation, toOrientation, true);
			_setCameraPosition(newOrientation * mStartCameraAnimationPosition);
		}
		_cameraLookAt(Ogre::Vector3::ZERO);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnQuit(wxCommandEvent& event)
{
	doQuit();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doQuit(void)
{
    Close(true);
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnCompile(wxCommandEvent& event)
{
	doCompile();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doCompile(void)
{
	// Check which page is active
	switch (mNotebook->GetSelection())
	{
		case NOTEBOOK_SCRIPT:
		{
			// This is the script page.
			mLogListener->suppressLogging(false); // Make logging available
			validateAndReparse();

			if (!mLogListener->errorsLogged())
			{
				showMessage(_("Compilation OK"), wxOK | wxICON_INFORMATION);
			}
			mLogListener->suppressLogging(true);
		}
		break;
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnBackgroundColour(wxCommandEvent& event)
{
	doBackgroundColour();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doBackgroundColour(void)
{
	wxColourDialog colourDialog(this);
	if (colourDialog.ShowModal() == wxID_OK)
	{
		wxColourData colourData = colourDialog.GetColourData();
		wxColour c = colourData.GetColour();
		mBackgroundColour.r = ((ParticleUniverse::Real)c.Red())/255.0f;
		mBackgroundColour.g = ((ParticleUniverse::Real)c.Green())/255.0f;
		mBackgroundColour.b = ((ParticleUniverse::Real)c.Blue())/255.0f;
		mBackgroundColour.a = ((ParticleUniverse::Real)c.Alpha())/255.0f;

		// Change the light
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingletonPtr()->getByName("ParticleUniverseEditor/BackgroundMaterial");
		material->getTechnique(0)->getPass(0)->setAmbient(mBackgroundColour);
		material->getTechnique(0)->getPass(0)->setDiffuse(mBackgroundColour);
		material->getTechnique(0)->getPass(0)->setSpecular(mBackgroundColour);
		mSceneManager->setSkyBox(true, "ParticleUniverseEditor/BackgroundMaterial");
		mRecorder->setBackgroundColour(mBackgroundColour);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnCameraReset(wxCommandEvent& event)
{
	doCameraReset();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doCameraReset(void)
{
	// Only if the camera is in persective mode it can be reset
	_setCameraPosition(mDefaultCameraPosition);
	_cameraLookAt(Ogre::Vector3::ZERO);
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnCentreObject(wxCommandEvent& event)
{
	doCentreObject();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doCentreObject(void)
{
	if (mGizmoManager && mGizmoManager->getAttachedNode())
	{
		mGizmoManager->getAttachedNode()->setPosition(Ogre::Vector3::ZERO);
		_cameraLookAt(Ogre::Vector3::ZERO);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnSaveCamera(wxCommandEvent& event)
{
	doSaveCamera();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doSaveCamera(void)
{
	// Save the option settings, including the camera position. Make this the new default camera setting
	mDefaultCameraPosition = _getCameraPosition();
	mConfigDialog->setCameraPosition(mDefaultCameraPosition);
	mConfigDialog->saveConfig();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnCameraProjection(wxCommandEvent& event)
{
	bool perspective = mUIMainToolbar->getMiscIcons()->doToggleCameraProjectionType();
	mGizmoManager->setVisibleOrthoCameraGizmo(!perspective); // If true => perspective; if false => orthographic
	Ogre::Vector3 d = _getCameraPosition(); // Distance from 0, 0, 0 
	Ogre::Real l = d.length();
	if (perspective)
	{
		// Perspective camera
		mUIMainToolbar->getMiscIcons()->enableOrthoCameraSelected(true);
		setOrthoGridVisible(false);
		showGridPlane(true);
		_getCamera()->setProjectionType(Ogre::PT_PERSPECTIVE);
		_getCamera()->setFOVy(Ogre::Radian(0.78539819f));
		mStartCameraAnimation = false;
		mAddEntitySceneNode->_setDerivedPosition(mPreviousPositionAddEntitySceneNode);
		doCameraReset();
	}
	else
	{
		// Orthographic camera
		mPreviousPositionAddEntitySceneNode = mAddEntitySceneNode->_getDerivedPosition();
		mAddEntitySceneNode->_setDerivedPosition(Ogre::Vector3::ZERO); // Temporay set the mesh in the centre
		mUIMainToolbar->getMiscIcons()->enableOrthoCameraSelected(false);
		_getCamera()->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
		_getCamera()->setFOVy(Ogre::Radian(1.0f));
		Ogre::Vector3 cameraPosition = _getCameraPosition();
		if (m45DegreesOrtho)
		{
			setOrthoGridVisible(true, Gizmo::AXIS_Y);
			Ogre::Quaternion q(0.3535533905932738f, 0.35355339059327373f, 0.14644660940672627f, 0.8535533905932738f); // 135 pitch (x) and 45 yaw (y)
			cameraPosition = Ogre::Vector3(0, -l, 0);
			cameraPosition = q * cameraPosition;
		}
		else
		{
			setOrthoGridVisible(true, Gizmo::AXIS_Z);
			cameraPosition = l * Ogre::Vector3(0, 0, -1);

		}
		_setCameraPosition(cameraPosition);
		_getCamera()->setOrthoWindowWidth(cameraPosition.length());
		_cameraLookAt(Ogre::Vector3::ZERO);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnCameraAngle(wxCommandEvent& event)
{
	if (!_isCameraOrthographic())
		return;

	m45DegreesOrtho = mUIMainToolbar->getMiscIcons()->doToggleCameraAngle();
	Ogre::Vector3 cameraPosition = _getCameraPosition();
	Ogre::Real l = cameraPosition.length();
	if (m45DegreesOrtho)
	{
		// 45 degree angle camera
		setOrthoGridVisible(true, Gizmo::AXIS_Y);
		Ogre::Quaternion q(0.3535533905932738f, 0.35355339059327373f, 0.14644660940672627f, 0.8535533905932738f); // 135 pitch (x) and 45 yaw (y)
		cameraPosition = Ogre::Vector3(0, -l, 0);
		cameraPosition = q * cameraPosition;
	}
	else
	{
		// Perpendicular camera
		setOrthoGridVisible(true, Gizmo::AXIS_Z);
		cameraPosition = l * Ogre::Vector3(0, 0, -1);
	}
	if (mGizmoManager)
	{
		mGizmoManager->set45DegreesOrtho(m45DegreesOrtho);
	}
	startCameraAnimation(cameraPosition);
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnResetTime(wxCommandEvent& event)
{
	doResetTime();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doResetTime(void)
{
	if (mUIMainToolbar)
	{
		mUIMainToolbar->resetTimeScale();
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnToggleGizmo(wxCommandEvent& event)
{
	doToggleGizmo();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doToggleGizmo(void)
{
	if (mGizmoManager)
	{
		bool visible = !mGizmoManager->isVisible();
		mGizmoManager->setVisible(visible);
		mMarker->setVisible(visible);
		mMarkerBold->setVisible(visible);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnToggleWorldLocalSpace(wxCommandEvent& event)
{
	bool worldspace = mUIMainToolbar->getMiscIcons()->doToggleWorldLocalSpace();
	mGizmoManager->setWorldspace(worldspace); // If true => worldspace; if false => localspace
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnToggleStats(wxCommandEvent& event)
{
	doToggleStats();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doToggleStats(void)
{
	if (mDebugOverlay)
	{
		showOverlay(!mDebugOverlay->isVisible());
	}
	else
	{
		createStatisticsOverlay();
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnToggleGridPlane(wxCommandEvent& event)
{
	doToggleGridPlane();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doToggleGridPlane(void)
{
	showGridPlane(!mGridBottom->isVisible());
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnGizmoMoveSelect(wxCommandEvent& event)
{
	if (mGizmoManager)
	{
		mGizmoManager->setGizmoType(Gizmo::GIZMO_MOVE);
		mGizmoManager->setVisible(true);
		if (mGizmoManager->getAttachedNode() != mMainSceneNode)
		{
			mMarker->setVisible(true);
		}
		mMarkerBold->setVisible(true);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnGizmoRotateSelect(wxCommandEvent& event)
{
	if (mGizmoManager)
	{
		mGizmoManager->setGizmoType(Gizmo::GIZMO_ROTATE);
		mGizmoManager->setVisible(true);
		mMarker->setVisible(true);
		mMarkerBold->setVisible(true);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnGizmoScaleSelect(wxCommandEvent& event)
{
	if (mGizmoManager)
	{
		mGizmoManager->setGizmoType(Gizmo::GIZMO_SCALE);
		mGizmoManager->setVisible(true);
		mMarker->setVisible(true);
		mMarkerBold->setVisible(true);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnOptions(wxCommandEvent& event)
{
	doOptions();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doOptions(void)
{
#ifdef PU_FULL_VERSION
	mConfigDialog->rebuildDirectoriesPanel();
	if (mConfigDialog->ShowModal() == wxID_OK)
	{
		SetCursor(*wxHOURGLASS_CURSOR);
		applyGeneralConfigOptions();
		applyPhysXConfigOptions();
		applyTextConfigOptions();

		if (mConfigDialog->getExcludeDirList().size() > 0)
		{
			// Dirlist has been changed, get the excluded ones
			ParticleUniverse::StringVector vec = mConfigDialog->getExcludeDirList();
			ParticleUniverse::StringVector::iterator it;
			ParticleUniverse::StringVector::iterator itEnd = vec.end();
			for (it = vec.begin(); it != itEnd; ++it)
			{
				// Remove each directory
				Ogre::ResourceGroupManager::getSingletonPtr()->removeResourceLocation(*it, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			}
			Ogre::ResourceGroupManager::getSingletonPtr()->clearResourceGroup(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyAllParticleSystemTemplates();
			Ogre::ResourceGroupManager::getSingletonPtr()->initialiseResourceGroup(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			LoadListOfTemplates();
		}
		mConfigDialog->saveConfig();
		SetCursor(wxNullCursor);
	}
	else
	{
		mConfigDialog->loadConfig();
	}
#endif // PU_FULL_VERSION
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::applyGeneralConfigOptions(void)
{
	mDefaultCameraPosition = mConfigDialog->getCameraPosition();
	mAutoStartRender = mConfigDialog->isAutoStart();
	wxString videoPath = mConfigDialog->getVideoPath();
	wxString suffix = mConfigDialog->getFileNameSuffix();
	Recorder::ImageFilter filter = mConfigDialog->getFilter();
	ParticleUniverse::Real framePerSeconds = mConfigDialog->getFPS();
	ParticleUniverse::Real pauseTime = mConfigDialog->getPauseTime();
	if (mConfigDialog->isVideoSelected())
	{
		// Force propagation from the video size to the image size
		mConfigDialog->getVideoSize(true);
	}
	ParticleUniverse::uint width = mConfigDialog->getImageWidth();
	ParticleUniverse::uint height = mConfigDialog->getImageHeight();
	if (mRecorder)
	{
		mRecorder->setVideoDirectory(wx2ogre(videoPath));
		mRecorder->setFileNameSuffix(wx2ogre(suffix));
		mRecorder->setPauseTime(pauseTime);
		mRecorder->setFramesPerSecond(framePerSeconds);
		mRecorder->setFilter(filter);
		mRecorder->setImageWidth(width, false);
		mRecorder->setImageHeight(height, true); // Realloc
	}
	setDistanceGridPlane(mConfigDialog->getGridPlaneDistance());
	setScaleGridPlane(mConfigDialog->getGridPlaneScale());
	showGridPlane(mConfigDialog->isGridPlane());
	showOverlay(mConfigDialog->isStatistics());
	showLight(mConfigDialog->isLightCheck());
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::applyPhysXConfigOptions(void)
{
	#ifdef PU_PHYSICS_PHYSX
	if (mUsePhysX)
	{
		mPhysXNormal = mConfigDialog->getPhysXPlaneNormal();
		mPhysXDistance = mConfigDialog->getPhysXPlaneDistance();
		ParticleUniverse::PhysXBridge::getSingletonPtr()->createPlane(mPhysXNormal, mPhysXDistance);
		mPhysXGravity = mConfigDialog->getPhysXGravity();
		ParticleUniverse::PhysXBridge::getSingletonPtr()->setGravity(mPhysXGravity);
	}
	#endif //PU_PHYSICS_PHYSX
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::applyTextConfigOptions(void)
{
	if (mTextCtrl)
	{
		mTextCtrl->setHighlightKeywords(mConfigDialog->isHighlight());
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnAbout(wxCommandEvent& event)
{
    doAbout();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doAbout(void)
{
#ifdef PU_FULL_VERSION
	wxMessageBox(_("Particle Universe Editor V1.5.1\nCopyright (c) 2012 Henry van Merode\n\nThe lightbulb image is royalty free from FreeDigitalPhotos.net\n\n"),
				_("About"), wxICON_INFORMATION);
#else
	wxMessageBox(_("Particle Universe Editor DEMO V1.5.1\nCopyright (c) 2012 Henry van Merode\n\nThe lightbulb image is royalty free from FreeDigitalPhotos.net\n\n"),
				_("About"), wxICON_INFORMATION);
#endif // PU_FULL_VERSION
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnTabChanging(wxCommandEvent& event)
{
	// Check which page is active
	switch (mNotebook->GetSelection())
	{
		case NOTEBOOK_RENDER:
		{
			synchronizeSmallOgreControlWithRenderTab();
			mUIMainToolbar->getMiscIcons()->disableAll();
			resetLeftSideWindow();
		}
		break;
		case NOTEBOOK_EDIT:
		{
			// Synchronize the particle system of the render and update the script, based on the components of the edit page
			mEditNotebookPage->Freeze();
			synchroniseScriptWithEditTab();
			resetLeftSideWindow();
		}
		break;
		case NOTEBOOK_SCRIPT:
		{
			// Parse the script and update both particle systems of the render-tab and the edit-tab
			validateAndReparse();
			mUIMainToolbar->getCompileIcons()->disableAll();
		}
		break;
		case NOTEBOOK_MATERIAL:
		{
			if (mMaterialNotebookPage)
			{
				mMaterialNotebookPage->saveSelectedMaterialName();
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnTabChanged(wxCommandEvent& event)
{
	wxCommandEvent t(0);
	OnStop(t);

	// Check which page is active
	switch (mNotebook->GetSelection())
	{
		case NOTEBOOK_RENDER:
		{
			if (mControlPanelWithSmallRenderWindow)
			{
				// Hide the small render window and reset the icons
				mControlPanelWithSmallRenderWindow->Hide();
				mEditSizer->Layout();
				mUIMainToolbar->getMiscIcons()->reset();
				if (mAddEntitySceneNode->numAttachedObjects() == 0)
				{
					mUIMainToolbar->getMiscIcons()->enableRemoveEntity(false);
				}
				if (mAddEntitySceneNode->numAttachedObjects() > 0)
				{
					setLeftSideWindow(mAnimationWindow);
				}
			}
			mEditNotebookPage->enableTools(false);
			if (mAutoStartRender)
			{
				wxCommandEvent t(0);
				OnPlay(t);
			}
			restoreFocus();
		}
		break;

		case NOTEBOOK_EDIT:
		{
			if (mControlPanelWithSmallRenderWindow)
			{
				// Show the small render window again
				mControlPanelWithSmallRenderWindow->Show();
				mEditSizer->Layout();
			}
			mEditNotebookPage->enableTools(false);
			setLeftSideWindow(mPropertyWindow);
			if (mAutoStartRender && mOgreControlSmall)
			{
				wxCommandEvent t(0);
				OnPlay(t);
			}
			if (mEditNotebookPage)
			{
				mEditNotebookPage->enableTools(true);
				mEditNotebookPage->Thaw();
				mEditNotebookPage->adjustPosition();
			}
		}
		break;

		case NOTEBOOK_SCRIPT:
		{
			if (mControlPanelWithSmallRenderWindow)
			{
				// Hide the small render window
				mControlPanelWithSmallRenderWindow->Hide();
				mEditSizer->Layout();
			}
			mEditNotebookPage->enableTools(false);
			mUIMainToolbar->getCompileIcons()->reset(mListOfTemplates->GetCount());
		}
		break;

		case NOTEBOOK_MATERIAL:
		{
			if (mControlPanelWithSmallRenderWindow)
			{
				// Hide the small render window
				mControlPanelWithSmallRenderWindow->Hide();
				mEditSizer->Layout();
			}
			mEditNotebookPage->enableTools(false);
			if (mMaterialNotebookPage)
			{
				mMaterialNotebookPage->Freeze();
				mMaterialNotebookPage->fillMaterials();
				mMaterialNotebookPage->Thaw();
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnRewind(wxCommandEvent& event)
{
	// Set play icons if 'rewind' has been pressed
	mListOfTemplates->Freeze();
	resetPlayIcons();
	wxTreeItemId prev = mListOfTemplates->getPreviousItem();
	if (prev.IsOk())
	{
		mListOfTemplates->SelectItem(prev);
		OnTemplatesClick((wxCommandEvent)0);
	}
	mListOfTemplates->Thaw();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnPlay(wxCommandEvent& event)
{
	doPlay();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doPlay(void)
{
	switch (mNotebook->GetSelection())
	{
		case NOTEBOOK_RENDER:
		{
			// Start/resume the particle system and set play icons if 'play' has been pressed
			mUIMainToolbar->getPlayIcons()->play();
			mListOfTemplates->setPlayInContextMenuEnabled(false);

			if (mCurrentParticleSystemForRenderer)
			{
				if (mCurrentParticleSystemForRenderer->getState() == ParticleUniverse::ParticleSystem::PSS_PAUSED)
				{
					mCurrentParticleSystemForRenderer->resume();
				}
				else
				{
					if (mParticlerSystemSceneNode->numAttachedObjects() == 0)
					{
						// If the particle system is not attached at all, attach it to the mParticlerSystemSceneNode
						if (mAnimationWindow && !mAnimationWindow->isParticleSystemAttachedToEntity())
						{
							mParticlerSystemSceneNode->attachObject(mCurrentParticleSystemForRenderer);
						}
					}
					resetOverlay();
					mCurrentParticleSystemForRenderer->start();
				}
			}
		}
		break;

		case NOTEBOOK_EDIT:
		{
			// Start/resume the particle system and set play icons if 'play' has been pressed
			if (mEditNotebookPage)
			{
				mUIMainToolbar->getPlayIcons()->play();
				mListOfTemplates->setPlayInContextMenuEnabled(false);
				if (mOgreControlSmall)
				{
					mOgreControlSmall->notifyPlay();
				}
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnRecord(wxCommandEvent& event)
{
	doRecord();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doRecord(void)
{
#ifdef PU_FULL_VERSION
	// Record the particle system and set play icons if 'record' has been pressed
	mUIMainToolbar->getPlayIcons()->record();
	mGizmoManager->setVisible(false);
	mMarker->setVisible(false);
	mMarkerBold->setVisible(false);
	doPlay();
	if (mRecorder)
	{
		ParticleUniverse::String delFrames = "del " + wx2ogre(mConfigDialog->getVideoPath()) + "\\Frame*.*";
		system(delFrames.c_str());
		mRecorder->record(mStartTime, mEndTime); // Endtime is set to 2 hours by default
	}
#endif // PU_FULL_VERSION
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnPause(wxCommandEvent& event)
{
	doPause();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doPause(void)
{
	// Pause the particle system and set play icons if 'pause' has been pressed
	mUIMainToolbar->getPlayIcons()->pause();
	mListOfTemplates->setPlayInContextMenuEnabled(true);
	if (mCurrentParticleSystemForRenderer)
	{
		mCurrentParticleSystemForRenderer->pause();
	}
	if (mOgreControlSmall)
	{
		mOgreControlSmall->notifyPause();
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnStop(wxCommandEvent& event)
{
	doStop();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doStop(void)
{
	// Stop the particle system and set play icons if 'stop' has been pressed
	resetPlayIcons();
	mListOfTemplates->setPlayInContextMenuEnabled(true);
	if (mGizmoManager)
	{
		mGizmoManager->setVisible(true);
		mMarkerBold->setVisible(true);
		if (mGizmoManager->getAttachedNode() != mMainSceneNode)
		{
			mMarker->setVisible(true);
		}
	}

	if (mRecorder && mRecorder->isRecording())
	{
		bool recording = mRecorder->isRecording();
		mRecorder->stop();
		_generateVideoIfNeeded(recording);
	}
	if (mCurrentParticleSystemForRenderer)
	{
		mCurrentParticleSystemForRenderer->stop();
	}
	if (mOgreControlSmall)
	{
		mOgreControlSmall->notifyStop();
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnWind(wxCommandEvent& event)
{
	// Set play icons if 'wind' has been pressed
	mListOfTemplates->Freeze();
	resetPlayIcons();
	wxTreeItemId next = mListOfTemplates->getNextItem();
	if (next.IsOk())
	{
		mListOfTemplates->SelectItem(next);
		OnTemplatesClick((wxCommandEvent)0);
	}
	mListOfTemplates->Thaw();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnClone(wxCommandEvent& event)
{
	doClone();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doClone(void)
{
	wxTreeItemId selected = mListOfTemplates->GetSelection();
	wxString templateName = mListOfTemplates->GetItemText(selected);
	if (!mListOfTemplates->isCategoryName(templateName))
	{
		// It is no category
		templateName = wxT("CopyOf") + templateName;
		wxTreeItemId category = mListOfTemplates->getCurrentCategory();
		if (category.IsOk())
		{
			wxTreeItemId newItem = mListOfTemplates->PrependItem(category, templateName);
		
			// Create new particle system template.
			ParticleUniverse::ParticleSystem* templateSystem = ParticleUniverse::ParticleSystemManager::getSingletonPtr()->createParticleSystemTemplate(
				wx2ogre(templateName), 
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
				*templateSystem = *mCurrentParticleSystemForRenderer;
				mCurrentParticleSystemForRenderer->copyAttributesTo(templateSystem);

			mListOfTemplates->SelectItem(newItem);
			OnTemplatesClick((wxCommandEvent)0);
			resetPlayIcons();
		}
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnRemove(wxCommandEvent& event)
{
	doRemove();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::doRemove(void)
{
	wxTreeItemId selected = mListOfTemplates->GetSelection();
	wxString templateName = mListOfTemplates->GetItemText(selected);
	if (!mListOfTemplates->isCategoryName(templateName))
	{
		// It is no category
		mListOfTemplates->Delete(selected);
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::String s = wx2ogre(templateName);
		particleSystemManager->destroyParticleSystemTemplate(s);
		OnTemplatesClick((wxCommandEvent)0);
		resetPlayIcons();
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnTemplatesClick(const wxCommandEvent& event)
{
	mControl->Freeze();
	mEditNotebookPage->Freeze();
	mTextCtrl->Freeze();
	mListOfTemplates->Freeze();

	mCheckFocusListOfTemplates = true;
	mListOfTemplates->resetContextMenu();
	mListOfTemplates->setPlayInContextMenuEnabled(true);
	resetOverlay();
	mTemplateSelected = true;
	wxTreeItemId sel = mListOfTemplates->GetSelection();
	if (sel.IsOk())
	{
		// Make the selected item visible
//		mListOfTemplates->ScrollTo(sel); // TODO: The effect is terrible. Commented!

		// Add check that determines whether the mCurrentParticleSystemForRenderer has been changed since the last save.
		wxString selection = mListOfTemplates->GetItemText(sel);

		// The selection must be a templatename
		if (!mListOfTemplates->isCategoryName(selection))
		{
#ifdef PU_FULL_VERSION
			if (mCurrentParticleSystemForRenderer && 
				(mScriptChanged || mEditNotebookPage->isSystemUpdatedByEditPage()) &&
				selection != ogre2wx(mCurrentParticleSystemForRenderer->getTemplateName()))
			{
				validateAndReparse();
	
				// Show warning
				wxString warn = _("You have selected a different Particle System. Do you want to save ");
				if (showMessage(warn + ogre2wx(mCurrentParticleSystemForRenderer->getTemplateName()) + wxT("?"), wxOK | wxCANCEL) == wxID_OK)
				{
					// Save it
					wxCommandEvent t(0);
					OnSave(t);
				}
			}
#endif // PU_FULL_VERSION

			createNewSystem(wx2ogre(selection));
		}
		mSavedFile = WX_STRING_BLANK;
		resetPlayIcons();
		mTemplateSelected = false;
		mLatestSelection = sel;
	}

	mListOfTemplates->Thaw();
	mTextCtrl->Thaw();
	mEditNotebookPage->Thaw();
	mControl->Thaw();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::notifyScriptChanged(void)
{
	mScriptChanged = true;
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::resetPlayIcons(void)
{
	if (mNotebook->GetSelection() == NOTEBOOK_SCRIPT || mNotebook->GetSelection() == NOTEBOOK_MATERIAL)
	{
		// Disable the icons if no renderer is on screen
		mUIMainToolbar->getPlayIcons()->disable(mListOfTemplates->GetCount());
	}
	else
	{
		mUIMainToolbar->getPlayIcons()->reset(mListOfTemplates->GetCount());
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::particleSystemStopped(void)
{
	// Reset the toolbar icons
	if (mRecorder)
	{
		bool recording = mRecorder->isRecording();
		mRecorder->stop();
		_generateVideoIfNeeded(recording);
	}
	resetPlayIcons();
	mListOfTemplates->setPlayInContextMenuEnabled(true);
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::_generateVideoIfNeeded(bool recording)
{
	if (!recording || !mConfigDialog || !mConfigDialog->isVideoSelected())
		return;

	// 1. Delete video file in target directory
	ParticleUniverse::String videoFileName = wx2ogre(mConfigDialog->getVideoPath()) + "\\PUVideo" + wx2ogre(mConfigDialog->getFileNameSuffix());
	ParticleUniverse::String delVideoFile = "del " + videoFileName;
	system(delVideoFile.c_str());

	// 2. Call ffmpeg
	SetCursor(*wxHOURGLASS_CURSOR);
	ParticleUniverse::String args = "-f image2 -i " + wx2ogre(mConfigDialog->getVideoPath()) + "\\Frame_%d.png -r ";
	args = args + ParticleUniverse::StringConverter::toString(mConfigDialog->getFPS()) + " -s " + wx2ogre(mConfigDialog->getVideoSize());
	args = args + " -qmin 1 -qmax 1 -pix_fmt rgba32 " + videoFileName;

	//showMessage(args, wxOK | wxICON_INFORMATION); // For debug

#if 0
	SHELLEXECUTEINFO sei = {0};
	sei.cbSize = sizeof(sei);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.lpVerb = wxT("open");
	sei.lpFile = wxT("bin\\ffmpeg.exe");
	wxString wxArgs = ogre2wx(args);
	sei.lpParameters = wxArgs;
	sei.nShow = SW_HIDE;
	BOOL bOK = ShellExecuteEx(&sei);
	if (bOK)
	{
		WaitForSingleObject(sei.hProcess, INFINITE); 
		CloseHandle(sei.hProcess);
	}
#endif

	// 2. Delete the generated image files
	ParticleUniverse::String delFrames = "del " + wx2ogre(mConfigDialog->getVideoPath()) + "\\Frame*.*";
	system(delFrames.c_str());

	SetCursor(wxNullCursor);
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::SetMenu()
{
    wxMenu* fmenu = new wxMenu();
	fmenu->Append(wxID_NEW, _("&New\tCtrl+N"), _("New particle system"));
	fmenu->Append(wxID_FILE, _("&Load file\tCtrl+L"), _("Load a *.pu or *.pua file"));
	fmenu->Append(wxID_OPEN, _("Add &resource location\tCtrl+R"), _("Load all resources from a directory"));
	fmenu->Append(wxID_SAVE, _("&Save\tCtrl+S"), _("Save a *.pu or *.pua file"));
	fmenu->Append(wxID_SAVEAS, _("Save As\tCtrl+Shift+S"), _("Save As"));
    fmenu->AppendSeparator();
    fmenu->Append(wxID_EXIT, _("&Quit\tCtrl+Q"), _("Quit"));

    wxMenu* imenu = new wxMenu();
	imenu->Append(ID_PLAY, _("Play\tF6"), _("Play"));
	imenu->Append(ID_RECORD, _("Record\tF7"), _("Record"));
	imenu->Append(ID_PAUSE, _("Pause\tF8"), _("Pause"));
	imenu->Append(ID_STOP, _("Stop\tF9"), _("Stop"));
	imenu->AppendSeparator();
	imenu->Append(ID_REMOVE, _("Remove"), _("Remove"));
	imenu->Append(ID_CLONE, _("Clone"), _("Clone"));

    wxMenu* bmenu = new wxMenu();
	bmenu->Append(wxID_REFRESH, _("Compile\tF5"), _("Compile"));

    wxMenu* gmenu = new wxMenu();
	gmenu->Append(ID_GIZMO_MOVE, _("Gizmo move\tShift+M"), _("Gizmo move"));
	gmenu->Append(ID_GIZMO_ROTATE, _("Gizmo rotate\tShift+R"), _("Gizmo rotate"));
	gmenu->Append(ID_GIZMO_SCALE, _("Gizmo scale\tShift+S"), _("Gizmo scale"));
	gmenu->Append(ID_TOGGLE_GIZMO, _("Gizmo on/off\tCtrl+G"), _("Gizmo on/off"));
	gmenu->Append(ID_WORLD_LOCAL_SPACE, _("Gizmo in world/object space\tCtrl+P"), _("Gizmo in world/object space"));

	wxMenu* cmenu = new wxMenu();
	cmenu->Append(ID_CAMERA_RESET, _("Reset camera\tCtrl+Z"), _("Reset camera"));
	cmenu->Append(ID_CAMERA_SAVE, _("Save camera\tShift+C"), _("Save camera"));
	cmenu->Append(ID_TOGGLE_CAMERA_PROJECTION, _("Toggle camera projection\tShift+P"), _("Toggle camera projection"));
	cmenu->Append(ID_TOGGLE_CAMERA_ANGLE, _("Toggle camera angle\tCtrl+A"), _("Toggle camera angle"));

    mTmenu = new wxMenu(); // Keep pointer to the menu
	mTmenu->Append(ID_CENTRE_OBJECT, _("Centre object\tCtrl+C"), _("Centre object"));
	mTmenu->Append(ID_BACKGROUND_COLOUR, _("Background colour\tCtrl+B"), _("Background colour"));
	mTmenu->Append(ID_TOGGLE_STATS, _("Toggle statistics\tCtrl+I"), _("Toggle statistics"));
	mTmenu->Append(ID_TOGGLE_GRID_PLANE, _("Toggle grid plane\tCtrl+T"), _("Toggle grid plane"));
	mTmenu->Append(ID_ADD_MESH_TO_SCENE, _("Add mesh to scene\tCtrl+M"), _("Add mesh to scene"));
	mTmenu->Append(ID_REMOVE_MESH_FROM_SCENE, _("Remove mesh from scene\tCtrl+X"), _("Remove mesh from scene"));
	mTmenu->Append(ID_TOGGLE_LIGHT, _("Toggle light\tCtrl+D"), _("Toggle light"));
	mTmenu->Append(ID_RESET_TIME, _("Reset time\tCtrl+K"), _("Reset time"));
	mTmenu->AppendSeparator();
	mTmenu->Append(ID_OPTIONS, _("Options\tCtrl+O"), _("Options"));

	wxMenu* hmenu = new wxMenu();
    hmenu->Append(wxID_ABOUT, _("About\tF1"), _("About"));

    wxMenuBar* menubar = new wxMenuBar();
    menubar->Append(fmenu, _("&File"));
    menubar->Append(imenu, _("List &items"));
    menubar->Append(bmenu, _("&Build"));
    menubar->Append(gmenu, _("&Gizmo"));
	menubar->Append(cmenu, _("&Camera"));
	menubar->Append(mTmenu, _("&Tools"));
    menubar->Append(hmenu, _("&Help"));

// If not a full version, then disable the menu items
#ifndef PU_FULL_VERSION
	fmenu->Enable(wxID_SAVE, false);
	fmenu->Enable(wxID_SAVEAS, false);
	imenu->Enable(ID_RECORD, false);
	bmenu->Enable(wxID_REFRESH, false);
	mTmenu->Enable(ID_OPTIONS, false);
#endif // PU_FULL_VERSION

	SetMenuBar(menubar);
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::validateAndReparse(void)
{
	// Check whether the particle system was changed by means of the script or the editor. If that is the case, update the template list and 
	// recreate the current particle system.
	if (!mCurrentParticleSystemForRenderer)
		return;

	if (mTextCtrl)
	{
		if (!(mTextCtrl->IsEmpty() || mCurrentParticleScript.empty()))
		{
			if (mCurrentParticleScript != wx2ogre(mTextCtrl->GetValue()))
			{
				// Script has changed. Reparse and update the templates
				Ogre::String oldTemplateName = mCurrentParticleSystemForRenderer->getTemplateName();
				ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
				particleSystemManager->destroyParticleSystemTemplate(oldTemplateName); // Delete the old template
				parseScript(wx2ogre(mTextCtrl->GetValue()));
	
				// Replace the old particle system with a new one, of which the name can also be changed.
				Ogre::String newTemplateName = particleSystemManager->getLastCreatedParticleSystemTemplateName();
				createNewSystem(newTemplateName);

				// Find the old name and remove it from the tree
				wxTreeItemId item = mListOfTemplates->findParticleSystem(ogre2wx(oldTemplateName));
				if (item)
				{
					mListOfTemplates->Delete(item);
				}

				// Insert the new name in the tree
				if (mCurrentParticleSystemForRenderer)
				{
					ParticleUniverse::String categoryName = mCurrentParticleSystemForRenderer->getCategory();
					mListOfTemplates->addItem(ogre2wx(categoryName), ogre2wx(newTemplateName));
				}
			}
		}
	}
}
//-----------------------------------------------------------------------
//void ParticleUniverseEditorFrame::validateScript(void)
//{
//	// Only validate script
//	if (mTextCtrl && !(mTextCtrl->IsEmpty()))
//	{
//		parseScript(wx2ogre(mTextCtrl->GetValue()));
//	}
//}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::replaceTemplateName(const ParticleUniverse::String& templateName)
{
	// Set the new template name in the particle systems (both renderer + small panel for edittab)
	wxString wxOldTemplateName = mListOfTemplates->GetItemText(mListOfTemplates->GetSelection());
	ParticleUniverse::String oldTemplateName = wx2ogre(wxOldTemplateName);
	ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
	wxString wxTemplateName = ogre2wx(templateName);
	if (mCurrentParticleSystemForRenderer)
	{
		mCurrentParticleSystemForRenderer->setTemplateName(templateName);
	}
	if (mOgreControlSmall && mOgreControlSmall->getParticleSystem())
	{
		mOgreControlSmall->getParticleSystem()->setTemplateName(templateName);
	}
	particleSystemManager->destroyParticleSystemTemplate(oldTemplateName);

	// Find the old name in the tree and remove it
	wxTreeItemId item = mListOfTemplates->findParticleSystem(wxOldTemplateName);
	if (item)
	{
		mListOfTemplates->Delete(item);
	}

	// Insert the new name in the tree
	if (mCurrentParticleSystemForRenderer)
	{
		ParticleUniverse::String categoryName = mCurrentParticleSystemForRenderer->getCategory();
		mListOfTemplates->addItem(ogre2wx(categoryName), wxTemplateName);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::notifyCategoryChanged(ParticleUniverse::ParticleSystem* pSys)
{
	if (!pSys)
		return;

	// Synchronize everything, because moving the item to a different category triggers OnTamplatesClick.
	synchroniseScriptWithEditTab();

	// Get the category name
	wxString name = ogre2wx(pSys->getTemplateName());
	wxString category = ogre2wx(pSys->getCategory());

	// Find the name in the tree and remove it
	wxTreeItemId item = mListOfTemplates->findParticleSystem(name);
	if (item)
	{
		mListOfTemplates->Delete(item);
	}

	// Insert it again in the tree
	mListOfTemplates->addItem(category, name);
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::parseScript(const ParticleUniverse::String& script)
{
	Ogre::ScriptCompilerManager* scriptCompilerManager = Ogre::ScriptCompilerManager::getSingletonPtr();
	char* buffer = new char[script.length() + 1];
	strcpy(buffer, script.c_str());
	Ogre::DataStreamPtr* datastream = new Ogre::DataStreamPtr(new Ogre::MemoryDataStream(buffer, strlen(buffer)));
	scriptCompilerManager->parseScript(*datastream, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	delete datastream;
	delete [] buffer;
}
//-----------------------------------------------------------------------
bool ParticleUniverseEditorFrame::createNewSystem(const ParticleUniverse::String& templateName, bool forcedByEditTab)
{
	bool particleSystemWasAttachedToEntity = false;
	Ogre::String boneName = Ogre::StringUtil::BLANK;
	if (mCurrentParticleSystemForRenderer)
	{
		// 1. Get state in case the particle system was attached to the entity
		if (mAnimationWindow)
		{
			/** Notify the animation window that a new particle system is created. This is needed in case the particle system
				is attached to the entity.
			*/
			if (mCurrentParticleSystemForRenderer->getTemplateName() == templateName)
			{
				/** Only if the name of the old particle system is equal to the new name and the particle system is attached to
					an entity, the newly created particle system should be reattached
				*/
				if (mAnimationWindow->isParticleSystemAttachedToEntity())
				{
					particleSystemWasAttachedToEntity = true;
					boneName = mAnimationWindow->getBoneNameToWhichParticleSystemIsAttached();
				}
			}
			mAnimationWindow->notifyParticleSystemDeleted(mAddEntity);
		}

		// 2. Detach the particle system if still attached to mParticlerSystemSceneNode
		mCurrentParticleSystemForRenderer->stop();
		mCurrentParticleSystemForRenderer->detachFromParent();
	}

	try
	{
		// 3. Create the new particle system
		ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyParticleSystem(CURRENT_PS_NAME, mSceneManager); // Always force a destroy
		ParticleUniverse::ParticleSystemManager::getSingletonPtr()->removeAndDestroyDanglingSceneNodes(mParticlerSystemSceneNode);
		mCurrentParticleSystemForRenderer = 0;
		mCurrentParticleSystemForRenderer = ParticleUniverse::ParticleSystemManager::getSingletonPtr()->createParticleSystem(
			CURRENT_PS_NAME, templateName, mSceneManager);
		doResetTime();

		if (mCurrentParticleSystemForRenderer)
		{
			// Only continue if the particle system was created
			mCurrentParticleSystemForRenderer->addParticleSystemListener(mSystemListener);
			ParticleUniverse::String script = ParticleUniverse::ParticleSystemManager::getSingleton().writeScript(mCurrentParticleSystemForRenderer);
			if (mRecorder)
			{
				mRecorder->setParticleSystem(mCurrentParticleSystemForRenderer);
			}
			if (!forcedByEditTab && mEditNotebookPage)
			{
				// 5. Also create a copy for the edit page (including all components)
				createParticleSystemCopyForEditPage(templateName);
			}
#ifdef PU_FULL_VERSION
			if (script != mCurrentParticleScript)
			{
				// 6. Load the script in the script tab
				mCurrentParticleScript = script;
				LoadTextControl(ogre2wx(mCurrentParticleScript));
			}
#endif // PU_FULL_VERSION


			// 7. If the particle systen was detached from the entity, it must be reattached in case particleSystemWasAttachedToEntity is true
			if (mAnimationWindow && particleSystemWasAttachedToEntity)
			{
				mAnimationWindow->attachParticleSystemToEntity(boneName, mAddEntity);
			}

			return true;
		}
	}

	catch (Ogre::Exception)
	{
		showMessage(_("This particle system template is in an incorrect state and cannot be used anymore"), wxOK | wxICON_ERROR);
		mListOfTemplates->Delete(mListOfTemplates->GetSelection());
		if (mCurrentParticleSystemForRenderer)
		{
			// Clean up the garbage (cannot be done by means of the ParticleSystemManager; apparently is doesn't exist in the ParticleSystemManager)
			delete mCurrentParticleSystemForRenderer;
			mCurrentParticleSystemForRenderer = 0;
		}
	}

	return false;
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::createParticleSystemCopyForEditPage(const ParticleUniverse::String& templateName)
{
	mEditNotebookPage->Freeze();
	mEditNotebookPage->destroyDanglingPUComponents();
	mEditNotebookPage->deleteParticleSystemComponents();
	if (mOgreControlSmall)
	{
		ParticleUniverse::ParticleSystem* particleSystem = mOgreControlSmall->notifyCreateNewSystem(templateName);
		EditComponent* particleSystemEditComponent = mEditNotebookPage->forceCreateParticleSystemEditComponent(); // 'Guarantees' a valid particleSystemEditComponent
		if (mEditNotebookPage->copyParticleSystemPropertiesToPropertyWindow(particleSystemEditComponent, particleSystem))
		{
			mEditNotebookPage->createParticleSystemComponents(particleSystemEditComponent, particleSystem);
		}
	}
	mEditNotebookPage->Thaw();
}
//-----------------------------------------------------------------------
int ParticleUniverseEditorFrame::showMessage(const wxString& message, long style)
{
	wxMessageDialog m(this, message, _("Message"), style);
	return m.ShowModal();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::hideEditPage(void)
{
	if (mEditNotebookPage && mNotebook->GetSelection() == NOTEBOOK_EDIT)
	{
		mEditNotebookPage->Hide();
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::showEditPage(void)
{
	if (mEditNotebookPage && mNotebook->GetSelection() == NOTEBOOK_EDIT)
	{
		mEditNotebookPage->Show();
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::synchroniseScriptWithEditTab(void)
{
	if (mEditNotebookPage->isSystemUpdatedByEditPage())
	{
		ParticleUniverse::ParticleSystem* system = mEditNotebookPage->getParticleSystemFromSystemComponent();
		if (system)
		{
			ParticleUniverse::String templateName = system->getTemplateName();
			
			// 1. Clone the current template from 'system'
			ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			particleSystemManager->replaceParticleSystemTemplate(templateName, system);

			// 2. Delete mCurrentParticleSystemForRenderer
			// 3. Create a new mCurrentParticleSystemForRenderer from the current template
			// 4. Create a new script from the current template
			// 5. Set mScriptChanged to true
			createNewSystem(templateName, true);
			mScriptChanged = true; // this forces the messagebox, asking whether to change.
		}
	}
}
//-----------------------------------------------------------------------
bool ParticleUniverseEditorFrame::isPhysXInstalled(void)
{
#if 0
	HKEY hKey = 0;
//	char buf[255] = {0};
//	DWORD dwBufSize = sizeof(buf);
	//const char* subkey = "SOFTWARE\\AGEIA Technologies\\PhysX_A32_Engines";
	const LPCWSTR subkey = wxT("SOFTWARE\\AGEIA Technologies\\PhysX_A32_Engines");

    if(RegOpenKey(HKEY_LOCAL_MACHINE, subkey, &hKey) == ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
		return true;
    }
    else
#endif
	{
		return false;
	}
}
//-----------------------------------------------------------------------
bool ParticleUniverseEditorFrame::isPhysXUsed(void)
{
	return mUsePhysX;
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::notifyTimeRescaled(ParticleUniverse::Real scale)
{
	if (mCurrentParticleSystemForRenderer)
	{
		mCurrentParticleSystemForRenderer->setScaleTime(scale);
	}
	if (mAnimationWindow)
	{
		// Notify animation window, so the animation is slowed down or speeded up
		mAnimationWindow->notifyTimeRescaled(scale);
	}

	// scale time of the particle system of the small rendering window
	if (mOgreControlSmall)
	{
		mOgreControlSmall->notifyTimeRescaled(scale);
	}

	if (mControl)
	{
		mControl->SetFocus();
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::createStatisticsOverlay(void)
{
	Ogre::FontManager* fontManager = Ogre::FontManager::getSingletonPtr();
	Ogre::ResourcePtr font = fontManager->create("MyFont", "General");
	font->setParameter("type", "truetype");
	font->setParameter("source", "bluehigh.ttf");
	font->setParameter("size", "32");
	font->load();

	mTextPanel = (Ogre::OverlayContainer*)(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "ParticleUniverseText"));
	mAverageFPS = (Ogre::TextAreaOverlayElement*)(Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", "ParticleUniverseStats"));
	mAverageFPS->_setPosition(0.0, 0.02);
	mAverageFPS->_setDimensions(0.3, 0.4);
	mAverageFPS->setColour(Ogre::ColourValue::White);
	mAverageFPS->setParameter("horz_align", "left");
	mAverageFPS->setFontName("MyFont");
	mTextPanel->addChild(mAverageFPS);
	mDebugOverlay = Ogre::OverlayManager::getSingleton().create("Core/DebugOverlay");
	mDebugOverlay->setZOrder(500);
	mDebugOverlay->add2D(mTextPanel);
	mDebugOverlay->show();
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::initialiseOrthoCameraGizmo(void)
{
	mOrthoCameraGizmoNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	_getCamera()->setProjectionType(Ogre::PT_PERSPECTIVE);
	_getCamera()->setFOVy(Ogre::Radian(0.78539819f));

	if (mGizmoManager)
	{
		mGizmoManager->attachOrthoCameraGizmo(mOrthoCameraGizmoNode);
		mGizmoManager->setVisibleOrthoCameraGizmo(false); // By default, a perspecive camera is used, so set this gizmo invisible
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::updateOverlay(void)
{
	if (!mDebugOverlay->isVisible())
		return;

	// Camera projection
	ParticleUniverse::String cameraProjectionType = "Perspective";
	if (_isCameraOrthographic())
	{
		cameraProjectionType = "Orthographic";
	}
	ParticleUniverse::String cameraProjection = " Camera projection: " + cameraProjectionType + "\n";
	ParticleUniverse::String caption = cameraProjection;

	// Average FPS
	const Ogre::RenderTarget::FrameStats& stats = mControl->GetRenderWindow()->getStatistics();
	ParticleUniverse::String averageFPSString = " Average FPS: " + ParticleUniverse::StringConverter::toString(stats.avgFPS) + "\n";

	// Position of the selected node
	Ogre::Vector3 pivot = getCurrentPivot();
	ParticleUniverse::String positionString = " Position: " + ParticleUniverse::StringConverter::toString(pivot.x) + " " +
		ParticleUniverse::StringConverter::toString(pivot.y) + " " +
		ParticleUniverse::StringConverter::toString(pivot.z) + "\n";
	caption = caption + averageFPSString + positionString;

	// Distance of the selected node
	Ogre::Vector3 d = _getCameraPosition() - pivot;
	ParticleUniverse::Real l = d.length();
	ParticleUniverse::String distanceString = " Camera distance: " + ParticleUniverse::StringConverter::toString(l) + "\n";
	caption = caption + distanceString;

	// Orientation of the selected node
	Ogre::SceneNode* node = mMainSceneNode;
	if (mGizmoManager && mGizmoManager->getAttachedNode())
	{
		node = mGizmoManager->getAttachedNode();
	}	ParticleUniverse::String orientationString;
	orientationString = " Orientation (wxyz): \n    " +
		ParticleUniverse::StringConverter::toString(node->_getDerivedOrientation().w) + " " +
		ParticleUniverse::StringConverter::toString(node->_getDerivedOrientation().z) + " " +
		ParticleUniverse::StringConverter::toString(node->_getDerivedOrientation().y) + " " +
		ParticleUniverse::StringConverter::toString(node->_getDerivedOrientation().z) + "\n";
	caption = caption + orientationString;

	if (mCurrentParticleSystemForRenderer)
	{
		// Number of emitted visual particles
		size_t numberOfVisualParticles = mCurrentParticleSystemForRenderer->getNumberOfEmittedParticles(ParticleUniverse::Particle::PT_VISUAL);
		mMaxNumberOfVisualParticles = std::max(mMaxNumberOfVisualParticles, numberOfVisualParticles);
		ParticleUniverse::String numberOfVisualParticlesString = " Total visual particles: " + 
			ParticleUniverse::StringConverter::toString(numberOfVisualParticles) +
			"\n";
		caption = caption + numberOfVisualParticlesString;
		ParticleUniverse::String maxNumberOfVisualParticlesString = " Max visual particles: " + 
			ParticleUniverse::StringConverter::toString(mMaxNumberOfVisualParticles) +
			"\n";
		caption = caption + maxNumberOfVisualParticlesString;

		// Number of emitted non-visual particles
		size_t numberOfEmittedParticles = mCurrentParticleSystemForRenderer->getNumberOfEmittedParticles() - 
			mCurrentParticleSystemForRenderer->getNumberOfEmittedParticles(ParticleUniverse::Particle::PT_VISUAL);
		mMaxNumberOfEmittedParticles = std::max(mMaxNumberOfEmittedParticles, numberOfEmittedParticles);
		ParticleUniverse::String numberOfNonVisualParticlesString = " Total non-visual particles: " + 
			ParticleUniverse::StringConverter::toString(numberOfEmittedParticles) +
			"\n";
		caption = caption + numberOfNonVisualParticlesString;
		ParticleUniverse::String maxNumberOfNonVisualParticlesString = " Max non-visual particles: " + 
			ParticleUniverse::StringConverter::toString(mMaxNumberOfEmittedParticles) +
			"\n";
		caption = caption + maxNumberOfNonVisualParticlesString;
	}

	mAverageFPS->setCaption(caption);
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::showOverlay(bool show)
{
	if (!mDebugOverlay)
		return;

	if (show)
	{
		mDebugOverlay->show();
	}
	else
	{
		mDebugOverlay->hide();
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::resetOverlay(void)
{
	mMaxNumberOfEmittedParticles = 0;
	mMaxNumberOfVisualParticles = 0;
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::createGridPlane(ParticleUniverse::Real distance)
{
	distance = Ogre::Math::Abs(distance);
	ParticleUniverse::Real uTile = 1.0f;
	ParticleUniverse::Real vTile = 1.0f;

	// Create the mesh (called "grid")
	Ogre::MeshManager::getSingleton().createPlane("gridplane", 
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::Plane(Ogre::Vector3::UNIT_Y, 0),
		5000, 
		5000, 
		8, 
		8, 
		true, 
		1, 
		1, 
		1, 
		Ogre::Vector3::UNIT_Z);

	// Create the grid entity
	mGridPlaneSceneNodeBottom = mSceneManager->getRootSceneNode()->createChildSceneNode();
	mGridPlaneSceneNodeX = mSceneManager->getRootSceneNode()->createChildSceneNode();
	mGridPlaneSceneNodeMinX = mSceneManager->getRootSceneNode()->createChildSceneNode();
	mGridPlaneSceneNodeZ = mSceneManager->getRootSceneNode()->createChildSceneNode();
	mGridPlaneSceneNodeMinZ = mSceneManager->getRootSceneNode()->createChildSceneNode();

	mGridPlaneSceneNodeBottom->_setDerivedPosition(Ogre::Vector3(0, -distance, 0));
	mGridPlaneSceneNodeX->_setDerivedPosition(Ogre::Vector3(distance, 0, 0));
	mGridPlaneSceneNodeMinX->_setDerivedPosition(Ogre::Vector3(-distance, 0, 0));
	mGridPlaneSceneNodeZ->_setDerivedPosition(Ogre::Vector3(0, 0, distance));
	mGridPlaneSceneNodeMinZ->_setDerivedPosition(Ogre::Vector3(0, 0, -distance));

	mGridBottom = mSceneManager->createEntity("Grid", "gridplane");
	mGridBottom->setMaterialName("ParticleUniverseEditor/TransparentGrid");
	mGridBottom->setRenderQueueGroup(Ogre::RENDER_QUEUE_2);
	mGridPlaneSceneNodeBottom->attachObject(mGridBottom);

	mGridX = mSceneManager->createEntity("GridX", "gridplane");
	mGridX->setMaterialName("ParticleUniverseEditor/TransparentGrid");
	mGridX->setRenderQueueGroup(Ogre::RENDER_QUEUE_2);
	mGridPlaneSceneNodeX->attachObject(mGridX);

	mGridMinX = mSceneManager->createEntity("GridMinX", "gridplane");
	mGridMinX->setMaterialName("ParticleUniverseEditor/TransparentGrid");
	mGridMinX->setRenderQueueGroup(Ogre::RENDER_QUEUE_2);
	mGridPlaneSceneNodeMinX->attachObject(mGridMinX);

	mGridZ = mSceneManager->createEntity("GridZ", "gridplane");
	mGridZ->setMaterialName("ParticleUniverseEditor/TransparentGrid");
	mGridZ->setRenderQueueGroup(Ogre::RENDER_QUEUE_2);
	mGridPlaneSceneNodeZ->attachObject(mGridZ);

	mGridMinZ = mSceneManager->createEntity("GridMinZ", "gridplane");
	mGridMinZ->setMaterialName("ParticleUniverseEditor/TransparentGrid");
	mGridMinZ->setRenderQueueGroup(Ogre::RENDER_QUEUE_2);
	mGridPlaneSceneNodeMinZ->attachObject(mGridMinZ);

	// Set orientation
	Ogre::Real sqrt0dot5 = sqrt(0.5);
	mGridPlaneSceneNodeBottom->_setDerivedOrientation(Ogre::Quaternion::IDENTITY);
	mGridPlaneSceneNodeX->_setDerivedOrientation(Ogre::Quaternion (sqrt0dot5, 0, 0, sqrt0dot5));
	mGridPlaneSceneNodeMinX->_setDerivedOrientation(Ogre::Quaternion (-sqrt0dot5, 0, 0, sqrt0dot5));
	mGridPlaneSceneNodeZ->_setDerivedOrientation(Ogre::Quaternion (-sqrt0dot5, sqrt0dot5, 0, 0));
	mGridPlaneSceneNodeMinZ->_setDerivedOrientation(Ogre::Quaternion (sqrt0dot5, sqrt0dot5, 0, 0));

	// Set to invisible, but show the bottom plane
	setOrthoGridVisible(false);
	showGridPlane(true);
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::showGridPlane(bool visible)
{
	if (mGridBottom)
	{
		mGridBottom->setVisible(visible);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::setOrthoGridVisible(bool visible, Gizmo::Axis axis)
{
	if (!mGridBottom || !mGridX || !mGridMinX || !mGridZ || !mGridMinZ)
		return;

	switch(axis)
	{
		case Gizmo::AXIS_ALL:
		{
			mGridBottom->setVisible(visible);
			mGridX->setVisible(visible);
			mGridMinX->setVisible(visible);
			mGridZ->setVisible(visible);
			mGridMinZ->setVisible(visible);
		}
		break;

		case Gizmo::AXIS_X:
		{
			mGridBottom->setVisible(!visible);
			mGridX->setVisible(visible);
			mGridMinX->setVisible(!visible);
			mGridZ->setVisible(!visible);
			mGridMinZ->setVisible(!visible);
		}
		break;

		case Gizmo::AXIS_MIN_X:
		{
			mGridBottom->setVisible(!visible);
			mGridX->setVisible(!visible);
			mGridMinX->setVisible(visible);
			mGridZ->setVisible(!visible);
			mGridMinZ->setVisible(!visible);
		}
		break;

		case Gizmo::AXIS_Y:
		{
			mGridBottom->setVisible(visible);
			mGridX->setVisible(!visible);
			mGridMinX->setVisible(!visible);
			mGridZ->setVisible(!visible);
			mGridMinZ->setVisible(!visible);
		}
		break;

		case Gizmo::AXIS_Z:
		{
			mGridBottom->setVisible(!visible);
			mGridX->setVisible(!visible);
			mGridMinX->setVisible(!visible);
			mGridZ->setVisible(visible);
			mGridMinZ->setVisible(!visible);
		}
		break;

		case Gizmo::AXIS_MIN_Z:
		{
			mGridBottom->setVisible(!visible);
			mGridX->setVisible(!visible);
			mGridMinX->setVisible(!visible);
			mGridZ->setVisible(!visible);
			mGridMinZ->setVisible(visible);
		}
		break;
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::setDistanceGridPlane(ParticleUniverse::Real distance)
{
	if (mGridPlaneSceneNodeBottom)
	{
		mGridPlaneSceneNodeBottom->_setDerivedPosition(Ogre::Vector3(0, distance, 0));
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::setScaleGridPlane(ParticleUniverse::Real scale)
{
	Ogre::MaterialPtr mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("ParticleUniverseEditor/GridMaterial"));
	if (!mat.isNull())
	{
		Ogre::Technique* tec = (mat)->getTechnique(0);
		if (tec)
		{
			Ogre::Pass* pass = tec->getPass(0);
			if (pass)
			{
				Ogre::TextureUnitState* tus = pass->getTextureUnitState(0);
				if (tus)
				{
					tus->setTextureScale(scale, scale);
				}
			}
		}
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::createLight(void)
{
	if (!mLightEntity)
	{
		mLightEntity = mSceneManager->createEntity("pu_light_entity", "pu_light.mesh");
		mLightEntity->setMaterialName("ParticleUniverseEditor/LightMaterial");
		mLightEntity->setQueryFlags(GizmoManager::OBJECT_FLAG);
		mLight = mSceneManager->createLight("pu_light");
		mLightSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
		mLightSceneNode->_setDerivedPosition(Ogre::Vector3(0, 100, 0));
		mLightSceneNode->attachObject(mLightEntity);
		mLightSceneNode->attachObject(mLight);
		mLightSceneNode->setScale(0.7f, 0.7f, 0.0f);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::showLight(bool show)
{
	if (mLightEntity && mLight)
	{
		mLightEntity->setVisible(show);
		mLight->setVisible(show);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::selectLightColour(wxPoint position)
{
	wxColourDialog colourDialog(this);
	colourDialog.SetPosition(mNotebook->GetScreenPosition() + position);
	if (colourDialog.ShowModal() == wxID_OK)
	{
		wxColourData colourData = colourDialog.GetColourData();
		Ogre::ColourValue colour;
		wxColour c = colourData.GetColour();
		colour.r = ((ParticleUniverse::Real)c.Red())/255.0f;
		colour.g = ((ParticleUniverse::Real)c.Green())/255.0f;
		colour.b = ((ParticleUniverse::Real)c.Blue())/255.0f;
		colour.a = ((ParticleUniverse::Real)c.Alpha())/255.0f;

		if (mLight)
		{
			mLight->setDiffuseColour(colour);
		}
	}
}
//------------------------------------------------------------------------------
void ParticleUniverseEditorFrame::doWindowLeave(wxMouseEvent& event)
{
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::activateQueryFlags(bool active)
{
	if (active)
	{
		mMarker->setQueryFlags(GizmoManager::OBJECT_FLAG);
		mMarkerBold->setQueryFlags(GizmoManager::OBJECT_FLAG);
		if (mAddEntity)
		{
			mAddEntity->setQueryFlags(GizmoManager::OBJECT_FLAG);
		}
		if (mLightEntity)
		{
			mLightEntity->setQueryFlags(GizmoManager::OBJECT_FLAG);
		}
	}
	else
	{
		mMarker->setQueryFlags(0);
		mMarkerBold->setQueryFlags(0);
		if (mAddEntity)
		{
			mAddEntity->setQueryFlags(0);
		}
		if (mLightEntity)
		{
			mLightEntity->setQueryFlags(0);
		}
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::synchronizeSmallOgreControlWithRenderTab(void)
{
	// Synchronize positions, orientations and scale of the camera and particle system
	Ogre::Camera* cameraRenderTab = _getCamera();
	Ogre::Camera* cameraEditTab = mOgreControlSmall->GetCamera();
	if (cameraRenderTab && cameraEditTab)
	{
		cameraEditTab->setPosition(cameraRenderTab->getPosition());
		cameraEditTab->setOrientation(cameraRenderTab->getOrientation());
		cameraEditTab->setDirection(cameraRenderTab->getDirection());
	}
	if (mCurrentParticleSystemForRenderer)
	{
		Ogre::Node* pSysNode = mCurrentParticleSystemForRenderer->getParentNode(); // This is either mMainSceneNode or a Bone (TagPoint)
		if (pSysNode)
		{
			Ogre::SceneNode* nodeEditTab = mOgreControlSmall->getMainSceneNode();
			nodeEditTab->setPosition(pSysNode->_getDerivedPosition());
			nodeEditTab->setOrientation(pSysNode->_getDerivedOrientation());
		}
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::OnKeyPressed(wxKeyEvent& event)
{
	// Check on TAB, q and Q
	if (event.GetKeyCode() == WXK_TAB || event.GetKeyCode() == 113 || event.GetKeyCode() == 81)
	{
		if (mGizmoManager->getAttachedNode() == mMainSceneNode)
		{
			// Attach to the entity node, otherwise attach to the light node
			if (mAddEntitySceneNode->numAttachedObjects() > 0)
			{
				mGizmoManager->attachToNode(mAddEntitySceneNode);
				_cameraLookAt(mAddEntitySceneNode->_getDerivedPosition());
			}
			else if (mLightSceneNode->numAttachedObjects() > 0 && mLightSceneNode->getAttachedObject(1)->isVisible())
			{
				mGizmoManager->attachToNode(mLightSceneNode);
				_cameraLookAt(mLightSceneNode->_getDerivedPosition());
			}
		}
		else if (mGizmoManager->getAttachedNode() == mAddEntitySceneNode)
		{
			// Attach to the light node, otherwise attach to the main node
			if (mLightSceneNode->numAttachedObjects() > 0 && mLightSceneNode->getAttachedObject(1)->isVisible())
			{
				if (!mGizmoManager->attachToNode(mLightSceneNode))
				{
					// Apparently, the gizmo cannot be attached to the light, so attach it to the main node instead
					mGizmoManager->attachToNode(mMainSceneNode);
					_cameraLookAt(mMainSceneNode->_getDerivedPosition());
				}
				else
				{
					_cameraLookAt(mLightSceneNode->_getDerivedPosition());
				}
			}
			else
			{
				mGizmoManager->attachToNode(mMainSceneNode);
				_cameraLookAt(mMainSceneNode->_getDerivedPosition());
			}
		}
		else if (mGizmoManager->getAttachedNode() == mLightSceneNode)
		{
			// Attach to the main node
			mGizmoManager->attachToNode(mMainSceneNode);
			_cameraLookAt(mMainSceneNode->_getDerivedPosition());
		}
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::callbackSpinCtrlXYZ(Ogre::Real angleX, Ogre::Real angleY, Ogre::Real angleZ, const Ogre::String& order)
{
	Ogre::SceneNode* parentNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	Ogre::SceneNode* childNode = parentNode->createChildSceneNode();
	Ogre::SceneNode* grandChildNode = childNode->createChildSceneNode();
	mGizmoManager->setOrientation(Ogre::Quaternion::IDENTITY);
	Ogre::Quaternion orientationX = Ogre::Quaternion::IDENTITY;
	Ogre::Quaternion orientationY = Ogre::Quaternion::IDENTITY;
	Ogre::Quaternion orientationZ = Ogre::Quaternion::IDENTITY;
	orientationX.FromAngleAxis(Ogre::Radian(Ogre::Degree(angleX)), Ogre::Vector3::UNIT_X);
	orientationY.FromAngleAxis(Ogre::Radian(Ogre::Degree(angleY)), Ogre::Vector3::UNIT_Y);
	orientationZ.FromAngleAxis(Ogre::Radian(Ogre::Degree(angleZ)), Ogre::Vector3::UNIT_Z);

	if (order == "XYZ")
	{
		grandChildNode->setOrientation(orientationZ);
		childNode->setOrientation(orientationY);
		parentNode->setOrientation(orientationX);
	}
	else if (order == "YXZ")
	{
		grandChildNode->setOrientation(orientationZ);
		childNode->setOrientation(orientationX);
		parentNode->setOrientation(orientationY);
	}
	else if (order == "ZXY")
	{
		grandChildNode->setOrientation(orientationY);
		childNode->setOrientation(orientationX);
		parentNode->setOrientation(orientationZ);
	}
	mGizmoManager->setOrientation(grandChildNode->_getDerivedOrientation());
	mSceneManager->getRootSceneNode()->removeAndDestroyChild(parentNode->getName());

	mGizmoManager->applyWorldLocalSpaceSetting(); // Apply the current worldspace settings to the Gizmo
}
//-----------------------------------------------------------------------
Ogre::SceneNode* ParticleUniverseEditorFrame::getAddEntitySceneNode(void)
{
	return mAddEntitySceneNode;
}
//-----------------------------------------------------------------------
Ogre::Entity* ParticleUniverseEditorFrame::getAddEntity(void)
{
	return mAddEntity;
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::restoreFocus(void)
{
	if (mListOfTemplates)
	{
		// Also to be able to zoom with the mouse on the render window
		mListOfTemplates->SetFocus();
	}
}
//-----------------------------------------------------------------------
Ogre::Quaternion ParticleUniverseEditorFrame::getOrientationMainSceneNode(void)
{
	if (mMainSceneNode)
	{
		return mMainSceneNode->getOrientation();
	}
	return Ogre::Quaternion::IDENTITY;
}
//-----------------------------------------------------------------------
ParticleUniverse::ParticleSystem* ParticleUniverseEditorFrame::getCurrentParticleSystemForRenderer(void)
{
	return mCurrentParticleSystemForRenderer;
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::notifyParticleSystemAttachToEntity(void)
{
	// Called when the particle system is going to be attached to an entity
	if (mParticlerSystemSceneNode->numAttachedObjects() > 0)
	{
		// Assume only the particle system is attached to mParticlerSystemSceneNode
		mParticlerSystemSceneNode->detachObject(mCurrentParticleSystemForRenderer);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::notifyParticleSystemDetachedFromEntity(void)
{
	// Called after the particle system is detached from an entity
	mParticlerSystemSceneNode->attachObject(mCurrentParticleSystemForRenderer);
	if (mGizmoManager && mGizmoManager->getAttachedNode() == mMainSceneNode)
	{
		mGizmoManager->applyWorldLocalSpaceSetting();
	}
}
//-----------------------------------------------------------------------
GizmoManager* ParticleUniverseEditorFrame::getGizmoManager(void)
{
	return mGizmoManager;
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::notifyAnimationStarted(void)
{
	// Nothing yet
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::notifyAnimationStopped(void)
{
	// Reset the Gizmo orientation if it was attached to the tagpoint
	if (mGizmoManager && mGizmoManager->getAttachedNode() == mMainSceneNode)
	{
		mGizmoManager->setOrientation(mAnimationWindow->_getDerivedBaseOrientation());
	}
}
//-----------------------------------------------------------------------
const Ogre::Vector3& ParticleUniverseEditorFrame::_getCameraPosition(void)
{
	if (_getCamera())
	{
		return _getCamera()->getPosition();
	}
	return Ogre::Vector3::ZERO;
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::_setCameraPosition(const Ogre::Vector3& position)
{
	if (_getCamera())
	{
		_getCamera()->setPosition(position);
	}
}
//-----------------------------------------------------------------------
const Ogre::Quaternion& ParticleUniverseEditorFrame::_getCameraOrientation(void)
{
	if (_getCamera())
	{
		return _getCamera()->getOrientation();
	}
	return Ogre::Quaternion::IDENTITY;
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::_setCameraOrientation(const Ogre::Quaternion& orientation)
{
	if (_getCamera())
	{
		return _getCamera()->setOrientation(orientation);
	}
}
//-----------------------------------------------------------------------
void ParticleUniverseEditorFrame::_cameraLookAt(const Ogre::Vector3& direction)
{
	// The orthographic camera always looks at the centre for convenience
	if (_getProjectionType() == Ogre::PT_ORTHOGRAPHIC)
	{
		return _getCamera()->lookAt(Ogre::Vector3::ZERO);
	}

	if (_getCamera())
	{
		return _getCamera()->lookAt(direction);
	}
}
//-----------------------------------------------------------------------
Ogre::ProjectionType ParticleUniverseEditorFrame::_getProjectionType(void)
{
	if (_getCamera())
	{
		return _getCamera()->getProjectionType();
	}

	return Ogre::PT_PERSPECTIVE;
}
//-----------------------------------------------------------------------
bool ParticleUniverseEditorFrame::_isCameraOrthographic(void)
{
	if (_getCamera())
	{
		return _getCamera()->getProjectionType() == Ogre::PT_ORTHOGRAPHIC;
	}

	return false;
}
//-----------------------------------------------------------------------
const Ogre::Vector3& ParticleUniverseEditorFrame::getCurrentPivot(void)
{
	Ogre::SceneNode* node = 0;
	if (mGizmoManager && mGizmoManager->getAttachedNode())
	{
		node = mGizmoManager->getAttachedNode();
	}
	else
	{
		node = mMainSceneNode;
	}
	if (node)
	{
		return node->_getDerivedPosition();
	}

	return Ogre::Vector3::ZERO;
}
//-----------------------------------------------------------------------
Ogre::Camera* ParticleUniverseEditorFrame::_getCamera(void)
{
	return mControl->GetCamera();
}