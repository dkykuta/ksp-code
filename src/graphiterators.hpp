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
#include "graphaux.hpp"

namespace haruki
{
  /* out */
  namespace EdgeOut {
    class iterator
    {
      std::vector<int> &firstEdgeEachV_;
      std::vector<EdgeInfo> &edgeInfoList_;
      std::vector<bool> &removed_;
      int numEdges_;
      int v_;
      int idx_;

    public:

      iterator(std::vector<int> &firstEdgeEachV, std::vector<EdgeInfo> &edgeInfoList, std::vector<bool> &removed, int numEdges, int idx)
          : firstEdgeEachV_{firstEdgeEachV}, edgeInfoList_{edgeInfoList}, removed_{removed}, numEdges_{numEdges}, idx_{idx}
      {
      }

      iterator(std::vector<EdgeInfo> &edgeInfoList, std::vector<int> &firstEdgeEachV, std::vector<bool> &removed, int numVert, int numEdges, int v)
          : firstEdgeEachV_{firstEdgeEachV}, edgeInfoList_{edgeInfoList}, removed_{removed}, numEdges_{numEdges}, v_{v}
      {
        idx_ = firstEdgeEachV[v];
        while (idx_ < numEdges_ && idx_ < firstEdgeEachV_[v_ + 1] && removed_[idx_]) {
          idx_++;
        }
        if (v < numVert - 1 && idx_ == firstEdgeEachV[v + 1])
        {
          idx_ = numEdges;
        }
      }

      EdgeInfo operator*() const { return edgeInfoList_[idx_]; }

      iterator &operator++()
      {
        idx_++;
        while (idx_ < numEdges_ && idx_ < firstEdgeEachV_[v_ + 1] && removed_[idx_]) {
          idx_++;
        }
        if (idx_ < numEdges_ && edgeInfoList_[idx_].tail != v_) {
          idx_ = numEdges_;
        }
        return *this;
      }

      bool operator!=(const iterator &o) const { return idx_ != o.idx_; }

      int getEdgeIdx() {
        return idx_;
      }
    };

    class range
    {
      std::vector<int> &firstEdgeEachV_;
      std::vector<EdgeInfo> &edgeInfoList_;
      std::vector<bool> &removed_;
      iterator begin_it_;
      int numEdges_;

    public:
      range(std::vector<EdgeInfo> &edgeInfoList, std::vector<int> &firstEdgeEachV, std::vector<bool> &removed, int numVert, int numEdges, int v)
          : firstEdgeEachV_{firstEdgeEachV}, edgeInfoList_{edgeInfoList}, removed_{removed}, begin_it_{iterator(edgeInfoList, firstEdgeEachV, removed, numVert, numEdges, v)}, numEdges_{numEdges} {}

      iterator begin() const { return begin_it_; }
      iterator end() const { return iterator(firstEdgeEachV_, edgeInfoList_, removed_, numEdges_, numEdges_); }
    };
  }

  /* in */
  namespace EdgeIn {
    class iterator
    {
      std::vector<int> &firstEdgeReverseV_;
      std::vector<int> &reverseTrace_;
      std::vector<EdgeInfo> &edgeInfoList_;
      std::vector<bool> &removed_;
      int numEdges_;
      int idx_;
      int v_;

    public:
      iterator(std::vector<int> &firstEdgeReverseV, std::vector<EdgeInfo> &edgeInfoList, std::vector<int> &reverseTrace, std::vector<bool> &removed, int numEdges, int idx)
          : firstEdgeReverseV_{firstEdgeReverseV}, reverseTrace_{reverseTrace}, edgeInfoList_{edgeInfoList}, removed_{removed}, numEdges_{numEdges}, idx_{idx}
      {
      }

      iterator(std::vector<EdgeInfo> &edgeInfoList, std::vector<int> &firstEdgeReverseV, std::vector<int> &reverseTrace, std::vector<bool> &removed, int numVert, int numEdges, int v)
          : firstEdgeReverseV_{firstEdgeReverseV}, reverseTrace_{reverseTrace}, edgeInfoList_{edgeInfoList}, removed_{removed}, numEdges_{numEdges}, v_{v}
      {
        idx_ = firstEdgeReverseV_[v];
        while (idx_ < numEdges_ && idx_ < firstEdgeReverseV_[v_ + 1] && removed_[reverseTrace_[idx_]]) {
          idx_++;
        }
        if (v < numVert - 1 && idx_ == firstEdgeReverseV_[v + 1])
        {
          idx_ = numEdges;
        }
      }

      EdgeInfo operator*() const { return edgeInfoList_[reverseTrace_[idx_]]; }

      iterator &operator++()
      {
        idx_++;
        while (idx_ < numEdges_ && idx_ < firstEdgeReverseV_[v_ + 1] && removed_[reverseTrace_[idx_]]) {
          idx_++;
        }
        if (idx_ < numEdges_ && edgeInfoList_[reverseTrace_[idx_]].head != v_) {
          idx_ = numEdges_;
        }
        return *this;
      }

      bool operator!=(const iterator &o) const { return idx_ != o.idx_; }

      int getEdgeIdx() {
        return reverseTrace_[idx_];
      }
    };

    class range
    {
      std::vector<int> &firstEdgeReverseV_;
      std::vector<int> &reverseTrace_;
      std::vector<EdgeInfo> &edgeInfoList_;
      std::vector<bool> &removed_;
      iterator begin_it_;
      int numEdges_;

    public:
      range(std::vector<EdgeInfo> &edgeInfoList, std::vector<int> &firstEdgeReverseV, std::vector<int> &reverseTrace, std::vector<bool> &removed, int numVert, int numEdges, int v)
          : firstEdgeReverseV_{firstEdgeReverseV}, reverseTrace_{reverseTrace}, edgeInfoList_{edgeInfoList}, removed_{removed}, begin_it_{iterator(edgeInfoList, firstEdgeReverseV, reverseTrace, removed, numVert, numEdges, v)}, numEdges_{numEdges} {}

      iterator begin() const { return begin_it_; }
      iterator end() const { return iterator(firstEdgeReverseV_, edgeInfoList_, reverseTrace_, removed_, numEdges_, numEdges_); }
    };
  }

  /* all edges (to do) */
  namespace EdgeList {
    class iterator
    {
      std::vector<EdgeInfo>::iterator it_;
      std::vector<bool>::iterator removedIt_;

    public:
      iterator(std::vector<EdgeInfo>::iterator it, std::vector<bool>::iterator removedIt)
      : it_{it}, removedIt_{removedIt}
      {
        while (*removedIt_) {
          ++it_;
          ++removedIt_;
        }
      }

      EdgeInfo& operator*() const { return *it_; }

      iterator &operator++()
      {
        ++it_;
        ++removedIt_;
        while (*removedIt_) {
          ++it_;
          ++removedIt_;
        }
        return *this;
      }

      bool operator!=(const iterator &o) const { return it_ != o.it_; }
    };

    class range
    {
      std::vector<EdgeInfo> &edgeInfoList_;
      std::vector<bool> &removed_;

    public:
      range(std::vector<EdgeInfo> &edgeInfoList, std::vector<bool> &removed)
          : edgeInfoList_{edgeInfoList}, removed_{removed} {}

      iterator begin() const { return iterator(edgeInfoList_.begin(), removed_.begin()); }
      iterator end() const { return iterator(edgeInfoList_.end(), removed_.end()); }
    };
  }
}
