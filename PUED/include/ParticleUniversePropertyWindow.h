/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_PROPERTY_WINDOW_H__
#define __PUED_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "wx/propgrid/propgrid.h"
#include "wx/propgrid/propdev.h"
#include "wx/hyperlink.h"
#include "ParticleUniverseDynamicAttribute.h"
#include "ParticleUniverseParticle.h"

// Dynamic Attributes
static wxString DYN_FIXED = wxT("");
static wxString DYN_RANDOM = wxT("");
static wxString DYN_CURVED = wxT("");
static wxString DYN_OSCILLATE = wxT("");
static wxString DYN_CURVED_LINEAR = wxT("");
static wxString DYN_CURVED_SPLINE = wxT("");

// Miscelanious
static wxString PRNL_NAME = wxT("");
static wxString PRNL_POST_RGB = wxT("");
static wxString PRNL_POST_ALPHA = wxT("");
static wxString PRNL_W = wxT("");
static wxString PRNL_X = wxT("");
static wxString PRNL_Y = wxT("");
static wxString PRNL_Z = wxT("");
static wxString PRNL_TYPE = wxT("");
static wxString PRNL_VALUE = wxT("");
static wxString PRNL_MIN_VALUE = wxT("");
static wxString PRNL_MAX_VALUE = wxT("");
static wxString PRNL_OSC_TYPE = wxT("");
static wxString PRNL_OSC_SINE = wxT("");
static wxString PRNL_OSC_SQUARE = wxT("");
static wxString PRNL_OSC_FREQUENCY = wxT("");
static wxString PRNL_OSC_PHASE = wxT("");
static wxString PRNL_OSC_BASE = wxT("");
static wxString PRNL_OSC_AMPLITUDE = wxT("");
static wxString PRNL_CURVED_INTERPOLATION = wxT("");
static wxString PRNL_CONTROL_POINT = wxT("");
static wxString PRNL_POSITION = wxT("");
static wxString PRNL_CONTROL_POINT_X = wxT("");
static wxString PRNL_CONTROL_POINT_Y = wxT("");
static wxString PRNL_POSITION_X = wxT("");
static wxString PRNL_POSITION_Y = wxT("");
static wxString PRNL_POSITION_Z = wxT("");
static wxString PRNL_FLOAT = wxT("");
static wxString PRNL_SINCE_START_SYSTEM = wxT("");
static wxString PRNL_COLLIDER_FRICTION = wxT("");
static wxString PRNL_COLLIDER_BOUNCYNESS = wxT("");
static wxString PRNL_INTERSECTION_TYPE = wxT("");
static wxString PRNL_COLLISION_TYPE = wxT("");
static wxString IST_POINT = wxT("");
static wxString IST_BOX = wxT("");
static wxString COLLT_NONE = wxT("");
static wxString COLLT_BOUNCE = wxT("");
static wxString COLLT_FLOW = wxT("");
static wxString PRNL_FORCE_VECTOR = wxT("");
static wxString PRNL_LINEAR_FORCE_APPLICATION = wxT("");
static wxString APP_ADD = wxT("");
static wxString APP_AVG = wxT("");
static wxString PRNL_MAX_DEVIATION = wxT("");
static wxString PRNL_TIME_STEP = wxT("");
static wxString PRNL_ROTATION = wxT("");
static wxString PRNL_ROTATION_SPEED = wxT("");
static wxString PRNL_ROTATION_AXIS = wxT("");
static wxString PRNL_MESH_NAME = wxT("");
static wxString PRNL_DIFFUSE_COLOUR = wxT("");
static wxString PRNL_SPECULAR_COLOUR = wxT("");
static wxString PRNL_PARTICLE_TYPE = wxT("");
static wxString PRNL_EXTERN_THRESHOLD = wxT("");
static wxString PRNL_COMPARE_NOT_USED = wxT("");
static wxString PRNL_COMPARE_LESS_THAN = wxT("");
static wxString PRNL_COMPARE_GREATER_THAN = wxT("");
static wxString PRNL_COMPARE_EQUALS = wxT("");
static wxString PRNL_USE_OWN_SPEED = wxT("");
static wxString PT_ALL = wxT("");
static wxString PT_VISUAL = wxT("");
static wxString PT_EMITTER = wxT("");
static wxString PT_AFFECTOR = wxT("");
static wxString PT_SYSTEM = wxT("");
static wxString PT_TECHNIQUE = wxT("");

