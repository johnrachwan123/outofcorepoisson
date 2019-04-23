/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Utility/FuncPtr.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1631 $
 * Last Updated: $Date: 2008-09-15 07:52:23 -0700 (Mon, 15 Sep 2008) $
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

#include "Utility/Nullable.hpp"
#include "System/Threading/Thread.hpp"
#include "System/Threading/Event.hpp"

namespace Bolitho
{

  namespace Internal
  {
    template<class R>
    class Slot0
    {
    public:
      virtual R Invoke() = 0;
    };

    template<class R, class T0>
    class Slot1
    {
    public:
      virtual R Invoke(T0 P0) = 0;
    };

    template<class R, class T0, class T1>
    class Slot2
    {
    public:
      virtual R Invoke(T0 P0, T1 P1) = 0;
    };

    template<class R, class T0, class T1, class T2>
    class Slot3
    {
    public:
      virtual R Invoke(T0 P0, T1 P1, T2 P2) = 0;
    };

    template<class R, class T0, class T1, class T2, class T3>
    class Slot4
    {
    public:
      virtual R Invoke(T0 P0, T1 P1, T2 P2, T3 P3) = 0;
    };


    template<class R>
    class GlobalFuncSlot0 : public Slot0<R>
    {
    public:
      typedef R (*FuncPtr)();
      GlobalFuncSlot0(FuncPtr F)
      { m_Func = F; }
      virtual R Invoke()
      { return m_Func(); }
    private:
      FuncPtr m_Func;
    };

    template<class R, class T0>
    class GlobalFuncSlot1 : public Slot1<R,T0>
    {
    public:
      typedef R (*FuncPtr)(T0);
      GlobalFuncSlot1(FuncPtr F)
      { m_Func = F; }
      virtual R Invoke(T0 P0)
      { return m_Func(P0); }
    private:
      FuncPtr m_Func;
    };

    template<class R, class T0, class T1>
    class GlobalFuncSlot2 : public Slot2<R,T0,T1>
    {
    private:
      typedef R (*FuncPtr)(T0,T1);
      FuncPtr m_Func;
    public:
      GlobalFuncSlot2(FuncPtr F)
      { m_Func = F; }
      virtual R Invoke(T0 P0, T1 P1)
      { return m_Func(P0,P1); }
    };

    template<class R, class T0, class T1, class T2>
    class GlobalFuncSlot3 : public Slot3<R,T0,T1,T2>
    {
    public:
      typedef R (*FuncPtr)(T0,T1,T2);
      GlobalFuncSlot3(FuncPtr F)
      { m_Func = F; }
      virtual R Invoke(T0 P0, T1 P1, T2 P2)
      { return m_Func(P0,P1,P2); }
    private:
      FuncPtr m_Func;
    };

    template<class R, class T0, class T1, class T2, class T3>
    class GlobalFuncSlot4 : public Slot4<R,T0,T1,T2,T3>
    {
    public:
      typedef R (*FuncPtr)(T0,T1,T2,T3);
      GlobalFuncSlot4(FuncPtr F)
      { m_Func = F; }
      virtual R Invoke(T0 P0, T1 P1, T2 P2, T3 P3)
      { return m_Func(P0,P1,P2,P3); }
    private:
      FuncPtr m_Func;
    };

    template<class C, class R>
    class MemberFuncSlot0 : public Slot0<R>
    {
    public:
      typedef R (C::*FuncPtr)();

      MemberFuncSlot0(C& Obj, FuncPtr F) 
        :
      m_Func(F), m_Obj(Obj)
      {}

      virtual R Invoke()
      { return (m_Obj.*m_Func)(); }

    private:
      FuncPtr m_Func;
      C& m_Obj;
    };

    template<class C, class R, class T0>
    class MemberFuncSlot1 : public Slot1<R,T0>
    {
    public:
      typedef R (C::*FuncPtr)(T0);

      MemberFuncSlot1(C& Obj, FuncPtr F) 
        :
      m_Func(F), m_Obj(Obj)
      {}

      virtual R Invoke(T0 P0)
      { return (m_Obj.*m_Func)(P0); }

