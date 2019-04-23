/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/OpenGL/Context.hpp $
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

#pragma once

#if defined(USE_OPENGL)

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/GLExt.h>
#include <gl/WGLExt.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

#include "UI/Window.hpp"
#include "Graphics/Image.hpp"
#include "Graphics/OpenGL/Light.hpp"
#include "Graphics/OpenGL/Material.hpp"
#include "Graphics/OpenGL/Texture.hpp"
#include "Graphics/OpenGL/VertexBuffer.hpp"
#include "Graphics/OpenGL/IndexBuffer.hpp"
#include "Graphics/OpenGL/RenderBuffer.hpp"
#include "Graphics/OpenGL/FrameBuffer.hpp"
#include "Graphics/OpenGL/VertexShader.hpp"
#include "Graphics/OpenGL/FragmentShader.hpp"
#include "Graphics/OpenGL/Program.hpp"
#include "Math/Matrix4.hpp"

namespace Bolitho
{
  namespace Graphics
  {
    namespace OpenGL
    {
      template<GLenum E>
      class EnableDisableState
      {
      public:
        EnableDisableState()
        {}

        VOID Enable()
        { glEnable(E); }
        VOID Disable()
        { glDisable(E); }

        VOID operator = (BOOL b)
        { if (b) Enable(); else Disable(); }
        VOID operator = (bool b)
        { if (b) Enable(); else Disable(); }

      private:
        EnableDisableState(CONST EnableDisableState&)
        {}

        GLenum m_Enum;
      };

      template<GLenum M, GLenum GM>
      class MatrixStack
      {
      public:
        VOID Push()
        { glMatrixMode(M); glPushMatrix(); m_StackDepth++; }
        VOID Pop()
        { glMatrixMode(M); Assert(m_StackDepth > 0); glPopMatrix(); m_StackDepth--; }

        VOID LoadIdentity()
        { glMatrixMode(M); glLoadIdentity(); }

        VOID Load(CONST Math::Matrix4F& Matrix)
        { glMatrixMode(M); glLoadMatrixf(Matrix.Transpose()); }
        VOID Multiply(CONST Math::Matrix4F& Matrix)
        { glMatrixMode(M); glMultMatrixf(Matrix.Transpose()); }

        VOID operator = (CONST Math::Matrix4F& M)
        { Load(M); }
        VOID operator *= (CONST Math::Matrix4F& M)
        { Multiply(M); }

        operator Math::Matrix4F()
        { Matrix4F M; glMatrixMode(M); glGetFloatv(GM, M); return M.Transpose(); }
      private:
        UINT m_StackDepth;
      };

      struct DepthTestFunction
      { enum { NEVER = GL_NEVER, LESS = GL_LESS, EQUAL = GL_EQUAL, LESSEQUAL = GL_LEQUAL, GREATER = GL_GREATER, NOTEQUAL = GL_NOTEQUAL, GREATEREQUAL = GL_GEQUAL, ALWAYS = GL_ALWAYS }; };

      struct ShadeModel
      { enum { FLAT = GL_FLAT, SMOOTH = GL_SMOOTH }; };

      struct Face
      { enum { FRONT = GL_FRONT, BACK = GL_BACK, FRONTANDBACK = GL_FRONT_AND_BACK }; };

      class LIBRARY_API Context : public Object
      {
        DeclareRuntimeType();
      public:
        Context(Ptr<UI::Window> pWindow, UINT ColorBits, UINT AlphaBits, UINT DepthBits, UINT MultiSample);
        Context(UINT ColorBits, UINT AlphaBits, UINT DepthBits, UINT MultiSample);

        virtual ~Context();

        VOID Activate();
        VOID Deactivate();
        VOID SwapBuffers();

        VOID SetViewport(Rect R);
        VOID SetViewport(LONG X, LONG Y, LONG Width, LONG Height);

        /* Render Target Control */
        VOID SetClearColor(CONST ColorF& C);
        VOID SetClearDepth(FLOAT Depth);

