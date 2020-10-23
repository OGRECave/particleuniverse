/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/TextureAnimatorPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
TextureAnimatorPropertyWindow::TextureAnimatorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
TextureAnimatorPropertyWindow::TextureAnimatorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
TextureAnimatorPropertyWindow::TextureAnimatorPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::TextureAnimator* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void TextureAnimatorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::TextureAnimator* affector = static_cast<ParticleUniverse::TextureAnimator*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_TIME_STEP)
	{
		// Time Step Animation: ParticleUniverse::Real
		affector->setAnimationTimeStep(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_ANIMATION_TYPE)
	{
		// Animation Type: List
		wxString animation = prop->GetValueAsString();
		if (animation == TAT_LOOP)
		{
			affector->setTextureAnimationType(ParticleUniverse::TextureAnimator::TAT_LOOP);
		}
		else if (animation == TAT_RANDOM)
		{
			affector->setTextureAnimationType(ParticleUniverse::TextureAnimator::TAT_RANDOM);
		}
		else if (animation == TAT_UP_DOWN)
		{
			affector->setTextureAnimationType(ParticleUniverse::TextureAnimator::TAT_UP_DOWN);
		}
	}
	else if (propertyName == PRNL_TEXCOORDS_START)
	{
		// Start Texture Coordinates: ParticleUniverse::uint16
		if (validateTextureCoord(prop, affector))
		{
			affector->setTextureCoordsStart(prop->DoGetValue().GetLong());
		}
	}
	else if (propertyName == PRNL_TEXCOORDS_END)
	{
		// End Texture Coordinates: ParticleUniverse::uint16
		if (validateTextureCoord(prop, affector))
		{
			affector->setTextureCoordsEnd(prop->DoGetValue().GetLong());
		}
	}
	else if (propertyName == PRNL_START_RANDOM)
	{
		// Random Start: bool
		affector->setStartRandom(prop->DoGetValue().GetBool());
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void TextureAnimatorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::TextureAnimator* textureAnimator = static_cast<ParticleUniverse::TextureAnimator*>(affector);

	// Time Step Animation: ParticleUniverse::Real
	doSetDouble(PRNL_TIME_STEP, textureAnimator->getAnimationTimeStep());

	// Animation Type: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_ANIMATION_TYPE);
	ParticleUniverse::TextureAnimator::TextureAnimationType animationType = textureAnimator->getTextureAnimationType();
	wxString animationTypeString = TAT_LOOP;
	if (animationType == ParticleUniverse::TextureAnimator::TAT_UP_DOWN)
	{
		animationTypeString = TAT_UP_DOWN;
	}
	else if (animationType == ParticleUniverse::TextureAnimator::TAT_RANDOM)
	{
		animationTypeString = TAT_RANDOM;
	}
	propTo->SetValueFromString(animationTypeString);

	// Start Texture Coordinates: ParticleUniverse::uint16
	doSetUint16(PRNL_TEXCOORDS_START, textureAnimator->getTextureCoordsStart());

	// End Texture Coordinates: ParticleUniverse::uint16
	doSetUint16(PRNL_TEXCOORDS_END, textureAnimator->getTextureCoordsEnd());

	// Random Start: bool
	doSetBool(PRNL_START_RANDOM, textureAnimator->isStartRandom());
}
//-----------------------------------------------------------------------
void TextureAnimatorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_ANIMATION_TYPE = _("Animation type");
	TAT_LOOP = _("Loop");
	TAT_UP_DOWN = _("Up - Down");
	TAT_RANDOM = _("Random");
	PRNL_TEXCOORDS_START = _("Texture coordinate start");
	PRNL_TEXCOORDS_END = _("Texture coordinate end");
	PRNL_START_RANDOM = _("Start random");
	PRNL_TIME_STEP = _("Time step");

	mHelpHtml = wxT("AffectorTextureAnimator.html");

	// Time Step Animation: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_TIME_STEP, PRNL_TIME_STEP, ParticleUniverse::TextureAnimator::DEFAULT_TIME_STEP));
	SetPropertyEditor(PRNL_TIME_STEP, wxPG_EDITOR(SpinCtrl));

	// Animation Type: List
	mAnimationTypes.Add(TAT_LOOP);
	mAnimationTypes.Add(TAT_UP_DOWN);
	mAnimationTypes.Add(TAT_RANDOM);
	wxPGId pid = Append(wxEnumProperty(PRNL_ANIMATION_TYPE, PRNL_ANIMATION_TYPE, mAnimationTypes));

	// Start Texture Coordinates: ParticleUniverse::uint16
	Append(wxUIntProperty(PRNL_TEXCOORDS_START, PRNL_TEXCOORDS_START, ParticleUniverse::TextureAnimator::DEFAULT_TEXCOORDS_START));
	SetPropertyEditor(PRNL_TEXCOORDS_START, wxPG_EDITOR(SpinCtrl));

	// End Texture Coordinates: ParticleUniverse::uint16
	Append(wxUIntProperty(PRNL_TEXCOORDS_END, PRNL_TEXCOORDS_END, ParticleUniverse::TextureAnimator::DEFAULT_TEXCOORDS_END));
	SetPropertyEditor(PRNL_TEXCOORDS_END, wxPG_EDITOR(SpinCtrl));

	// Random Start: bool
	Append(wxBoolProperty(PRNL_START_RANDOM, PRNL_START_RANDOM, ParticleUniverse::TextureAnimator::DEFAULT_START_RANDOM));
}