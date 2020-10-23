/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_MATERIAL_TAB_H__
#define __PUED_MATERIAL_TAB_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseMaterialListBox.h"
#include "wx/notebook.h"

static wxString SBT_ADD = wxT("");
static wxString SBT_TRANSPARENT_ALPHA = wxT("");
static wxString SBT_TRANSPARENT_COLOUR = wxT("");
static wxString SBT_MODULATE = wxT("");
static wxString SBT_REPLACE = wxT("");
static wxString SBT_SOURCE_COLOUR_ONE = wxT("");
static wxString SBT_SOURCE_COLOUR_ZERO = wxT("");
static wxString SBT_SOURCE_COLOUR_DEST_COLOUR = wxT("");
static wxString SBT_DEST_COLOUR_ONE = wxT("");
static wxString SBT_DEST_COLOUR_SOURCE_COLOUR = wxT("");
static wxString TAM_WRAP = wxT("");
static wxString TAM_MIRROR = wxT("");
static wxString TAM_CLAMP = wxT("");

/**	Material Tab: Class that defines the actual material tab.
*/
class MaterialTab : public wxPanel
{
	public:

		// Constructor / Destructor
		MaterialTab(wxWindow* parent, wxWindow* rootParent);
		~MaterialTab(void);

		/**	Reload listbox and fill with materials (names)
		*/
		void fillMaterials(void);

		/**	A material has been selected. Fill the properties.
		*/
		void selectMaterial(wxString& materialName);

		/**	Display the image associated with the TextureUnitState
		*/
		void viewTexture(Ogre::TextureUnitState* textureUnitState);

		/**	Select the material that was previously selected.
		*/
		//void setLastSelectedMaterial(void);

		/**	Store the name of the material that is currently selected.
		*/
		void saveSelectedMaterialName(void);

		/**	Restore the name of the saved material name.
		*/
		void restoreSelectedMaterialName(void);

		/**	Convenient functions to return pass and texture.
		*/
		Ogre::Pass* getFirstPass(void);
		Ogre::TextureUnitState* getFirstTexture(void);
		Ogre::TextureUnitState* getFirstTexture(Ogre::Pass* pass);
		Ogre::TextureUnitState* forceCreateFirstTexture(const Ogre::String textureName);

	protected:
		wxWindow* mRootParent;
		MaterialListBox* mMaterialListBox;
		wxTextCtrl* mTxtMaterialName;
		wxCheckBox* mCheckLighting;
		wxCheckBox* mCheckDepthCheck;
		wxCheckBox* mCheckDepthWrite;
		wxComboBox* mSceneBlendList;
		wxComboBox* mAddressingModeList;
		wxButton* mNewButton;
		wxButton* mSaveButton;
		wxButton* mLoadButton;
		wxTextCtrl* mTxtTextureLoad;
		wxButton* mAmbientButton;
		wxButton* mDiffuseButton;
		wxButton* mSpecularButton;
		wxButton* mEmissiveButton;
		wxPanel* mTexturePanel;

		unsigned int mNewMaterialCounter;
		wxString mLastSelectedMaterial;
		wxString mResourceLocationTexture;
		wxString mResourceLocationMaterial;
		Ogre::String mCurrentTextureFileName;
		wxBitmap* mCurrentTextureBitmap;
		wxImage* mCurrentTextureImage;

		/**	Returns the sceneblending type as a string.
		*/
		const wxString& getSceneBlending(const Ogre::Pass* pass);

		/**	Returns the texture addressing mode as a string.
		*/
		const wxString& getTextureAddressingMode(const Ogre::TextureUnitState* texture);

		/**	New material button pressed.
		*/
		void OnNewMaterial(wxCommandEvent& event);

		/**	Name of the material changed.
		*/
		void OnChangeName(wxCommandEvent& event);

		/**	Checkbox Lighting event
		*/
		void OnCheckLighting(wxCommandEvent& event);

		/**	Ambient button event
		*/
		void OnAmbient(wxCommandEvent& event);

		/**	Diffuse button event
		*/
		void OnDiffuse(wxCommandEvent& event);

		/**	Specular button event
		*/
		void OnSpecular(wxCommandEvent& event);

		/**	Emissive button event
		*/
		void OnEmissive(wxCommandEvent& event);

		/**	Checkbox DepthCheck event
		*/
		void OnCheckDepthCheck(wxCommandEvent& event);

		/**	Checkbox DepthWrite event
		*/
		void OnCheckDepthWrite(wxCommandEvent& event);

		/**	Combobox SceneBlend event
		*/
		void OnSceneBlendSelected(wxCommandEvent& event);

		/**	Save the currently selected material
		*/
		void OnSaveMaterial(wxCommandEvent& event);

		/**	Load a texture
		*/
		void OnLoadTexture(wxCommandEvent& event);

		/**	Determine whether the selected material is already used by one of the particle systems.
		*/
		bool isSelectedMaterialInUse();

		/**	Display the texture.
		*/
		void OnPaint(wxPaintEvent& event);
};

#endif
