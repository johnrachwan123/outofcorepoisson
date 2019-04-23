/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/StreamingLaplacianSolver.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 911 $
 * Last Updated: $Date: 2008-12-29 12:34:14 -0800 (Mon, 29 Dec 2008) $
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

#include "Config.hpp"
#include "StreamingLaplacianSolver.hpp"

#include "Utility/Bits.hpp"
#include "System/Threading/Interlocked.hpp"
using Bolitho::InterlockedAccumulate;

//---------------------------------------------------------------------------------------------------
VOID LaplacianMatrixSolver::LaplacianProjectionFunction::ProcessNode(Tree* pTree, TreeNode& N, THREADLOCAL& L)
{
  L.m_NeighborKey2.ComputeNeighbors(N);

  INT Start[3], End[3];
  UINT cIndex[3];
  DOUBLE DotXY, D2DotXY;
  DOUBLE Dot[3][5], DDot[3][5], D2Dot[3][5];
  DOUBLE Value = N.Value();

  DOUBLE v[3][5];
  DOUBLE vXY;
  UINT centerIndex[3];
  DOUBLE WeightContribution = N.WeightContribution();
  USHORT Depth = N.Depth();

  Start[0] = Start[1] = Start[2] = 0;
  End[0] = End[1] = End[2] = 5;
  
  if(Depth == 0)
    return;
  
  // Update the bounds
  TreeNode P = pTree->GetNode(Depth - 1, L.m_NeighborKey2(Depth - 1, 2, 2, 2));

  Cube::FactorCornerIndex((UINT)(N.StreamOffset() - P.Children()), cIndex[0], cIndex[1], cIndex[2]);
  for(INT i=0;i<3;i++)
  {
    if(cIndex[i])
      Start[i] = 1;
    else
      End[i] = 4;
  }

  for(INT d=Depth-1;d>=0;d--)
  {
    TreeNode Node=P;
    USHORT dd;
    UINT off[3];
    P.GetDepthAndOffset(dd,off);
    
    VertexData::CenterKey(d,off,m_pFDI->FunctionData.Depth(),centerIndex);

    // Initialize the appropriate table values
    for(INT i=0;i<3;i++)
    {
      ZeroMemory(v[i],sizeof(DOUBLE)*5);
      ZeroMemory(Dot[i],sizeof(DOUBLE)*5);
      ZeroMemory(DDot[i],sizeof(DOUBLE)*5);
      ZeroMemory(D2Dot[i],sizeof(DOUBLE)*5);

      for(INT j=0;j<5;j++)
      {
        INT idx=Node.Offset(i)-2+j;
        INT tIdx=idx-((1<<d)-1);
        if(tIdx>=0 && tIdx<(1<<d))
        {
          Dot[i][j]=m_pFDI->FunctionData.DotTables[Depth](idx,N.Offset(i));
          DDot[i][j]=m_pFDI->FunctionData.DDotTables[Depth](idx,N.Offset(i));
          D2Dot[i][j]=m_pFDI->FunctionData.D2DotTables[Depth](idx,N.Offset(i));
        }
      }
      for(int j=1;j<4;j++)
      {
        INT Index = centerIndex[i] + ((j-2) << (m_pFDI->FunctionData.Depth()+1-d));
        if(Index >= 0 && Index < (INT)m_pFDI->FunctionData.SampleCount())
          v[i][j] = m_pFDI->FunctionData.ValueTables(N.Offset(i), Index);
      }
    }

    DOUBLE WeightCorrection = 1.0 / pow(4.0,d*1.5);
    for(int j=1; j<4; j++)
      v[0][j] *= WeightCorrection;

    for(INT i=Start[0];i<End[0];i++)
      for(INT j=Start[1];j<End[1];j++)
      {
        vXY=v[0][i]*v[1][j];
        DotXY=Dot[0][i]*Dot[1][j];
        D2DotXY=D2Dot[0][i]+D2Dot[1][j];
        for(INT k=Start[2];k<End[2];k++)
        {
          if(L.m_NeighborKey2(d,i,j,k) != TreeNode::INVALIDINDEX)
          {
            TreeNode Temp=pTree->GetNode(d,L.m_NeighborKey2(d,i,j,k));
            DOUBLE dTemp=DotXY*Dot[2][k];
            DOUBLE lap=dTemp*(D2DotXY+D2Dot[2][k]);

            if(Temp.Normal().SquareLength() > FLT_EPSILON)
            {
              Value += dTemp * (DDot[0][i] * Temp.Normal()[0] + DDot[1][j] * Temp.Normal()[1] + DDot[2][k] * Temp.Normal()[2]);
              WeightContribution += Temp.Normal().Length() * vXY * v[2][k];
            }
            Value -= Temp.Solution()*lap;
          }
        }
      }

      // Set the starting and ending bounds of the iteration
      if(d)
      {
        P = pTree->GetNode(d-1,L.m_NeighborKey2(d-1,2,2,2));

        Cube::FactorCornerIndex(INT(Node.StreamOffset()-P.Children()),cIndex[0],cIndex[1],cIndex[2]);
        for(INT i=0;i<3;i++)
        {
          if (cIndex[i])
            Start[i] = 1;
          else
            End[i] = 4;
        }
      }
  }

  N.Value() = (FLOAT)Value;
  N.WeightContribution() = WeightContribution;
}
//---------------------------------------------------------------------------------------------------
VOID LaplacianMatrixSolver::LaplacianMatrixFunction::ProcessNode(Tree* pTree, TreeNode& N, THREADLOCAL& L)
{
  UINT Count = 0;
	UINT Index = m_Index[L.PartitionId]++;
  
  TreeNode::Neighbors2& Neighbors = L.m_NeighborKey2.ComputeNeighbors(N);

  for(INT i=0;i<5;i++)
  {
    for(INT j=0;j<5;j++)
    {
      for(INT k=0;k<5;k++)
      {
        if(Neighbors(i,j,k) != TreeNode::INVALIDINDEX)
        {
          TreeNode Temp = pTree->GetNode(N.Depth(), Neighbors(i,j,k));
          DOUBLE Laplacian = m_LaplacianValues[i][j][k];

          if(i == 2 && j == 2 && k == 2)
          {
						m_pDiagonal[L.PartitionId].Set(Index, SparseMatrix<MATRIXREAL>::MATRIXENTRY(N.StreamOffset(), (FLOAT)Laplacian));
          }
          else
          {
            L.RowElements[Count].Value = (FLOAT)Laplacian;
            L.RowElements[Count].N = Temp.StreamOffset();
            Count++;
          }
        }
      }
    }
  }
  
  m_pLaplacian[L.PartitionId].SetRowSize(Index, Count);
  CopyMemory(m_pLaplacian[L.PartitionId].GetRow(Index), L.RowElements, sizeof(SparseMatrix<MATRIXREAL>::MATRIXENTRY)*Count);
 
}
//---------------------------------------------------------------------------------------------------
VOID LaplacianMatrixSolver::IsoValueFunction::ProcessNode(Tree* pTree, TreeNode& N, THREADLOCAL& L) /* ThreadSafe */
{
  InterlockedAccumulate(&m_IsoValue, (DOUBLE)(N.Solution()*N.WeightContribution()));
}
//---------------------------------------------------------------------------------------------------
VOID LaplacianMatrixSolver::NodeIndexSetFunction::ProcessNode(Tree* pTree, TreeNode& N, THREADLOCAL& L) /* ThreadSafe */
{
	m_Index[L.PartitionId]++;
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
LaplacianMatrixSolver::LaplacianMatrixSolver()
{
  m_pTree = 0;
  m_IsoValue = 0;
}
//---------------------------------------------------------------------------------------------------
LaplacianMatrixSolver::~LaplacianMatrixSolver()
{
  m_pTree = 0;
}
//---------------------------------------------------------------------------------------------------
VOID LaplacianMatrixSolver::Init(Ptr<Tree> pTree)
{
  m_pTree = pTree;

  INT RingSize = 0;

  m_FDI.Initialize(pTree->Height()-1);
  m_FDI.FunctionData.InitializeDotTables(FunctionDataUsage::DOT | FunctionDataUsage::DDOT | FunctionDataUsage::D2DOT);
  m_FDI.FunctionData.InitializeValueTables(FunctionDataUsage::VALUE);

  m_RingSize.Resize(m_pTree->Height());
  for(USHORT i=pTree->MaximumDepth(); i<=pTree->MaximumDepth(); i--)
  {
    m_RingSize[i] = RingSize;
    RingSize = (RingSize / 2) + 3;
  }

  m_LPF.m_pFDI = &m_FDI;
  m_LMF.m_pFDI = &m_FDI;

  Pair<INT,INT>* Ranges = new Pair<INT,INT>[pTree->Height()];
  for(USHORT i=0; i<pTree->Height(); i++)
  {
    Ranges[i].First = m_RingSize[i] + 2;
    Ranges[i].Second = 3;
  }

  pTree->Initialise(Ranges, DataAccess::READ, DataAccess::READWRITE, DataAccess::NONE);
  delete[] Ranges;

}
//---------------------------------------------------------------------------------------------------
VOID LaplacianMatrixSolver::SetIndices(USHORT Depth)
{
	for (UINT i=0; i<m_pTree->GetThreadCount(); i++)
		m_NISF.m_Index[i] = 0;
  
  m_pTree->IterateNodesBrood(0, Depth, m_NISF);
  
	for (UINT i=0; i<m_pTree->GetThreadCount(); i++)
	{
		m_Laplacian[i].Resize(m_NISF.m_Index[i]);
		m_Diagonal[i].Resize(m_NISF.m_Index[i]);
	}
  
  m_LMF.m_pLaplacian = &m_Laplacian[0];
  m_LMF.m_pDiagonal = &m_Diagonal[0];

	for (UINT i=0; i<m_pTree->GetThreadCount(); i++)
		m_LMF.m_Index[i] = 0;
}
//---------------------------------------------------------------------------------------------------
VOID LaplacianMatrixSolver::ProjectSolution(USHORT Depth, INT RelativeSliceIndex)
{
  m_pTree->IterateNodesBrood(RelativeSliceIndex, Depth, m_LPF);
}
//---------------------------------------------------------------------------------------------------
VOID LaplacianMatrixSolver::GetLaplacianMatrix(USHORT Depth)
{
  DOUBLE Width = 1.0 / (1<<Depth);
  DOUBLE Dot[5];
  DOUBLE D2Dot[5];
  
  for(INT i=-2;i<=2;i++)
  {
    Dot[i+2] = m_FDI.FunctionData.DotProduct(0, Width, i*Width, Width);
    D2Dot[i+2] = m_FDI.FunctionData.D2DotProduct(0, Width, i*Width, Width);
  }

  for(INT i=0; i<5; i++)
    for(INT j=0; j<5; j++)
      for(INT k=0; k<5; k++)
        m_LMF.m_LaplacianValues[i][j][k] = Dot[i]*Dot[j]*D2Dot[k] + Dot[i]*Dot[k]*D2Dot[j] + Dot[j]*Dot[k]*D2Dot[i];

  m_pTree->IterateNodesBrood(0, Depth, m_LMF);
}
//---------------------------------------------------------------------------------------------------
DWORD LaplacianMatrixSolver::SolveLaplacianProc(SolveLaplacianInfo* pSolverInfo)
{
	pSolverInfo->pSolver->SolveLaplacian(pSolverInfo->Depth, pSolverInfo->SolverIterations, pSolverInfo->PartitionId);
	return 0;
}
//---------------------------------------------------------------------------------------------------
VOID LaplacianMatrixSolver::SolveLaplacian(USHORT Depth, UINT SolverIterations)
{
  #if defined(CONFIG_PROFILE)
  TimeAllFunctionCalls();
  #endif

	ThreadPool& Pool = m_pTree->GetThreadPool();
	for (UINT i=0; i<m_pTree->GetThreadCount(); i++)
	{
		SolveLaplacianInfo* pInfo = new SolveLaplacianInfo();
		pInfo->pSolver = this;
		pInfo->Depth = Depth;
		pInfo->SolverIterations = SolverIterations;
		pInfo->PartitionId = i;

    DWORD (WINAPI *pFunc)(SolveLaplacianInfo*) = &LaplacianMatrixSolver::SolveLaplacianProc;
		Pool.Enqueue((LPTHREAD_START_ROUTINE)pFunc, pInfo);
	}

	Pool.Wait();
}
//---------------------------------------------------------------------------------------------------
VOID LaplacianMatrixSolver::SolveLaplacian(USHORT Depth, UINT SolverIterations, UINT PartitionId)
{
  FLOAT* pValues = &m_pTree->GetNodeStream(Depth, 4).Get<PARTITION4>(0).Value;
  FLOAT* pSolution = &m_pTree->GetNodeStream(Depth, 5).Get<PARTITION5>(0).Solution;

	SparseMatrix<MATRIXREAL>::SolveJacobi(m_Laplacian[PartitionId], m_Diagonal[PartitionId], pValues, SolverIterations, pSolution, FALSE);
}
//---------------------------------------------------------------------------------------------------
VOID LaplacianMatrixSolver::ComputeIsoValue(USHORT Depth)
{
  m_IVF.m_IsoValue = 0.0;
  m_pTree->IterateNodesBrood(0, Depth, m_IVF);
  m_IsoValue += m_IVF.m_IsoValue;
}
//---------------------------------------------------------------------------------------------------
VOID LaplacianMatrixSolver::InitSolver()
{
  #if defined(CONFIG_PROFILE)
  TimeAllFunctionCalls();
  #endif

  for(USHORT Depth=0; Depth<m_pTree->Height(); Depth++)
  {
    ProjectSolution(Depth, 0);
    for(INT i=0; i<m_RingSize[Depth]; i++)
      SolverStep(Depth);
  }
}
//---------------------------------------------------------------------------------------------------
VOID LaplacianMatrixSolver::UpdateSolver(USHORT FromDepth)
{
  #if defined(CONFIG_PROFILE)
  TimeAllFunctionCalls();
  #endif

  for(USHORT Depth=FromDepth; Depth<m_pTree->Height(); Depth++)
    SolverStep(Depth);
}
//---------------------------------------------------------------------------------------------------
VOID LaplacianMatrixSolver::SolverStep(USHORT Depth)
{
  // Update the values by projecting the parents' solution
  ProjectSolution(Depth, 2);
  
  // Set the indices of the nodes of interest and allocate memory
  SetIndices(Depth);
  
  /* Build the linear system */
  GetLaplacianMatrix(Depth);
    
  /* Solve the linear system */
  SolveLaplacian(Depth, 10);
  
  /* Update iso-Value computation */
  ComputeIsoValue(Depth);
  
  // Advance the stream pointer
  m_pTree->AdvanceNodes(Depth, 2);
}
//---------------------------------------------------------------------------------------------------
VOID LaplacianMatrixSolver::FinalizeSolver()
{
  #if defined(CONFIG_PROFILE)
  TimeAllFunctionCalls();
  #endif

  m_pTree->Finalise();
  m_pTree->m_State.IsoValue = m_IsoValue / m_pTree->m_State.WeightSum;

}
//---------------------------------------------------------------------------------------------------
