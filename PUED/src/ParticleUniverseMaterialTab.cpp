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
#include "ParticleUniverseMaterialTab.h"
#include "ParticleUniverseEditor.h"
#include <wx/colordlg.h>

//-----------------------------------------------------------------------
MaterialTab::MaterialTab(wxWindow* parent, wxWindow* rootParent) : 
	wxPanel(parent, wxID_ANY),
	mNewMaterialCounter(0),
	mLastSelectedMaterial(wxT("")),
	mResourceLocationTexture(wxT("")),
	mResourceLocationMaterial(wxT("")),
	mRootParent(rootParent),
	mCurrentTextureFileName(Ogre::StringUtil::BLANK),
	mCurrentTextureBitmap(0),
	mCurrentTextureImage(0)
{
	SBT_ADD = _("Add");
	SBT_TRANSPARENT_ALPHA = _("Transparent alpha");
	SBT_TRANSPARENT_COLOUR = _("Transparent colour");
	SBT_MODULATE = _("Modulate");
	SBT_REPLACE = _("Replace");
	SBT_SOURCE_COLOUR_ONE = _("Source colour - One");
	SBT_SOURCE_COLOUR_ZERO = _("Source colour - Zero");
	SBT_SOURCE_COLOUR_DEST_COLOUR = _("Source colour - Destination colour");
	SBT_DEST_COLOUR_ONE = _("Destination colour - One");
	SBT_DEST_COLOUR_SOURCE_COLOUR = _("Destination colour - Source colour");
	TAM_WRAP = _("Wrap");
	TAM_MIRROR = _("Mirror");
	TAM_CLAMP = _("Clamp");

	// Create new resourcegroups
	Ogre::ResourceGroupManager::getSingletonPtr()->createResourceGroup(TEMP_MATERIAL_GROUP);
	Ogre::ResourceGroupManager::getSingletonPtr()->createResourceGroup(TEMP_TEXTURE_GROUP);

	// Setup dialog
	wxBoxSizer* mMainSizer = new wxBoxSizer(wxHORIZONTAL); // Main sizer
	wxBoxSizer* mMaterialEditSizer = new wxBoxSizer(wxVERTICAL);
	mMaterialListBox = new MaterialListBox(this, this, wxID_ANY);
	mMainSizer->Add(mMaterialListBox, 1, wxEXPAND);
	mMainSizer->Add(mMaterialEditSizer, 3, wxEXPAND);

	// ---- Add material edit controls ----

	// New button
	mNewButton = new wxButton(this, wxID_ANY, _("New material"), wxDefaultPosition, wxSize(120, 24));
	mMaterialEditSizer->Add(mNewButton, 0, wxALL, 8);

	// Material name and save
	wxBoxSizer* materialSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* materialNameAndSaveSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* materialPropertiesSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticText* txtMaterial = new wxStaticText(this, wxID_ANY, _("Material"));
	mTxtMaterialName = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(256, 24));

	mSaveButton = new wxButton(this, wxID_ANY, _("Save"), wxDefaultPosition, wxSize(120, 24));
#ifndef PU_FULL_VERSION
	mSaveButton->Disable();
#endif // PU_FULL_VERSION
	materialNameAndSaveSizer->Add(mTxtMaterialName, 0, wxALL, 8);
	materialNameAndSaveSizer->Add(mSaveButton, 0, wxALL, 8);
	materialPropertiesSizer->Add(materialNameAndSaveSizer, 0, wxALL);
	materialSizer->Add(txtMaterial, 0, wxALIGN_CENTER_VERTICAL);
	materialSizer->AddSpacer(112);
	materialSizer->Add(materialPropertiesSizer, 0, wxALIGN_CENTER_VERTICAL);
	mMaterialEditSizer->Add(materialSizer, 0, wxALL, 8);

	// Pass properties
	wxBoxSizer* passSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* passPropertiesSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* passLightingSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* passLightingColourSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* passSceneBlendSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* txtPass = new wxStaticText(this, wxID_ANY, _("Pass"));
	mCheckLighting = new wxCheckBox(this, wxID_ANY, _("Lighting"), wxDefaultPosition, wxSize(120, 16));
	mAmbientButton = new wxButton(this, wxID_ANY, _("Ambient"), wxDefaultPosition, wxSize(120, 24));
	mDiffuseButton = new wxButton(this, wxID_ANY, _("Diffuse"), wxDefaultPosition, wxSize(120, 24));
	mSpecularButton = new wxButton(this, wxID_ANY, _("Specular"), wxDefaultPosition, wxSize(120, 24));
	mEmissiveButton = new wxButton(this, wxID_ANY, _("Emissive"), wxDefaultPosition, wxSize(120, 24));
	mCheckDepthCheck = new wxCheckBox(this, wxID_ANY, _("Depth check"), wxDefaultPosition, wxSize(120, 16));
    mCheckDepthWrite = new wxCheckBox(this, wxID_ANY, _("Depth write"), wxDefaultPosition, wxSize(120, 16));

	wxStaticText* txtSceneBlend = new wxStaticText(this, wxID_ANY, _("Scene blend"), wxDefaultPosition, wxSize(136, 16));
	wxString sceneBlendChoices[10];