    private:
      FuncPtr m_Func;
      C& m_Obj;
    };

    template<class C, class R, class T0, class T1>
    class MemberFuncSlot2 : public Slot2<R,T0,T1>
    {
    public:
      typedef R (C::*FuncPtr)(T0,T1);

      MemberFuncSlot2(C& Obj, FuncPtr F) 
        :
      m_Func(F), m_Obj(Obj)
      {}

      virtual R Invoke(T0 P0, T1 P1)
      { return (m_Obj.*m_Func)(P0,P1); }
    private:
      FuncPtr m_Func;
      C& m_Obj;
    };

    template<class C, class R, class T0, class T1, class T2>
    class MemberFuncSlot3 : public Slot3<R,T0,T1,T2>
    {
    public:
      typedef R (C::*FuncPtr)(T0,T1,T2);

      MemberFuncSlot3(C& Obj, FuncPtr F) 
        :
      m_Func(F), m_Obj(Obj)
      {}

      virtual R Invoke(T0 P0, T1 P1, T2 P2)
      { return (m_Obj.*m_Func)(P0,P1,P2); }
    private:
      FuncPtr m_Func;
      C& m_Obj;
    };

    template<class C, class R, class T0, class T1, class T2, class T3>
    class MemberFuncSlot4 : public Slot4<R,T0,T1,T2,T3>
    {
    public:
      typedef R (C::*FuncPtr)(T0,T1,T2,T3);

      MemberFuncSlot4(C& Obj, FuncPtr F) 
        :
      m_Func(F), m_Obj(Obj)
      {}

      virtual R Invoke(T0 P0, T1 P1, T2 P2, T3 P3)
      { return (m_Obj.*m_Func)(P0,P1,P2,P3); }
    private:
      FuncPtr m_Func;
      C& m_Obj;
    };


    template<class C, class R>
    class ConstMemberFuncSlot0 : public Slot0<R>
    {
    public:
      typedef R (C::*FuncPtr)() CONST;

      ConstMemberFuncSlot0(CONST C& Obj, FuncPtr F) 
        :
      m_Func(F), m_Obj(Obj)
      {}

      virtual R Invoke()
      { return (m_Obj.*m_Func)(); }

    private:
      FuncPtr m_Func;
      CONST C& m_Obj;
    };

    template<class C, class R, class T0>
    class ConstMemberFuncSlot1 : public Slot1<R,T0>
    {
    public:
      typedef R (C::*FuncPtr)(T0) CONST;

      ConstMemberFuncSlot1(CONST C& Obj, FuncPtr F) 
        :
      m_Func(F), m_Obj(Obj)
      {}

      virtual R Invoke(T0 P0)
      { return (m_Obj.*m_Func)(P0); }

    private:
      FuncPtr m_Func;
      CONST C& m_Obj;
    };

    template<class C, class R, class T0, class T1>
    class ConstMemberFuncSlot2 : public Slot2<R,T0,T1>
    {
    public:
      typedef R (C::*FuncPtr)(T0,T1) CONST;

      ConstMemberFuncSlot2(CONST C& Obj, FuncPtr F) 
        :
      m_Func(F), m_Obj(Obj)
      {}

      virtual R Invoke(T0 P0, T1 P1)
      { return (m_Obj.*m_Func)(P0,P1); }
    private:
      FuncPtr m_Func;
      CONST C& m_Obj;
    };

    template<class C, class R, class T0, class T1, class T2>
    class ConstMemberFuncSlot3 : public Slot3<R,T0,T1,T2>
    {
    public:
      typedef R (C::*FuncPtr)(T0,T1,T2) CONST;

      ConstMemberFuncSlot3(CONST C& Obj, FuncPtr F) 
        :
      m_Func(F), m_Obj(Obj)
      {}

      virtual R Invoke(T0 P0, T1 P1, T2 P2)
      { return (m_Obj.*m_Func)(P0,P1,P2); }
    private:
      FuncPtr m_Func;
      CONST C& m_Obj;
    };

    template<class C, class R, class T0, class T1, class T2, class T3>
    class ConstMemberFuncSlot4 : public Slot4<R,T0,T1,T2,T3>
    {
    public:
      typedef R (C::*FuncPtr)(T0,T1,T2,T3) CONST;

