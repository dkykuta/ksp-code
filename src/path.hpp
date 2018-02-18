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
#include <stdexcept>
#include "graphaux.hpp"

namespace haruki
{

class Path
{
private:
  std::vector<int> vertList_;
  std::vector<double> edgesCostList_;
  int numVert_;
  double totalCost_;

public:
  Path() : numVert_(0), totalCost_(0) {}
  Path(int initialVertex) : numVert_(1), totalCost_(0) { vertList_.push_back(initialVertex); }
  const std::vector<int> &getVertList() const { return vertList_; };
  void addEdge(std::pair<int, int> edge, double edgeCost);
  void addEdge(int tail, int head, double edgeCost);
  void addEdge(const EdgeInfo& edgeInfo);
  std::pair<int, int> getEdge(int i);
  EdgeInfo getEdgeInfo(int i);
  double getEdgeCost(int i);

  Path subpath(int i, int j);

  int size() const { return numVert_; }
  double cost() const { return totalCost_; }
  bool operator<(const haruki::Path &rhs) const;
  bool operator>(const haruki::Path &rhs) const;
  bool operator==(const haruki::Path &rhs) const;
  Path operator+(const haruki::Path &rhs) const;
};
}
