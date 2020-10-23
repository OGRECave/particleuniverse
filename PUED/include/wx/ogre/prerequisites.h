/*
 * Copyright (C) 2007-2008 Martin Pieuchot 
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#ifndef _WX_OGRE_PREREQUISITES_H_
#define _WX_OGRE_PREREQUISITES_H_

#include "ParticleUniversePrerequisites.h"
#include <wx/wx.h>
#include <Ogre.h>

// Comment this line if NVidia's PhysX is not used
//#define PU_PHYSICS_PHYSX

// Comment this line if the application must be compiled in Demo-mode
#define PU_FULL_VERSION

// Comment this line if the application may NOT generate to MP4 (because of license issues)
//# define PU_USE_MP4

#define S(s) STRINGIFY(s)
#define STRINGIFY(s) #s

#if defined( _DEBUG ) || defined( DEBUG )
#   define WXOGRE_DEBUG 1
#else
#   define WXOGRE_DEBUG 0
#endif

#define WXOGRE_VERSION_MAJOR 0 
#define WXOGRE_VERSION_MINOR 1 

#define WXOGRE_STR_VER S(WXOGRE_VERSION_MAJOR) "." S(WXOGRE_VERSION_MINOR) 


#define WXOGRE_OGRE_VER ( (OGRE_VERSION_MAJOR*100) +  \
                          (OGRE_VERSION_MINOR *10) +  \
                          (OGRE_VERSION_PATCH)     )

#define WXOGRE_OGRE_STR_VER S(OGRE_VERSION_MAJOR) "." \
                            S(OGRE_VERSION_MINOR) "." \
                            S(OGRE_VERSION_PATCH)

// Screen constants
static const int APP_WIDTH = 1024;
static const int APP_HEIGHT = 768;
static const int MAX_WIDTH = 4096;
static const int MAX_HEIGHT = 3072;
static const ParticleUniverse::Real APP_WIDTH_FRACTION = 0.65f;
static const ParticleUniverse::Real APP_HEIGHT_FRACTION = 0.67f;
static const int TAB_POS_X = (1.0 - APP_WIDTH_FRACTION) * APP_WIDTH;
static const int TAB_POS_Y = 26;
static const int TAB_WIDTH = APP_WIDTH_FRACTION * APP_WIDTH;
static const int TAB_HEIGHT =  APP_HEIGHT - TAB_POS_Y - 24;
static const int LB_TEMPLATES_WIDTH = (1.0 - APP_WIDTH_FRACTION) * APP_WIDTH;
static const int LB_TEMPLATES_HEIGHT =  (1.0 - APP_HEIGHT_FRACTION) * APP_HEIGHT - TAB_POS_Y - 24;
static const int PROPERTIES_WIDTH = LB_TEMPLATES_WIDTH;
static const int PROPERTIES_HEIGHT =  APP_HEIGHT_FRACTION * APP_HEIGHT - TAB_POS_Y - 24;
static const int TOOLS_EDIT_WIDTH = 48;
static const int TOOLS_EDIT_HEIGHT = 240;
static const int TOOLBAR_EDIT_WIDTH = 24;
static const int TOOLBAR_EDIT_HEIGHT = 240;

// Notebook constants
static const int NOTEBOOK_RENDER = 0;
static const int NOTEBOOK_EDIT = 1;
static const int NOTEBOOK_SCRIPT = 2;
static const int NOTEBOOK_MATERIAL = 3;

// Other constants
const static Ogre::String PU_EDITOR_RESOURCE_LOCATION = "ParticleUniverseEditorResources";
static const unsigned int MAX_NUMBER_OF_CONNECTIONS = 20;
static const Ogre::String DRAW_DEFAULT_COLOURCODE = Ogre::String("#000000");
static const Ogre::String DRAW_EMITTED_COLOURCODE = Ogre::String("#FF0000");
static const Ogre::String DRAW_OBSERVE_TYPE_COLOURCODE = Ogre::String("#7D2252");
static const Ogre::String DRAW_SPECIAL_CASE_COLOURCODE = Ogre::String("#387C44");
static const wxString WX_STRING_BLANK = wxT("");

// ID's
#define ID_EDITOR_WINDOW wxID_HIGHEST + 99
#define ID_REWIND wxID_HIGHEST + 100
#define ID_PLAY wxID_HIGHEST + 101
#define ID_RECORD wxID_HIGHEST + 102
#define ID_PAUSE wxID_HIGHEST + 103
#define ID_STOP wxID_HIGHEST + 104
#define ID_WIND wxID_HIGHEST + 105
#define ID_TAB_RENDER wxID_HIGHEST + 106
#define ID_TAB_SCRIPT wxID_HIGHEST + 107
#define ID_TAB_EDIT wxID_HIGHEST + 108
#define ID_TAB_MATERIAL wxID_HIGHEST + 109
#define ID_LISTOF_TEMPLATES wxID_HIGHEST + 110
#define ID_COMPILE wxID_HIGHEST + 115
#define ID_OPTIONS wxID_HIGHEST + 116
#define ID_CAMERA_RESET wxID_HIGHEST + 117
#define ID_BACKGROUND_COLOUR wxID_HIGHEST + 118
#define ID_CAMERA_SAVE wxID_HIGHEST + 119
#define ID_RESET_TIME wxID_HIGHEST + 120
#define ID_TOGGLE_GIZMO wxID_HIGHEST + 121
#define ID_TOGGLE_STATS wxID_HIGHEST + 122
#define ID_TOGGLE_GRID_PLANE wxID_HIGHEST + 123
#define ID_ADD_MESH_TO_SCENE wxID_HIGHEST + 124
#define ID_REMOVE_MESH_FROM_SCENE wxID_HIGHEST + 125
#define ID_TOGGLE_LIGHT wxID_HIGHEST + 126
#define ID_ADD_SYSTEM wxID_HIGHEST + 127
#define ID_ADD_TECHNIQUE wxID_HIGHEST + 128
#define ID_ADD_RENDERER wxID_HIGHEST + 129
#define ID_ADD_EMITTER wxID_HIGHEST + 130
#define ID_ADD_AFFECTOR wxID_HIGHEST + 131
#define ID_ADD_OBSERVER wxID_HIGHEST + 132
#define ID_ADD_HANDLER wxID_HIGHEST + 133
#define ID_ADD_BEHAVIOUR wxID_HIGHEST + 134
#define ID_ADD_EXTERN wxID_HIGHEST + 135
#define ID_CONNECT wxID_HIGHEST + 136
#define ID_DISCONNECT wxID_HIGHEST + 137
#define ID_HELP_HTML wxID_HIGHEST + 138
#define ID_CURSOR wxID_HIGHEST + 139
#define ID_CLONE wxID_HIGHEST + 140
#define ID_REMOVE wxID_HIGHEST + 141
#define ID_ATTACH_TO_MESH wxID_HIGHEST + 142
#define ID_DETACH_FROM_MESH wxID_HIGHEST + 143
#define ID_GIZMO_MOVE wxID_HIGHEST + 144
#define ID_GIZMO_ROTATE wxID_HIGHEST + 145
#define ID_GIZMO_SCALE wxID_HIGHEST + 146
#define ID_X_LABEL wxID_HIGHEST + 147
#define ID_Y_LABEL wxID_HIGHEST + 148
#define ID_Z_LABEL wxID_HIGHEST + 149
#define wxID_SPIN_X wxID_HIGHEST + 150
#define wxID_SPIN_Y wxID_HIGHEST + 151
#define wxID_SPIN_Z wxID_HIGHEST + 152
#define ID_TOGGLE_CAMERA_PROJECTION wxID_HIGHEST + 154
#define ID_CENTRE_OBJECT wxID_HIGHEST + 155
#define ID_TOGGLE_BONES wxID_HIGHEST + 156
#define ID_PLAY_ANIMATION wxID_HIGHEST + 157
#define ID_TOGGLE_LOOP wxID_HIGHEST + 158
#define ID_LISTOF_BONES wxID_HIGHEST + 159
#define ID_ATTACH_DETACH wxID_HIGHEST + 160
#define ID_WORLD_LOCAL_SPACE wxID_HIGHEST + 161
#define ID_TOGGLE_INHERIT_ORIENTATION_FROM_BONE wxID_HIGHEST + 162
#define ID_MESH_INFO wxID_HIGHEST + 163
#define ID_TOGGLE_CAMERA_ANGLE wxID_HIGHEST + 164
#define ID_RESET wxID_HIGHEST + 999

// Path
static const wxString ICONS_DIR = wxT("media/core/icons/");
static const wxString MATERIALS_DIR = wxT("media/core/materials/");
static const Ogre::String TEMP_MATERIAL_GROUP = "TEMP_MATERIAL_GROUP";
static const Ogre::String TEMP_TEXTURE_GROUP = "TEMP_TEXTURE_GROUP";

typedef wxString ComponentType;
typedef wxString ComponentSubType;

// Enum: Component Relation
enum ComponentRelation
{
	CR_INCLUDE,
	CR_EXCLUDE,
	CR_HANDLE,
	CR_EMIT,
	CR_EMIT_SAME_TYPE,
	CR_AFFECT,
	CR_OBSERVE,
	CR_INTERFACE,
	CR_SLAVE,
	CR_MASTER,
	CR_ENABLE,
	CR_FORCE,
	CR_PLACE,
	CR_UNKNOWN
};

// Enum: Component Relation
enum ComponentRelationDirection
{
	CRDIR_PRIMARY,
	CRDIR_SECUNDAIRY,
	CRDIR_EQUAL
};

// Function to return the opposite relation direction
static ComponentRelationDirection getOppositeRelationDirection(ComponentRelationDirection relationDirection)
{
	if (relationDirection == CRDIR_PRIMARY)
	{
		return CRDIR_SECUNDAIRY;
	}
	else if (relationDirection == CRDIR_SECUNDAIRY)
	{
		return CRDIR_PRIMARY;
	}
	return CRDIR_EQUAL;
};

static const ComponentSubType CST_UNDEFINED = wxT("<undefined>");


#endif
