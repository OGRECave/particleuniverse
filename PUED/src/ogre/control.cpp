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

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseEditor.h"
#include "wx/ogre/prerequisites.h"
#include "wx/ogre/control.h"
#include "wx/ogre/utils.h"

#ifdef __WXGTK20__
extern "C" {
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <wx/gtk/win_gtk.h>
}
#endif

unsigned int wxOgreControl::m_instances = 0;

BEGIN_EVENT_TABLE(wxOgreControl, wxControl)
    EVT_MOUSE_EVENTS    (wxOgreControl::OnMouseMove)
    EVT_ERASE_BACKGROUND(wxOgreControl::OnEraseBackground) 
    EVT_PAINT           (wxOgreControl::OnPaint)
    EVT_SIZE            (wxOgreControl::OnSize)
	EVT_IDLE            (wxOgreControl::OnIdle)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxOgreControl, wxControl)    

//------------------------------------------------------------------------------
wxOgreControl::wxOgreControl()
{
    Init();
}
//------------------------------------------------------------------------------
wxOgreControl::wxOgreControl(wxWindow* parent, wxWindowID id, 
                             const wxPoint& pos, const wxSize& size, long style,
                             const wxValidator& val, const wxString& name)
{
    Init();
    Create(parent, id, pos, size, style, val, name);
}
//------------------------------------------------------------------------------
wxOgreControl::~wxOgreControl()
{
    Destroy();
}
//------------------------------------------------------------------------------
bool wxOgreControl::Create(wxWindow* parent, wxWindowID id,
                           const wxPoint& pos, const wxSize& size, long style,
                           const wxValidator& val, const wxString& name)
{
    wxString instance_name = name + wxString::Format(wxT("%u"), m_instances); 

    if (!wxControl::Create(parent, id, pos, size, style, val, instance_name)) {
        wxFAIL_MSG(_("wxOgreControl creation failed"));
        return false;
    }

    CreateRenderWindow(wx2std(instance_name));

    return true;
}
//------------------------------------------------------------------------------
void wxOgreControl::Init()
{
    m_cam  = 0;
    m_rwin = 0;
    m_vp   = 0;

    m_x   = 0;
    m_y   = 0;

    m_root = Ogre::Root::getSingletonPtr();
    m_instances++;
}
//------------------------------------------------------------------------------
bool wxOgreControl::Destroy()
{
	if (m_cam) {
        m_sm->destroyCamera(m_cam);
        m_cam = 0;
    }

    /* Don't delete the SceneManager, it can be used by others. */
    m_sm = 0;

    if (m_vp) {
        m_rwin->removeAllViewports();
        m_vp = 0;
    }

    DestroyRenderWindow();

    return true;
}
//------------------------------------------------------------------------------
void wxOgreControl::Update()
{
	m_root->renderOneFrame();

//    m_root->_fireFrameStarted();

//	if (m_rwin)
//       m_rwin->update();

//	m_root->_updateAllRenderTargets();

//    m_root->_fireFrameEnded();
}
//------------------------------------------------------------------------------
void wxOgreControl::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    //  wxWidgets documentation: Note that In a paint event handler,
    // the application must *always* create a wxPaintDC object, even
    // if you do not use it. Otherwise, under MS Windows, refreshing
    // for this and other windows will go wrong.
    wxPaintDC dc(this);
    Update();
}
//------------------------------------------------------------------------------
void wxOgreControl::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
    Update();
}
//------------------------------------------------------------------------------
void wxOgreControl::OnSize(wxSizeEvent& WXUNUSED(event))
{
    int width, height;
    GetSize(&width, &height);

    if (m_rwin) {
        m_rwin->resize(width, height);
        m_rwin->windowMovedOrResized();
    }

#ifdef __WXGTK20__
    // Fix because it is not automaticaly done with gtk+
    if (m_vp)
        m_vp->_updateDimensions();
#endif

    // Let Ogre know the window has been resized;
    // Set the aspect ratio for the new size;
    if (m_cam)
        m_cam->setAspectRatio(ParticleUniverse::Real(width) / ParticleUniverse::Real(height));
}
//------------------------------------------------------------------------------
void wxOgreControl::OnIdle(wxIdleEvent& WXUNUSED(event))
{
	SetSize(GetParent()->GetSize()); // This is needed to auto resize so it always fits its parent
	Refresh();
}
//------------------------------------------------------------------------------
void wxOgreControl::AddViewport(Ogre::Camera* cam, int ZOrder, float left,
                     float top, float  width, float height)
{
    if (m_vp)
        m_rwin->removeAllViewports();

    if (m_rwin) {
        m_vp = m_rwin->addViewport(cam, ZOrder, left, top, width, height);
    }
}
//------------------------------------------------------------------------------
Ogre::RenderWindow* wxOgreControl::CreateRenderWindow(const Ogre::String& name)
{
    if (!m_root->isInitialised())
        m_rwin = m_root->initialise(false);

	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    Ogre::NameValuePairList params;
    GetParentWindowHandle(params); 

    int w, h;
    GetSize(&w, &h);

    try {
        m_rwin = m_root->createRenderWindow(name, w, h, false, &params);
        m_rwin->setActive(true);

        // Even if we are not always using Ogre's
        // rendering loop, set it as AutoUpdated
        // in case of...
        m_rwin->setAutoUpdated(true);
    } catch (Ogre::Exception& e) {
        wxOgreExceptionBox(e);
    }

    return m_rwin;
}
//------------------------------------------------------------------------------
void wxOgreControl::DestroyRenderWindow()
{
    if (m_rwin) {
        m_root->detachRenderTarget(m_rwin);
        m_rwin = 0;
        SetBackgroundStyle(wxBG_STYLE_SYSTEM);
    }
}
//------------------------------------------------------------------------------
void wxOgreControl::setCallbackFrame(ParticleUniverseEditorFrame* frame)
{
	mCallbackFrame = frame;
}
//------------------------------------------------------------------------------
void wxOgreControl::GetParentWindowHandle(Ogre::NameValuePairList& pl)
{
#ifdef __WXMSW__

    pl["externalWindowHandle"] = all2std((size_t)GetHandle());

#elif defined(__WXGTK20__)

    /* 
     * Ok here is the most important comment about the GTK+
     * part of this lib.
     *
     * Why we don't use GetHandle() here? Because it returns a
     * generic GtkWidget* that isn't one of the internals used
     * by wxGTK and can't be passed to the GTK_PIZZA() macro.
     *
     * This becomes a problem when we need to know the window ID
     * of the current widget. If you know Gtk+ you may want to use
     * gtk_widget_get_window() but in that case it doesn't return
     * the good pointer and the Ogre render window will be painted
     * under the background of this wxControl.
     *
     * Look at "wx/gtk/win_gtk.c" for more detailes.
     */
    GtkWidget* widget = m_wxwindow;

    /* May prevent from flickering */
    gtk_widget_set_double_buffered(widget, false);

    /* 
     * The frame need to be realize unless the parent
     * is already shown.
     */
    gtk_widget_realize(widget);

    /* Get the window: this Control */
    GdkWindow* gdkWin = GTK_PIZZA(widget)->bin_window;
    XID        window = GDK_WINDOW_XWINDOW(gdkWin);


#if WXOGRE_OGRE_VER < 150

    /* Get the display */
    Display* display = GDK_WINDOW_XDISPLAY(gdkWin);

    /* Get the Screen */
    unsigned int screen = DefaultScreen(display);

    pl["parentWindowHandle"] = all2std((unsigned long)display) + ":"
                             + all2std(screen) + ":"
                             + all2std(window);

#else // WXOGRE_OGRE_VER < 150

    pl["parentWindowHandle"] = all2std(window);

#endif

#else
# error Not supported on this platform.
#endif
}
//------------------------------------------------------------------------------
Ogre::SceneManager* wxOgreControl::CreateSceneManager(const Ogre::String& tn,
                                                    const Ogre::String& name)
{
    SetSceneManager(Ogre::Root::getSingleton().createSceneManager(tn, name));
    return m_sm;
}
//------------------------------------------------------------------------------
Ogre::SceneManager* wxOgreControl::CreateSceneManager(Ogre::SceneTypeMask tm,
                                                    const Ogre::String& name)
{
	SetSceneManager(Ogre::Root::getSingleton().createSceneManager(tm, name));
    return m_sm;
}
//------------------------------------------------------------------------------
void wxOgreControl::SetSceneManager(Ogre::SceneManager* sm)
{
    //SetCamera(sm->createCamera(wx2std(GetName()) + "Cam"));
	SetCamera(sm->createCamera("MainCamera"));
    m_sm = sm;
}
//------------------------------------------------------------------------------
Ogre::SceneManager* wxOgreControl::GetSceneManager(void)
{
	return m_sm;
}
//------------------------------------------------------------------------------
void wxOgreControl::SetCamera(Ogre::Camera* cam)
{
    if (m_cam) 
        m_sm->destroyCamera(m_cam);

    int width, height;
    GetSize(&width, &height);

	m_cam = cam;
    m_cam->setAspectRatio(ParticleUniverse::Real(width) / ParticleUniverse::Real(height));

	// TESTTESTTESTTESTTESTTESTTEST
	//m_cam->setProjectionType(Ogre::PT_ORTHOGRAPHIC);

    AddViewport(m_cam);
}
//------------------------------------------------------------------------------
void wxOgreControl::RotateCamera(float relX, float relY, float relZ)
{
	if (m_cam) {
		m_cam->roll((Ogre::Radian)relZ);
		m_cam->yaw((Ogre::Radian)relY);
		m_cam->pitch((Ogre::Radian)relX);
	}
}
//------------------------------------------------------------------------------
void wxOgreControl::TranslateCamera(float x, float y, float z)
{
	if (m_cam) {
        m_cam->moveRelative(Ogre::Vector3(x, y, z));
	}
}
//------------------------------------------------------------------------------
void wxOgreControl::SetPolygonMode(const Ogre::PolygonMode& pm)
{
    if (m_cam)
        m_cam->setPolygonMode(pm);
}
//------------------------------------------------------------------------------
void wxOgreControl::ProcessSelection(const wxPoint& pt)
{
}
//------------------------------------------------------------------------------
void wxOgreControl::OnMouseMove(wxMouseEvent& event)
{
	mCallbackFrame->OnMouseMoveCallback(event);
    Refresh();
}
