/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/OpenGL/Context.cpp $
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

#include "Graphics/OpenGL/Context.hpp"
#include "Graphics/OpenGL/OpenGLException.hpp"
#include "Debug/Trace.hpp"

using namespace Bolitho;
using namespace Bolitho::Graphics;
using namespace Bolitho::Graphics::OpenGL;
using namespace Bolitho::UI;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Graphics::OpenGL,Context,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
Context::Context(UINT ColorBits, UINT AlphaBits, UINT DepthBits, UINT MultiSample)
{
  m_pWindow = new Window();
  m_pWindow->Create();
  Create(ColorBits, AlphaBits, DepthBits, MultiSample);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Context::Context(Ptr<UI::Window> pWindow, UINT ColorBits, UINT AlphaBits, UINT DepthBits, UINT MultiSample) 
  : m_pWindow(pWindow)
{
  Create(ColorBits, AlphaBits, DepthBits, MultiSample);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::Create(UINT ColorBits, UINT AlphaBits, UINT DepthBits, UINT MultiSample)
{
  Window W;
  W.Create();

  m_hDeviceContext = GetDC(W.Handle());

  PIXELFORMATDESCRIPTOR PixelFormat;
  ZeroMemory(&PixelFormat, sizeof(PixelFormat));

  PixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  PixelFormat.nVersion   = 1;
  PixelFormat.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SUPPORT_COMPOSITION | PFD_SWAP_COPY;
  PixelFormat.iPixelType = PFD_TYPE_RGBA;
  PixelFormat.cColorBits = ColorBits + AlphaBits;
  PixelFormat.cDepthBits = DepthBits;

  INT PixelFormatIndex = ChoosePixelFormat(m_hDeviceContext, &PixelFormat);
  SetPixelFormat(m_hDeviceContext, PixelFormatIndex, &PixelFormat);

  m_hGLContext = wglCreateContext(m_hDeviceContext); 

  Activate();

  PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

  if (wglChoosePixelFormatARB == 0)
    MultiSample = 0;

  while (MultiSample > 0)
  {
    INT PixelFormatAttributes[] = 
    {
      WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
      WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
      WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
      WGL_COLOR_BITS_ARB, ColorBits,
      WGL_ALPHA_BITS_ARB, AlphaBits,
      WGL_DEPTH_BITS_ARB, DepthBits,
      WGL_STENCIL_BITS_ARB, 0,
      WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
      WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
      WGL_SAMPLES_ARB, MultiSample,
      0,
    };

    UINT N = 0;
    PixelFormatIndex = 0;
    wglChoosePixelFormatARB(m_hDeviceContext, PixelFormatAttributes, NULL, 1, &PixelFormatIndex, &N);
    if (N > 0)
      break;

    MultiSample >>= 1;
  }

  ReleaseDC(W.Handle(), m_hDeviceContext);
  wglDeleteContext(m_hGLContext);
  W.Destroy();

  m_hDeviceContext = GetDC(m_pWindow->Handle());

  ZeroMemory(&PixelFormat, sizeof(PixelFormat));

  PixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  PixelFormat.nVersion   = 1;
  PixelFormat.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SUPPORT_COMPOSITION | PFD_SWAP_COPY;
  PixelFormat.iPixelType = PFD_TYPE_RGBA;
  PixelFormat.cColorBits = ColorBits + AlphaBits;
  PixelFormat.cDepthBits = DepthBits;

  if (MultiSample == 0)
    PixelFormatIndex = ChoosePixelFormat(m_hDeviceContext, &PixelFormat);
  SetPixelFormat(m_hDeviceContext, PixelFormatIndex, &PixelFormat);

  m_hGLContext = wglCreateContext(m_hDeviceContext); 

  Activate();

  TraceD(TEXT("OpenGL Renderer: %s"), glGetString(GL_RENDERER));
  TraceD(TEXT("OpenGL Vendor: %s"), glGetString(GL_VENDOR));
  TraceD(TEXT("OpenGL Version: %s"), glGetString(GL_VERSION));
  TraceD(TEXT("MultiSampling: %d"), MultiSample);

  InitExtensions();

  m_pWindow = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::InitExtensions()
{
  glBindBuffer = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
  glGenBuffers = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
  glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
  glBufferData = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
  glBufferSubData = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
  glMapBuffer = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
  glUnMapBuffer = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");

  glIsRenderbufferEXT = (PFNGLISRENDERBUFFEREXTPROC)wglGetProcAddress("glIsRenderbufferEXT");
  glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
  glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
  glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
  glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
  glGetRenderbufferParameterivEXT = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)wglGetProcAddress("glGetRenderbufferParameterivEXT");
  glIsFramebufferEXT = (PFNGLISFRAMEBUFFEREXTPROC)wglGetProcAddress("glIsFramebufferEXT");
  glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
  glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
  glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
  glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
  glFramebufferTexture1DEXT = (PFNGLFRAMEBUFFERTEXTURE1DEXTPROC)wglGetProcAddress("glFramebufferTexture1DEXT");
  glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
  glFramebufferTexture3DEXT = (PFNGLFRAMEBUFFERTEXTURE3DEXTPROC)wglGetProcAddress("glFramebufferTexture3DEXT");
  glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
  glGetFramebufferAttachmentParameterivEXT = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
  glGenerateMipmapEXT = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress("glGenerateMipmapEXT");

  glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
  glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
  glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
  glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
  glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
  glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
  glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
  glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
  glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
  glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
  glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
  glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Context::~Context()
{
  if (m_hGLContext)
  {
    wglDeleteContext(m_hGLContext);
    m_hGLContext = 0;
  }

  if (m_hDeviceContext)
  {
    DeleteDC(m_hDeviceContext);
    m_hDeviceContext = 0;
  }

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::Activate()
{
  wglMakeCurrent(m_hDeviceContext, m_hGLContext);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::Deactivate()
{
  wglMakeCurrent(NULL, NULL);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::SwapBuffers()
{
  ::SwapBuffers(m_hDeviceContext);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::SetClearColor(CONST ColorF& C)
{
  glClearColor(C[0], C[1], C[2], C[3]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::SetClearDepth(FLOAT Depth)
{
  glClearDepth(Depth);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::Clear(DWORD Flags)
{
  glClear(Flags);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::SetGlobalAmbient(CONST ColorF& C)
{
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, C);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::SetDepthTestFunction(DWORD Func)
{
  glDepthFunc(Func);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::SetPointSize(FLOAT Size)
{
  glPointSize(Size);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::SetShadeModel(DWORD Model)
{
  glShadeModel(Model);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::SetLineWidth(FLOAT Width)
{
  glLineWidth(Width);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::SetPolygonOffset(FLOAT Factor, FLOAT Units)
{
  glPolygonOffset(Factor, Units);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::SetColorWriteMask(BOOL Red, BOOL Green, BOOL Blue, BOOL Alpha)
{
  glColorMask(Red, Green, Blue, Alpha);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::SetColorWriteMask(BOOL Color)
{
  glColorMask(Color, Color, Color, Color);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::SetDepthWriteMask(BOOL Depth)
{
  glDepthMask(Depth);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::SetViewport(Rect R)
{
  glViewport(R.left, R.top, R.Width(), R.Height()); 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::SetViewport(LONG X, LONG Y, LONG Width, LONG Height)
{
  glViewport(X, Y, Width, Height);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Texture> Context::CreateTexture(DWORD Format)
{
  return new Texture(this, Format);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Texture> Context::CreateTexture(DWORD Format, UINT Width, UINT Height)
{
  Texture* pTexture = new Texture(this, Format);
  pTexture->LoadTexture(0, Width, Height, (CONST BYTE*)0);

  return pTexture;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::SetTexture(UINT TextureUnit, Ptr<Texture> pTexture)
{
  if (m_pCurrentTexture[TextureUnit] == pTexture)
    return;

  m_pCurrentTexture[TextureUnit] = pTexture;
  //glActiveTextureARB(GL_TEXTURE0_ARB); 
  pTexture->Bind();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<RenderBuffer> Context::CreateRenderBuffer(DWORD Format, UINT Width, UINT Height)
{
  return new RenderBuffer(this, Format, Width, Height);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<FrameBuffer> Context::CreateFrameBuffer()
{
  return new FrameBuffer(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Image> Context::ReadFrameBuffer()
{
  Size S = m_pWindow->GetClientSize();
  Image* pImage = new Image(S.cx, S.cy, PixelFormat::B8G8R8A8);

  glReadPixels(0,0,S.cx,S.cy, GL_BGRA, GL_UNSIGNED_BYTE, pImage->GetPixelData());
  VerifyOpenGLErrorState();

  return pImage;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Image> Context::ReadFrameBuffer(Rect R)
{
  Image* pImage = new Image(R.Width(), R.Height(), PixelFormat::B8G8R8A8);

  glReadPixels(R.left, R.top, R.Width(), R.Height(), GL_BGRA, GL_UNSIGNED_BYTE, pImage->GetPixelData());
  VerifyOpenGLErrorState();

  return pImage;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<VertexShader> Context::CreateVertexShader()
{
  return new VertexShader(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<FragmentShader> Context::CreateFragmentShader()
{
  return new FragmentShader(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<GPUProgram> Context::CreateGPUProgram()
{
  return new GPUProgram(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<GPUProgram> Context::CreateGPUProgram(Ptr<VertexShader> pVertexShader, Ptr<FragmentShader> pFragmentShader)
{
  GPUProgram* pProgram = new GPUProgram(this);
  if (pVertexShader)
    pProgram->Attach(pVertexShader);
  if (pFragmentShader)
    pProgram->Attach(pFragmentShader);
  return pProgram;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<GPUProgram> Context::CreateGPUProgram(LPCTSTR VertexShaderSource, LPCTSTR FragmentShaderSource)
{
  VertexShader* pVertexShader = 0;
  FragmentShader* pFragmentShader = 0;

  if (VertexShaderSource)
  {
    pVertexShader = new VertexShader(this);
    pVertexShader->Compile(VertexShaderSource);
  }

  if (FragmentShaderSource)
  {
    pFragmentShader = new FragmentShader(this);
    pFragmentShader->Compile(FragmentShaderSource);
  }

  return CreateGPUProgram(pVertexShader, pFragmentShader);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Context::SetGPUProgram(Ptr<GPUProgram> pProgram)
{
  if (m_pCurrentProgram != pProgram)
  {
    m_pCurrentProgram = pProgram;

    if (pProgram)
      glUseProgram(pProgram->m_ProgramId);
    else
      glUseProgram(0);

    VerifyOpenGLErrorState();
  }
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<VertexBuffer> Context::CreateVertexBuffer(UINT Length, DWORD Usage)
{
  return new VertexBuffer(this, Length, Usage);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<IndexBuffer> Context::CreateIndexBuffer(UINT Length, DWORD Usage)
{
  return new IndexBuffer(this, Length, Usage);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

#endif
