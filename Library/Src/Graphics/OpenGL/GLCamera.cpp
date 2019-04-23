/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/OpenGL/GLCamera.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1588 $
 * Last Updated: $Date: 2008-08-23 09:36:02 -0700 (Sat, 23 Aug 2008) $
 * 
 * 
 * Copyright (c) 2004-2007, Matthew G Bolitho
 * All rights reserved.
 *
 *
 * Microsoft Reference License (Ms-RL)
 *   
 * This license governs use of the accompanying software. If you use the software, you accept 
 * this license. If you do not accept the license, do not use the software.
 *
 * 1. Definitions
 *  - The terms "reproduce," "reproduction" and "distribution" have the same meaning here as under 
 *    U.S. copyright law.
 *  - "You" means the licensee of the software.
 *  - "Your company" means the company you worked for when you downloaded the software.
 *  - "Reference use" means use of the software within your company as a reference, in read only 
 *    form, for the sole purposes of debugging your products, maintaining your products, or 
 *    enhancing the interoperability of your products with the software, and specifically excludes 
 *    the right to distribute the software outside of your company.
 *  - "Licensed patents" means any Licensor patent claims which read directly on the software as 
 *    distributed by the Licensor under this license.
 * 
 * 2. Grant of Rights
 *  (A) Copyright Grant- Subject to the terms of this license, the Licensor grants you a non-transferable, 
 *      non-exclusive, worldwide, royalty-free copyright license to reproduce the software for reference use.
 *  (B) Patent Grant- Subject to the terms of this license, the Licensor grants you a non-transferable,
 *      non-exclusive, worldwide, royalty-free patent license under licensed patents for reference use.
 * 
 * 3. Limitations
 *  (A) No Trademark License - This license does not grant you any rights to use the Licensor's name
 *      logo, or trademarks.
 *  (B) If you begin patent litigation against the Licensor over patents that you think may apply 
 *      to the software (including a cross-claim or counterclaim in a lawsuit), your license to the 
 *      software ends automatically.
 *  (C) The software is licensed "as-is." You bear the risk of using it. The Licensor gives no express 
 *      warranties, guarantees or conditions. You may have additional consumer rights under your local 
 *      laws which this license cannot change. To the extent permitted under your local laws, the 
 *      Licensor excludes the implied warranties of merchantability, fitness for a particular purpose 
 *      and non-infringement.
 *
 ***************************************************************************************************/




#include "CodeLibrary.hpp"

#if defined(USE_OPENGL)

#include "Graphics/OpenGL/Camera.hpp"
#include "Graphics/OpenGL/Context.hpp"

#include "Debug/Trace.hpp"

