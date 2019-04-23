/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Collections/TreeImpl.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1675 $
 * Last Updated: $Date: 2008-10-29 07:05:05 -0700 (Wed, 29 Oct 2008) $
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

#include "Math/Math.hpp"

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T Degree, class NodeData, class Allocator, class ThreadingModel>
CONST Bolitho::Type Tree<Degree,NodeData,Allocator,ThreadingModel>::RuntimeType = Bolitho::Type("Bolitho", "Tree", &Object::RuntimeType, 0,0);   
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T Degree, class NodeData, class Allocator, class ThreadingModel>
CONST Bolitho::Type* Tree<Degree,NodeData,Allocator,ThreadingModel>::GetType() CONST
{
  return &RuntimeType;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class NodeData>
TreeNode<NodeData>::TreeNode()
{
  m_pChildren = 0;
  m_pParent = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T Degree, class NodeData, class Allocator, class ThreadingModel>
SIZE_T Tree<Degree,NodeData,Allocator,ThreadingModel>::GetLevelNodeCount(SIZE_T Level)
{
  return Math::Pow(Degree, Level);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T Degree, class NodeData, class Allocator, class ThreadingModel>
SIZE_T Tree<Degree,NodeData,Allocator,ThreadingModel>::GetCumulativeNodeCount(SIZE_T Level)
{
  return GetLevelNodeCount(Level+1) - 1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T Degree, class NodeData, class Allocator, class ThreadingModel>
TreeNode<NodeData>* Tree<Degree,NodeData,Allocator,ThreadingModel>::CreateChildren(TreeNode<NodeData>* pParent)
{
  Assert(pParent != 0);
  Assert(pParent->m_pChildren == 0);

  pParent->m_pChildren = m_Allocator.New(Degree);

  for (SIZE_T i=0; i<Degree; i++)
    pParent->m_pChildren[i].m_pParent = pParent;
  m_NodeCount += Degree;

  return pParent->m_pChildren;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T Degree, class NodeData, class Allocator, class ThreadingModel>
VOID Tree<Degree,NodeData,Allocator,ThreadingModel>::FreeChildren(TreeNode<NodeData>* pParent)
{
  Assert(pParent != 0);

  if (pParent->m_pChildren)
  {
    for (SIZE_T i=0; i<Degree; i++)
      FreeChildren(&pParent->m_pChildren[i]);
    m_Allocator.Delete(pParent->m_pChildren, Degree);
    m_NodeCount -= Degree;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T Degree, class NodeData, class Allocator, class ThreadingModel>
Tree<Degree,NodeData,Allocator,ThreadingModel>::Tree()
{
  m_pRoot = 0;
  m_NodeCount = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T Degree, class NodeData, class Allocator, class ThreadingModel>
Tree<Degree,NodeData,Allocator,ThreadingModel>::~Tree()
{
  if (m_pRoot)
    FreeRoot();

  Assert(m_NodeCount == 0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T Degree, class NodeData, class Allocator, class ThreadingModel>
TreeNode<NodeData>* Tree<Degree,NodeData,Allocator,ThreadingModel>::CreateRoot()
{
  m_pRoot = m_Allocator.New();
  m_NodeCount++;
  return m_pRoot;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T Degree, class NodeData, class Allocator, class ThreadingModel>
VOID Tree<Degree,NodeData,Allocator,ThreadingModel>::FreeRoot()
{
  FreeChildren(m_pRoot);
  m_Allocator.Delete(m_pRoot);
  m_NodeCount--;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T Degree, class NodeData, class Allocator, class ThreadingModel>
template<class NODEFUNCTION>
VOID Tree<Degree,NodeData,Allocator,ThreadingModel>::DepthFirstTraversal(NODEFUNCTION& N)
{
  InternalDepthFirstTraversal(N, m_pRoot);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T Degree, class NodeData, class Allocator, class ThreadingModel>
template<class NODEFUNCTION>
VOID Tree<Degree,NodeData,Allocator,ThreadingModel>::BreadthFirstTraversal(NODEFUNCTION& N)
{
  InternalBreadthFirstTraversal(N, m_pRoot);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T Degree, class NodeData, class Allocator, class ThreadingModel>
template<class NODEFUNCTION>
VOID Tree<Degree,NodeData,Allocator,ThreadingModel>::InternalDepthFirstTraversal(NODEFUNCTION& N, TreeNode<NodeData>* pNode)
{
  Assert(pNode != 0);

  N(pNode);
  if (pNode->m_pChildren)
    for (SIZE_T i=0; i<Degree; i++)
      InternalDepthFirst(N, &pNode->m_pChildren[i]);

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<SIZE_T Degree, class NodeData, class Allocator, class ThreadingModel>
template<class NODEFUNCTION>
VOID Tree<Degree,NodeData,Allocator,ThreadingModel>::InternalBreadthFirstTraversal(NODEFUNCTION& N, TreeNode<NodeData>* pNode)
{
  Assert(pNode != 0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
