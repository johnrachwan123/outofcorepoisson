/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/SparseMatrix.hpp $
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

#pragma once 

#include "Math/Vector.hpp"
#include "System/Threading/ThreadPool.hpp"

namespace Bolitho
{
  namespace Math
  {
    template<class T>
    class SparseMatrix
    {
    public:
      struct MATRIXENTRY
      {
        MATRIXENTRY()
        { N = MAXUINT; Value = 0; }
        MATRIXENTRY(UINT i)
        { N = i; Value = 0; }
        MATRIXENTRY(UINT i, T V)
        { N = i; Value = V; }

        UINT N;
        T Value;
      };

      SparseMatrix();
      SparseMatrix(UINT Rows);
      SparseMatrix(CONST SparseMatrix& M);
      ~SparseMatrix();

      VOID Resize(UINT Rows);
      VOID SetRowSize(UINT Row, UINT Count);

      UINT Count();
      UINT RowCount(UINT i);
      CONST MATRIXENTRY* GetRow(UINT i) CONST;
      MATRIXENTRY* GetRow(UINT i);

      template<class U>
      VOID Multiply(CONST U* In, U* Out) CONST;

      //template<class U>
      //VOID Multiply(System::ThreadPool& Pool, CONST U* In, U* Out) CONST;

      // This implementation of the Jacobi solver does not assume that the matrix is square:
      // The two vectors must be of the same size (equal to the range of entries in the MatrixEntry objects,
      // not M.rows)
      // The diagonal must be of the same size as M.rows and corresponds to the entries in solution that will
      // be updated.
      template<class U>
      static VOID SolveJacobi(CONST SparseMatrix<T>& M, CONST Vector<typename SparseMatrix<U>::MATRIXENTRY>& Diagonal, CONST U* b, UINT SolverIterations, U* Solution, BOOL ZeroSolutionVector = TRUE);

      template<class U>
      static VOID SolveJacobi(System::ThreadPool& Pool, CONST SparseMatrix<T>& M, CONST Vector<typename SparseMatrix<U>::MATRIXENTRY>& Diagonal, CONST U* b, UINT SolverIterations, U* Solution, BOOL ZeroSolutionVector = TRUE);

      template<class U>
      STATIC T ComputeResidual(CONST SparseMatrix<T>& M, CONST Vector<typename SparseMatrix<U>::MATRIXENTRY>& Diagonal, CONST Vector<U>& b, Vector<U>& x);

      VOID Write(LPCTSTR Filename);
      VOID Read(LPCTSTR Filename);
     private:
      UINT m_Rows;
      UINT* m_RowSizes;
      MATRIXENTRY** m_ppElements;

    };

    #include "SparseMatrixImpl.hpp"
  }
}