      ConstMemberFuncSlot4(CONST C& Obj, FuncPtr F) 
        :
      m_Func(F), m_Obj(Obj)
      {}

      virtual R Invoke(T0 P0, T1 P1, T2 P2, T3 P3)
      { return (m_Obj.*m_Func)(P0,P1,P2,P3); }
    private:
      FuncPtr m_Func;
      CONST C& m_Obj;
    };
  }

  template<class R, class T0=NULL_T, class T1=NULL_T, class T2=NULL_T, class T3=NULL_T>
  class FuncPtr
  {
  private:
    class ThreadParams
    {
    public:
      ThreadParams(Bolitho::Internal::Slot4<R,T0,T1,T2,T3>* F) : pFunc(F), Ev(FALSE, TRUE)
      {}

      System::Event Ev;
      Bolitho::Internal::Slot4<R,T0,T1,T2,T3>* pFunc;
      R ReturnValue;
      T0 Param0;
      T1 Param1;
      T2 Param2;
      T3 Param3;
    };

    static DWORD WINAPI ThreadProc(LPVOID p)
    {
      ThreadParams* pParam = (ThreadParams*)p;
      pParam->ReturnValue = pParam->pFunc->Invoke(pParam->Param0, pParam->Param1, pParam->Param2, pParam->Param3);
      pParam->Ev.Set();
      return 0;
    }

  public:
    class AsyncResult
    {
    public:
      AsyncResult(ThreadParams* pParams) : m_pParams(pParams)
      {}
      ~AsyncResult()
      { Wait(); delete m_pParams; }

      R ReturnValue()
      { Wait(); return m_pParams->ReturnValue; }

      BOOL Wait(DWORD Timeout = INFINITE)
      { return m_pParams->Ev.Wait(Timeout); }

    private:
      ThreadParams* m_pParams;
    };

  public:
    FuncPtr()
      : m_pSlot(0)
    {}

    FuncPtr(Bolitho::Internal::Slot4<R,T0,T1,T2,T3>* pSlot)
      : m_pSlot(pSlot)
    {}

    operator Bolitho::Internal::Slot4<R,T0,T1,T2,T3>* ()
    { return m_pSlot; }

    inline R Invoke(T0 P0, T1 P1, T2 P2, T3 P3)
    { if (m_pSlot) return m_pSlot->Invoke(P0,P1,P2,P3); }

    inline R operator()(T0 P0, T1 P1, T2 P2, T3 P3)
    { if (m_pSlot) return m_pSlot->Invoke(P0,P1,P2,P3); }

    inline AsyncResult InvokeAsync(T0 P0, T1 P1, T2 P2, T3 P3)
    {
      ThreadParams* pParams = new ThreadParams(m_pSlot);
      pParams->Param0 = P0;
      pParams->Param1 = P1;
      pParams->Param2 = P2;
      pParams->Param3 = P3;

      Ptr<System::Thread> pThread = System::Thread::Create(&ThreadProc, pParams);
      return AsyncResult(pParams);
    }


  private:
    Bolitho::Internal::Slot4<R,T0,T1,T2,T3>* m_pSlot;

  };

  template<class R, class T0, class T1, class T2>
  class FuncPtr<R,T0,T1,T2,NULL_T>
  {
  private:
    class ThreadParams
    {
    public:
      ThreadParams(Bolitho::Internal::Slot3<R,T0,T1,T2>* F) : pFunc(F), Ev(FALSE, TRUE)
      {}

      System::Event Ev;
      Bolitho::Internal::Slot3<R,T0,T1,T2>* pFunc;
      R ReturnValue;
      T0 Param0;
      T1 Param1;
      T2 Param2;
    };

    static DWORD WINAPI ThreadProc(LPVOID p)
    {
      ThreadParams* pParam = (ThreadParams*)p;
      pParam->ReturnValue = pParam->pFunc->Invoke(pParam->Param0, pParam->Param1, pParam->Param2);
      pParam->Ev.Set();
      return 0;
    }