// 'Enums: Component SubType'

// Renderers
static ComponentSubType CST_RENDERER_BEAM = wxT("");
static ComponentSubType CST_RENDERER_BILLBOARD = wxT("");
static ComponentSubType CST_RENDERER_SPHERE = wxT("");
static ComponentSubType CST_RENDERER_BOX = wxT("");
static ComponentSubType CST_RENDERER_ENTITY = wxT("");
static ComponentSubType CST_RENDERER_LIGHT = wxT("");
static ComponentSubType CST_RENDERER_RIBBONTRAIL = wxT("");

// Emitters
static ComponentSubType CST_EMITTER_BOX = wxT("");
static ComponentSubType CST_EMITTER_CIRCLE = wxT("");
static ComponentSubType CST_EMITTER_LINE = wxT("");
static ComponentSubType CST_EMITTER_MESH_SURFACE = wxT("");
static ComponentSubType CST_EMITTER_POINT = wxT("");
static ComponentSubType CST_EMITTER_POSITION = wxT("");
static ComponentSubType CST_EMITTER_SLAVE = wxT("");
static ComponentSubType CST_EMITTER_SPHERE = wxT("");
static ComponentSubType CST_EMITTER_VERTEX = wxT("");

// Affector
static ComponentSubType CST_AFFECTOR_ALIGN = wxT("");
static ComponentSubType CST_AFFECTOR_BOX_COLLIDER = wxT("");
static ComponentSubType CST_AFFECTOR_COLLISION_AVOIDANCE = wxT("");
static ComponentSubType CST_AFFECTOR_COLOUR = wxT("");
static ComponentSubType CST_AFFECTOR_FLOCK_CENTERING = wxT("");
static ComponentSubType CST_AFFECTOR_FORCEFIELD = wxT("");
static ComponentSubType CST_AFFECTOR_GEOMETRY_ROTATOR = wxT("");
static ComponentSubType CST_AFFECTOR_GRAVITY = wxT("");
static ComponentSubType CST_AFFECTOR_INTER_PARTICLE_COLLIDER = wxT("");
static ComponentSubType CST_AFFECTOR_JET = wxT("");
static ComponentSubType CST_AFFECTOR_LINE = wxT("");
static ComponentSubType CST_AFFECTOR_LINEAR_FORCE = wxT("");
static ComponentSubType CST_AFFECTOR_PARTICLE_FOLLOWER = wxT("");
static ComponentSubType CST_AFFECTOR_PATH_FOLLOWER = wxT("");
static ComponentSubType CST_AFFECTOR_PLANE_COLLIDER = wxT("");
static ComponentSubType CST_AFFECTOR_RANDOMISER = wxT("");
static ComponentSubType CST_AFFECTOR_SCALE = wxT("");
static ComponentSubType CST_AFFECTOR_SCALE_VELOCITY = wxT("");
static ComponentSubType CST_AFFECTOR_SINE_FORCE = wxT("");
static ComponentSubType CST_AFFECTOR_SPHERE_COLLIDER = wxT("");
static ComponentSubType CST_AFFECTOR_TEXTURE_ANIMATOR = wxT("");
static ComponentSubType CST_AFFECTOR_TEXTURE_ROTATOR = wxT("");
static ComponentSubType CST_AFFECTOR_VELOCITY_MATCHING = wxT("");
static ComponentSubType CST_AFFECTOR_VORTEX = wxT("");

