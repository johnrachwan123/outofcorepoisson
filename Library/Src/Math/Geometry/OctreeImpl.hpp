/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Geometry/OctreeImpl.hpp $
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




ImplementTemplate3RuntimeType(Bolitho::Math::Geometry,Octree,Tree);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class NodeData,class Allocator,class ThreadingModel>
Octree<NodeData,Allocator,ThreadingModel>::Octree(Vector3<FLOAT> ExtentMin, Vector3<FLOAT> ExtentMax) : m_ExtentMin(ExtentMin), m_ExtentMax(ExtentMax)
{
  CreateRoot();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class NodeData,class Allocator,class ThreadingModel>
TreeNode<NodeData>* Octree<NodeData,Allocator,ThreadingModel>::FindNode(Vector3<FLOAT> P)
{
  Vector3<FLOAT> Min = m_ExtentMin;
  Vector3<FLOAT> Max = m_ExtentMax;
  Vector3<FLOAT> Center = Min + (Max-Min) * 0.5;

  TreeNode<NodeData>* pNode = m_pRoot;

  while (pNode->HasChildren())
    pNode = pNode->GetChild(GetOctant(Min, Max, P));

  return pNode;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class NodeData,class Allocator,class ThreadingModel>
template<class NodeSplitFunction>
TreeNode<NodeData>* Octree<NodeData,Allocator,ThreadingModel>::CreateNode(Vector3<FLOAT> P)
{
  Vector3<FLOAT> Min = m_ExtentMin;
  Vector3<FLOAT> Max = m_ExtentMax;

  /* Traverse as far down the tree as possible */
  TreeNode<NodeData>* pNode = m_pRoot;
  while (pNode->HasChildren())
    pNode = pNode->GetChild(GetOctant(Min, Max, P));

  /* Decide if we need to split */
  while (NodeSplitFunction::NeedToSplit(pNode))
  {
    CreateChildren(pNode);
    NodeSplitFunction::Split(pNode, Min, Max);
    pNode = pNode->GetChild(GetOctant(Min, Max, P));
  }
  NodeSplitFunction::Splat(pNode, Min, Max, P);

  return pNode;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class NodeData,class Allocator,class ThreadingModel>
SIZE_T Octree<NodeData,Allocator,ThreadingModel>::GetOctant(Vector3<FLOAT>& Min, Vector3<FLOAT>& Max, Vector3<FLOAT>& P)
{
  Vector3<FLOAT> Center = Min + (Max-Min) * 0.5;
  SIZE_T Octant = 0;
  if (P[0] > Center[0])
  {
    Octant |= 0x1;
    Min[0] = Center[0];
  }
  else
  {
    Max[0] = Center[0];
  }
  if (P[1] > Center[1])
  {
    Octant |= 0x2;
    Min[1] = Center[1];
  }
  else
  {
    Max[1] = Center[1];
  }
  if (P[2] > Center[2])
  {
    Octant |= 0x4;
    Min[2] = Center[2];
  }
  else
  {
    Max[2] = Center[2];
  }

  return Octant;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class PointData>
BOOL OctreeSpatialIndexNode<N,PointData>::NeedToSplit(TreeNode<OctreeSpatialIndexNode>* pNode)
{
  return pNode->NodeData.Count >= N;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class PointData>
VOID OctreeSpatialIndexNode<N,PointData>::Split(TreeNode<OctreeSpatialIndexNode>* pSplit, CONST Vector3<FLOAT>& ExtentMin, CONST Vector3<FLOAT>& ExtentMax)
{
  for (SIZE_T i=0; i<pSplit->NodeData.Count; i++)
  {
    Vector3<FLOAT> Min = ExtentMin;
    Vector3<FLOAT> Max = ExtentMax;

    TreeNode<OctreeSpatialIndexNode>* pNode = pSplit->GetChild(Octree<NULL_T>::GetOctant(Min, Max, pSplit->NodeData.Point[i]));
    
    pNode->NodeData.Point[pNode->NodeData.Count] = pSplit->NodeData.Point[i];
    pNode->NodeData.Data[pNode->NodeData.Count] = pSplit->NodeData.Data[i];
    pNode->NodeData.Count++;
  }
  pSplit->NodeData.Count = 0;

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T N, class PointData>
VOID OctreeSpatialIndexNode<N,PointData>::Splat(TreeNode<OctreeSpatialIndexNode>* pNode,  CONST Vector3<FLOAT>& ExtentMin, CONST Vector3<FLOAT>& ExtentMax, CONST Vector3<FLOAT>& P)
{
  pNode->NodeData.Point[pNode->NodeData.Count++] = P;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T MaximumPointsPerNode, class PointData, class Allocator, class ThreadingModel>
OctreeSpatialIndex<MaximumPointsPerNode,PointData,Allocator,ThreadingModel>::OctreeSpatialIndex(Vector3<FLOAT> ExtentMin, Vector3<FLOAT> ExtentMax) : Octree(ExtentMin, ExtentMax)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T MaximumPointsPerNode, class PointData, class Allocator, class ThreadingModel>
VOID OctreeSpatialIndex<MaximumPointsPerNode,PointData,Allocator,ThreadingModel>::AddPoint(Vector3<FLOAT> P, PointData& D)
{
  TreeNode<OctreeSpatialIndexNode<MaximumPointsPerNode,PointData>>* pNode = CreateNode<OctreeSpatialIndexNode<MaximumPointsPerNode,PointData>>(P);
  pNode->NodeData.Data[pNode->NodeData.Count-1] = D;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T MaximumPointsPerNode, class PointData, class Allocator, class ThreadingModel>
BOOL OctreeSpatialIndex<MaximumPointsPerNode,PointData,Allocator,ThreadingModel>::FindClosest(Vector3<FLOAT> P, Vector3<FLOAT>& Point, PointData& Data, FLOAT MaxDistance)
{
  

}