  public:
    class AsyncResult
    {
    public:
      AsyncResult(ThreadParams* pParams) : m_pParams(pParams)
      {}
      ~AsyncResult()
      { Wait(); delete m_pParams; }

      R ReturnValue()
      { Wait(); return m_pParams->ReturnValue; }

      BOOL Wait(DWORD Timeout = INFINITE)
      { return m_pParams->Ev.Wait(Timeout); }

    private:
      ThreadParams* m_pParams;
    };

  public:
    FuncPtr()
      : m_pSlot(0)
    {}

    FuncPtr(Bolitho::Internal::Slot3<R,T0,T1,T2>* pSlot)
      : m_pSlot(pSlot)
    {}

    operator Bolitho::Internal::Slot3<R,T0,T1,T2>* ()
    { return m_pSlot; }

    inline R Invoke(T0 P0, T1 P1, T2 P2)
    { if (m_pSlot) return m_pSlot->Invoke(P0,P1,P2); }

    inline R operator()(T0 P0, T1 P1, T2 P2)
    { if (m_pSlot) return m_pSlot->Invoke(P0,P1,P2); }

    inline AsyncResult InvokeAsync(T0 P0, T1 P1, T2 P2)
    {
      ThreadParams* pParams = new ThreadParams(m_pSlot);
      pParams->Param0 = P0;
      pParams->Param1 = P1;
      pParams->Param2 = P2;
      Ptr<System::Thread> pThread = System::Thread::Create(&ThreadProc, pParams);
      return AsyncResult(pParams);
    }


  private:
    Bolitho::Internal::Slot3<R,T0,T1,T2>* m_pSlot;
  };

  template<class R, class T0, class T1>
  class FuncPtr<R,T0,T1,NULL_T,NULL_T>
  {
  private:
    class ThreadParams
    {
    public:
      ThreadParams(Bolitho::Internal::Slot2<R,T0,T1>* F) : pFunc(F), Ev(FALSE, TRUE)
      {}

      System::Event Ev;
      Bolitho::Internal::Slot2<R,T0,T1>* pFunc;
      R ReturnValue;
      T0 Param0;
      T1 Param1;
    };

    static DWORD WINAPI ThreadProc(LPVOID p)
    {
      ThreadParams* pParam = (ThreadParams*)p;
      pParam->ReturnValue = pParam->pFunc->Invoke(pParam->Param0, pParam->Param1);
      pParam->Ev.Set();
      return 0;
    }

  public:
    class AsyncResult
    {
    public:
      AsyncResult(ThreadParams* pParams) : m_pParams(pParams)
      {}
      ~AsyncResult()
      { Wait(); delete m_pParams; }

      R ReturnValue()
      { Wait(); return m_pParams->ReturnValue; }

      BOOL Wait(DWORD Timeout = INFINITE)
      { return m_pParams->Ev.Wait(Timeout); }

    private:
      ThreadParams* m_pParams;

    };

  public:
    FuncPtr()
      : m_pSlot(0)
    {}

    FuncPtr(Bolitho::Internal::Slot2<R,T0,T1>* pSlot)
      : m_pSlot(pSlot)
    {}

    operator Bolitho::Internal::Slot2<R,T0,T1>* ()
    { return m_pSlot; }

    inline R Invoke(T0 P0, T1 P1)
    { if (m_pSlot) return m_pSlot->Invoke(P0,P1); }

    inline R operator()(T0 P0, T1 P1)
    { if (m_pSlot) return m_pSlot->Invoke(P0,P1); }

    inline AsyncResult InvokeAsync(T0 P0, T1 P1)
    {
      ThreadParams* pParams = new ThreadParams(m_pSlot);
      pParams->Param0 = P0;
      pParams->Param1 = P1;
      Ptr<System::Thread> pThread = System::Thread::Create(&ThreadProc, pParams);
      return AsyncResult(pParams);
    }


  private:
    Bolitho::Internal::Slot2<R,T0,T1>* m_pSlot;
  };