// Observers
static ComponentSubType CST_OBSERVER_ON_CLEAR = wxT("");
static ComponentSubType CST_OBSERVER_ON_COLLISION = wxT("");
static ComponentSubType CST_OBSERVER_ON_COUNT = wxT("");
static ComponentSubType CST_OBSERVER_ON_EMISSION = wxT("");
static ComponentSubType CST_OBSERVER_ON_EVENTFLAG = wxT("");
static ComponentSubType CST_OBSERVER_ON_EXPIRE = wxT("");
static ComponentSubType CST_OBSERVER_ON_POSITION = wxT("");
static ComponentSubType CST_OBSERVER_ON_QUOTA = wxT("");
static ComponentSubType CST_OBSERVER_ON_RANDOM = wxT("");
static ComponentSubType CST_OBSERVER_ON_TIME = wxT("");
static ComponentSubType CST_OBSERVER_ON_VELOCITY = wxT("");

// Event Handlers
static ComponentSubType CST_HANDLER_DO_AFFECTOR = wxT("");
static ComponentSubType CST_HANDLER_DO_ENABLE_COMPONENT = wxT("");
static ComponentSubType CST_HANDLER_DO_EXPIRE = wxT("");
static ComponentSubType CST_HANDLER_DO_FREEZE = wxT("");
static ComponentSubType CST_HANDLER_DO_PLACEMENT_PARTICLE = wxT("");
static ComponentSubType CST_HANDLER_DO_SCALE = wxT("");
static ComponentSubType CST_HANDLER_DO_STOP_SYSTEM = wxT("");

// Behaviour
static ComponentSubType CST_BEHAVIOUR_SLAVE = wxT("");

// Extern
static ComponentSubType CST_EXTERN_BOX_COLLIDER = wxT("");
static ComponentSubType CST_EXTERN_SPHERE_COLLIDER = wxT("");
static ComponentSubType CST_EXTERN_GRAVITY = wxT("");
static ComponentSubType CST_EXTERN_PHYSX_ACTOR = wxT("");
static ComponentSubType CST_EXTERN_PHYSX_FLUID = wxT("");
static ComponentSubType CST_EXTERN_VORTEX = wxT("");

// Particle Universe Renderer names (must not be internationalized)
static const Ogre::String RENDERER_BEAM = "Beam";
static const Ogre::String RENDERER_BILLBOARD = "Billboard";
static const Ogre::String RENDERER_SPHERE = "Sphere";
static const Ogre::String RENDERER_BOX = "Box";
static const Ogre::String RENDERER_ENTITY = "Entity";
static const Ogre::String RENDERER_LIGHT = "Light";
static const Ogre::String RENDERER_RIBBONTRAIL = "RibbonTrail";

// Particle Universe Emitter names (must not be internationalized)
static const Ogre::String EMITTER_BOX = "Box";
static const Ogre::String EMITTER_CIRCLE = "Circle";
static const Ogre::String EMITTER_LINE = "Line";
static const Ogre::String EMITTER_MESH_SURFACE = "MeshSurface";
static const Ogre::String EMITTER_POINT = "Point";
static const Ogre::String EMITTER_POSITION = "Position";
static const Ogre::String EMITTER_SLAVE = "Slave";
static const Ogre::String EMITTER_SPHERE = "SphereSurface";
static const Ogre::String EMITTER_VERTEX = "Vertex";

