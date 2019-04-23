/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Graphics/VideoCapture.hpp $
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

#if defined(PLATFORM_WINDOWS)
#include "Com/ComException.hpp"
#include "Com/ComPointer.hpp"

#include "dshow.h"
#include "qedit.h"
#pragma comment(lib, "quartz.lib")
#pragma comment(lib, "strmiids.lib")

#include "System/Threading/Event.hpp"

namespace Bolitho
{
  namespace Graphics
  {
    class LIBRARY_API VideoCapture : public Object
    {
      DeclareRuntimeType();
    public:
      /* Constructs a still image capture context for a device */
      VideoCapture(LPCTSTR DeviceName, UINT Width, UINT Height);
      
      /* Begins raw continous capture mode */
      VOID BeginRawCapture();

      /* Begins a capture sequence */
      VOID BeginRawCapture(UINT SettleFrameCount, UINT CaptureFrameCount);

      /* Waits for a capture sequence to complete */
      VOID Capture(UINT SettleFrameCount, UINT CaptureFrameCount);

      /* Ends raw capture mode */
      VOID EndRawCapture();

      /* Sets the camera Exposure setting */
      VOID SetExposure(LONG ExposureSetting);

      /* Gets the camera Exposure setting */
      LONG GetExposure();

      /* Returns an array of capture device names */
      static Array<StringPair> EnumerateCaptureDevices();

      /* The width of the frame */
      UINT Width() CONST
      { return m_Width; }

      /* The height of the frame */
      UINT Height()
      { return m_Height; }

      /* The number of frames collected */
      UINT GetFrameCount()
      { return m_SampleCount; }

    private:
      System::Event m_CaptureCompleteEvent;

      ComPtr<IGraphBuilder> m_pGraphBuilder;
      ComPtr<ICaptureGraphBuilder2> m_pCaptureGraphBuilder;
      ComPtr<IBaseFilter> m_pVideoBaseFilter;
      ComPtr<ISampleGrabber> m_pSampleGrabber;
      ComPtr<IBaseFilter> m_pSampleGrabberFilter;
      ComPtr<IBaseFilter> m_pNullRenderer;
      ComPtr<IAMStreamConfig> m_pStreamConfig;
      ComPtr<IMediaControl> m_pMediaControl;
      ComPtr<IAMCameraControl> m_pCameraControl;

      UINT m_TargetFrameCount;
      UINT m_SampleCount;
      UINT m_SettleCount;
      UINT m_Width;
      UINT m_Height;

      BOOL m_Accumulate;
      Buffer<FLOAT> m_AccumlatedFrames;
      
      Buffer<BYTE> m_LastFrame;
    };
  }
}

#endif

