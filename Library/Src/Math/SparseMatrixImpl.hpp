/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/SparseMatrixImpl.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 1718 $
 * Last Updated: $Date: 2008-12-10 13:18:55 -0800 (Wed, 10 Dec 2008) $
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

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
SparseMatrix<T>::SparseMatrix()
{
  m_Rows = 0;
  m_RowSizes = 0;
  m_ppElements = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
SparseMatrix<T>::SparseMatrix(UINT Rows)
{
  m_Rows = 0;
  m_RowSizes = 0;
  m_ppElements = 0;
  Resize(Rows);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID SparseMatrix<T>::Write(LPCTSTR Filename)
{
  using namespace Bolitho::IO;

  Ptr<File> pFile = File::Open(Filename, FileMode::CREATE, FileAccess::WRITE);

  pFile->Write(&m_Rows, sizeof(m_Rows));
  for (UINT i=0; i<m_Rows; i++)
  {
    pFile->Write(&m_RowSizes[i], sizeof(m_RowSizes[i]));
    pFile->Write(m_ppElements[i], sizeof(MATRIXENTRY)*m_RowSizes[i]);
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID SparseMatrix<T>::Read(LPCTSTR Filename)
{
  using namespace Bolitho::IO;

  Ptr<File> pFile = File::Open(Filename, FileMode::OPEN, FileAccess::READ);

  UINT Rows = 0;
  pFile->Write(&Rows, sizeof(Rows));
  Resize(Rows);

  for (UINT i=0; i<m_Rows; i++)
  {
    pFile->Read(&m_RowSizes[i], sizeof(m_RowSizes[i]));
    m_ppElements[i] = new MATRIXENTRY[m_RowSizes[i]];
    pFile->Read(m_ppElements[i], sizeof(MATRIXENTRY)*m_RowSizes[i]);
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
SparseMatrix<T>::SparseMatrix(CONST SparseMatrix& M)
{
  Resize(M.m_Rows);
  for (UINT i=0; i<m_Rows; i++)
  {
    SetRowSize(i, M.m_RowSizes[i]);
    for(UINT j=0; j<m_RowSizes[i]; j++)
      m_ppElements[i][j] = M.m_ppElements[i][j];
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
UINT SparseMatrix<T>::Count()
{
  UINT Count = 0;
  for(UINT i=0; i<m_Rows; i++)
    Count += m_RowSizes[i];
  return Count;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
UINT SparseMatrix<T>::RowCount(UINT i)
{
  Assert(i < m_Rows);
  return m_RowSizes[i];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST typename SparseMatrix<T>::MATRIXENTRY* SparseMatrix<T>::GetRow(UINT i) CONST
{
  Assert(i < m_Rows);
  return m_ppElements[i];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
typename SparseMatrix<T>::MATRIXENTRY* SparseMatrix<T>::GetRow(UINT i)
{
  Assert(i < m_Rows);
  return m_ppElements[i];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
/*
template<class T>
SparseMatrix<T>& SparseMatrix<T>::operator = (CONST SparseMatrix<T>& M)
{
Resize(M.m_Rows);
for (SIZE_T i=0; i<m_Rows; i++)
{
SetRowSize(i,M.m_RowSizes[i]);
for (SIZE_T j=0; j<m_RowSizes[i]; j++)
m_ppElements[i][j] = M.m_ppElements[i][j];
}
return *this;
}
*/
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
SparseMatrix<T>::~SparseMatrix()
{
  Resize(0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID SparseMatrix<T>::Resize(UINT Rows)
{
  if(m_Rows > 0)
  {
    for(SIZE_T i=0; i<m_Rows; i++)
      if(m_RowSizes[i])
        delete[] m_ppElements[i];
    delete[] m_ppElements;
    delete[] m_RowSizes;
  }

  m_Rows = Rows;

  if(Rows > 0)
  {
    m_RowSizes = new UINT[Rows];
    m_ppElements = new MATRIXENTRY*[Rows];
    ZeroMemory(m_RowSizes, sizeof(UINT)*Rows); 
    ZeroMemory(m_ppElements, sizeof(MATRIXENTRY*)*Rows); 
  }

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID SparseMatrix<T>::SetRowSize(UINT Row, UINT Count)
{
  Assert(Row < m_Rows);

  if(m_RowSizes[Row])
    delete[] m_ppElements[Row];
  if(Count > 0)
    m_ppElements[Row] = new MATRIXENTRY[Count];
  m_RowSizes[Row] = Count;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
VOID SparseMatrix<T>::Multiply(CONST U* In, U* Out) CONST
{
  for (UINT i=0; i<m_Rows; i++)
  {
    U Temp = U();
    for(UINT j=0; j<m_RowSizes[i]; j++)
      Temp += m_ppElements[i][j].Value * In[m_ppElements[i][j].N];
    Out[i] = Temp;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class U>
struct MultiplyInfo
{
  UINT Offset;
  UINT Count;
  UINT* pRowSizes;
  typename SparseMatrix<T>::MATRIXENTRY** ppElements;
  CONST U* pIn;
  U* pOut;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class U>
DWORD MultiplyThreadProc(MultiplyInfo<T,U>* pInfo)
{
  for (UINT i=pInfo->Offset; i<pInfo->Offset + pInfo->Count; i++)
  {
    U Temp = U();
    for(UINT j=0; j<pInfo->pRowSizes[i]; j++)
      Temp += pInfo->ppElements[i][j].Value * pInfo->pIn[pInfo->ppElements[i][j].N];
    pInfo->pOut[i] = Temp;
  }

  delete pInfo;
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
/*
template<class T>
template<class U>
VOID SparseMatrix<T>::Multiply(System::ThreadPool& Pool, CONST U* In, U* Out) CONST
{
  UINT BlockCount = Pool.GetMaximumThreadCount();

  if (m_Rows > BlockCount * 64)
  {
    UINT CountPerBlock = m_Rows / BlockCount;
    UINT Offset = 0;

    while (Offset < m_Rows)
    {
      UINT Count = Min(CountPerBlock, m_Rows - Offset);

      MultiplyInfo<T,U>* pInfo = new MultiplyInfo<T,U>();
      pInfo->Offset = Offset;
      pInfo->Count = Count;
      pInfo->pRowSizes = m_RowSizes;
      pInfo->ppElements = m_ppElements;
      pInfo->pIn = &In[0];
      pInfo->pOut = &Out[0];

      DWORD (*pFunc)(MultiplyInfo<T,U>*) = &MultiplyThreadProc;
      Pool.Enqueue((LPTHREAD_START_ROUTINE)pFunc, pInfo);

      Offset += Count;
    }
    Pool.Wait();
  }
  else
  {
    Multiply(In, Out);
  }
}
*/
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
VOID SparseMatrix<T>::SolveJacobi(CONST SparseMatrix<T>& M, CONST Vector<typename SparseMatrix<U>::MATRIXENTRY>& Diagonal, CONST U* b, UINT SolverIterations, U* Solution, BOOL Reset)
{
  if (M.m_Rows == 0)
    return;

	Vector<U> Md;

  if(Reset)
  {
		Assert(FALSE);
    //Solution.Resize(b.Dimensions());
    //Solution.SetZero();
  }

  Md.Resize(M.m_Rows);
  for(UINT i=0; i<SolverIterations; i++)
  {
    M.Multiply(Solution, &Md[0]);
    for(UINT j=0; j<M.m_Rows; j++)
    {
      UINT Index = Diagonal[j].N;
      Solution[Index] = (b[Index] - Md[j]) / Diagonal[j].Value;
    }
  }

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
VOID SparseMatrix<T>::SolveJacobi(System::ThreadPool& Pool, CONST SparseMatrix<T>& M, CONST Vector<typename SparseMatrix<U>::MATRIXENTRY>& Diagonal, CONST U* b, UINT SolverIterations, U* Solution, BOOL Reset)
{
  if (M.m_Rows == 0)
    return;

  Vector<U> Md;
  
  if(Reset)
  {
    Assert(FALSE);
    //Solution.Resize(M.m_Rows);
    //Solution.SetZero();
  }

  Md.Resize(M.m_Rows);
  for(UINT i=0; i<SolverIterations; i++)
  {
    M.Multiply<U>(Pool, Solution, &Md[0]);
    

    for(UINT j=0; j<M.m_Rows; j++)
    {
      UINT Index = Diagonal[j].N;
      Solution[Index] = (b[Index] - Md[j]) / Diagonal[j].Value;
    }

  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
template<class U>
T SparseMatrix<T>::ComputeResidual(CONST SparseMatrix<T>& M, CONST Vector<typename SparseMatrix<U>::MATRIXENTRY>& Diagonal, CONST Vector<U>& b, Vector<U>& x)
{
  Vector<U> b1;
  b1.Resize(M.m_Rows);
  b1.SetZero();

  M.Multiply(x, b1);
  for (UINT i=0; i<M.m_Rows; i++)
    b1[i] += Diagonal[i].Value * x[Diagonal[i].N];

  b1 -= b;
  return b1.Norm(2);
}



/*
// Solve for x s.t. M(x)=b by solving for x s.t. M^tM(x)=M^t(b)
template<class T>
INT SparseMatrix<T>::Solve(CONST SparseMatrix<T>& M,CONST Vector<T>& b,INT iters,Vector<T>& solution,CONST T eps){
SparseMatrix mTranspose=M.Transpose();
Vector<T> bb=mTranspose*b;
Vector<T> d,r,Md;
T alpha,beta,rDotR;
INT i;

solution.Resize(M.Columns());
solution.SetZero();

d=r=bb;
rDotR=r.Dot(r);

for(i=0;i<iters && rDotR>eps;i++){
T temp;
Md=mTranspose*(M*d);
alpha=rDotR/d.Dot(Md);
solution+=d*alpha;
r-=Md*alpha;
temp=r.Dot(r);
beta=temp/rDotR;
rDotR=temp;
d=r+d*beta;
}
return i;
}
*/
//-----------------------------------------------------------------------------------------------------------------------------------------------------

/*
template<class T>
SparseMatrix<T> SparseMatrix<T>::operator * (CONST SparseMatrix<T>& M) CONST
{
return Multiply(M);
}
*/
//-----------------------------------------------------------------------------------------------------------------------------------------------------
/*
template<class T>
template<class T2>
Vector<T2> SparseMatrix<T>::operator * (CONST Vector<T2>& V) CONST
{
return Multiply(V);
}
*/
//-----------------------------------------------------------------------------------------------------------------------------------------------------
/*
template<class T>
SparseMatrix<T> SparseMatrix<T>::Transpose() CONST
{
SparseMatrix<T> M(this->Columns(), this->Rows());

for (INT i=0; i<this->Rows(); i++)
{
for(INT ii=0;ii<m_ppElements[i].size();ii++){
M(m_ppElements[i][ii].N,i) = m_ppElements[i][ii].Value;
}
}
return M;
}
*/
//-----------------------------------------------------------------------------------------------------------------------------------------------------
/*
template<class T>
template<class T2>
INT SparseMatrix<T>::SolveSymmetric(CONST SparseMatrix<T>& M,CONST Vector<T2>& b,INT iters,Vector<T2>& solution,CONST T2 eps,INT reset){
Vector<T2> d,r,Md;
T2 alpha,beta,rDotR;
Md.Resize(b.Dimensions());
if(reset){
solution.Resize(b.Dimensions());
solution.SetZero();
}
d=r=b-M.Multiply(solution);
rDotR=r.Dot(r);
if(b.Dot(b)<=eps){
solution.SetZero();
return 0;
}

INT i;
for(i=0;i<iters;i++){
T2 temp;
M.Multiply(d,Md);
temp=d.Dot(Md);
if(temp<=eps){break;}
alpha=rDotR/temp;
r.SubtractScaled(Md,alpha);
temp=r.Dot(r);
if(temp/b.Dot(b)<=eps){break;}
beta=temp/rDotR;
solution.AddScaled(d,alpha);
if(beta<=eps){break;}
rDotR=temp;
Vector<T2>::Add(d,beta,r,d);
}
return i;
}
*/

/*
template<class T>
SparseMatrix<T> SparseMatrix<T>::operator * (CONST T& V) CONST
{
SparseMatrix<T> M(*this);
M *= V;
return M;
}
*/
//-----------------------------------------------------------------------------------------------------------------------------------------------------
/*
template<class T>
SparseMatrix<T> SparseMatrix<T>::Multiply(CONST SparseMatrix<T>& M) CONST
{
SparseMatrix<T> R(this->Rows(), M.Columns());
for(INT i=0; i<R.Rows(); i++)
{
for(INT ii=0;ii<m_ppElements[i].size();ii++)
{
INT N=m_ppElements[i][ii].N;
T Value=m_ppElements[i][ii].Value;
for(INT jj=0;jj<M.m_ppElements[N].size();jj++)
{
R(i,M.m_ppElements[N][jj].N) += Value * M.m_ppElements[N][jj].Value;
}
}
}
return R;		
}
*/
//-----------------------------------------------------------------------------------------------------------------------------------------------------
/*
template<class T>
template<class T2>
Vector<T2> SparseMatrix<T>::Multiply(CONST Vector<T2>& V) CONST
{
Vector<T2> R(rows);

for (INT i=0; i<rows; i++)
{
T2 temp = T2();
for(INT ii=0;ii<rowSizes[i];ii++)
{
temp += m_ppElements[i][ii].Value * V.m_pV[m_ppElements[i][ii].N];
}
R(i) = temp;
}
return R;
}
*/