  template<class R, class T0>
  class FuncPtr<R,T0,NULL_T,NULL_T,NULL_T>
  {
  private:
    class ThreadParams
    {
    public:
      ThreadParams(Bolitho::Internal::Slot1<R,T0>* F) : pFunc(F), Ev(FALSE, TRUE)
      {}

      System::Event Ev;
      Bolitho::Internal::Slot1<R,T0>* pFunc;
      R ReturnValue;
      T0 Param0;
    };

    static DWORD WINAPI ThreadProc(LPVOID p)
    {
      ThreadParams* pParam = (ThreadParams*)p;
      pParam->ReturnValue = pParam->pFunc->Invoke(pParam->Param0);
      pParam->Ev.Set();
      return 0;
    }

  public:
    class AsyncResult
    {
    public:
      AsyncResult(ThreadParams* pParams) : m_pParams(pParams)
      {}
      ~AsyncResult()
      { Wait(); delete m_pParams; }

      R ReturnValue()
      { Wait(); return m_pParams->ReturnValue; }

      BOOL Wait(DWORD Timeout = INFINITE)
      { return m_pParams->Ev.Wait(Timeout); }

    private:
      ThreadParams* m_pParams;

    };

    FuncPtr()
      : m_pSlot(0)
    {}

    FuncPtr(Bolitho::Internal::Slot1<R,T0>* pSlot)
      : m_pSlot(pSlot)
    {}

    operator Bolitho::Internal::Slot1<R,T0>* ()
    { return m_pSlot; }

    inline R Invoke(T0 P0)
    { Assert(m_pSlot != 0); if (m_pSlot) return m_pSlot->Invoke(P0); }

    inline AsyncResult InvokeAsync(T0 P0)
    {
      ThreadParams* pParams = new ThreadParams(m_pSlot);
      pParams->Param0 = P0;
      Ptr<System::Thread> pThread = System::Thread::Create(&ThreadProc, pParams);
      return AsyncResult(pParams);
    }

    inline R operator()(T0 P0)
    { if (m_pSlot) return m_pSlot->Invoke(P0); }

  private:
    Bolitho::Internal::Slot1<R,T0>* m_pSlot;
  };

  template<class R>
  class FuncPtr<R,NULL_T,NULL_T,NULL_T,NULL_T>
  {
  private:
    class ThreadParams
    {
    public:
      ThreadParams(Bolitho::Internal::Slot0<R>* F) : pFunc(F), Ev(FALSE, TRUE)
      {}

      System::Event Ev;
      Bolitho::Internal::Slot0<R>* pFunc;
      R ReturnValue;
    };

    static DWORD WINAPI ThreadProc(LPVOID p)
    {
      ThreadParams* pParam = (ThreadParams*)p;
      pParam->ReturnValue = pParam->pFunc->Invoke();
      pParam->Ev.Set();
      return 0;
    }

  public:
    class AsyncResult
    {
    public:
      AsyncResult(ThreadParams* pParams) : m_pParams(pParams)
      {}
      ~AsyncResult()
      { Wait(); delete m_pParams; }

      R ReturnValue()
      { Wait(); return m_pParams->ReturnValue; }

      BOOL Wait(DWORD Timeout = INFINITE)
      { return m_pParams->Ev.Wait(Timeout); }

    private:
      ThreadParams* m_pParams;

    };

  public:
    FuncPtr()
      : m_pSlot(0)
    {}

    FuncPtr(Bolitho::Internal::Slot0<R>* pSlot)
      : m_pSlot(pSlot)
    {}

    operator Bolitho::Internal::Slot0<R>* ()
    { return m_pSlot; }

    inline R Invoke()
    { if (m_pSlot) return m_pSlot->Invoke(); }

    inline R operator()()
    { if (m_pSlot) return m_pSlot->Invoke(); }

    inline AsyncResult InvokeAsync()
    {
      ThreadParams* pParams = new ThreadParams(m_pSlot);
      Ptr<System::Thread> pThread = System::Thread::Create(&ThreadProc, pParams);
      return AsyncResult(pParams);
    }

  private:
    Bolitho::Internal::Slot0<R>* m_pSlot;
  };

