/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/JHU/Research/StreamingSurfaceReconstruction/Trunk/Reconstructor/Src/BinaryNode.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Rev: 785 $
 * Last Updated: $Date: 2008-07-31 21:16:10 -0700 (Thu, 31 Jul 2008) $
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

#include "Collections/Tree.hpp"

#pragma once

template<class Real=FLOAT>
class BinaryNode
{
public:
  static inline INT Index(INT Depth, INT Offset)
  {
    return (1 << Depth) + Offset - 1;
  }
	
  static inline INT CornerIndex(INT MaxDepth, INT Depth, INT Offset, INT ForwardCorner)
  {
    return (Offset + ForwardCorner) << (MaxDepth - Depth);
  }
	
  static inline Real CornerIndexPosition(INT Index, INT MaxDepth)
  {
    return Real(Index) / (1 << MaxDepth);
  }
	
  static inline Real Width(INT Depth)
  {
    return Real(1.0 / (1 << Depth));
  }

	static inline void CenterAndWidth(INT Depth, INT Offset, Real& Center, Real& Width)
	{
		Width = Real(1.0 / (1 << Depth));
		Center = Real((0.5+Offset) * Width);
	}
	
  static inline void CenterAndWidth(INT Index, Real& Center, Real& Width)
	{
		INT Depth;
    INT Offset;
		DepthAndOffset(Index, Depth, Offset);
		CenterAndWidth(Depth, Offset, Center, Width);
	}

	static inline void DepthAndOffset(INT Index, INT& Depth, INT& Offset)
	{
		INT i = Index + 1;
		Depth = -1;
		while(i)
    {
			i >>= 1;
			Depth++;
		}
		Offset = (Index + 1) - (1 << Depth);
	}

};

