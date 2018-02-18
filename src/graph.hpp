/*
 * Copyright (C) 2018 Diogo Haruki Kykuta
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
*/
#pragma once

#include <vector>
#include "graphaux.hpp"
#include "graphiterators.hpp"

namespace haruki
{

class Graph
{
private:
  std::vector<int> firstEdgeEachV_;
  std::vector<int> firstEdgeReverseV_;
  std::vector<int> reverseTrace_;
  std::vector<EdgeInfo> edgeInfoList_;
  std::vector<bool> edgesRemoved_;
  int numVert_;
  int numEdges_;

  const int getEdgeIndex(int tail, int head) const;

public:
  Graph(GraphBuilder pg);
  Graph(Graph& g);
  Graph(
      std::vector<int> firstEdgeEachV,
      std::vector<int> firstEdgeReverseV,
      std::vector<int> reverseTrace,
      std::vector<EdgeInfo> edgeInfoList,
      std::vector<bool> edgesRemoved,
      int numVert,
      int numEdges
  ): firstEdgeEachV_{firstEdgeEachV},
  firstEdgeReverseV_{firstEdgeReverseV},
  reverseTrace_{reverseTrace},
  edgeInfoList_{edgeInfoList},
  edgesRemoved_{edgesRemoved},
  numVert_{numVert},
  numEdges_{numEdges}
  {}


  const std::vector<EdgeInfo> getEdgesByTail(int tail) const;
  EdgeOut::range getEdgesOut(int v);
  EdgeIn::range getEdgesIn(int v);
  EdgeList::range getAllEdges();
  int getNumVert() const { return numVert_; }
  int getNumEdges() const { return numEdges_; }
  void removeEdge(int tail, int head);
  void setRemovedEdgeFlag(int tail, int head, bool flag);
  void removeEdges(std::vector<EdgeInfo> edges);
  void setAllEdgesRemoved();
  void setRemovedForIncomingEdges(int v, bool flag);
  const double getEdgeCost(int tail, int head) const;
  const bool isRemoved(int edgeIdx) const;
  void resetEdgesRemoved();

  /* special for feng */
  void fengRemoveArtificialEdges(std::vector<int> &vertToRemove);
  void fengAddArtificialEdges(std::vector<int> &newExpressVertices);
  void fengSetEdgeRemovedFlag(int edgeIndex, bool flag);
  std::vector<int> getReverseTrace() const {return reverseTrace_;}
  std::vector<int> getFirstEdgeEachV() const { return firstEdgeEachV_;}
  std::vector<int> getFirstEdgeReverseV() const { return firstEdgeReverseV_;}
  std::vector<EdgeInfo> getEdgeInfoList() {return edgeInfoList_;}
};
}