        VOID Clear(DWORD Flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Ptr<RenderBuffer> CreateRenderBuffer(DWORD Format, UINT Width, UINT Height);
        Ptr<FrameBuffer> CreateFrameBuffer();

        VOID SetFrameBuffer(Ptr<FrameBuffer> pFrameBuffer);

        Ptr<Image> ReadFrameBuffer();
        Ptr<Image> ReadFrameBuffer(Rect R);

        /* Programmability */
        Ptr<VertexShader> CreateVertexShader();
        Ptr<FragmentShader> CreateFragmentShader();
        Ptr<GPUProgram> CreateGPUProgram();
        Ptr<GPUProgram> CreateGPUProgram(Ptr<VertexShader> pVertexShader, Ptr<FragmentShader> pFragmentShader);
        Ptr<GPUProgram> CreateGPUProgram(LPCTSTR VertexShader, LPCTSTR FragmentShader);

        VOID SetGPUProgram(Ptr<GPUProgram> pProgram);

        /* Vertex/Index Buffers */
        Ptr<VertexBuffer> CreateVertexBuffer(UINT Length, DWORD Usage = GL_STATIC_DRAW);
        Ptr<IndexBuffer> CreateIndexBuffer(UINT Length, DWORD Usage = GL_STATIC_DRAW);

        VOID SetVertexBuffer(Ptr<VertexBuffer> pVertexBuffer);
        VOID SetIndexBuffer(Ptr<IndexBuffer> pIndexBuffer);

        /* Textures */
        Ptr<Texture> CreateTexture(DWORD Format);
        Ptr<Texture> CreateTexture(DWORD Format, UINT Width, UINT Height);

        VOID SetTexture(UINT TextureUnit, Ptr<Texture> pTexture);

        /* Lighting Control */
        VOID SetGlobalAmbient(CONST ColorF& C);

        TLightState<0> Light0;
        TLightState<1> Light1;
        TLightState<2> Light2;
        TLightState<3> Light3;
        TLightState<4> Light4;
        TLightState<5> Light5;
        TLightState<6> Light6;
        TLightState<7> Light7;

        LightingState Lighting;

        /* Enable/Disable State */
        EnableDisableState<GL_DEPTH_TEST> DepthTest;
        EnableDisableState<GL_BLEND> AlphaBlending;
        EnableDisableState<GL_COLOR_MATERIAL> ColorMaterial;
        EnableDisableState<GL_DITHER> ColorDither;
        EnableDisableState<GL_NORMALIZE> Normalization;
        EnableDisableState<GL_POLYGON_OFFSET_FILL> PolygonOffsetFill;
        EnableDisableState<GL_CULL_FACE> FaceCulling;

        /* Matrix Stacks */
        MatrixStack<GL_PROJECTION, GL_PROJECTION_MATRIX> Projection;
        MatrixStack<GL_MODELVIEW, GL_MODELVIEW_MATRIX> ModelView;

        /* Depth Test Function */
        VOID SetDepthTestFunction(DWORD Func);

        VOID SetShadeModel(DWORD Model);
        VOID SetPointSize(FLOAT Size);
        VOID SetLineWidth(FLOAT Width);
        VOID SetPolygonOffset(FLOAT Factor, FLOAT Units);

        VOID SetColorWriteMask(BOOL Red, BOOL Green, BOOL Blue, BOOL Alpha);
        VOID SetColorWriteMask(BOOL Color);
        VOID SetDepthWriteMask(BOOL Depth);

        /* Material Control */
        MaterialState Material;

        /* Extension Function Pointers */
        PFNGLBINDBUFFERARBPROC glBindBuffer;
        PFNGLGENBUFFERSARBPROC glGenBuffers;
        PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;
        PFNGLBUFFERSUBDATAARBPROC glBufferSubData;
        PFNGLBUFFERDATAARBPROC glBufferData;
        PFNGLMAPBUFFERARBPROC glMapBuffer;
        PFNGLUNMAPBUFFERARBPROC glUnMapBuffer;

        PFNGLISRENDERBUFFEREXTPROC glIsRenderbufferEXT;
        PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT;
        PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT;
        PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT;
        PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT;
        PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT;
        PFNGLISFRAMEBUFFEREXTPROC glIsFramebufferEXT;
        PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
        PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;
        PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
        PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;
        PFNGLFRAMEBUFFERTEXTURE1DEXTPROC glFramebufferTexture1DEXT;
        PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
        PFNGLFRAMEBUFFERTEXTURE3DEXTPROC glFramebufferTexture3DEXT;
        PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;
        PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT;
        PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT;

        PFNGLATTACHSHADERPROC glAttachShader;
        PFNGLCOMPILESHADERPROC glCompileShader;
        PFNGLCREATEPROGRAMPROC glCreateProgram;
        PFNGLCREATESHADERPROC glCreateShader;
        PFNGLDELETEPROGRAMPROC glDeleteProgram;
        PFNGLDELETESHADERPROC glDeleteShader;
        PFNGLSHADERSOURCEPROC glShaderSource;
        PFNGLGETSHADERIVPROC glGetShaderiv;
        PFNGLLINKPROGRAMPROC glLinkProgram;
        PFNGLUSEPROGRAMPROC glUseProgram;
        PFNGLGETPROGRAMIVPROC glGetProgramiv;
        PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;

      private:
        VOID Create(UINT ColorBits, UINT AlphaBits, UINT DepthBits, UINT MultiSample);

        VOID InitExtensions();

        HGLRC m_hGLContext;
        HDC m_hDeviceContext;
        Ptr<UI::Window> m_pWindow;

        Ptr<Texture> m_pCurrentTexture[8];
        Ptr<GPUProgram> m_pCurrentProgram;
        Ptr<FrameBuffer> m_pCurrentFrameBuffer;

      };

    }
  }
}

#endif