using namespace Bolitho;
using namespace Bolitho::Math;
using namespace Bolitho::Graphics::OpenGL;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Graphics::OpenGL,Camera,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
Camera::Camera( FLOAT FieldOfView )
{
	m_DoSpin = FALSE;
	m_MouseState = 0;
	m_SpinSpeed = 0;
	m_FieldOfView = FieldOfView;
	m_SurfaceDepth = 0;
	m_ClickDepth = 0;
	m_View.SetIdentity();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------

#define DOF 10.0f
#define MAXDOF 10000.0f
#define SPIN_TIME 0.1f
#define SPIN_SPEED 0.05f
#define TRACKBALL_R 0.8f
#define DEPTH_FUDGE (1.0f + 0.2f * m_FieldOfView)
#define MOVEZ_MULT 5.0f
#define WHEEL_MOVE 0.2f
#define MAX_LIGHT M_PI


BOOL Camera::ReadDepth(Point ScreenSpace, Vector3<FLOAT>& WorldSpace) CONST
{
	GLdouble M[16], P[16]; GLint V[4];
	glGetDoublev(GL_MODELVIEW_MATRIX, M);
	glGetDoublev(GL_PROJECTION_MATRIX, P);
	glGetIntegerv(GL_VIEWPORT, V);

	CONST FLOAT dx[] = { 0, 1,-1,-1, 1, 3,-3, 0, 0, 6,-6,-6, 6, 25,-25,  0,  0 };
	CONST FLOAT dy[] = { 0, 1, 1,-1,-1, 0, 0, 3,-3, 6, 6,-6,-6,  0,  0, 25,-25 };
	CONST FLOAT scale = 0.01f;
	CONST INT displacements = sizeof(dx) / sizeof(FLOAT);

	int xmin = V[0], xmax = V[0]+V[2]-1, ymin = V[1], ymax = V[1]+V[3]-1;

	for (int i = 0 ; i < displacements; i++) 
	{
		int xx = Min<INT>(Max<INT>(ScreenSpace.x + int(dx[i]*scale*V[2]), xmin), xmax);
		int yy = Min<INT>(Max<INT>(ScreenSpace.y + int(dy[i]*scale*V[3]), ymin), ymax);
		float d;
		glReadPixels(xx, yy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &d);

		static float maxd = 0.0f;
		if (!maxd) 
		{
			glScissor(xx, yy, 1, 1);
			glEnable(GL_SCISSOR_TEST);
			glClearDepth(1);
			glClear(GL_DEPTH_BUFFER_BIT);
			glReadPixels(xx, yy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &maxd);
			if (maxd) 
			{
				glClearDepth(d / maxd);
				glClear(GL_DEPTH_BUFFER_BIT);
			}
			glDisable(GL_SCISSOR_TEST);
			glClearDepth(1);
			if (!maxd)
				return FALSE;
		}

		d /= maxd;
		if (d > 0.0001f && d < 0.9999f) 
		{
			GLdouble X, Y, Z;
			gluUnProject(xx, yy, d, M, P, V, &X, &Y, &Z);
			WorldSpace = Vector3<FLOAT>( (FLOAT)X, (FLOAT)Y, (FLOAT)Z );
			return TRUE;
		}
	}
	return FALSE;
}

VOID Camera::StartSpin()
{
//	FLOAT dt = now() - last_time;
//	if (dt < SPIN_TIME && fabs(spinspeed) > SPIN_SPEED)
//		dospin = true;
}


Vector3<FLOAT> Camera::MouseToTrackball(PointF P)
{
	float r2 = P.x*P.x + P.y*P.y;
	float t = 0.5f * TRACKBALL_R*TRACKBALL_R;

	Vector3<FLOAT> Pos(P.x, P.y, 0);
	if (r2 < t)
		Pos[2] = Sqrt(2*t - r2);
	else
		Pos[2] = t / Sqrt(r2);
	return Pos;
}

VOID Camera::Rotate(Point P, Matrix4<FLOAT>& xf)
{
	PointF OldPosition = (m_MousePosition - m_TrackballPosition) / m_TrackballScreenSize;
	PointF NewPosition = (P - m_TrackballPosition) / m_TrackballScreenSize;

	Vector3<FLOAT> Pos1 = MouseToTrackball(OldPosition);
	Vector3<FLOAT> Pos2 = MouseToTrackball(NewPosition);
	
	m_SpinAxis = Pos1.Cross(Pos2);

	FLOAT SpinAmount = Sqrt(Sqr(NewPosition.x - OldPosition.x) + Sqr(NewPosition.y - OldPosition.y) );

	xf = Matrix4<FLOAT>::Translation(m_SpinCenter) * Matrix4<FLOAT>::Rotation(SpinAmount, m_SpinAxis) * Matrix4<FLOAT>::Translation(-m_SpinCenter) * xf;

//	float dt = now() - last_time;
//	if (dt > SPIN_TIME)
//		spinspeed = spinamount / SPIN_TIME;
//	else
//		spinspeed = (spinamount / SPIN_TIME) +
//			    (1.0f-dt/SPIN_TIME)*spinspeed;
}


// Mouse helper - translate
VOID Camera::MoveXY(Point P, Matrix4<FLOAT>& xf)
{
	float dx = m_PixelScale * m_ClickDepth * (P.x - m_MousePosition.x);
	float dy = m_PixelScale * m_ClickDepth * (P.y - m_MousePosition.y);

	xf = Matrix4<FLOAT>::Translation(dx, dy, 0) * xf;
}


// Mouse helper - translate in Z
// In order to be extra-clever, though, this actually translates along the
// direction of the center of the trackball
VOID Camera::MoveZ(Point P, Matrix4<FLOAT>& xf)
{
	float delta = MOVEZ_MULT / m_FieldOfView * m_PixelScale * ((P.x-m_MousePosition.x) - (P.y-m_MousePosition.y));
	float dz = m_ClickDepth * (Exp(-delta) - 1.0f);

	//xf = xform::trans(0, 0, -dz) * xf;

	xf = Matrix4<FLOAT>::Translation(dz * m_SpinCenter / m_SpinCenter.Length() ) * xf;

	m_SurfaceDepth += dz;
	if (m_SurfaceDepth < 0)
		m_SurfaceDepth = 0;

	m_ClickDepth += dz;
	if (m_ClickDepth < 0)
		m_ClickDepth = 0;
}


// Mouse helper - wheel motion
/*
VOID Camera::Wheel(UINT updown, xform &xf)
{
	float dz = m_ClickDepth * WHEEL_MOVE;
	if (updown == Mouse::WHEELUP)
		dz = -dz;
	xf = xform::trans(0, 0, dz) * xf;

	surface_depth -= dz;
	if (surface_depth < 0)
		surface_depth = 0;
	m_ClickDepth -= dz;
	if (m_ClickDepth < 0)
		m_ClickDepth = 0;
}
*/


// Mouse helper - change lighting direction
/*
VOID GLCamera::relight(int mousex, int mousey)
{
	GLint V[4];
	glGetIntegerv(GL_VIEWPORT, V);

	float x = 2.0f * float(mousex - V[0]) / float(V[2]) - 1.0f;
	float y = 2.0f * float(mousey - V[1]) / float(V[3]) - 1.0f;

	float theta = MAX_LIGHT * min(sqrt(x*x+y*y), 1.0f);
	float phi = atan2(y, x);

	lightdir[0] = sin(theta)*cos(phi);
	lightdir[1] = sin(theta)*sin(phi);
	lightdir[2] = cos(theta);
}
*/


// Handle a mouse click - sets up rotation center, m_PixelScale, and m_ClickDepth
VOID Camera::MouseClick(Point P)
{
	GLdouble ModelViewMatrix[16], ProjectionMatrix[16]; GLint Viewport[4];

#if 0
	glGetDoublev(GL_MODELVIEW_MATRIX, M);
#else
	ModelViewMatrix[0] = ModelViewMatrix[5] = ModelViewMatrix[10] = ModelViewMatrix[15] = 1.0;
	ModelViewMatrix[1] = ModelViewMatrix[2] = ModelViewMatrix[3] = ModelViewMatrix[4] =
	ModelViewMatrix[6] = ModelViewMatrix[7] = ModelViewMatrix[8] = ModelViewMatrix[9] =
	ModelViewMatrix[11] = ModelViewMatrix[12] = ModelViewMatrix[13] = ModelViewMatrix[14] = 0;
#endif

	glGetDoublev(GL_PROJECTION_MATRIX, ProjectionMatrix);
	glGetIntegerv(GL_VIEWPORT, Viewport);

	// Assume glFrustum only...
	m_PixelScale = 2.0f / (FLOAT)Max(ProjectionMatrix[0]*Viewport[2], ProjectionMatrix[5]*Viewport[3]);

	Vector3<FLOAT> SurfacePoint;
	if (ReadDepth(P, SurfacePoint))
		m_ClickDepth = -SurfacePoint[2];
	else
		m_ClickDepth = m_SurfaceDepth;

  GLdouble cx, cy, cz;
	gluProject(m_SceneCenter[0], m_SceneCenter[1], m_SceneCenter[2], ModelViewMatrix, ProjectionMatrix, Viewport, &cx, &cy, &cz);

	double csize = Max(Viewport[2], Viewport[3]);
	int xmin = Viewport[0], xmax = Viewport[0]+Viewport[2], ymin = Viewport[1], ymax = Viewport[1]+Viewport[3];
	if (m_SceneCenter[2] < 0 && m_SceneCenter.Length() > m_SceneSize) 
	{
		csize = -m_SceneSize / m_SceneCenter[2] / m_PixelScale;
		xmin = Min(Max(int(cx - csize), Viewport[0]), Viewport[0]+Viewport[2]);
		xmax = Min(Max(int(cx + csize), Viewport[0]), Viewport[0]+Viewport[2]);
		ymin = Min(Max(int(cy - csize), Viewport[1]), Viewport[1]+Viewport[3]);
		ymax = Min(Max(int(cy + csize), Viewport[1]), Viewport[1]+Viewport[3]);
	}

	GLdouble s[3];
	gluUnProject((xmin+xmax)/2, (ymin+ymax)/2, 1, ModelViewMatrix, ProjectionMatrix, Viewport, &s[0], &s[1], &s[2]);
	
	m_SpinCenter = Vector3<FLOAT>( (FLOAT)s[0], (FLOAT)s[1], (FLOAT)s[2] );
	m_SpinCenter.Normalize();

	if (ReadDepth( Point((xmin+xmax)/2, (ymin+ymax)/2), SurfacePoint))
		m_SpinCenter *=  DEPTH_FUDGE * SurfacePoint[2] / m_SpinCenter[2];
	else
		m_SpinCenter *= -DEPTH_FUDGE * m_ClickDepth / m_SpinCenter[2];

	FLOAT f = (FLOAT)csize / (FLOAT)Max(Viewport[2], Viewport[3]);
	f = Min(Max(2.0f * f - 1.0f, 0.0f), 1.0f);
	m_SpinCenter = f * m_SpinCenter + (1.0f - f) * m_SceneCenter;

	gluProject(m_SpinCenter[0], m_SpinCenter[1], m_SpinCenter[2], ModelViewMatrix, ProjectionMatrix, Viewport, &cx, &cy, &cz);

	m_TrackballPosition = PointF( (FLOAT)cx, (FLOAT)cy);

	m_TrackballScreenSize = (FLOAT)csize;
	m_TrackballScreenSize = Min(m_TrackballScreenSize, 0.7f * Min(Viewport[2], Viewport[3]));
	m_TrackballScreenSize = Max(m_TrackballScreenSize, 0.3f * Min(Viewport[2], Viewport[3]));
}

LRESULT Camera::HandleMessage( Ptr<UI::Window> pWindow, UINT Message, WPARAM wParam, LPARAM lParam )
{
	switch (Message)
	{
	case WM_MOUSEMOVE:
		{
		Point P(lParam);
		GLint V[4];
		glGetIntegerv(GL_VIEWPORT, V);
		P.y = V[1] + V[3] - 1 - P.y;

		if (m_MouseState == MK_LBUTTON)
		{
			Rotate(P, m_View);
			pWindow->Invalidate();
		}
    else if (m_MouseState == MK_RBUTTON)
    {
      MoveZ(P, m_View);
      pWindow->Invalidate();
    }
    else if (m_MouseState == MK_MBUTTON)
    {
      MoveXY(P, m_View);
      pWindow->Invalidate();
    }

		m_MousePosition = P;
    m_MouseState = wParam;
		break;
		}
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
		Point P(lParam);
		GLint V[4];
		glGetIntegerv(GL_VIEWPORT, V);
		P.y = V[1] + V[3] - 1 - P.y;
		
		MouseClick(P);
		m_MouseState = wParam;
		break;
		}
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		m_MouseState = wParam;
		break;
	};
	return 0;

}

