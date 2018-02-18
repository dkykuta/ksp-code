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
#include "path.hpp"
#include "graphaux.hpp"

namespace haruki {

  void Path::addEdge(std::pair<int, int> edge, double edgeCost) {
    addEdge(edge.first, edge.second, edgeCost);
  }

  void Path::addEdge(int tail, int head, double edgeCost) {
    if (numVert_ == 0) {
      vertList_.push_back(tail);
      numVert_++;
    }
    if (vertList_[numVert_ - 1] != tail) {
      throw std::invalid_argument( "received invalid edge (tail does not match last vertex in path)" );
    }
    vertList_.push_back(head);
    edgesCostList_.push_back(edgeCost);
    totalCost_ += edgeCost;
    numVert_++;
  }

  void Path::addEdge(const EdgeInfo& edgeInfo) {
    addEdge(edgeInfo.tail, edgeInfo.head, edgeInfo.cost);
  }

  Path Path::subpath(int i, int j) {
    if (i >= numVert_) {
      return Path(-1);
    }

    Path sp(vertList_[i]);
    int limit = j - 1;
    if (limit > numVert_) {
      limit = numVert_;
    }
    for (int k = i; k < limit; k++) {
      sp.addEdge(vertList_[k], vertList_[k+1], edgesCostList_[k]);
    }
    return sp;
  }

  std::pair<int, int> Path::getEdge(int i) {
    return std::make_pair(vertList_[i], vertList_[i + 1]);
  }

  EdgeInfo Path::getEdgeInfo(int i) {
    return EdgeInfo(vertList_[i], vertList_[i + 1], edgesCostList_[i]);
  }

  double Path::getEdgeCost(int i) {
    return edgesCostList_[i];
  }

  bool Path::operator<(const haruki::Path& rhs) const
  {
    if (totalCost_ != rhs.totalCost_) {
      return totalCost_ < rhs.totalCost_;
    } else if (numVert_ != rhs.numVert_) {
      return numVert_ < rhs.numVert_;
    }
    for (int i = 0; i < numVert_; i++) {
      if (vertList_[i] != rhs.vertList_[i]) {
        return vertList_[i] < rhs.vertList_[i];
      }
    }
    return false;
  }

  bool Path::operator>(const haruki::Path& rhs) const
  {
    if (totalCost_ != rhs.totalCost_) {
      return totalCost_ > rhs.totalCost_;
    } else if (numVert_ != rhs.numVert_) {
      return numVert_ > rhs.numVert_;
    }
    for (int i = 0; i < numVert_; i++) {
      if (vertList_[i] != rhs.vertList_[i]) {
        return vertList_[i] > rhs.vertList_[i];
      }
    }
    return false;
  }

  bool Path::operator==(const haruki::Path& rhs) const
  {
    if (totalCost_ != rhs.totalCost_) {
      return false;
    } else if (numVert_ != rhs.numVert_) {
      return false;
    }
    for (int i = 0; i < numVert_; i++) {
      if (vertList_[i] != rhs.vertList_[i]) {
        return false;
      }
    }
    return true;
  }

  Path Path::operator+(const haruki::Path& rhs) const
  {
    Path third(vertList_[0]);
    for (int i = 0; i < numVert_ - 1; i++) {
      third.addEdge(vertList_[i], vertList_[i + 1], edgesCostList_[i]);
    }
    for (int i = 0; i < rhs.numVert_ - 1; i++) {
      third.addEdge(rhs.vertList_[i], rhs.vertList_[i + 1], rhs.edgesCostList_[i]);
    }
    return third;
  }
}