  template<class R, class T0, class T1, class T2, class T3>
  FuncPtr<R,T0,T1,T2,T3> CreateFuncPtr(R(*pFunc)(T0,T1,T2,T3))
  { return FuncPtr<R,T0,T1,T2,T3>(new Bolitho::Internal::GlobalFuncSlot4<R,T0,T1,T2,T3>(pFunc)); }

  template<class C, class R, class T0, class T1, class T2, class T3>
  FuncPtr<R,T0,T1,T2,T3> CreateFuncPtr(C& Obj, R (C::*pFunc)(T0,T1,T2,T3))
  { return FuncPtr<R,T0,T1,T2,T3>(new Bolitho::Internal::MemberFuncSlot4<C,R,T0,T1,T2,T3>(Obj, pFunc)); }

  template<class C, class R, class T0, class T1, class T2, class T3>
  FuncPtr<R,T0,T1> CreateFuncPtr(C* Obj, R (C::*pFunc)(T0,T1,T2,T3))
  { return FuncPtr<R,T0,T1,T2,T3>(new Bolitho::Internal::MemberFuncSlot4<C,R,T0,T1,T2,T3>(*Obj, pFunc)); }

  template<class C, class R, class T0, class T1, class T2, class T3>
  FuncPtr<R,T0,T1,T2,T3> CreateFuncPtr(CONST C& Obj, R (C::*pFunc)(T0,T1,T2,T3) CONST)
  { return FuncPtr<R,T0,T1,T2,T3>(new Bolitho::Internal::ConstMemberFuncSlot4<C,R,T0,T1,T2,T3>(Obj, pFunc)); }

  template<class C, class R, class T0, class T1, class T2, class T3>
  FuncPtr<R,T0,T1> CreateFuncPtr(CONST C* Obj, R (C::*pFunc)(T0,T1,T2,T3) CONST)
  { return FuncPtr<R,T0,T1,T2,T3>(new Bolitho::Internal::ConstMemberFuncSlot4<C,R,T0,T1,T2,T3>(*Obj, pFunc)); }

  template<class R, class T0, class T1, class T2>
  FuncPtr<R,T0,T1,T2> CreateFuncPtr(R(*pFunc)(T0,T1,T2))
  { return FuncPtr<R,T0,T1,T2>(new Bolitho::Internal::GlobalFuncSlot3<R,T0,T1,T2>(pFunc)); }

  template<class C, class R, class T0, class T1, class T2>
  FuncPtr<R,T0,T1,T2> CreateFuncPtr(C& Obj, R (C::*pFunc)(T0,T1,T2))
  { return FuncPtr<R,T0,T1,T2>(new Bolitho::Internal::MemberFuncSlot3<C,R,T0,T1,T2>(Obj, pFunc)); }

  template<class C, class R, class T0, class T1, class T2>
  FuncPtr<R,T0,T1> CreateFuncPtr(C* Obj, R (C::*pFunc)(T0,T1,T2))
  { return FuncPtr<R,T0,T1,T2>(new Bolitho::Internal::MemberFuncSlot3<C,R,T0,T1,T2>(*Obj, pFunc)); }

  template<class C, class R, class T0, class T1, class T2>
  FuncPtr<R,T0,T1,T2> CreateFuncPtr(CONST C& Obj, R (C::*pFunc)(T0,T1,T2) CONST)
  { return FuncPtr<R,T0,T1,T2>(new Bolitho::Internal::ConstMemberFuncSlot3<C,R,T0,T1,T2>(Obj, pFunc)); }

  template<class C, class R, class T0, class T1, class T2>
  FuncPtr<R,T0,T1> CreateFuncPtr(CONST C* Obj, R (C::*pFunc)(T0,T1,T2) CONST)
  { return FuncPtr<R,T0,T1,T2>(new Bolitho::Internal::ConstMemberFuncSlot3<C,R,T0,T1,T2>(*Obj, pFunc)); }

  template<class R, class T0, class T1>
  FuncPtr<R,T0,T1> CreateFuncPtr(R(*pFunc)(T0,T1))
  { return FuncPtr<R,T0,T1>(new Bolitho::Internal::GlobalFuncSlot2<R,T0,T1>(pFunc)); }