/*
// Handle a mouse event 
VOID Camera::Mouse(Point P, UINT b, VOID point &scene_center, float scene_size, xform &xf)
{
	if (b == Mouse::NONE && lastb == Mouse::NONE)
		return;

	GLint V[4];
	glGetIntegerv(GL_VIEWPORT, V);
	mousey = V[1] + V[3] - 1 - mousey;

	dospin = false;
	if ((b != lastb) && (b != Mouse::NONE))
		mouse_click(mousex, mousey, scene_center, scene_size);

	// Handle rotation
	if ((b == Mouse::ROTATE) && (lastb == Mouse::NONE))
		spinspeed = 0;
	if ((b == Mouse::ROTATE) && (lastb == Mouse::ROTATE))
		rotate(mousex, mousey, xf);
	if ((b == Mouse::NONE) && (lastb == Mouse::ROTATE))
		startspin();

	// Handle translation
	if ((b == Mouse::MOVEXY) && (lastb == Mouse::MOVEXY))
		movexy(mousex, mousey, xf);
	if ((b == Mouse::MOVEZ) && (lastb == Mouse::MOVEZ))
		movez(mousex, mousey, xf);
	if (b == Mouse::WHEELUP || b == Mouse::WHEELDOWN)
		wheel(b, xf);

	// Handle lighting
	if (b == Mouse::LIGHT)
		relight(mousex, mousey);


	lastmousex = mousex;  lastmousey = mousey;  lastb = b;
	last_time = now();
}
*/

