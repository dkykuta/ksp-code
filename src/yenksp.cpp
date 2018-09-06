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
#include "yenksp.hpp"
#include "dijkstra.hpp"
#include "graph.hpp"
#include "path.hpp"
#include "candidateset.hpp"
#include "candidatepath.hpp"

#ifdef HRK_COUNT_
extern int hrk_response_path_size;
extern int hrk_response_count;
#endif

namespace haruki
{

void YenKSP::preproc(Graph &g, int s, int t, int k)
{
}

std::vector<Path> YenKSP::posproc(Graph &g, int s, int t, int k, std::vector<Path> &response)
{
  return response;
}

std::vector<Path> YenKSP::ksp(Graph &g, int s, int t, int k)
{
  int sizeR = 0;
  std::vector<Path> response;
  haruki::CandidateSet<haruki::CandidatePath> candidateSet(k, CandidatePath());

  haruki::Path minPath = haruki::dijkstra::minPath(g, s, t);

  candidateSet.addCandidate(haruki::CandidatePath(0, minPath));

  for (int iter = 0; !candidateSet.empty() && iter < k; iter++)
  {
    haruki::CandidatePath popped = candidateSet.popFirst();
    haruki::Path pPath = popped.path();
    if (sizeR < k - 1)
    {
      std::set<CandidatePath> candidates = generateCandidates(g, t, response, pPath, popped.deviationIndex());
      for (std::set<CandidatePath>::iterator it = candidates.begin(); it != candidates.end(); it++)
      {
        candidateSet.addCandidate(*it);
      }
    }
#ifdef HRK_COUNT_
    hrk_response_path_size += pPath.size();
    hrk_response_count++;
#endif
    response.push_back(pPath); 
    sizeR++;
  }
  return response;
}

std::set<CandidatePath> YenKSP::generateCandidates(Graph &g, int t, std::vector<Path> &R, Path &path, int devIdx) 
{
  std::set<CandidatePath> candidates;

  for (int j = devIdx; j < path.size() - 1; j++)
  {
    removeEdgesSharedPrefix(g, t, R, path, j);
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

Path YenKSP::generateCandidateAtEdge(Graph &h, int t, std::vector<Path> &R, Path &path, int j)
{
  int fromVertex = path.getEdgeInfo(j).tail;

  haruki::Path minPathAux = haruki::dijkstra::minPath(h, fromVertex, t);
  if (minPathAux.size() == 0)
  {
    return minPathAux;
  }
  haruki::Path cand = path.subpath(0, j + 1) + minPathAux;
  return cand;
}

void YenKSP::removeEdgesSharedPrefix(Graph &h, int t, std::vector<Path> &R, Path &path, int j) {
  std::pair<int, int> auxEdge;
  for (std::vector<Path>::iterator it = R.begin(); it != R.end(); it++)
  {
    Path r = *it;
    if (r.subpath(0, j + 1) == path.subpath(0, j + 1))
    {
      auxEdge = r.getEdge(j);
      h.removeEdge(auxEdge.first, auxEdge.second);
    }
  }
  auxEdge = path.getEdge(j);
  h.removeEdge(auxEdge.first, auxEdge.second);

  for (int i = 0; i < j; i++)
  {
    auxEdge = path.getEdge(i);

    EdgeOut::range edges = h.getEdgesOut(auxEdge.first);
    for (EdgeOut::iterator it = edges.begin(); it != edges.end(); ++it)
    {
      h.removeEdge((*it).tail, (*it).head);
    }
  }
}
}
