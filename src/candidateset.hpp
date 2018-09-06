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

#include <queue>
#include <vector>
#include <functional>
#include <utility>

namespace haruki
{

template <class T>
class CandidateSet
{
  private:
    std::priority_queue<T, std::vector<T>, std::greater<T> > candQ_;
    T max_;
    int inCount_;
    int k_;

  public:
    CandidateSet(int k, T minValue): max_(minValue), inCount_(0), k_(k) {};

    T popFirst()
    {
        T a = candQ_.top();
        candQ_.pop();
        return a;
    }

    void addCandidate(T candidate)
    {
        if (k_ > 0 && inCount_ >= k_) {
            if (candidate > max_) {
                return;
            }
        }
        candQ_.push(candidate);
        inCount_++;
        if (candidate > max_) {
            max_ = candidate;
        }
    }

    int size()
    {
        return candQ_.size();
    }

    bool empty()
    {
        return candQ_.empty();
    }
};
}