// Idle loop - handles auto-rotate.  Returns true iff auto-rotating
/*
bool Camera::autospin(xform &xf)
{
	if (!dospin)
		return false;

	float dt = now() - last_time;
	float spinamount = spinspeed * dt;

	xf = xform::trans(spincenter) * xform::rot(spinamount, spinaxis) *
	     xform::trans(-spincenter) * xf;

	last_time = now();
	return true;
}
*/


// Set up the OpenGL camera for rendering
VOID Camera::Initialize(CONST Vector3<FLOAT>& SceneCenter, FLOAT SceneSize)
{
	m_SceneCenter = m_View * SceneCenter;
	m_SceneSize = SceneSize;

	GLint V[4];
	glGetIntegerv(GL_VIEWPORT, V);
	int width = V[2], height = V[3];

	Vector3<FLOAT> surface_point;
	if (ReadDepth( Point(width/2, height/2), surface_point))
		m_SurfaceDepth = -surface_point[2];

	float fardist  = Max(-(m_SceneCenter[2] - SceneSize), SceneSize / DOF);
	float neardist = Max(-(m_SceneCenter[2] + SceneSize), SceneSize / MAXDOF);

	m_SurfaceDepth = Min(m_SurfaceDepth, fardist);
	m_SurfaceDepth = Max(m_SurfaceDepth, neardist);
	m_SurfaceDepth = Max(m_SurfaceDepth, fardist / MAXDOF);
	neardist = Max(neardist, m_SurfaceDepth / DOF);

	float diag = Sqrt(float(Sqr(width) + Sqr(height)));
	float top = (float) height/diag * 0.5f*m_FieldOfView * neardist;
	float bottom = -top;
	float right = (float) width/diag * 0.5f*m_FieldOfView * neardist;
	float left = -right;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(left, right, bottom, top, neardist, fardist);

	glMatrixMode(GL_MODELVIEW);
}

VOID Camera::ResetView()
{
	//m_View = Matrix4<FLOAT>::Translation(0, 0, -5.0f*m_SceneSize) * Matrix4<FLOAT>::Translation(-m_SceneCenter);
}

#endif
