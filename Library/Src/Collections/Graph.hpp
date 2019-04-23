/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Collections/Graph.hpp $
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




#pragma once


namespace Bolitho
{
  template<class VertexData = NULL_T, class EdgeData = NULL_T, class ThreadingModel=SingleThreadedModel>
  class Graph : public Object
  {
    DeclareRuntimeType();
  public:
    class Vertex;

		class Edge : public Object
    {
    public:
      Edge(Graph& G, SIZE_T Vertex0, SIZE_T Vertex1, CONST EdgeData& Data) :
          m_Graph(G), m_Data(Data), m_Vertex0(Vertex0), m_Vertex1(Vertex1)
      {}

    private:
      /* The graph the edge belongs to */
      Graph& m_Graph;
      
      /* Data associated with this edge */
      EdgeData m_Data;
      
      /* The vertices connected by this edge */
      SIZE_T m_Vertex0;
      SIZE_T m_Vertex1;
    };

		class Vertex : public Object
    {
    public:
      Vertex(Graph& G, CONST VertexData& Data) :
          m_Graph(G), m_Data(Data)
      {}

      SIZE_T AddEdge(SIZE_T To, CONST EdgeData& Data = EdgeData());
    private:
      /* The graph the vertex belongs to */
      Graph& m_Graph;
      
      /* Data associated with this vertex */
      VertexData m_Data;
      
      /* An index of all vertices adjacent with this vertex */
      Array<SIZE_T> m_Edges;
    };

    /* Adds a new vertex to the graph */
    SIZE_T AddVertex(CONST VertexData& Data = VertexData());
    /* Removes a vertex from the graph.  Also removes all edges connected to the vertex */
    VOID RemoveVertex(SIZE_T Vertex);
 
    /* Adds a new edge between two vertices */
    SIZE_T AddEdge(SIZE_T Vertex0, SIZE_T Vertex1, CONST EdgeData& Data = EdgeData());
    /* Removes an edge from the graph */
    VOID RemoveEdge(SIZE_T Edge);

    /* Access to Edges */
    CONST Edge& GetEdge(SIZE_T i) CONST;
    Edge& GetEdge(SIZE_T i);

    SIZE_T EdgeCount() CONST
    { return m_Edges.Length(); }

    /* Access the vertices */
    CONST Vertex& GetVertex(SIZE_T i) CONST;
    Vertex& GetVertex(SIZE_T i);

    CONST Vertex& operator()(SIZE_T i) CONST;
    Vertex& operator()(SIZE_T i);

    SIZE_T VertexCount() CONST
    { return m_Vertices.Length(); }


    /* Traversal algorithms */
    template<class VERTEXFUNCTION, class EDGEFUNCTION>
    VOID DepthFirst(SIZE_T FromVertex, VERTEXFUNCTION& N, EDGEFUNCTION& E);

    template<class VERTEXFUNCTION, class EDGEFUNCTION>
    VOID BreadthFirst(SIZE_T FromVertex, VERTEXFUNCTION& N, EDGEFUNCTION& E);




  private:
    /* The collection of vertices */
    Array<Vertex> m_Vertices;
    /* The collection of edges */
    Array<Edge> m_Edges;

  };

  #include "GraphImpl.hpp"

}