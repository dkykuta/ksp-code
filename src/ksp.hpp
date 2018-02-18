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
#include <chrono>
#include "path.hpp"
#include "graph.hpp"

namespace haruki
{

template <class KSPAlgorithm>
class KSP : private KSPAlgorithm
{
  public:
    std::vector<haruki::Path> run(haruki::Graph &g, int s, int t, int k) {
      haruki::Graph h = g;

      auto startAlg = std::chrono::high_resolution_clock::now();
      KSPAlgorithm::preproc(h, s, t, k);

      auto endPreproc = std::chrono::high_resolution_clock::now();
      std::vector<haruki::Path> rawRet = KSPAlgorithm::ksp(h, s, t, k);

      auto endCore = std::chrono::high_resolution_clock::now();
      std::vector<haruki::Path> ret = KSPAlgorithm::posproc(g, s, t, k, rawRet);

      auto endPosproc = std::chrono::high_resolution_clock::now();

      auto preprocDur = std::chrono::duration_cast<std::chrono::milliseconds>(endPreproc-startAlg);
      auto coreDur = std::chrono::duration_cast<std::chrono::milliseconds>(endCore-endPreproc);
      auto posprocDur = std::chrono::duration_cast<std::chrono::milliseconds>(endPosproc-endCore);

      auto totalDur = std::chrono::duration_cast<std::chrono::milliseconds>(endPosproc-startAlg);

      std::cout << "PREPROC|" << preprocDur.count() << "\n";
      std::cout << "CORE|" << coreDur.count() << "\n";
      std::cout << "POSPROC|" << posprocDur.count() << "\n";
      std::cout << "TOTAL|" << totalDur.count() << "\n";

      return ret;
    }
};
}
