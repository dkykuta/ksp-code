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
#include <set>
#include "path.hpp"
#include "graph.hpp"
#include "pascoalksp.hpp"
#include <chrono>

namespace haruki
{

class FengKSP : public PascoalKSP
{
protected:
  std::vector<std::vector<int> > dag_paths_upstream_;
  std::vector<int> colors_;
  Graph *yellowGraph_;

  virtual std::set<Path> generateCandidates(Graph& g, int t, std::vector<Path> &R, Path &path);
  virtual Path generateCandidateAtEdge(Graph &h, int t, std::vector<Path> &R, Path &path, int j);
  virtual void fixColorsNewDeviationVertex(Graph &g, int t, std::vector<Path> &R, Path &path, int deviationVertex);
  virtual void initializeYellowGraph(Graph &g);

public:
  virtual void preproc(Graph &g, int s, int t, int k);
  virtual std::vector<Path> ksp(Graph &g, int s, int t, int k);
  virtual std::vector<Path> posproc(Graph &g, int s, int t, int k, std::vector<Path> response);
};
}
