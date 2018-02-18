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

namespace haruki
{

class EdgeInfo
{
public:
  int tail;
  int head;
  double cost;
  EdgeInfo(int t, int h, double c)
  {
    tail = t;
    head = h;
    cost = c;
  }

  int compare(const haruki::EdgeInfo &rhs) const {
    if (tail == rhs.tail) {
      // if (head == rhs.head) {
      //   double diff = rhs.cost - cost;
      //   if (diff < 0) {
      //     return -1;
      //   } else if (diff > 0) {
      //     return 1;
      //   }
      //   return 0;
      // }
      return rhs.head - head;
    }
    return rhs.tail - tail;
  }

  bool operator<(const haruki::EdgeInfo &rhs) const {
    return compare(rhs) < 0;
  }
  bool operator>(const haruki::EdgeInfo &rhs) const {
    return compare(rhs) > 0;
  }
  bool operator==(const haruki::EdgeInfo &rhs) const {
    return compare(rhs) == 0;
  }
};

class GraphBuilder
{
private:
  int numVert_ = 0;
  std::vector<EdgeInfo> edgeInfoList_;

public:
  void setNumVert(int numVert) { numVert_ = numVert; edgeInfoList_.reserve(numVert); }
  void setNumEdges(int numEdges) { edgeInfoList_.reserve(numEdges); }
  int getNumVert() { return numVert_; }
  void addEdge(int tail, int head, double cost);
  void addEdge(const EdgeInfo& edgeInfo);
  std::vector<EdgeInfo> getEdgeInfoList() { return edgeInfoList_; }
};
}
