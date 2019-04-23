/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/StreamingLaplacianSolver.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 893 $
 * Last Updated: $Date: 2008-12-10 13:18:45 -0800 (Wed, 10 Dec 2008) $
 * 
 * 
 * Copyright (c) 2006-2007, Matthew G Bolitho;  Michael Kazhdan
 * All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer. Redistributions in binary form must reproduce
 * the above copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the distribution. 
 * 
 * Neither the name of the Johns Hopkins University nor the names of its contributors
 * may be used to endorse or promote products derived from this software without specific
 * prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 ***************************************************************************************************/

#pragma once

#include "Math/SparseMatrix.hpp"
using Bolitho::Math::SparseMatrix;
using Bolitho::Math::Vector;

#include "Collections/Array.hpp"
using Bolitho::Array;

#include "System/Threading/ThreadPool.hpp"
using Bolitho::System::ThreadPool;

#include "FunctionDataInfo.hpp"
#include "StreamingOctreeTraversal.hpp"

typedef FLOAT REAL;
typedef FLOAT MATRIXREAL;

class LaplacianMatrixSolver
{
  class NodeIndexSetFunction
  {
  public:
    UINT m_Index[CONFIG_MAXTHREADS];

    struct THREADLOCAL { UINT ThreadId; UINT RegionId; UINT PartitionId; };
    VOID InitThreadLocal(Tree* pTree, THREADLOCAL& L) {}

    VOID ProcessNode(Tree* pTree, TreeNode& N, THREADLOCAL& L);

    CONST STATIC BOOL ThreadSafe = TRUE;
  };

  class IsoValueFunction
  {
  public:
    DOUBLE m_IsoValue;

    struct THREADLOCAL { UINT ThreadId; UINT RegionId; UINT PartitionId; };
    VOID InitThreadLocal(Tree* pTree, THREADLOCAL& L) {}

    VOID ProcessNode(Tree* pTree, TreeNode& N, THREADLOCAL& L);

    CONST STATIC BOOL ThreadSafe = TRUE;
  };

  class LaplacianMatrixFunction
  {
  public:
    DOUBLE m_LaplacianValues[5][5][5];

    NodeFunctionDataInfo* m_pFDI;
    UINT m_Index[CONFIG_MAXTHREADS];

    SparseMatrix<MATRIXREAL>* m_pLaplacian;
    Vector<SparseMatrix<REAL>::MATRIXENTRY>* m_pDiagonal;

    struct THREADLOCAL
    {
      TreeNode::NeighborCache2 m_NeighborKey2;
      SparseMatrix<MATRIXREAL>::MATRIXENTRY RowElements[125];
      UINT ThreadId;
      UINT RegionId;
			UINT PartitionId;
    };

    VOID InitThreadLocal(Tree* pTree, THREADLOCAL& L)
    { L.m_NeighborKey2.Initialize(pTree); }

    VOID ProcessNode(Tree* pTree, TreeNode& N, THREADLOCAL& L);

    CONST STATIC BOOL ThreadSafe = TRUE;
  };

  class LaplacianProjectionFunction
  {
  public:
    NodeFunctionDataInfo* m_pFDI;

    struct THREADLOCAL
    {
      TreeNode::NeighborCache2 m_NeighborKey2;
      UINT ThreadId;
      UINT RegionId;
			UINT PartitionId;
		};


    VOID InitThreadLocal(Tree* pTree, THREADLOCAL& L)
    { L.m_NeighborKey2.Initialize(pTree); }

    VOID ProcessNode(Tree* pTree, TreeNode& N, THREADLOCAL& L);

    CONST STATIC BOOL ThreadSafe = TRUE;
  };

  NodeIndexSetFunction m_NISF;
  IsoValueFunction m_IVF;
  LaplacianMatrixFunction m_LMF;
  LaplacianProjectionFunction m_LPF;

  Array<INT> m_RingSize;

  SparseMatrix<MATRIXREAL> m_Laplacian[CONFIG_MAXTHREADS];
  Vector<SparseMatrix<REAL>::MATRIXENTRY> m_Diagonal[CONFIG_MAXTHREADS];
  Ptr<Tree> m_pTree;
  NodeFunctionDataInfo m_FDI;

  VOID SetIndices(USHORT Depth);
  VOID ProjectSolution(USHORT Depth, INT RelativeSliceIndex);
  VOID GetLaplacianMatrix(USHORT Depth);
  VOID SolveLaplacian(USHORT Depth, UINT SolverIterations);
  VOID SolveLaplacian(USHORT Depth, UINT SolverIterations, UINT PartitionId);
  VOID ComputeIsoValue(USHORT Depth);

	#if defined(CONFIG_MULTITHREADED)
	struct SolveLaplacianInfo
  {
    LaplacianMatrixSolver* pSolver;
		USHORT Depth;
		UINT SolverIterations;
		UINT ThreadId;
		UINT PartitionId;
  };
	
	static DWORD WINAPI SolveLaplacianProc(SolveLaplacianInfo* pSolverInfo);
	#endif

public:
  LaplacianMatrixSolver();
  ~LaplacianMatrixSolver();

  /* Initializes internal data structures */
  VOID Init(Ptr<Tree> pTree);

  /* Advance the solution by a single step at a given depth */
  VOID SolverStep(USHORT Depth);

  /* Initialise the solver state
   * Some depths are solved enough that the first step of the finest depth can be solved 
   */
  VOID InitSolver();

  /* Solves a single step for all depths greater than 'FromDepth' */
  VOID UpdateSolver(USHORT FromDepth);

  /* Finalizes the solver */
  VOID FinalizeSolver();

  DOUBLE m_IsoValue;
};
