/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/StillImageCapture.cpp $
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
#include "Graphics/StillImageCapture.hpp"
#include "Debug/Trace.hpp"
#include "Com/Variant.hpp"

using namespace Bolitho;
using namespace Bolitho::Com;
using namespace Bolitho::Graphics;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Graphics,StillImageCapture,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#pragma warning(suppress:4355)
StillImageCapture::StillImageCapture(LPCTSTR DeviceName, UINT Width, UINT Height) : m_Callback(*this), m_CaptureCompleteEvent(FALSE)
{
  m_Width = Width;
  m_Height = Height;

  m_pGraphBuilder.CreateInstance(CLSID_FilterGraph);
  VerifyComResult(m_pCaptureGraphBuilder.CreateInstance(CLSID_CaptureGraphBuilder2));
  m_pCaptureGraphBuilder->SetFiltergraph(m_pGraphBuilder);

  VerifyComResult(CoGetObject(StringW(DeviceName), 0, __uuidof(IBaseFilter), (VOID**)&m_pVideoBaseFilter));
  VerifyComResult(m_pGraphBuilder->AddFilter(m_pVideoBaseFilter, L"VideoDeviceFilter"));
  
  VerifyComResult(m_pSampleGrabber.CreateInstance(CLSID_SampleGrabber));
  VerifyComResult(m_pSampleGrabberFilter.QueryInterface(m_pSampleGrabber));
  VerifyComResult(m_pGraphBuilder->AddFilter(m_pSampleGrabberFilter, L"SampleGrabber"));  
  
  AM_MEDIA_TYPE MediaType;
  ZeroMemory(&MediaType, sizeof(MediaType));

  MediaType.majortype = MEDIATYPE_Video;
  MediaType.subtype = MEDIASUBTYPE_RGB24;
  VerifyComResult(m_pSampleGrabber->SetMediaType(&MediaType));

  VerifyComResult(m_pSampleGrabber->SetCallback(&m_Callback, 1));

  VerifyComResult(m_pNullRenderer.CreateInstance(CLSID_NullRenderer));
  VerifyComResult(m_pGraphBuilder->AddFilter(m_pNullRenderer, L"NullRenderer"));

  VerifyComResult(m_pCaptureGraphBuilder->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_pVideoBaseFilter, __uuidof(IAMStreamConfig), (VOID**)&m_pStreamConfig));
  VerifyComResult(m_pMediaControl.QueryInterface(m_pGraphBuilder));

  /* Set resolution */
  AM_MEDIA_TYPE* pMediaType;
  VerifyComResult(m_pStreamConfig->GetFormat(&pMediaType));

  VIDEOINFOHEADER* pVideoInfoHeader = (VIDEOINFOHEADER*)pMediaType->pbFormat;
  pVideoInfoHeader->bmiHeader.biWidth = Width;
  pVideoInfoHeader->bmiHeader.biHeight = Height;

  VerifyComResult(m_pStreamConfig->SetFormat(pMediaType));
  CoTaskMemFree(pMediaType);

  m_pCameraControl.QueryInterface(m_pVideoBaseFilter);

  /* Render the Graph */
  VerifyComResult(m_pCaptureGraphBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_pVideoBaseFilter, m_pSampleGrabberFilter, m_pNullRenderer));

  m_LastFrame.Resize(m_Width*m_Height*3);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID StillImageCapture::SetExposure(LONG ExposureSetting)
{
  VerifyComResult(m_pCameraControl->Set(CameraControl_Exposure, ExposureSetting, CameraControl_Flags_Manual));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LONG StillImageCapture::GetExposure()
{
  LONG Exposure;
  LONG Flags;

  VerifyComResult(m_pCameraControl->Get(CameraControl_Exposure, &Exposure, &Flags));

  return Exposure;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID StillImageCapture::BeginRawCapture(BOOL Accumulate)
{
  BeginRawCapture(0, MAXUINT, Accumulate);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID StillImageCapture::EndRawCapture()
{
  m_pMediaControl->Stop();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID StillImageCapture::BeginRawCapture(UINT SettleFrameCount, UINT CaptureFrameCount, BOOL Accumulate)
{
  m_TargetFrameCount = CaptureFrameCount;
  m_SettleCount = SettleFrameCount;
  m_SampleCount = 0;
  m_AccumlatedFrames.Resize(m_Width*m_Height*3);
  m_CaptureCompleteEvent.Reset();
  m_Accumulate = Accumulate;
  m_pMediaControl->Run();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID StillImageCapture::Capture(UINT SettleFrameCount, UINT CaptureFrameCount, BOOL Accumulate)
{
  BeginRawCapture(SettleFrameCount, CaptureFrameCount, Accumulate);
  m_CaptureCompleteEvent.Wait();
  EndRawCapture();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Buffer<BYTE> StillImageCapture::GetLastRawFrame()
{
  return m_LastFrame;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Buffer<FLOAT> StillImageCapture::GetAccumulationBuffer()
{
  return m_AccumlatedFrames;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Array<StringPair> StillImageCapture::EnumerateCaptureDevices()
{
  Array<StringPair> VideoDevices;

  ComPtr<ICreateDevEnum> pSysDevEnum;
  pSysDevEnum.CreateInstance(CLSID_SystemDeviceEnum);

  /* Enumerate Video Capture Devices */
  ComPtr<IEnumMoniker> pEnum;
  if (pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum, 0) == S_FALSE)
    return VideoDevices;
  
  while(TRUE)
  {
    ULONG Count;
    ComPtr<IMoniker> pMoniker;
    if (pEnum->Next(1, &pMoniker, &Count) == S_FALSE)
      break;

    ComPtr<IPropertyBag> pPropertyBag;
    pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (VOID**)&pPropertyBag);
    
    String FriendlyName;
    if (pPropertyBag)
    {
      Variant Value;
      pPropertyBag->Read(L"FriendlyName", &Value, 0);
      FriendlyName = Value.bstrVal;
    }

    LPOLESTR Name;
    pMoniker->GetDisplayName(0, 0, &Name);
    VideoDevices.Add(StringPair(Name, FriendlyName));
    CoTaskMemFree(Name);
  }

  return VideoDevices;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT StillImageCapture::Callback::BufferCB(DOUBLE ElapsedTime, BYTE* pBuffer, LONG Length)
{
  UINT Exposure = m_Capture.GetExposure();

  if (m_Capture.m_SettleCount > 0)
  {
    m_Capture.m_SettleCount--;
  }
  else if (m_Capture.m_TargetFrameCount > 0)
  {
    m_Capture.m_SampleCount++;
    CopyMemory(m_Capture.m_LastFrame.GetBuffer(), pBuffer, Length);
    m_Capture.m_TargetFrameCount--;

    DOUBLE Sum = 0.0f;
    for (UINT i=0; i<(UINT)Length; i++)
      Sum += (DOUBLE)m_Capture.m_LastFrame[i];

    Sum = Sum / Length;

    Trace("Capture Norm: %lf", Sum);

    if (m_Capture.m_Accumulate)
    {
      for (UINT i=0; i<(UINT)Length; i++)
        m_Capture.m_AccumlatedFrames[i] += (FLOAT)m_Capture.m_LastFrame[i];
    }
  }

  if (m_Capture.m_TargetFrameCount == 0)
    m_Capture.m_CaptureCompleteEvent.Set();

  return S_OK;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