// Particle Universe Affector names (must not be internationalized)
static const Ogre::String AFFECTOR_ALIGN = "Align";
static const Ogre::String AFFECTOR_BOX_COLLIDER = "BoxCollider";
static const Ogre::String AFFECTOR_COLLISION_AVOIDANCE = "CollisionAvoidance";
static const Ogre::String AFFECTOR_COLOUR = "Colour";
static const Ogre::String AFFECTOR_FLOCK_CENTERING = "FlockCentering";
static const Ogre::String AFFECTOR_FORCEFIELD = "ForceField";
static const Ogre::String AFFECTOR_GEOMETRY_ROTATOR = "GeometryRotator";
static const Ogre::String AFFECTOR_GRAVITY = "Gravity";
static const Ogre::String AFFECTOR_INTER_PARTICLE_COLLIDER = "InterParticleCollider";
static const Ogre::String AFFECTOR_JET = "Jet";
static const Ogre::String AFFECTOR_LINE = "Line";
static const Ogre::String AFFECTOR_LINEAR_FORCE = "LinearForce";
static const Ogre::String AFFECTOR_PARTICLE_FOLLOWER = "ParticleFollower";
static const Ogre::String AFFECTOR_PATH_FOLLOWER = "PathFollower";
static const Ogre::String AFFECTOR_PLANE_COLLIDER = "PlaneCollider";
static const Ogre::String AFFECTOR_RANDOMISER = "Randomiser";
static const Ogre::String AFFECTOR_SCALE = "Scale";
static const Ogre::String AFFECTOR_SCALE_VELOCITY = "ScaleVelocity";
static const Ogre::String AFFECTOR_SINE_FORCE = "SineForce";
static const Ogre::String AFFECTOR_SPHERE_COLLIDER = "SphereCollider";
static const Ogre::String AFFECTOR_TEXTURE_ANIMATOR = "TextureAnimator";
static const Ogre::String AFFECTOR_TEXTURE_ROTATOR = "TextureRotator";
static const Ogre::String AFFECTOR_VELOCITY_MATCHING = "VelocityMatching";
static const Ogre::String AFFECTOR_VORTEX = "Vortex";

// Particle Universe Observer names (must not be internationalized)
static const Ogre::String OBSERVER_ON_CLEAR = "OnClear";
static const Ogre::String OBSERVER_ON_COLLISION = "OnCollision";
static const Ogre::String OBSERVER_ON_COUNT = "OnCount";
static const Ogre::String OBSERVER_ON_EMISSION = "OnEmission";
static const Ogre::String OBSERVER_ON_EVENTFLAG = "OnEventFlag";
static const Ogre::String OBSERVER_ON_EXPIRE = "OnExpire";
static const Ogre::String OBSERVER_ON_POSITION = "OnPosition";
static const Ogre::String OBSERVER_ON_QUOTA = "OnQuota";
static const Ogre::String OBSERVER_ON_RANDOM = "OnRandom";
static const Ogre::String OBSERVER_ON_TIME = "OnTime";
static const Ogre::String OBSERVER_ON_VELOCITY = "OnVelocity";

// Particle Universe Event Handler names (must not be internationalized)
static const Ogre::String HANDLER_DO_AFFECTOR = "DoAffector";
static const Ogre::String HANDLER_DO_ENABLE_COMPONENT = "DoEnableComponent";
static const Ogre::String HANDLER_DO_EXPIRE = "DoExpire";
static const Ogre::String HANDLER_DO_FREEZE = "DoFreeze";
static const Ogre::String HANDLER_DO_PLACEMENT_PARTICLE = "DoPlacementParticle";
static const Ogre::String HANDLER_DO_SCALE = "DoScale";
static const Ogre::String HANDLER_DO_STOP_SYSTEM = "DoStopSystem";

// Particle Universe Behaviour names (must not be internationalized)
static const Ogre::String BEHAVIOUR_SLAVE = "Slave";

// Particle Universe Extern names (must not be internationalized)
static const Ogre::String EXTERN_BOX_COLLIDER = "BoxCollider";
static const Ogre::String EXTERN_SPHERE_COLLIDER = "SphereCollider";
static const Ogre::String EXTERN_GRAVITY = "Gravity";
static const Ogre::String EXTERN_PHYSX_ACTOR = "PhysXActor";
static const Ogre::String EXTERN_PHYSX_FLUID = "PhysXFluid";
static const Ogre::String EXTERN_VORTEX = "Vortex";

class EditComponent;

/**	Class that combines a listbox with a button. After clicking, new controlpoints are added
*/
class EnumPropertyWithButton : public wxEnumPropertyClass
{
	public:
		EnumPropertyWithButton(
			const wxString& label, 
			const wxString& name,
			const wxArrayString& choices);
		virtual ~EnumPropertyWithButton(void){};

		/**	
		*/
		virtual bool OnEvent (wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event);
};

