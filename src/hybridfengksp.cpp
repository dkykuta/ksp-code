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
#include "hybridfengksp.hpp"
#include "dijkstra.hpp"
#include "graph.hpp"
#include "path.hpp"
#include "candidateset.hpp"
#include <queue>
#include <iostream>

namespace haruki
{

#define FENG_COLOR_RED 1
#define FENG_COLOR_YELLOW 2
#define FENG_COLOR_GREEN 3

void HybridFengKSP::preproc(Graph &g, int s, int t, int k)
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
}

std::vector<Path> HybridFengKSP::ksp(Graph &g, int s, int t, int k)
{
  std::vector<Path> retAux = PascoalKSP::ksp(g, s, t, k);
  return retAux;
}

std::vector<Path> HybridFengKSP::posproc(Graph &g, int s, int t, int k, std::vector<Path> response)
{
  std::vector<Path> ret;
  for (std::vector<Path>::iterator it = response.begin(); it != response.end(); it++)
  {
    Path p = PascoalKSP::fixCosts(g, *it);
    ret.push_back(p);
  }
  return ret;
}

std::set<Path> HybridFengKSP::generateCandidates(Graph &g, int t, std::vector<Path> &R, Path &path)
{
  std::set<Path> candidates;
  colors_ = std::vector<int>(g.getNumVert(), FENG_COLOR_GREEN);

  initializeYellowGraph(g);

  for (int j = 0; j < path.size() - 1; j++)
  {
    removeEdgesSharedPrefix(g, t, R, path, j);
    fixColorsNewDeviationVertex(g, t, R, path, j);
    haruki::Path cand = generateCandidateAtEdge(g, t, R, path, j);
    g.resetEdgesRemoved();
    if (cand.size() == 0)
    {
      continue;
    }
    candidates.insert(cand);
  }

  delete yellowGraph_;

  return candidates;
}

Path HybridFengKSP::generateCandidateAtEdge(Graph &h, int t, std::vector<Path> &R, Path &path, int j)
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
            yellowGraph_->fengSetEdgeRemovedFlag(idx + ei.tail, false);
          }
          else if (colors_[ei.head] == FENG_COLOR_GREEN) {
            yellowGraph_->fengSetEdgeRemovedFlag(idx + ei.tail, false);
            yellowGraph_->setRemovedEdgeFlag(ei.head, h.getNumVert(), false);
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

void HybridFengKSP::fixColorsNewDeviationVertex(Graph &g, int t, std::vector<Path> &R, Path &path, int deviationIdx)
{
  std::vector<int> pathVertList = path.getVertList();
  int devVertex = pathVertList[deviationIdx];
  int oldDevVertColor = colors_[devVertex];

  if (oldDevVertColor == FENG_COLOR_GREEN)
  {
    std::queue<int> vertexQueue;
    vertexQueue.push(devVertex);
    while (!vertexQueue.empty())
    {
      int v = vertexQueue.front();
      vertexQueue.pop();
      if (colors_[v] == FENG_COLOR_GREEN)
      {
        colors_[v] = FENG_COLOR_YELLOW;
        std::vector<int> upstream = dag_paths_upstream_[v];
        for (std::vector<int>::iterator it = upstream.begin(); it != upstream.end(); ++it)
        {
          vertexQueue.push(*it);
        }
      }
    }
  } /* else it is already yellow */

  if (deviationIdx > 0)
  {
    int oldDevVertex = pathVertList[deviationIdx - 1];
    colors_[oldDevVertex] = FENG_COLOR_RED;
  }
}

void HybridFengKSP::initializeYellowGraph(Graph &g) {
  int numVert = g.getNumVert() + 1;
  int numEdges = g.getNumEdges() + g.getNumVert();

  int artificialEndVertex = g.getNumVert();

  std::vector<EdgeInfo> edgeInfoList;
  std::vector<int> reverseTrace = g.getReverseTrace();
  std::vector<int> newFirstEdgeEachV = g.getFirstEdgeEachV();
  std::vector<int> newFirstEdgeReverseV = g.getFirstEdgeReverseV();
  newFirstEdgeReverseV.push_back(g.getNumEdges());

  for (int v = 0; v < g.getNumVert(); v++) {
    newFirstEdgeEachV[v] += v;
  }
  newFirstEdgeEachV.push_back(numEdges);

  // EdgeList::range allEdges = g.getAllEdges();
  std::vector<EdgeInfo> allEdges = g.getEdgeInfoList();

  int i = 0;
  int eIdx = 0;
  // for (EdgeList::iterator it = allEdges.begin(); it != allEdges.end(); ++it) {
  for (std::vector<EdgeInfo>::iterator it = allEdges.begin(); it != allEdges.end(); ++it) {
    while (i < (*it).tail) {
      edgeInfoList.push_back(EdgeInfo(i, artificialEndVertex, 0));
      i++;
      reverseTrace.push_back(eIdx);
      eIdx++;
    }
    edgeInfoList.push_back(*it);
    eIdx++;
  }
  while (i < g.getNumVert()) {
    edgeInfoList.push_back(EdgeInfo(i, artificialEndVertex, 0));
    i++;
    reverseTrace.push_back(eIdx);
    eIdx++;
  }

  yellowGraph_ = new Graph(newFirstEdgeEachV, newFirstEdgeReverseV,
    reverseTrace, edgeInfoList, std::vector<bool>(numEdges, true), numVert, numEdges);
  yellowGraph_->setRemovedForIncomingEdges(artificialEndVertex, false);
}

void HybridFengKSP::removeEdgesSharedPrefix(Graph &h, int t, std::vector<Path> &R, Path &path, int j) {
  std::set<EdgeInfo> edgesToRemove;

  for (std::vector<Path>::iterator it = R.begin(); it != R.end(); it++)
  {
    Path r = *it;
    if (r.subpath(0, j + 1) == path.subpath(0, j + 1))
    {
      edgesToRemove.insert(r.getEdgeInfo(j));
    }
  }

  EdgeInfo auxEdge = path.getEdgeInfo(j);

  edgesToRemove.insert(auxEdge);

  h.removeEdges(std::vector<EdgeInfo>(edgesToRemove.begin(), edgesToRemove.end()));
}
}