  template<class C, class R, class T0, class T1>
  FuncPtr<R,T0,T1> CreateFuncPtr(C& Obj, R (C::*pFunc)(T0,T1))
  { return FuncPtr<R,T0,T1>(new Bolitho::Internal::MemberFuncSlot2<C,R,T0,T1>(Obj, pFunc)); }

  template<class C, class R, class T0, class T1>
  FuncPtr<R,T0,T1> CreateFuncPtr(C* Obj, R (C::*pFunc)(T0,T1))
  { return FuncPtr<R,T0,T1>(new Bolitho::Internal::MemberFuncSlot2<C,R,T0,T1>(*Obj, pFunc)); }

  template<class C, class R, class T0, class T1>
  FuncPtr<R,T0,T1> CreateFuncPtr(CONST C& Obj, R (C::*pFunc)(T0,T1) CONST)
  { return FuncPtr<R,T0,T1>(new Bolitho::Internal::ConstMemberFuncSlot2<C,R,T0,T1>(Obj, pFunc)); }

  template<class C, class R, class T0, class T1>
  FuncPtr<R,T0,T1> CreateFuncPtr(CONST C* Obj, R (C::*pFunc)(T0,T1) CONST)
  { return FuncPtr<R,T0,T1>(new Bolitho::Internal::ConstMemberFuncSlot2<C,R,T0,T1>(*Obj, pFunc)); }

  template<class R, class T0>
  FuncPtr<R,T0> CreateFuncPtr(R(*pFunc)(T0))
  { return FuncPtr<R,T0>(new Bolitho::Internal::GlobalFuncSlot1<R,T0>(pFunc)); }

  template<class C, class R, class T0>
  FuncPtr<R,T0> CreateFuncPtr(C& Obj, R (C::*pFunc)(T0))
  { return FuncPtr<R,T0>(new Bolitho::Internal::MemberFuncSlot1<C,R,T0>(Obj, pFunc)); }

  template<class C, class R, class T0>
  FuncPtr<R,T0> CreateFuncPtr(C* Obj, R (C::*pFunc)(T0))
  { return FuncPtr<R,T0>(new Bolitho::Internal::MemberFuncSlot1<C,R,T0>(*Obj, pFunc)); }

  template<class C, class R, class T0>
  FuncPtr<R,T0> CreateFuncPtr(CONST C& Obj, R (C::*pFunc)(T0) CONST)
  { return FuncPtr<R,T0>(new Bolitho::Internal::ConstMemberFuncSlot1<C,R,T0>(Obj, pFunc)); }

  template<class C, class R, class T0>
  FuncPtr<R,T0> CreateFuncPtr(CONST C* Obj, R (C::*pFunc)(T0) CONST)
  { return FuncPtr<R,T0>(new Bolitho::Internal::ConstMemberFuncSlot1<C,R,T0>(*Obj, pFunc)); }

  template<class R>
  FuncPtr<R> CreateFuncPtr(R(*pFunc)())
  { return FuncPtr<R>(new Bolitho::Internal::GlobalFuncSlot0<R>(pFunc)); }

  template<class C, class R>
  FuncPtr<R> CreateFuncPtr(C& Obj,  R (C::*pFunc)())
  { return FuncPtr<R>(new Bolitho::Internal::MemberFuncSlot0<C,R>(Obj, pFunc)); }

  template<class C, class R>
  FuncPtr<R> CreateFuncPtr(C* Obj,  R (C::*pFunc)())
  { return FuncPtr<R>(new Bolitho::Internal::MemberFuncSlot0<C,R>(*Obj, pFunc)); }

  template<class C, class R>
  FuncPtr<R> CreateFuncPtr(CONST C& Obj,  R (C::*pFunc)() CONST)
  { return FuncPtr<R>(new Bolitho::Internal::ConstMemberFuncSlot0<C,R>(Obj, pFunc)); }

  template<class C, class R>
  FuncPtr<R> CreateFuncPtr(CONST C* Obj,  R (C::*pFunc)() CONST)
  { return FuncPtr<R>(new Bolitho::Internal::ConstMemberFuncSlot0<C,R>(*Obj, pFunc)); }

}

