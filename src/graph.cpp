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
#include "graph.hpp"
#include <algorithm>
#include <numeric>

namespace haruki
{

bool compareEdgeInfo(EdgeInfo a, EdgeInfo b)
{
  if (a.tail != b.tail)
  {
    return a.tail < b.tail;
  }
  return a.head < b.head;
}

class reverseEdgeInfoComparator
{
private:
  std::vector<EdgeInfo> &edgeInfoList_;

public:
  reverseEdgeInfoComparator(std::vector<EdgeInfo> &edgeInfoList) : edgeInfoList_{edgeInfoList} {}
  bool operator()(int a, int b)
  {
    if (edgeInfoList_[a].head != edgeInfoList_[b].head)
    {
      return edgeInfoList_[a].head < edgeInfoList_[b].head;
    }
    return edgeInfoList_[a].tail < edgeInfoList_[b].tail;
  }
};

Graph::Graph(GraphBuilder pg)
{
  numVert_ = pg.getNumVert();

  edgeInfoList_ = pg.getEdgeInfoList();
  std::sort(edgeInfoList_.begin(), edgeInfoList_.end(), compareEdgeInfo);

  int i = 0;
  int j = 0;
  firstEdgeEachV_ = std::vector<int>(numVert_, 0);
  for (std::vector<EdgeInfo>::iterator it = edgeInfoList_.begin(); it != edgeInfoList_.end(); it++)
  {
    EdgeInfo ei = *it;
    while (i <= ei.tail)
    {
      firstEdgeEachV_[i++] = j;
    }
    j++;
  }
  while (i < numVert_)
  {
    firstEdgeEachV_[i++] = j;
  }
  numEdges_ = j;

  reverseTrace_ = std::vector<int>(numEdges_);
  std::iota(std::begin(reverseTrace_), std::end(reverseTrace_), 0);
  std::sort(std::begin(reverseTrace_), std::end(reverseTrace_), reverseEdgeInfoComparator(edgeInfoList_));

  i = 0;
  j = 0;
  firstEdgeReverseV_ = std::vector<int>(numVert_, 0);
  for (std::vector<int>::iterator it = std::begin(reverseTrace_); it != std::end(reverseTrace_); ++it)
  {
    EdgeInfo ei = edgeInfoList_[*it];
    while (i <= ei.head)
    {
      firstEdgeReverseV_[i++] = j;
    }
    j++;
  }
  while (i < numVert_)
  {
    firstEdgeReverseV_[i++] = j;
  }

  edgesRemoved_ = std::vector<bool>(numEdges_, false);
  // resetEdgesRemoved();
}

Graph::Graph(Graph &g)
{
  firstEdgeEachV_ = g.firstEdgeEachV_;
  firstEdgeReverseV_ = g.firstEdgeReverseV_;
  reverseTrace_ = g.reverseTrace_;
  edgeInfoList_ = g.edgeInfoList_;
  edgesRemoved_ = g.edgesRemoved_;
  numVert_ = g.numVert_;
  numEdges_ = g.numEdges_;
}

const std::vector<EdgeInfo> Graph::getEdgesByTail(int tail) const
{
  std::vector<EdgeInfo> edges;
  int endIdx = (tail + 1 < numVert_ ? firstEdgeEachV_[tail + 1] : numEdges_);
  for (int idx = firstEdgeEachV_[tail]; idx < endIdx; idx++)
  {
    if (!edgesRemoved_[idx])
    {
      edges.push_back(edgeInfoList_[idx]);
    }
  }

  return edges;
}

EdgeOut::range Graph::getEdgesOut(int v)
{
  return EdgeOut::range(edgeInfoList_, firstEdgeEachV_, edgesRemoved_, numVert_, numEdges_, v);
}

EdgeIn::range Graph::getEdgesIn(int v)
{
  return EdgeIn::range(edgeInfoList_, firstEdgeReverseV_, reverseTrace_, edgesRemoved_, numVert_, numEdges_, v);
}

EdgeList::range Graph::getAllEdges()
{
  return EdgeList::range(edgeInfoList_, edgesRemoved_);
}

const int Graph::getEdgeIndex(int tail, int head) const
{
  int startIdx = firstEdgeEachV_[tail];
  int endIdx = (tail + 1 < numVert_ ? firstEdgeEachV_[tail + 1] : numEdges_);

  int middle;
  while (startIdx < endIdx)
  {
    middle = (endIdx + startIdx) / 2;
    if (edgeInfoList_[middle].head == head)
    {
      return middle;
    }
    else if (edgeInfoList_[middle].head < head)
    {
      startIdx = middle + 1;
    }
    else
    {
      endIdx = middle;
    }
  }
  return -1;
}

const double Graph::getEdgeCost(int tail, int head) const
{
  int edgeIdx = getEdgeIndex(tail, head);
  if (edgeIdx != -1 && !edgesRemoved_[edgeIdx])
  {
    return edgeInfoList_[edgeIdx].cost;
  }
  return -1.0;
}

void Graph::removeEdge(int tail, int head)
{
  setRemovedEdgeFlag(tail, head, true);
}

void Graph::setRemovedEdgeFlag(int tail, int head, bool flag)
{
  int edgeIdx = getEdgeIndex(tail, head);
  if (edgeIdx != -1)
  {
    edgesRemoved_[edgeIdx] = flag;
  }
}

void Graph::setRemovedEdgeFlag(int edgeIndex, bool flag)
{
  edgesRemoved_[edgeIndex] = flag;
}

void Graph::removeEdges(std::vector<EdgeInfo> edges)
{
  for (auto it = edges.begin(); it != edges.end(); ++it)
  {
    removeEdge((*it).tail, (*it).head);
  }
}

const bool Graph::isRemoved(int edgeIdx) const
{
  return edgesRemoved_[edgeIdx];
}
 
const bool Graph::isRemoved(int tail, int head) const
{
  int edgeIdx = getEdgeIndex(tail, head);
  if (edgeIdx != -1)
  {
    return isRemoved(edgeIdx);
  }

  return true;
}

void Graph::setAllEdgesRemoved()
{
  setAllEdges(EDGE_DISABLED);
}

void Graph::resetEdgesRemoved()
{
  setAllEdges(EDGE_ENABLED);
}

void Graph::setAllEdges(bool flag)
{
  std::fill(edgesRemoved_.begin(), edgesRemoved_.end(), flag);
}

void Graph::setRemovedForIncomingEdges(int v, bool flag)
{
  int endIdx = (v + 1 < numVert_ ? firstEdgeReverseV_[v + 1] : numEdges_);
  for (int idx = firstEdgeReverseV_[v]; idx < endIdx; idx++)
  {
    edgesRemoved_[reverseTrace_[idx]] = flag;
  }
}

void Graph::setRemovedForOutgoingEdges(int v, bool flag) {
  int endIdx = (v + 1 < numVert_ ? firstEdgeEachV_[v + 1] : numEdges_);
  for (int idx = firstEdgeEachV_[v]; idx < endIdx; idx++)
  {
    edgesRemoved_[idx] = flag;
  }
}

void Graph::removeVertex(int v) {
  setRemovedForOutgoingEdges(v, EDGE_DISABLED);
  setRemovedForIncomingEdges(v, EDGE_DISABLED);
}

void Graph::fengRemoveArtificialEdges(std::vector<int> &vertToRemove)
{
  // std::sort(vertToRemove.begin(), vertToRemove.end());

  for (auto it = vertToRemove.begin(); it != vertToRemove.end(); ++it)
  {
    setRemovedEdgeFlag(*it, numVert_ - 1, EDGE_DISABLED);
  }
}

void Graph::fengAddArtificialEdges(std::vector<int> &newExpressVertex)
{
  if (newExpressVertex.empty())
  {
    return;
  }
  for (auto it = newExpressVertex.begin(); it != newExpressVertex.end(); ++it)
  {
    setRemovedEdgeFlag(*it, numVert_ - 1, EDGE_ENABLED);
  }
}

void Graph::fengSetArtificialEdge(int fromVertex, bool flag) {
  int idx = -1;
  if (fromVertex < numVert_ -1) {
    idx = firstEdgeEachV_[fromVertex+1] -1;
  }
  else {
    idx = numEdges_ - 1;
  }
  setRemovedEdgeFlag(idx, flag);
}



/*
 * GraphBuilder
 */
void GraphBuilder::addEdge(int tail, int head, double cost)
{
  addEdge(EdgeInfo(tail, head, cost));
}

void GraphBuilder::addEdge(const EdgeInfo &edgeInfo)
{
  if (edgeInfo.head + 1 > numVert_)
  {
    numVert_ = edgeInfo.head + 1;
  }
  if (edgeInfo.tail + 1 > numVert_)
  {
    numVert_ = edgeInfo.tail + 1;
  }
  edgeInfoList_.push_back(edgeInfo);
}
}