//	sceneBlendChoices[0] = wxT(SBT_ADD);
//	sceneBlendChoices[1] = wxT(SBT_TRANSPARENT_ALPHA);
//	sceneBlendChoices[2] = wxT(SBT_TRANSPARENT_COLOUR);
//	sceneBlendChoices[3] = wxT(SBT_MODULATE);
//	sceneBlendChoices[4] = wxT(SBT_REPLACE);
//	sceneBlendChoices[5] = wxT(SBT_SOURCE_COLOUR_ONE);
//	sceneBlendChoices[6] = wxT(SBT_SOURCE_COLOUR_ZERO);
//	sceneBlendChoices[7] = wxT(SBT_SOURCE_COLOUR_DEST_COLOUR);
//	sceneBlendChoices[8] = wxT(SBT_DEST_COLOUR_ONE);
//	sceneBlendChoices[9] = wxT(SBT_DEST_COLOUR_SOURCE_COLOUR);
	sceneBlendChoices[0] = SBT_ADD;
	sceneBlendChoices[1] = SBT_TRANSPARENT_ALPHA;
	sceneBlendChoices[2] = SBT_TRANSPARENT_COLOUR;
	sceneBlendChoices[3] = SBT_MODULATE;
	sceneBlendChoices[4] = SBT_REPLACE;
	sceneBlendChoices[5] = SBT_SOURCE_COLOUR_ONE;
	sceneBlendChoices[6] = SBT_SOURCE_COLOUR_ZERO;
	sceneBlendChoices[7] = SBT_SOURCE_COLOUR_DEST_COLOUR;
	sceneBlendChoices[8] = SBT_DEST_COLOUR_ONE;
	sceneBlendChoices[9] = SBT_DEST_COLOUR_SOURCE_COLOUR;
	mSceneBlendList = new wxComboBox(this, 
		wxID_ANY, 
		SBT_ADD, 
		wxDefaultPosition, 
		wxSize(256, 24), 
		10,
		sceneBlendChoices, 
		wxCB_READONLY);
	passLightingColourSizer->Add(mAmbientButton, 0, wxALL, 8);
	passLightingColourSizer->Add(mDiffuseButton, 0, wxALL, 8);
	passLightingColourSizer->Add(mSpecularButton, 0, wxALL, 8);
	passLightingColourSizer->Add(mEmissiveButton, 0, wxALL, 8);
	passLightingSizer->Add(mCheckLighting, 0, wxALL, 8);
	passLightingSizer->Add(passLightingColourSizer, 0, wxALL);
	passPropertiesSizer->Add(passLightingSizer, 0, wxALL);
	passPropertiesSizer->Add(mCheckDepthCheck, 0, wxALL, 8);
	passPropertiesSizer->Add(mCheckDepthWrite, 0, wxALL, 8);
	passSceneBlendSizer->Add(txtSceneBlend, 0, wxALIGN_CENTER_VERTICAL, 8);
	passSceneBlendSizer->Add(mSceneBlendList, 0, wxALIGN_CENTER_VERTICAL, 8);
	passPropertiesSizer->Add(passSceneBlendSizer, 0, wxALL, 8);
	passSizer->Add(txtPass, 0, wxALIGN_TOP);
	passSizer->AddSpacer(132);
	passSizer->Add(passPropertiesSizer, 0, wxALIGN_TOP);
	mMaterialEditSizer->Add(passSizer, 0, wxALL, 8);

	// Texture properties
	wxBoxSizer* textureSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* texturePropertiesSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* textureLoadSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* textureAddressingModeSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* txtTexture = new wxStaticText(this, wxID_ANY, _("Texture"));
	mLoadButton = new wxButton(this, wxID_ANY, _("Load"), wxDefaultPosition, wxSize(124, 24));
	mTxtTextureLoad = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(256, 24));
	mTxtTextureLoad->Disable();

	wxStaticText* txtAddressingMode = new wxStaticText(this, wxID_ANY, _("Address mode"), wxDefaultPosition, wxSize(140, 16));
	wxString addressingModeChoices[3];
	addressingModeChoices[0] = TAM_WRAP;
	addressingModeChoices[1] = TAM_CLAMP;
	addressingModeChoices[2] = TAM_MIRROR ;
	mAddressingModeList = new wxComboBox(this, 
		wxID_ANY, 
		TAM_WRAP, 
		wxDefaultPosition, 
		wxSize(120, 24), 
		3, 
		addressingModeChoices, 
		wxCB_READONLY);

	mTexturePanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(320, 320), wxBORDER_SIMPLE);
	textureLoadSizer->Add(mLoadButton, 0, wxALL, 8);
	textureLoadSizer->Add(mTxtTextureLoad, 0, wxALL, 8);
	texturePropertiesSizer->Add(textureLoadSizer, 0, wxALL);
	textureAddressingModeSizer->Add(txtAddressingMode, 0, wxALIGN_CENTER_VERTICAL, 8);
	textureAddressingModeSizer->Add(mAddressingModeList, 0, wxALIGN_CENTER_VERTICAL, 8);
	texturePropertiesSizer->Add(textureAddressingModeSizer, 0, wxALL, 8);
	texturePropertiesSizer->AddSpacer(8);
	texturePropertiesSizer->Add(mTexturePanel, 0, wxALL, 8);
	textureSizer->Add(txtTexture, 0, wxALIGN_TOP);
	textureSizer->AddSpacer(112);
	textureSizer->Add(texturePropertiesSizer, 0, wxALIGN_TOP);
	mMaterialEditSizer->AddSpacer(16);
	mMaterialEditSizer->Add(textureSizer, 0, wxALL, 8);

	SetSizer(mMainSizer);
	mMainSizer->Layout();

	// Events
	mNewButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MaterialTab::OnNewMaterial), NULL, this);
	mSaveButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MaterialTab::OnSaveMaterial), NULL, this);
	mTxtMaterialName->Connect(wxEVT_KILL_FOCUS, wxCommandEventHandler(MaterialTab::OnChangeName), NULL, this);
	mCheckLighting->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MaterialTab::OnCheckLighting), NULL, this);
	mAmbientButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MaterialTab::OnAmbient), NULL, this);
	mDiffuseButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MaterialTab::OnDiffuse), NULL, this);
	mSpecularButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MaterialTab::OnSpecular), NULL, this);
	mEmissiveButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MaterialTab::OnEmissive), NULL, this);
	mCheckDepthCheck->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MaterialTab::OnCheckDepthCheck), NULL, this);
	mCheckDepthWrite->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MaterialTab::OnCheckDepthWrite), NULL, this);
	mSceneBlendList->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(MaterialTab::OnSceneBlendSelected), NULL, this);
	mLoadButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MaterialTab::OnLoadTexture), NULL, this);
	mTexturePanel->Connect(wxEVT_PAINT, wxPaintEventHandler(MaterialTab::OnPaint), NULL, this);
}
//-----------------------------------------------------------------------
MaterialTab::~MaterialTab(void)
{
	// Unload resources????
}
//-----------------------------------------------------------------------
void MaterialTab::fillMaterials(void)
{
	mMaterialListBox->loadMaterials();
	restoreSelectedMaterialName();
	selectMaterial(mMaterialListBox->GetStringSelection());
}
//-----------------------------------------------------------------------
void MaterialTab::selectMaterial(wxString& materialName)
{
	Ogre::TextureUnitState* textureUnitState = 0;
	mTxtMaterialName->SetValue(materialName);
	if (isSelectedMaterialInUse())
	{
		mTxtMaterialName->Disable();
	}
	else
	{
		mTxtMaterialName->Enable();
	}
	mLastSelectedMaterial = materialName;
	Ogre::String name = wx2ogre(materialName);
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(name);
	if (!material.isNull() && material->getNumTechniques() > 0)
	{
		material->load();
		mTxtTextureLoad->SetValue(wxT(""));
		Ogre::Technique* technique = material->getBestTechnique(); // Get the best technique
		if (technique && technique->getNumPasses() > 0)
		{
			Ogre::Pass* pass = technique->getPass(0); // Get the first
			if (pass)
			{
				// Set pass properties
				mCheckLighting->SetValue(pass->getLightingEnabled());
				mCheckDepthCheck->SetValue(pass->getDepthCheckEnabled());
				mCheckDepthWrite->SetValue(pass->getDepthWriteEnabled());
				mSceneBlendList->SetValue(getSceneBlending(pass));
				if (pass->getNumTextureUnitStates() > 0)
				{
					textureUnitState = pass->getTextureUnitState(0); // Get the first
					if (textureUnitState)
					{
						// Set texture properties
						if (textureUnitState->getNumFrames() > 0)
						{
							wxString name = ogre2wx(textureUnitState->getFrameTextureName(0));
							mTxtTextureLoad->SetValue(name);
						}

						mAddressingModeList->SetValue(getTextureAddressingMode(textureUnitState));
					}
				}
			}
		}
	}

	// Display image
	viewTexture(textureUnitState); // Clear the old texture if no TextureUnitState
}
//-----------------------------------------------------------------------
void MaterialTab::viewTexture(Ogre::TextureUnitState* textureUnitState)
{
	if (!textureUnitState)
	{
		mCurrentTextureFileName = Ogre::StringUtil::BLANK;
		mTexturePanel->Refresh();
		return;
	}

	try
	{
		// Put this between try-catch, because it can cause an exception for certain (font) materials
		Ogre::ResourceGroupManager& resGrpMgr = Ogre::ResourceGroupManager::getSingleton();
		Ogre::String textureName = textureUnitState->getTextureName();
		Ogre::String resourceGroup = resGrpMgr.findGroupContainingResource(textureName);
		Ogre::FileInfoListPtr fileInfoList = resGrpMgr.findResourceFileInfo(resourceGroup, textureName);
		Ogre::VectorIterator<Ogre::FileInfoList> fileInfoListIterator(*fileInfoList);
		if (fileInfoListIterator.hasMoreElements())
		{
			Ogre::FileInfo fileInfo = fileInfoListIterator.getNext();
			mCurrentTextureFileName = fileInfo.archive->getName() + "/" + fileInfo.filename;
		}
	}
	catch (Ogre::Exception e)
	{
		mCurrentTextureFileName = Ogre::StringUtil::BLANK;
	}
	mTexturePanel->Refresh();
}
//-----------------------------------------------------------------------
void MaterialTab::saveSelectedMaterialName(void)
{
	mLastSelectedMaterial = mMaterialListBox->GetStringSelection();
}
//-----------------------------------------------------------------------
void MaterialTab::restoreSelectedMaterialName(void)
{
	if (mLastSelectedMaterial == wxT(""))
	{
		mMaterialListBox->Select(0);
	}
	else
	{
		mMaterialListBox->SetStringSelection(mLastSelectedMaterial);
	}
}
//-----------------------------------------------------------------------
Ogre::Pass* MaterialTab::getFirstPass(void)
{
	wxString materialName = mMaterialListBox->GetStringSelection();
	Ogre::String name = wx2ogre(materialName);
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(name);
	if (!material.isNull() && material->getNumTechniques() > 0)
	{
		Ogre::Technique* technique = 0;
		material->load();
		technique = material->getBestTechnique(); // Get the best technique
		if (technique && technique->getNumPasses() > 0)
		{
			return technique->getPass(0); // Get the first
		}
	}
	return 0;
}
//-----------------------------------------------------------------------
Ogre::TextureUnitState* MaterialTab::getFirstTexture(void)
{
	wxString materialName = mMaterialListBox->GetStringSelection();
	Ogre::String name = wx2ogre(materialName);
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(name);
	if (!material.isNull() && material->getNumTechniques() > 0)
	{
		material->load();
		Ogre::Technique* technique = material->getBestTechnique(); // Get the best technique
		if (technique && technique->getNumPasses() > 0)
		{
			Ogre::Pass* pass = technique->getPass(0); // Get the first
			if (pass->getNumTextureUnitStates() > 0)
			{
				return pass->getTextureUnitState(0); // Get the first
			}
		}
	}
	return 0;
}
//-----------------------------------------------------------------------
Ogre::TextureUnitState* MaterialTab::getFirstTexture(Ogre::Pass* pass)
{
	if (pass->getNumTextureUnitStates() > 0)
	{
		return pass->getTextureUnitState(0); // Get the first
	}
	return 0;
}
//-----------------------------------------------------------------------
Ogre::TextureUnitState* MaterialTab::forceCreateFirstTexture(const Ogre::String textureName)
{
	// Ignore some materials because they result in a crash while unloading
	wxString materialName = mMaterialListBox->GetStringSelection();
	if (materialName == wxT("DefaultSettings"))
		return 0;

	Ogre::Technique* technique = 0;
	Ogre::TextureUnitState* texture = 0;
	Ogre::Pass* pass = getFirstPass();
	if (pass)
	{
		// There is a pass, check for textures or create one
		if (pass->getNumTextureUnitStates() > 0)
		{
			pass->removeAllTextureUnitStates();
		}
		texture = pass->createTextureUnitState(textureName);
	}
	else
	{
		// There is no pass
		wxString materialName = mMaterialListBox->GetStringSelection();
		Ogre::String name = wx2ogre(materialName);
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(name);
		if (!material.isNull())
		{
			material->load();
			if (material->getNumTechniques() > 0)
			{
				technique = material->getBestTechnique(); // Get the best technique
				pass = technique->createPass();
				texture = pass->createTextureUnitState(textureName);
			}
			else
			{
				// There is no technique, no pass and no textureunitstate
				technique = material->createTechnique();
				pass = technique->createPass();
				texture = pass->createTextureUnitState(textureName);
			}
		}
	}

	return texture;
}
//-----------------------------------------------------------------------
const wxString& MaterialTab::getSceneBlending(const Ogre::Pass* pass)
{
	if (!pass)
	{
		return SBT_ADD;
	}
	Ogre::SceneBlendFactor source = pass->getSourceBlendFactor();
	Ogre::SceneBlendFactor dest = pass->getDestBlendFactor();
	if (source == Ogre::SBF_SOURCE_ALPHA && dest == Ogre::SBF_ONE_MINUS_SOURCE_ALPHA)
	{
		return SBT_TRANSPARENT_ALPHA;
	}
	else if (source == Ogre::SBF_SOURCE_COLOUR && dest == Ogre::SBF_ONE_MINUS_SOURCE_COLOUR)
	{
		return SBT_TRANSPARENT_COLOUR;
	}
	else if (source == Ogre::SBF_DEST_COLOUR && dest == Ogre::SBF_ZERO)
	{
		return SBT_MODULATE;
	}
	else if (source == Ogre::SBF_ONE && dest == Ogre::SBF_ONE)
	{
		return SBT_ADD;
	}
	else if (source == Ogre::SBF_ONE && dest == Ogre::SBF_ZERO)
	{
		return SBT_REPLACE;
	}
	else if (source == Ogre::SBF_SOURCE_COLOUR && dest == Ogre::SBF_ONE)
	{
		return SBT_SOURCE_COLOUR_ONE;
	}
	else if (source == Ogre::SBF_SOURCE_COLOUR && dest == Ogre::SBF_ZERO)
	{
		return SBT_SOURCE_COLOUR_ZERO;
	}
	else if (source == Ogre::SBF_SOURCE_COLOUR && dest == Ogre::SBF_DEST_COLOUR)
	{
		return SBT_SOURCE_COLOUR_DEST_COLOUR;
	}
	else if (source == Ogre::SBF_DEST_COLOUR && dest == Ogre::SBF_ONE)
	{
		return SBT_DEST_COLOUR_ONE;
	}
	else if (source == Ogre::SBF_DEST_COLOUR && dest == Ogre::SBF_SOURCE_COLOUR)
	{
		return SBT_DEST_COLOUR_SOURCE_COLOUR;
	}

	return SBT_ADD;
}
//-----------------------------------------------------------------------
const wxString& MaterialTab::getTextureAddressingMode(const Ogre::TextureUnitState* texture)
{
	if (!texture)
	{
		return TAM_WRAP;
	}

	// Treat u, v and w the same way.
	if (texture->getTextureAddressingMode().u ==  Ogre::TextureUnitState::TAM_WRAP)
	{
		return TAM_WRAP;
	}
	else if (texture->getTextureAddressingMode().u ==  Ogre::TextureUnitState::TAM_CLAMP)
	{
		return TAM_CLAMP;
	}
	else if (texture->getTextureAddressingMode().u ==  Ogre::TextureUnitState::TAM_MIRROR)
	{
		return TAM_MIRROR;
	}

	return TAM_WRAP;
}
//-----------------------------------------------------------------------
void MaterialTab::OnNewMaterial(wxCommandEvent& event)
{
	if (mMaterialListBox)
	{
		wxString name = wxT("Material") + ogre2wx(ParticleUniverse::StringConverter::toString(mNewMaterialCounter));
		mMaterialListBox->addMaterialName(name);
		mNewMaterialCounter++;
		Ogre::MaterialManager* materialManager = Ogre::MaterialManager::getSingletonPtr();
		Ogre::String nameString = wx2ogre(name);
		materialManager->create(nameString, TEMP_MATERIAL_GROUP);
		Ogre::MaterialPtr material = materialManager->getByName(nameString, TEMP_MATERIAL_GROUP);
		selectMaterial(name);
	}
}
//-----------------------------------------------------------------------
void MaterialTab::OnChangeName(wxCommandEvent& event)
{
	wxString newName = mTxtMaterialName->GetValue();
	wxString oldName = mMaterialListBox->GetStringSelection();
	Ogre::String newMaterialName = wx2ogre(newName);
	Ogre::String oldMaterialName = wx2ogre(oldName);
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(oldMaterialName);
	if (!material.isNull())
	{
		Ogre::MaterialPtr clonedMaterial = material->clone(newMaterialName);
		if (!clonedMaterial.isNull())
		{
			// Load the new resource, unload the old one, remove it from the list and set the new material as the selected
			clonedMaterial->load();
			Ogre::MaterialManager* materialManager = Ogre::MaterialManager::getSingletonPtr();
			materialManager->remove(oldMaterialName);
			mMaterialListBox->Delete(mMaterialListBox->GetSelection());
			mMaterialListBox->addMaterialName(newName);
			selectMaterial(newName);
		}
	}
}
//-----------------------------------------------------------------------
bool MaterialTab::isSelectedMaterialInUse()
{
	wxString materialName = mMaterialListBox->GetStringSelection();
	Ogre::String name = wx2ogre(materialName);
	ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
	ParticleUniverse::vector<ParticleUniverse::String> names;
	particleSystemManager->particleSystemTemplateNames(names);
	ParticleUniverse::vector<ParticleUniverse::String>::iterator it;
	ParticleUniverse::vector<ParticleUniverse::String>::iterator itEnd = names.end();
	ParticleUniverse::ParticleSystem* pSys = 0;
	for (it = names.begin(); it != itEnd; ++it)
	{
		pSys = particleSystemManager->getParticleSystemTemplate(*it);
		if (pSys)
		{
			size_t numTechniques = pSys->getNumTechniques();
			for (size_t tech = 0; tech < numTechniques; ++tech)
			{
				ParticleUniverse::ParticleTechnique* particleTechnique = pSys->getTechnique(tech);
				if (particleTechnique)
				{
					if (particleTechnique->getMaterialName() == name)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}
//-----------------------------------------------------------------------
void MaterialTab::OnCheckLighting(wxCommandEvent& event)
{
	Ogre::Pass* pass = getFirstPass();
	if (pass)
	{
		pass->setLightingEnabled(mCheckLighting->GetValue());
	}
}
//-----------------------------------------------------------------------
void MaterialTab::OnAmbient(wxCommandEvent& event)
{
	Ogre::Pass* pass = getFirstPass();
	if (pass)
	{
		Ogre::ColourValue colour = pass->getAmbient();
		wxColour wxCol(colour.r * 255, colour.g * 255, colour.b * 255, colour.a * 255);
		wxColourData colourData;
		colourData.SetColour(wxCol); // Set the current pass colour
		wxColourDialog colourDialog(this, &colourData);
		if (event.GetEventObject() && event.GetEventObject()->IsKindOf(CLASSINFO(wxButton)))
		{
			colourDialog.SetPosition(((wxButton*)event.GetEventObject())->GetPosition());
		}
		if (colourDialog.ShowModal() == wxID_OK)
		{
			// Set colour in pass
			colourData = colourDialog.GetColourData();
			wxCol = colourData.GetColour();
			colour = Ogre::ColourValue(((ParticleUniverse::Real)wxCol.Red())/255.0f, 
			((ParticleUniverse::Real)wxCol.Green())/255.0f, 
			((ParticleUniverse::Real)wxCol.Blue())/255.0f, 
			((ParticleUniverse::Real)wxCol.Alpha())/255.0f);
		}
		pass->setAmbient(colour);
	}
}
//-----------------------------------------------------------------------
void MaterialTab::OnDiffuse(wxCommandEvent& event)
{
	Ogre::Pass* pass = getFirstPass();
	if (pass)
	{
		Ogre::ColourValue colour = pass->getDiffuse();
		wxColour wxCol(colour.r * 255, colour.g * 255, colour.b * 255, colour.a * 255);
		wxColourData colourData;
		colourData.SetColour(wxCol); // Set the current pass colour
		wxColourDialog colourDialog(this, &colourData);
		if (event.GetEventObject() && event.GetEventObject()->IsKindOf(CLASSINFO(wxButton)))
		{
			colourDialog.SetPosition(((wxButton*)event.GetEventObject())->GetPosition());
		}
		if (colourDialog.ShowModal() == wxID_OK)
		{
			// Set colour in pass
			colourData = colourDialog.GetColourData();
			wxCol = colourData.GetColour();
			colour = Ogre::ColourValue(((ParticleUniverse::Real)wxCol.Red())/255.0f, 
			((ParticleUniverse::Real)wxCol.Green())/255.0f, 
			((ParticleUniverse::Real)wxCol.Blue())/255.0f, 
			((ParticleUniverse::Real)wxCol.Alpha())/255.0f);
		}
		pass->setDiffuse(colour);
	}
}
//-----------------------------------------------------------------------
void MaterialTab::OnSpecular(wxCommandEvent& event)
{
	Ogre::Pass* pass = getFirstPass();
	if (pass)
	{
		Ogre::ColourValue colour = pass->getSpecular();
		wxColour wxCol(colour.r * 255, colour.g * 255, colour.b * 255, colour.a * 255);
		wxColourData colourData;
		colourData.SetColour(wxCol); // Set the current pass colour
		wxColourDialog colourDialog(this, &colourData);
		if (event.GetEventObject() && event.GetEventObject()->IsKindOf(CLASSINFO(wxButton)))
		{
			colourDialog.SetPosition(((wxButton*)event.GetEventObject())->GetPosition());
		}
		if (colourDialog.ShowModal() == wxID_OK)
		{
			// Set colour in pass
			colourData = colourDialog.GetColourData();
			wxCol = colourData.GetColour();
			colour = Ogre::ColourValue(((ParticleUniverse::Real)wxCol.Red())/255.0f, 
			((ParticleUniverse::Real)wxCol.Green())/255.0f, 
			((ParticleUniverse::Real)wxCol.Blue())/255.0f, 
			((ParticleUniverse::Real)wxCol.Alpha())/255.0f);
		}
		pass->setSpecular(colour);
	}
}
//-----------------------------------------------------------------------
void MaterialTab::OnEmissive(wxCommandEvent& event)
{
	Ogre::Pass* pass = getFirstPass();
	if (pass)
	{
		Ogre::ColourValue colour = pass->getSelfIllumination();
		wxColour wxCol(colour.r * 255, colour.g * 255, colour.b * 255, colour.a * 255);
		wxColourData colourData;
		colourData.SetColour(wxCol); // Set the current pass colour
		wxColourDialog colourDialog(this, &colourData);
		if (event.GetEventObject() && event.GetEventObject()->IsKindOf(CLASSINFO(wxButton)))
		{
			colourDialog.SetPosition(((wxButton*)event.GetEventObject())->GetPosition());
		}
		if (colourDialog.ShowModal() == wxID_OK)
		{
			// Set colour in pass
			colourData = colourDialog.GetColourData();
			wxCol = colourData.GetColour();
			colour = Ogre::ColourValue(((ParticleUniverse::Real)wxCol.Red())/255.0f, 
			((ParticleUniverse::Real)wxCol.Green())/255.0f, 
			((ParticleUniverse::Real)wxCol.Blue())/255.0f, 
			((ParticleUniverse::Real)wxCol.Alpha())/255.0f);
		}
		pass->setSelfIllumination(colour);
	}
}
//-----------------------------------------------------------------------
void MaterialTab::OnCheckDepthCheck(wxCommandEvent& event)
{
	Ogre::Pass* pass = getFirstPass();
	if (pass)
	{
		pass->setDepthCheckEnabled(mCheckDepthCheck->GetValue());
	}
}
//-----------------------------------------------------------------------
void MaterialTab::OnCheckDepthWrite(wxCommandEvent& event)
{
	Ogre::Pass* pass = getFirstPass();
	if (pass)
	{
		pass->setDepthWriteEnabled(mCheckDepthWrite->GetValue());
	}
}
//-----------------------------------------------------------------------
void MaterialTab::OnSceneBlendSelected(wxCommandEvent& event)
{
	Ogre::Pass* pass = getFirstPass();
	if (pass)
	{
		wxString sceneBlendingString = mSceneBlendList->GetValue();
		if (sceneBlendingString == SBT_ADD)
		{
			pass->setSceneBlending(Ogre::SBT_ADD);
		}
		else if (sceneBlendingString == SBT_TRANSPARENT_ALPHA)
		{
			pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
		}
		else if (sceneBlendingString == SBT_TRANSPARENT_COLOUR)
		{
			pass->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);
		}
		else if (sceneBlendingString == SBT_MODULATE)
		{
			pass->setSceneBlending(Ogre::SBT_MODULATE);
		}
		else if (sceneBlendingString == SBT_REPLACE)
		{
			pass->setSceneBlending(Ogre::SBT_REPLACE);
		}
		else if (sceneBlendingString == SBT_SOURCE_COLOUR_ONE)
		{
			pass->setSceneBlending(Ogre::SBF_SOURCE_COLOUR, Ogre::SBF_ONE);
		}
		else if (sceneBlendingString == SBT_SOURCE_COLOUR_ZERO)
		{
			pass->setSceneBlending(Ogre::SBF_SOURCE_COLOUR, Ogre::SBF_ZERO);
		}
		else if (sceneBlendingString == SBT_SOURCE_COLOUR_DEST_COLOUR)
		{
			pass->setSceneBlending(Ogre::SBF_SOURCE_COLOUR, Ogre::SBF_DEST_COLOUR);
		}
		else if (sceneBlendingString == SBT_DEST_COLOUR_ONE)
		{
			pass->setSceneBlending(Ogre::SBF_DEST_COLOUR, Ogre::SBF_ONE);
		}
		else if (sceneBlendingString == SBT_DEST_COLOUR_SOURCE_COLOUR)
		{
			pass->setSceneBlending(Ogre::SBF_DEST_COLOUR, Ogre::SBF_SOURCE_COLOUR);
		}
	}
}
//-----------------------------------------------------------------------
void MaterialTab::OnSaveMaterial(wxCommandEvent& event)
{
#ifdef PU_FULL_VERSION
	if (mMaterialListBox)
	{
		wxString filename = mMaterialListBox->GetStringSelection();
		wxFileDialog saveDialog(this, 
			_("Save the material"), 
			wxT(""), 
			filename,
			_("material scripts (*.material;*.osm)|*.material;*.osm"),
			wxFD_SAVE | wxFD_OVERWRITE_PROMPT, 
			wxDefaultPosition, 
			wxDefaultSize, _("Save"));
		if (saveDialog.ShowModal() == wxID_OK)
		{
			SetCursor(*wxHOURGLASS_CURSOR);

			wxString materialName = mMaterialListBox->GetStringSelection();
			Ogre::String name = wx2ogre(materialName);
			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(name);
			if (!material.isNull())
			{
				// User has pressed ok, so save the material
				Ogre::MaterialSerializer ms;
				Ogre::String path = wx2ogre(saveDialog.GetPath());
				Ogre::String baseName;
				Ogre::String extension;
				Ogre::String dir;
				Ogre::StringUtil::splitFullFilename(path, baseName, extension, dir);
				ms.exportMaterial(material, path);
				
				// Save the selected material + resource locations of material and texture
				ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
				mResourceLocationMaterial = ogre2wx(dir);
				frame->addResourcePathToConfig(mResourceLocationMaterial);
				frame->addResourcePathToConfig(mResourceLocationTexture);
			}
			
			SetCursor(wxNullCursor);
		}
	}
#endif // PU_FULL_VERSION
}
//-----------------------------------------------------------------------
void MaterialTab::OnLoadTexture(wxCommandEvent& event)
{
	// Add a new directory and parse all resources in that directory
	wxFileDialog loadDialog(this, 
		_("Load a texture"), 
		wxT(""), 
		wxT(""),
		_("Textures (*.*)|*.*"),
		wxFD_OPEN, 
		wxDefaultPosition, 
		wxDefaultSize, _("Load"));
	if (loadDialog.ShowModal() == wxID_OK)
	{
		SetCursor(*wxHOURGLASS_CURSOR);
		Ogre::TextureUnitState* tut = 0;

		// User has pressed ok, so load the texture
		Ogre::String fileName = wx2ogre(loadDialog.GetFilename());
		Ogre::String path = wx2ogre(loadDialog.GetPath());
		Ogre::String baseName; // should be the same as fileName
		Ogre::String extension;
		Ogre::String dir;
		Ogre::StringUtil::splitFullFilename(path, baseName, extension, dir);

		// Validate the extension
		Ogre::String types = "bmp,ico,jpg,jif,jpeg,jpe,jng,koa,iff,lbm,mng,pbm,pbm,pcd,pcx,pgm,pgm,png,ppm,ppm,ras,tga,targa,tif,tiff,wap,wbmp,wbm,psd,cut,xbm,xpm,gif,hdr,g3,sgi,exr,j2k,j2c,jp2,pfm,pct,pict,pic,bay,bmq,cr2,crw,cs1,dc2,dcr,dng,erf,fff,hdr,k25,kdc,mdc,mos,mrw,nef,orf,pef,pxn,raf,raw,rdc,sr2,srf,arw,3fr,cine,ia,kc2,mef,nrw,qtk,rw2,sti,drf,dsc,ptx,cap,iiq,rwz,dds";
		Ogre::StringUtil::toLowerCase(extension);
		if (types.find(extension) == Ogre::String::npos)
			return; // Unsupported format

		// Load the resource
		Ogre::ResourceGroupManager::getSingletonPtr()->removeResourceLocation(dir, TEMP_TEXTURE_GROUP);
		Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(dir, "FileSystem", TEMP_TEXTURE_GROUP);
		Ogre::ResourceGroupManager::getSingletonPtr()->clearResourceGroup(TEMP_TEXTURE_GROUP);
		Ogre::ResourceGroupManager::getSingletonPtr()->initialiseResourceGroup(TEMP_TEXTURE_GROUP);
		Ogre::TextureManager::getSingleton().prepare(fileName, TEMP_TEXTURE_GROUP);
		Ogre::TextureManager::getSingleton().load(fileName, TEMP_TEXTURE_GROUP);
		Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().getByName(fileName, TEMP_TEXTURE_GROUP);

		if (!texture.isNull())
		{
			// Set the texture name
			tut = forceCreateFirstTexture(fileName); // Return texture unit state
			if (tut)
			{
				tut->setName(fileName);
				mTxtTextureLoad->SetValue(loadDialog.GetFilename());
			}

			// Compile the material
			wxString materialName = mMaterialListBox->GetStringSelection();
			Ogre::String name = wx2ogre(materialName);
			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(name);
			if (!material.isNull())
			{
				material->compile();
			}

			mResourceLocationTexture = ogre2wx(dir);
		}

		// Display image
		viewTexture(tut); // Clear the old texture if no TextureUnitState

		SetCursor(wxNullCursor);
	}
}
//-----------------------------------------------------------------------
void MaterialTab::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(mTexturePanel);
    int neww, newh;
    dc.GetSize( &neww, &newh );
	bool drawn = false;

	if (mCurrentTextureFileName != Ogre::StringUtil::BLANK)
	{
		if (mCurrentTextureBitmap)
		{
			delete mCurrentTextureBitmap;
			mCurrentTextureBitmap = 0;
		}
		if (mCurrentTextureImage)
		{
			delete mCurrentTextureImage;
			mCurrentTextureImage = 0;
		}

		mCurrentTextureImage = new wxImage(ogre2wx(mCurrentTextureFileName), wxBITMAP_TYPE_ANY);
		if (mCurrentTextureImage && mCurrentTextureImage->IsOk())
		{
			mCurrentTextureImage->Rescale(neww, newh);
			mCurrentTextureBitmap = new wxBitmap(*mCurrentTextureImage);
			mCurrentTextureBitmap->SetWidth(neww);
			mCurrentTextureBitmap->SetHeight(newh);
	
			if (mCurrentTextureBitmap)
			{
				dc.DrawBitmap(*mCurrentTextureBitmap, 0, 0);
				drawn = true;
			}
		}
	}

	if (!drawn)
	{
		dc.Clear();
	}
}
