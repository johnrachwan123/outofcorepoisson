/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Geometry/MinimalAreaTriangulation.cpp $
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

#include "CodeLibrary.hpp"
#include "Utility/NumberTraits.hpp"
#include "Math/Geometry/MinimalAreaTriangulation.hpp"

using namespace Bolitho;
using namespace Bolitho::Math;
using namespace Bolitho::Math::Geometry;

#define INVALIDINDEX (SIZE_T)-1

VOID GetTriangulation(SIZE_T i, SIZE_T j, Array<Vector3<FLOAT> >& Vertices, Array<INDEXEDTRIANGLE>& Triangles, FLOAT* BestTriangulation, SIZE_T* MidPoint)
{
  INDEXEDTRIANGLE tIndex;
  SIZE_T eCount = Vertices.Length();
  SIZE_T ii = i;
  if(i < j)
    ii += eCount;
  if(j + 1 >= ii)
    return;
  ii = MidPoint[i*eCount+j];
  if(ii != INVALIDINDEX)
  {
    tIndex.Vertex[0] = (UINT)i;
    tIndex.Vertex[1] = (UINT)j;
    tIndex.Vertex[2] = (UINT)ii;
    Triangles.Add(tIndex);
    GetTriangulation(i, ii, Vertices, Triangles, BestTriangulation, MidPoint);
    GetTriangulation(ii, j, Vertices, Triangles, BestTriangulation, MidPoint);
  }
}

FLOAT GetArea(SIZE_T i, SIZE_T j, Array<Vector3<FLOAT> >& Vertices, FLOAT* BestTriangulation, SIZE_T* MidPoint)
{
  FLOAT a = FLT_MAX, temp;
  SIZE_T Count = Vertices.Length();
  SIZE_T idx = i*Count+j;
  SIZE_T ii=i;

  if(i < j)
  {
    ii += Count;
  }
  if(j + 1 >= ii)
  {
    BestTriangulation[idx]=0;
    return 0;
  }

  if(MidPoint[idx] != INVALIDINDEX)
    return BestTriangulation[idx];

  SIZE_T mid = INVALIDINDEX;
  for(SIZE_T r=j+1; r<ii; r++)
  {
    SIZE_T rr = r % Count;
    SIZE_T idx1 = i * Count + rr;
    SIZE_T idx2 = rr * Count + j;

    Vector3<FLOAT> p,p1,p2;
    p1 = Vertices[i] - Vertices[rr];
    p2 = Vertices[j] - Vertices[rr];
    p = p1.Cross(p2);
    temp = p.Length();

    if(MidPoint[idx1] != INVALIDINDEX)
    {
      temp += BestTriangulation[idx1];
      if(temp > a)
        continue;
      if(MidPoint[idx2] != INVALIDINDEX)
        temp += BestTriangulation[idx2];
      else
        temp += GetArea(rr, j, Vertices, BestTriangulation, MidPoint);
    }
    else
    {
      if(MidPoint[idx2] != INVALIDINDEX)
        temp += BestTriangulation[idx2];
      else
        temp += GetArea(rr, j, Vertices, BestTriangulation, MidPoint);
      if(temp > a)
        continue;
      temp += GetArea(i, rr, Vertices, BestTriangulation, MidPoint);
    }

    if(temp < a)
    {
      a = temp;
      mid = rr;
    }
  }

  BestTriangulation[idx] = a;
  MidPoint[idx] = mid;

  return a;
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------
Array<INDEXEDTRIANGLE> Bolitho::Math::Geometry::GetMinimalTriangulation(Array<Vector3<FLOAT> >& Vertices)
{
  Array<INDEXEDTRIANGLE> Triangles;
  if(Vertices.Length() == 3)
  {
    Triangles.Resize(1);
    Triangles[0].Vertex[0] = 0;
    Triangles[0].Vertex[1] = 1;
    Triangles[0].Vertex[2] = 2;
    return Triangles;
  }
  else if(Vertices.Length() == 4)
  {
    INDEXEDTRIANGLE PossibleTriangles[2][2];
    FLOAT Area[2];

    Area[0] = Area[1] = 0.0f;
    Triangles.Resize(2);

    PossibleTriangles[0][0].Vertex[0] = 0;
    PossibleTriangles[0][0].Vertex[1] = 1;
    PossibleTriangles[0][0].Vertex[2] = 2;
    PossibleTriangles[0][1].Vertex[0] = 2;
    PossibleTriangles[0][1].Vertex[1] = 3;
    PossibleTriangles[0][1].Vertex[2] = 0;

    PossibleTriangles[1][0].Vertex[0] = 0;
    PossibleTriangles[1][0].Vertex[1] = 1;
    PossibleTriangles[1][0].Vertex[2] = 3;
    PossibleTriangles[1][1].Vertex[0] = 3;
    PossibleTriangles[1][1].Vertex[1] = 1;
    PossibleTriangles[1][1].Vertex[2] = 2;

    Vector3<FLOAT> N, p1, p2;
    for(SIZE_T i=0; i<2; i++)
    {
      for(SIZE_T j=0; j<2; j++)
      {
        p1 = Vertices[PossibleTriangles[i][j].Vertex[1]] - Vertices[PossibleTriangles[i][j].Vertex[0]];
        p2 = Vertices[PossibleTriangles[i][j].Vertex[2]] - Vertices[PossibleTriangles[i][j].Vertex[0]];
        N = p1.Cross(p2);
        Area[i] += N.Length();
      }
    }

    if(Area[0] > Area[1])
    {
      Triangles[0] = PossibleTriangles[1][0];
      Triangles[1] = PossibleTriangles[1][1];
    }
    else
    {
      Triangles[0] = PossibleTriangles[0][0];
      Triangles[1] = PossibleTriangles[0][1];
    }
    return Triangles;
  }


  SIZE_T Count = Vertices.Length()*Vertices.Length();

  FLOAT* BestTriangulation = new FLOAT[Count];
  SIZE_T* MidPoint = new SIZE_T[Count];
  for(SIZE_T i=0; i<Count; i++)
  {
    BestTriangulation[i] = -1.0f;
    MidPoint[i] = INVALIDINDEX;
  }

  GetArea(0, 1, Vertices, BestTriangulation, MidPoint);
  GetTriangulation(0, 1, Vertices, Triangles, BestTriangulation, MidPoint);

  delete[] BestTriangulation;
  delete[] MidPoint;

  return Triangles;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

