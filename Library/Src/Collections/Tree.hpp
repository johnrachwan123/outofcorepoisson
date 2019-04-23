/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Collections/Tree.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1674 $
 * Last Updated: $Date: 2008-10-29 07:04:34 -0700 (Wed, 29 Oct 2008) $
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

namespace Bolitho
{
  
  template<class NodeData=NULL_T>
  class TreeNode;
  template<SIZE_T Degree, class NodeData=NULL_T, class Allocator=DefaultAllocator< TreeNode<NodeData> >, class ThreadingModel=SingleThreadedModel>
  class Tree;

  template<class NodeData>
  class TreeNode
  {
    template<SIZE_T Degree, class TreeNodeData, class Allocator, class ThreadingModel>
    friend class Tree;
  public:
    TreeNode();

    NodeData Data;

    TreeNode* GetChild(SIZE_T i)
    { return &m_pChildren[i]; }
    CONST TreeNode* GetChild(SIZE_T i) CONST
    { return &m_pChildren[i]; }

    TreeNode* GetParent()
    { return m_pParent; }
    CONST TreeNode* GetParent() CONST
    { return m_pParent; }

    BOOL HasChildren() CONST
    { return m_pChildren != 0; }

  protected:
    TreeNode* m_pChildren;
    TreeNode* m_pParent;
  };

  template<SIZE_T Degree, class NodeData, class Allocator, class ThreadingModel>
  class Tree : public Object
  {
    DeclareRuntimeType();
  public:

    Tree();
    virtual ~Tree();

    /* Creates the Root node of the tree */
    TreeNode<NodeData>* CreateRoot();

    /* Frees the Root node of the tree */
    VOID FreeRoot();

    /* Creates a brood of children for a given node */
    TreeNode<NodeData>* CreateChildren(TreeNode<NodeData>* pParent);
    
    /* Frees a nodes children */
    VOID FreeChildren(TreeNode<NodeData>* pParent);

    /* Access to the root of the tree */
    CONST TreeNode<NodeData>* Root() CONST
    { return m_pRoot; }
    TreeNode<NodeData>* Root()
    { return m_pRoot; }

    /* Traversal algorithms */
    template<class NODEFUNCTION>
    VOID DepthFirstTraversal(NODEFUNCTION& N);

    template<class NODEFUNCTION>
    VOID BreadthFirstTraversal(NODEFUNCTION& N);

    STATIC SIZE_T GetLevelNodeCount(SIZE_T Level);
    STATIC SIZE_T GetCumulativeNodeCount(SIZE_T Level);
    STATIC SIZE_T GetLinearOffset(SIZE_T Level, SIZE_T LevelOffset);
  protected:
    template<class NODEFUNCTION>
    VOID InternalDepthFirstTraversal(NODEFUNCTION& N, TreeNode<NodeData>* pNode);

    template<class NODEFUNCTION>
    VOID InternalBreadthFirstTraversal(NODEFUNCTION& N, TreeNode<NodeData>* pNode);

    TreeNode<NodeData>* m_pRoot;
    SIZE_T m_NodeCount;
    Allocator m_Allocator;

  };

  #include "TreeImpl.hpp"

}