/**	Class that creates a button property. After clicking, new vector3 entries are added
*/
class ParentPropertyWithButtonAndPositions : public wxParentPropertyClass
{
	public:
		ParentPropertyWithButtonAndPositions(
			const wxString& label, 
			const wxString& name);
		virtual ~ParentPropertyWithButtonAndPositions(void){};

		/**	
		*/
		virtual bool OnEvent (wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event);

		/**	Add the position
		*/
		wxPGId* addPosition(wxPropertyGrid* propgrid, Ogre::Vector3 vec3 = Ogre::Vector3::ZERO);

		/**	Returns the number of positions
		*/
		unsigned int getNumberOfPositions(void);

		/**	Returns the position of index
		*/
		const Ogre::Vector3& getPosition(wxPropertyGrid* propgrid, unsigned int index, Ogre::Vector3& vector);

		/**	Removes all the child properties
		*/
		void reset(void);

	protected:
		unsigned int mPosition;
};

/**	Class that creates a button property. After clicking, new float entries are added
*/
class ParentPropertyWithButtonAndFloats : public wxParentPropertyClass
{
	public:
		ParentPropertyWithButtonAndFloats(
			const wxString& label, 
			const wxString& name);
		virtual ~ParentPropertyWithButtonAndFloats(void){};

		/**	
		*/
		virtual bool OnEvent (wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event);

		/**	Add a float
		*/
		wxPGId* addFloat(wxPropertyGrid* propgrid, float value = 0.0f);

		/**	Get number of floats
		*/
		unsigned int getNumberOfFloats(void);

		/**	Returns a float by index
		*/
		const ParticleUniverse::Real getFloat(wxPropertyGrid* propgrid, unsigned int index);

		/**	Removes all the child properties
		*/
		void reset(void);

	protected:
		unsigned int mFloat;
};


