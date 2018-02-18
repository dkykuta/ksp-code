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
#include "path.hpp"
#include "dijkstra.hpp"
#include <queue>
#include <vector>
#include <functional>
#include <utility>
#include <iostream>
#include <set>

#ifdef HRK_COUNT_
extern int hrk_dijkstra_count;
#endif

namespace haruki
{
namespace dijkstra
{
typedef std::pair<double, int> pairVertCost;

Path buildPathFromParents(Graph &g, std::vector<int> parents, int s, int t);

Path minPath(Graph &g, int s, int t)
{
  std::vector<int> parents;
  std::vector<double> distances;

#ifdef HRK_COUNT_
  hrk_dijkstra_count++;
#endif

  dijkstra_parents(g, s, t, true, parents, distances);
  return buildPathFromParents(g, parents, s, t);
}

void dijkstra_parents(Graph &g, int s, int t, bool stopFound, std::vector<int>& parents, std::vector<double>& distances)
{
  std::vector<int> frj(g.getNumVert(), -1);

  std::set< std::pair<double, int> > pq;

  parents.resize(g.getNumVert(), -1);
  distances.resize(g.getNumVert(), -1);


  parents[s] = s;
  distances[s] = 0.0;
  EdgeOut::range edges = g.getEdgesOut(s);
  for (EdgeOut::iterator it = edges.begin(); it != edges.end(); ++it) {
    int v = (*it).head;
    distances[v] = (*it).cost;
    pq.emplace(distances[v], v);
    frj[v] = s;
  }

  while (!pq.empty())
  {
    std::set<std::pair<double, int> >::iterator it = pq.begin();
    std::pair<double, int> elem = *it;
    pq.erase(it);
    int w = elem.second;
    if (parents[w] != -1)
    {
      continue;
    }
    parents[w] = frj[w];

    if (stopFound && w == t) {
      break;
    }

    EdgeOut::range edges2 = g.getEdgesOut(w);
    for (EdgeOut::iterator it = edges2.begin(); it != edges2.end(); ++it) {
      int v = (*it).head;
      if (parents[v] != -1)
      {
        continue;
      }
      double newdist = distances[w] + (*it).cost;
      if (frj[v] == -1 || newdist < distances[v])
      {
        auto x = pq.find(std::pair<double, int>(distances[v], v));
        if (x != pq.end()) {
          pq.erase(x);
        }
        distances[v] = newdist;
        pq.emplace(distances[v], v);
        frj[v] = w;
      }
    }
  }
}

Path buildPathFromParents(Graph &g, std::vector<int> parents, int s, int t) {
  if (s == t) {
    return Path();
  }
  if (parents[t] == -1) {
    return Path();
  }
  Path p = buildPathFromParents(g, parents, s, parents[t]);
  p.addEdge(parents[t], t, g.getEdgeCost(parents[t], t));
  return p;
}
}
}
