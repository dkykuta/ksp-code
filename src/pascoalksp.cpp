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
#include "pascoalksp.hpp"
#include "dijkstra.hpp"
#include "graph.hpp"
#include "path.hpp"
#include "candidateset.hpp"
#include <iostream>

#ifdef HRK_COUNT_
extern int hrk_pascoal_shortcuts;
extern int hrk_pascoal_fallback;
#endif

namespace haruki {

  void PascoalKSP::preproc(Graph &g, int s, int t, int k) {
    haruki::GraphBuilder pg;
    for (int i = 0; i < g.getNumVert(); i++) {
      EdgeOut::range edges = g.getEdgesOut(i);
      for (EdgeOut::iterator it = edges.begin(); it != edges.end(); ++it) {
        int j = (*it).head;
        pg.addEdge(j, i, g.getEdgeCost(i, j));
      }
    }
    haruki::Graph h(pg);

    std::vector<double> distances;
    haruki::dijkstra::dijkstra_parents(h, t, s, false, dag_paths_next_, distances);

    EdgeList::range allEdges = g.getAllEdges();
    for (EdgeList::iterator it = allEdges.begin(); it != allEdges.end(); ++it) {
      int i = (*it).tail;
      int j = (*it).head;
      double reducedCost = (*it).cost - distances[i] + distances[j];
      (*it).cost = reducedCost;
    }
  }

  std::vector<Path> PascoalKSP::ksp(Graph &g, int s, int t, int k) {
    std::vector<Path> retAux = YenKSP::ksp(g, s, t, k);
    return retAux;
  }

  std::vector<Path> PascoalKSP::posproc(Graph &g, int s, int t, int k, std::vector<Path> response) {
    std::vector<Path> ret;
    for (std::vector<Path>::iterator it = response.begin(); it != response.end(); it++) {
      Path p = fixCosts(g, *it);
      ret.push_back(p);
    }
    return ret;
  }

  Path PascoalKSP::fixCosts(const Graph &g, const Path &path) {
    Path p;
    std::vector<int> vertList = path.getVertList();
    std::vector<int>::iterator it2 = vertList.begin();
    int prev = *it2;
    it2++;
    for (; it2 != vertList.end(); it2++) {
      p.addEdge(prev, *it2, g.getEdgeCost(prev, *it2));
      prev = *it2;
    }
    return p;
  }

  Path PascoalKSP::generateCandidateAtEdge(Graph &h, int t, std::vector<Path> &R, Path &path, int j) {
    std::pair<int, int> edge = path.getEdge(j);
    std::pair<int, int> auxEdge;
    for (std::vector<Path>::iterator it = R.begin(); it != R.end(); it++) {
      Path r = *it;
      if (r.subpath(0, j+1) == path.subpath(0, j+1)) {
        auxEdge = r.getEdge(j);
        h.removeEdge(auxEdge.first, auxEdge.second);
      }
    }
    h.removeEdge(edge.first, edge.second);

    float minCostFound = -1;
    int headSelected = -1;

    EdgeOut::range edges = h.getEdgesOut(edge.first);
    for (EdgeOut::iterator it = edges.begin(); it != edges.end(); ++it) {
      int i = (*it).head;
      if (dag_paths_next_[i] != -1) {
        if (minCostFound == -1 || (*it).cost < minCostFound) {
          minCostFound = (*it).cost;
          headSelected = i;
        }
      }
    }

    if (minCostFound != -1) {
      Path selectedPath = path.subpath(0, j+1);
      selectedPath.addEdge(edge.first, headSelected, h.getEdgeCost(edge.first, headSelected));

      int prev = headSelected;
      int prnt = dag_paths_next_[headSelected];
      haruki::Path p;
      while (prev != t && prnt != -1) {
        selectedPath.addEdge(prev, prnt, 0);
        prev = prnt;
        prnt = dag_paths_next_[prnt];
      }

      std::vector<int> vertList = selectedPath.getVertList();

      std::set<int> st(vertList.begin(), vertList.end());

      if (st.size() == vertList.size()) {
#ifdef HRK_COUNT_
        hrk_pascoal_shortcuts++;
#endif
        return selectedPath;
      }
    }

#ifdef HRK_COUNT_
  hrk_pascoal_fallback++;
#endif
    return YenKSP::generateCandidateAtEdge(h, t, R, path, j);
  }
}
