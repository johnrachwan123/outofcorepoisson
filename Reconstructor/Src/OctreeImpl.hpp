/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/OctreeImpl.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 881 $
 * Last Updated: $Date: 2008-11-17 10:09:37 -0800 (Mon, 17 Nov 2008) $
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

//---------------------------------------------------------------------------------------------------
template<class AllocatingClass>
OctNode* OctNode::GetOrCreateChild(USHORT i, AllocatingClass* ac)
{
  //UINT LockNumber = 0;


  /*
  if (m_pParent != 0)
    LockNumber = GetChildIndex();

  INT Depth, Offset[3];
  GetDepthAndOffset(Depth, Offset);

  if (Depth > 3)
  {
    LockNumber = (Offset[0] >> (Depth-3))
               | ((Offset[1] >> (Depth-3) << 3))
               | ((Offset[2] >> (Depth-3) << 6));
  }
  else if (Depth > 2)
  {
    LockNumber = (Offset[0] >> (Depth-2))
               | ((Offset[1] >> (Depth-2) << 2))
               | ((Offset[2] >> (Depth-2) << 4));
  }
  else
  {
    LockNumber = (Offset[0] >> (Depth-1))
               | ((Offset[1] >> (Depth-1) << 1))
               | ((Offset[2] >> (Depth-1) << 2));
  }

  OctNode* pChild = 0;
  m_Lock[LockNumber].Lock();
  if (m_pChildren == 0)
    CreateChildren(ac);
  pChild = GetChild(i);
  m_Lock[LockNumber].Unlock();

  */

  if (m_pChildren == 0)
  {
    OctNode* pChild = CreateChildren(ac);
    InterlockedCompareExchange64((LONGLONG*)&m_pChildren, (LONGLONG)pChild, 0);
  }
  return GetChild(i);
}
//---------------------------------------------------------------------------------------------------
template<class AllocatingClass>
OctNode* OctNode::CreateChildren(AllocatingClass* ac)
{
  INT i,j,k;

  OctNode* pChildren = ac->GetAllocator(this, ThreadPool::GetThreadId()).New(8);

  INT d;
  INT off[3];
  GetDepthAndOffset(d,off);

  for(i=0;i<2;i++)
  {
    for(j=0;j<2;j++)
    {
      for(k=0;k<2;k++)
      {
        SIZE_T idx = Cube::CornerIndex(i,j,k);
        pChildren[idx].m_pParent = this;
        pChildren[idx].m_pChildren = 0;

        SIZE_T off2[3];
        off2[0] = (off[0] << 1) + i;
        off2[1] = (off[1] << 1) + j;
        off2[2] = (off[2] << 1) + k;

        Index(d+1, off2, pChildren[idx].m_Depth, pChildren[idx].m_Offset);

      }
    }
  }

  return pChildren;
}
//---------------------------------------------------------------------------------------------------
template<class AllocatingClass>
OctNode::Neighbors& OctNode::NeighborCache::InternalCreateNeighbors(OctNode* node, AllocatingClass* ac)
{
  INT d=node->Depth();
  if(node!=m_pNeighbors[d].m_pNeighbors[1][1][1])
  {
    m_pNeighbors[d].Clear();

    if(!node->m_pParent){m_pNeighbors[d].m_pNeighbors[1][1][1]=node;}
    else{
      Neighbors& temp = InternalCreateNeighbors(node->m_pParent,ac);
      INT i,j,k;
      UINT x1,y1,z1;
      UINT x2,y2,z2;
      INT idx=INT(node-node->m_pParent->m_pChildren);
      Cube::FactorCornerIndex( idx   ,x1,y1,z1);
      Cube::FactorCornerIndex((~idx)&7,x2,y2,z2);
      for(i=0;i<2;i++)
        for(j=0;j<2;j++)
          for(k=0;k<2;k++)
            m_pNeighbors[d].m_pNeighbors[x2+i][y2+j][z2+k] = &node->m_pParent->m_pChildren[Cube::CornerIndex(i,j,k)];

      // Set the m_pNeighbors from across the faces
      i = x1 << 1;
      if(temp.m_pNeighbors[i][1][1])
      {
        temp.m_pNeighbors[i][1][1]->GetOrCreateChild(0, ac);
        for(j=0;j<2;j++)
          for(k=0;k<2;k++)
            m_pNeighbors[d].m_pNeighbors[i][y2+j][z2+k] = &temp.m_pNeighbors[i][1][1]->m_pChildren[Cube::CornerIndex(x2,j,k)];
      }
      j = y1 << 1;
      if(temp.m_pNeighbors[1][j][1])
      {
        temp.m_pNeighbors[1][j][1]->GetOrCreateChild(0, ac);
        for(i=0;i<2;i++)
          for(k=0;k<2;k++)
            m_pNeighbors[d].m_pNeighbors[x2+i][j][z2+k] = &temp.m_pNeighbors[1][j][1]->m_pChildren[Cube::CornerIndex(i,y2,k)];
      }
      k = z1 << 1;
      if(temp.m_pNeighbors[1][1][k])
      {
        temp.m_pNeighbors[1][1][k]->GetOrCreateChild(0, ac);
        for(i=0;i<2;i++)
          for(j=0;j<2;j++)
            m_pNeighbors[d].m_pNeighbors[x2+i][y2+j][k] = &temp.m_pNeighbors[1][1][k]->m_pChildren[Cube::CornerIndex(i,j,z2)];
      }

      // Set the m_pNeighbors from across the edges
      i=x1<<1;	j=y1<<1;
      if(temp.m_pNeighbors[i][j][1])
      {
        temp.m_pNeighbors[i][j][1]->GetOrCreateChild(0, ac);
        for(k=0;k<2;k++)
          m_pNeighbors[d].m_pNeighbors[i][j][z2+k] = &temp.m_pNeighbors[i][j][1]->m_pChildren[Cube::CornerIndex(x2,y2,k)];
      }
      i=x1<<1;	k=z1<<1;
      if(temp.m_pNeighbors[i][1][k])
      {
        temp.m_pNeighbors[i][1][k]->GetOrCreateChild(0, ac);
        for(j=0;j<2;j++)
          m_pNeighbors[d].m_pNeighbors[i][y2+j][k] = &temp.m_pNeighbors[i][1][k]->m_pChildren[Cube::CornerIndex(x2,j,z2)];
      }
      j=y1<<1;	k=z1<<1;
      if(temp.m_pNeighbors[1][j][k])
      {
        temp.m_pNeighbors[1][j][k]->GetOrCreateChild(0, ac);
        for(i=0;i<2;i++)
          m_pNeighbors[d].m_pNeighbors[x2+i][j][k] = &temp.m_pNeighbors[1][j][k]->m_pChildren[Cube::CornerIndex(i,y2,z2)];
      }

      // Set the neighbor from across the corner
      i=x1<<1;	j=y1<<1;	k=z1<<1;
      if(temp.m_pNeighbors[i][j][k])
      {
        temp.m_pNeighbors[i][j][k]->GetOrCreateChild(0, ac);
        m_pNeighbors[d].m_pNeighbors[i][j][k] = &temp.m_pNeighbors[i][j][k]->m_pChildren[Cube::CornerIndex(x2,y2,z2)];
      }
    }
  }
  return m_pNeighbors[d];
}
//---------------------------------------------------------------------------------------------------
template<class AllocatingClass>
OctNode::Neighbors& OctNode::NeighborCache::CreateNeighbors(OctNode* node, AllocatingClass* ac)
{
  SIZE_T d = node->Depth();
  if(node != m_pNeighbors[d].m_pNeighbors[1][1][1])
    for(SIZE_T i=d; i<=m_Depth; i++)
      m_pNeighbors[d].Clear();

  return InternalCreateNeighbors(node,ac);
}
//---------------------------------------------------------------------------------------------------
template<class AllocatingClass>
OctNode::Neighbors2& OctNode::NeighborCache2::CreateNeighbors(OctNode* node, AllocatingClass* ac)
{
  INT d=node->Depth();
  if(node != m_pNeighbors[d].m_pNeighbors[2][2][2])
    for(INT i=d; i<=m_Depth; i++)
      m_pNeighbors[d].Clear();
  return InternalCreateNeighbors(node, ac);
}
//---------------------------------------------------------------------------------------------------
template<class AllocatingClass>
typename OctNode::Neighbors2& OctNode::NeighborCache2::InternalCreateNeighbors(OctNode* node, AllocatingClass* ac)
{
  INT d=node->Depth();
  if(node!=m_pNeighbors[d].m_pNeighbors[2][2][2]){
    m_pNeighbors[d].Clear();

    if(!node->m_pParent){m_pNeighbors[d].m_pNeighbors[2][2][2]=node;}
    else{
      Neighbors2& temp = _setNeighbors(node->m_pParent,ac);
      UINT x1,y1,z1,x2,y2,z2;
      INT ii,jj,kk;
      UINT idx = (UINT)(node - node->m_pParent->m_pChildren);

      Cube::FactorCornerIndex( idx   ,x1,y1,z1);
      Cube::FactorCornerIndex((~idx)&7,x2,y2,z2);

      for(INT i=0;i<5;i++)
      {
        x2=i+x1;
        ii=x2&1;
        x2=1+(x2>>1);
        for(INT j=0;j<5;j++)
        {
          y2=j+y1;
          jj=y2&1;
          y2=1+(y2>>1);
          for(INT k=0;k<5;k++)
          {
            z2=k+z1;
            kk=z2&1;
            z2=1+(z2>>1);
            if(temp.m_pNeighbors[x2][y2][z2])
            {
              if(!temp.m_pNeighbors[x2][y2][z2]->m_pChildren)
                temp.m_pNeighbors[x2][y2][z2]->CreateChildren(ac);
              m_pNeighbors[d].m_pNeighbors[i][j][k]=
                &temp.m_pNeighbors[x2][y2][z2]->m_pChildren[Cube::CornerIndex(ii,jj,kk)];
            }
          }
        }
      }
    }
  }
  return m_pNeighbors[node->Depth()];
}
//---------------------------------------------------------------------------------------------------

