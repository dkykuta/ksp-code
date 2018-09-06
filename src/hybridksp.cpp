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
#include "hybridksp.hpp"
#include "dijkstra.hpp"
#include "graph.hpp"
#include "path.hpp"
#include "candidateset.hpp"
#include "candidatepath.hpp"
#include <queue>
#include <iostream>

#ifdef HRK_COUNT_
extern int hrk_feng_yellow_total_size;
extern int hrk_feng_yellow_graph_count;
extern int hrk_feng_yellow_path_size;
extern int hrk_feng_yellow_path_count;
#ifdef HRK_VERBOSE_
extern std::vector<int> hrk_feng_yellow_path_list;
#endif
#endif

namespace haruki
{

#define FENG_COLOR_RED 1
#define FENG_COLOR_YELLOW 2
#define FENG_COLOR_GREEN 3

void HybridKSP::preproc(Graph &g, int s, int t, int k)
{
  PascoalKSP::preproc(g, s, t, k);

  for (int i = 0; i < g.getNumVert(); i++)
  {
    dag_paths_upstream_.push_back(std::vector<int>());
  }

  for (int i = 0; i < g.getNumVert(); i++)
  {
    int v = dag_paths_next_[i];
    if (v != -1)
    {
      dag_paths_upstream_[v].push_back(i);
    }
  }
    initializeYellowGraph(g);
}

std::vector<Path> HybridKSP::ksp(Graph &g, int s, int t, int k)
{
  std::vector<Path> retAux = PascoalKSP::ksp(g, s, t, k);

  return retAux;
}

std::vector<Path> HybridKSP::posproc(Graph &g, int s, int t, int k, std::vector<Path> response)
{
  std::vector<Path> ret;
  for (std::vector<Path>::iterator it = response.begin(); it != response.end(); it++)
  {
    Path p = PascoalKSP::fixCosts(g, *it);
    ret.push_back(p);
  }
  delete yellowGraph_;
  return ret;
}

std::set<CandidatePath> HybridKSP::generateCandidates(Graph &g, int t, std::vector<Path> &R, Path &path, int oldDeviationIdx)
{
  std::set<CandidatePath> candidates;
  colors_ = std::vector<int>(g.getNumVert(), FENG_COLOR_GREEN);

  yellowGraph_->setAllEdgesRemoved();

  std::vector<int> newYellowList = initialColor(g, t, R, path, oldDeviationIdx);

  for (int j = oldDeviationIdx; j < path.size() - 1; j++)
  {
    removeEdgesSharedPrefix(g, t, R, path, j);

    std::vector<int> newYellowList2 = updateColor(g, t, R, path, j);

    haruki::Path cand = generateCandidateAtEdge(g, t, R, path, j);
    g.resetEdgesRemoved();
    if (cand.size() == 0)
    {
      continue;
    }
    candidates.insert(CandidatePath(j, cand));
  }

  return candidates;
}

Path HybridKSP::generateCandidateAtEdge(Graph &h, int t, std::vector<Path> &R, Path &path, int j)
{
  EdgeInfo auxEdge = path.getEdgeInfo(j);
  int deviationVertex = auxEdge.tail;
  int artificialEndVertex = h.getNumVert();

  double minCostFound = -1;
  int headSelected = -1;

  EdgeOut::range edges_out = h.getEdgesOut(deviationVertex);
  for (EdgeOut::iterator it = edges_out.begin(); it != edges_out.end(); ++it) {
    EdgeInfo ei = *it;
    if (minCostFound == -1 || ei.cost < minCostFound) {
      if (dag_paths_next_[ei.head] != -1) {
        headSelected = ei.head;
        minCostFound = ei.cost;
      }
    }
  }

  haruki::Path cand = path.subpath(0, j+1);
  std::pair<int, int> lastEdge;

  if (minCostFound != -1 && colors_[headSelected] == FENG_COLOR_GREEN) {
    /* estrategia de pascoal da certo, nao precisa do grafo amarelo */
    cand.addEdge(deviationVertex, headSelected, minCostFound);
    lastEdge = std::pair<int, int>(headSelected, headSelected);
  }
  else {
    std::vector<EdgeInfo> edgeInfoList = h.getEdgeInfoList();

    yellowGraph_->setAllEdgesRemoved();

    int idx = 0;
    for (std::vector<EdgeInfo>::iterator it = edgeInfoList.begin(); it != edgeInfoList.end(); ++it) {
      EdgeInfo ei = *it;
      if (! h.isRemoved(idx)) {
        if (colors_[ei.tail] == FENG_COLOR_YELLOW) {
          if (colors_[ei.head] == FENG_COLOR_YELLOW) {
            yellowGraph_->setRemovedEdgeFlag(idx + ei.tail, EDGE_ENABLED);
          }
          else if (colors_[ei.head] == FENG_COLOR_GREEN) {
            yellowGraph_->setRemovedEdgeFlag(idx + ei.tail, EDGE_ENABLED);
            yellowGraph_->fengSetArtificialEdge(ei.head, EDGE_ENABLED);
          }
        }
      }

      idx++;
    }

    haruki::Path minPathAux = haruki::dijkstra::minPath(*yellowGraph_, deviationVertex, artificialEndVertex);

    if (minPathAux.size() == 0) {
      return minPathAux;
    }

    for (int i = 0; i < minPathAux.size() - 2; i++) {
      cand.addEdge(minPathAux.getEdgeInfo(i));
    }

    lastEdge = minPathAux.getEdge(minPathAux.size() - 2);
  }

  int prev = lastEdge.first;
  int prnt = dag_paths_next_[lastEdge.first];
  while (prev != t && prnt != -1)
  {
    cand.addEdge(prev, prnt, 0);
    prev = prnt;
    prnt = dag_paths_next_[prnt];
  }

  // haruki::Path cand = path.subpath(0, j+1) + minPathAux;
return cand;
}

void HybridKSP::initializeYellowGraph(Graph &g)
{
  int numVert = g.getNumVert() + 1;
  int numEdges = g.getNumEdges() + g.getNumVert();

  int artificialEndVertex = g.getNumVert();

  std::vector<EdgeInfo> edgeInfoList;
  edgeInfoList.reserve(numEdges);
  std::vector<int> reverseTrace = g.getReverseTrace();
  std::vector<int> newFirstEdgeEachV = g.getFirstEdgeEachV();
  newFirstEdgeEachV.reserve(numVert);
  std::vector<int> newFirstEdgeReverseV = g.getFirstEdgeReverseV();
  newFirstEdgeReverseV.push_back(g.getNumEdges());

  for (int v = 0; v < g.getNumVert(); v++)
  {
    newFirstEdgeEachV[v] += v;
  }
  newFirstEdgeEachV.push_back(numEdges);

  std::vector<EdgeInfo> allEdges = g.getEdgeInfoList();

  for (unsigned int z = 0; z < reverseTrace.size(); z++) {
    reverseTrace[z] = reverseTrace[z] + allEdges[reverseTrace[z]].tail;
  }

  int i = 0;
  int eIdx = 0;
  for (std::vector<EdgeInfo>::iterator it = allEdges.begin(); it != allEdges.end(); ++it)
  {
    while (i < (*it).tail)
    {
      edgeInfoList.push_back(EdgeInfo(i, artificialEndVertex, 0));
      i++;
      reverseTrace.push_back(eIdx);
      eIdx++;
    }
    edgeInfoList.push_back(*it);
    eIdx++;
  }
  while (i < g.getNumVert())
  {
    edgeInfoList.push_back(EdgeInfo(i, artificialEndVertex, 0));
    i++;
    reverseTrace.push_back(eIdx);
    eIdx++;
  }

  yellowGraph_ = new Graph(newFirstEdgeEachV, newFirstEdgeReverseV,
                           reverseTrace, edgeInfoList, std::vector<bool>(numEdges, true), numVert, numEdges);
}

std::vector<int> HybridKSP::initialColor(Graph &g, int t, std::vector<Path> &R, Path &path, int oldDeviationIdx) {
  std::queue<int> vertexQueue;
  std::vector<int> newYellowList;

  auto vlist = path.getVertList();
  for (int i = 0; i < oldDeviationIdx; i++) {
    colors_[vlist[i]] = FENG_COLOR_RED;
    vertexQueue.push(vlist[i]);
  }
  colors_[vlist[oldDeviationIdx]] = FENG_COLOR_YELLOW;
  vertexQueue.push(vlist[oldDeviationIdx]);
  newYellowList.push_back(vlist[oldDeviationIdx]);
  while (!vertexQueue.empty()) {
    int v = vertexQueue.front();
    vertexQueue.pop();
    std::vector<int> upstream = dag_paths_upstream_[v];
    for (auto it = upstream.begin(); it != upstream.end(); ++it)
    {
      if (colors_[*it] == FENG_COLOR_GREEN) {
        colors_[*it] = FENG_COLOR_YELLOW;
        vertexQueue.push(*it);
        newYellowList.push_back(*it);
      }
    }
  }

  return newYellowList;
}

std::vector<int> HybridKSP::updateColor(Graph &g, int t, std::vector<Path> &R, Path &path, int deviationIdx) {
  std::queue<int> vertexQueue;
  std::vector<int> newYellowList;

  auto vlist = path.getVertList();
  if (deviationIdx > 0) {
    colors_[vlist[deviationIdx - 1]] = FENG_COLOR_RED;
  }
  colors_[vlist[deviationIdx]] = FENG_COLOR_YELLOW;
  vertexQueue.push(vlist[deviationIdx]);
  newYellowList.push_back(vlist[deviationIdx]);
  while (!vertexQueue.empty()) {
    int v = vertexQueue.front();
    vertexQueue.pop();
    std::vector<int> upstream = dag_paths_upstream_[v];
    for (auto it = upstream.begin(); it != upstream.end(); ++it)
    {
      if (colors_[*it] == FENG_COLOR_GREEN) {
        colors_[*it] = FENG_COLOR_YELLOW;
        vertexQueue.push(*it);
        newYellowList.push_back(*it);
      }
    }
  }

  return newYellowList;
}

void HybridKSP::updateArtificialEdges(Graph &g, std::vector<int> newYellowList, int oldDeviation, int deviation) {
  if (oldDeviation != -1) { 
    yellowGraph_->removeVertex(oldDeviation);
  }
  yellowGraph_->removeVertex(deviation);
  // int artificialEndVertex = g.getNumVert();
  for (auto it = newYellowList.begin(); it != newYellowList.end(); ++it) {
    EdgeIn::range edIn = g.getEdgesIn(*it);
    for (EdgeIn::iterator it2 = edIn.begin(); it2 != edIn.end(); ++it2)
    {
      EdgeInfo edge = *it2;
      if (colors_[edge.tail] == FENG_COLOR_YELLOW) {
        /* it could be an express edge before */
        // yellowGraph_->setRemovedEdgeFlag(edge.head, artificialEndVertex, EDGE_DISABLED);
        yellowGraph_->fengSetArtificialEdge(edge.head, EDGE_DISABLED);
        // yellowGraph_->setRemovedEdgeFlag(edge.tail, edge.head, EDGE_ENABLED);
        int idx = it2.getEdgeIdx() + edge.tail;
        yellowGraph_->setRemovedEdgeFlag(idx, EDGE_ENABLED);
      }
    }
    EdgeOut::range edOut = g.getEdgesOut(*it);
    for (EdgeOut::iterator it2 = edOut.begin(); it2 != edOut.end(); ++it2)
    {
      EdgeInfo edge = *it2;
      int edgeIdx = it2.getEdgeIdx() + edge.tail;
      if (colors_[edge.head] == FENG_COLOR_GREEN) {
        /* it is a new express edge */
        // yellowGraph_->setRemovedEdgeFlag(edge.head, artificialEndVertex, EDGE_ENABLED);
        yellowGraph_->fengSetArtificialEdge(edge.head, EDGE_ENABLED);
        // yellowGraph_->setRemovedEdgeFlag(edge.tail, edge.head, EDGE_ENABLED);
        yellowGraph_->setRemovedEdgeFlag(edgeIdx, EDGE_ENABLED);
      } else if (colors_[edge.head] == FENG_COLOR_YELLOW) {
        // yellowGraph_->setRemovedEdgeFlag(edge.tail, edge.head, EDGE_ENABLED);
        yellowGraph_->setRemovedEdgeFlag(edgeIdx, EDGE_ENABLED); 
      }
    }
  }
}

}