/**	
*/
class PropertyWindow : public wxPropertyGrid
{
	public:
		PropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name);
		~PropertyWindow(void){};

		// public attributes
		typedef std::map<wxString, ParticleUniverse::DynamicAttributeCurved::ControlPointList> ControlPointListMap;
		ControlPointListMap mControlPointListMap;

		// public attributes
		typedef std::map<wxString, ParticleUniverse::InterpolationType> InterpolationTypeMap;
		InterpolationTypeMap mInterpolationTypeMap;

		/**
			Get/Set name
		*/
		const Ogre::String& getComponentName() const;
		void setComponentName(const Ogre::String& name);

		/**
			Bool-, Float-, UInt16-, Long and String-properties
		*/
		void doSetBool(const wxString& name, bool boolValue);
		bool doGetBool(const wxString& name);

		void doSetDouble(const wxString& name, double doubleValue);
		double doGetDouble(const wxString& name);

		void doSetLong(const wxString& name, long longValue);
		long doGetLong(const wxString& name);

		void doSetUint16(const wxString& name, wxUint16 uIntValue);
		wxUint16 doGetUint16(const wxString& name);

		void doSetString(const wxString& name, const wxString& stringValue);
		const wxString& doGetString(const wxString& name);

		/**
			Colour property with alpha
		*/
		void appendColourWithAlphaProperty(const wxString& label, const wxString& name, const wxColour& colour);
		void appendInColourWithAlphaProperty(wxPGId& id, const wxString& label, const wxString& name, const wxColour& colour);
		void doSetColourWithAlpha(const wxString& name, const wxColour& colour, bool nameIsBaseName = false);
		const wxColour& doGetColourWithAlpha(const wxString& name, wxColour& colour, bool nameIsBaseName = false);

		/**
			Vector3 property
		*/
		void appendVector3(const wxString& label, const wxString& name, const Ogre::Vector3& vector3);
		void doSetVector3(const wxString& name, const Ogre::Vector3& vector3);
		const Ogre::Vector3& doGetVector3(const wxString& name, Ogre::Vector3& vector3);

		/**
			Vector4 property
		*/
		void appendVector4(const wxString& label, const wxString& name, const Ogre::Vector4& vector4);
		void doSetVector4(const wxString& name, const Ogre::Vector4& vector4);
		const Ogre::Vector4& doGetVector4(const wxString& name, Ogre::Vector4& vector4);

		/**
			Quaternion property
		*/
		void appendQuaternion(const wxString& label, const wxString& name, const Ogre::Quaternion& quaternion);
		void doSetQuaternion(const wxString& name, const Ogre::Quaternion& quaternion);
		const Ogre::Quaternion& doGetQuaternion(const wxString& name, Ogre::Quaternion& quaternion);

		/**
			Dynamic Attribute property
		*/
		void appendDynamicAttribute(const wxString& label, const wxString& name, ParticleUniverse::DynamicAttribute& dynamicAttribute);
		void doSetDynamicAttribute(const wxString& name, PropertyWindow* propertyWindow);
		void doSetDynamicAttribute(const wxString& name, ParticleUniverse::DynamicAttribute* dynamicAttribute);
		//ParticleUniverse::DynamicAttribute* DoGetDynamicAttribute(const wxString& name);

		/**
			Returns the owner of this property window (this is not the parent).
		*/
		EditComponent* getOwner(void);

		/**
			Handles the property changes
		*/
		virtual void onPropertyChanged(wxPropertyGridEvent& event);

		/**
			Notify the parent that a property has been changed.
		*/
		void notifyPropertyChanged(void);

		/**
			Update the ParticleUniverse::ParticleSystem component with the property.
		*/
		void copyAttributeToComponent(wxPGProperty* prop, wxString propertyName);

		/**
			Update the ParticleUniverse::DynamicAttribute.
		@remarks
			If a DynamicAttribute pointer is returned, the type has been changed, so it needs to be set.
		*/
		ParticleUniverse::DynamicAttribute* copyValuesToDynamicAttribute(wxString name, 
			wxPGProperty* prop, 
			wxString baseName, 
			ParticleUniverse::DynamicAttribute* dynAttr);

		/**
			Copy all controlpoints from a property (defined with baseName) to a ParticleUniverse::DynamicAttributeCurved.
		*/
		void copyControlPointsToDynamicAttribute(wxString baseName, ParticleUniverse::DynamicAttribute* dynAttr);

		/**
			Start, stop particle system if needed
		*/
		bool _mustStopParticleSystem(ParticleUniverse::ParticleSystem* system);
		void _mustRestartParticleSystem(ParticleUniverse::ParticleSystem* system, bool wasStarted);

		/**
			Unprepares emitted emitters, emitted affectors, ...
		*/
		void _unprepare(ParticleUniverse::IElement* element, 
						ParticleUniverse::Particle::ParticleType elementType, 
						ParticleUniverse::Particle::ParticleType unprepareType);

		/**
			Testfunction to dump all properties
		*/
		void propertyDump(wxPropertyGrid* propgrid);

		/**
			Return the help html
		*/
		const wxString& getHelpHtml(void) const;

	protected:
		EditComponent* mOwner;
		Ogre::String mName;
		wxString mHelpHtml;

		/**
			Initialises basic properties
		*/
		virtual void _initProperties(void);

		/**
			Various validation functions. Checks whether the name equals the property name.
		*/
		bool _validatePropertyStringNoSpaces(wxPGProperty* prop, const wxString& name);
		bool _validatePropertyColourWithAlpha(wxPGProperty* prop, const wxString& name);
		bool _validatePropertyIntMinMax(wxPGProperty* prop, const wxString& name, int min, int max);
		bool _validatePropertyIntPositive(wxPGProperty* prop, const wxString& name);
		bool _validatePropertyFloatMinMax(wxPGProperty* prop, const wxString& name, float min, float max);
		bool _validatePropertyFloatPositive(wxPGProperty* prop, const wxString& name);
		bool _validatePropertyDynamicAttribute(wxPGProperty* prop, const wxString& name);

		/**
			Set the property in error state
		*/
		bool _setPropertyError(wxPGProperty* prop, const wxString& message);

		/**
			Show message (used for proerty validation
		*/
		void _showMessage(const wxString& message, long style = wxOK | wxICON_EXCLAMATION);
};

#endif

