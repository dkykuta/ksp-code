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
#include "path.hpp"

namespace haruki
{

class CandidatePath
{
private:
  int deviationIndex_;
  Path path_;

public:
  CandidatePath(): deviationIndex_(0), path_(Path()){};
  // CandidatePath(int x): deviationIndex_(x), path_(Path()){};
  CandidatePath(int deviationIndex, Path path) : deviationIndex_(deviationIndex), path_(path) {};
  Path path() {return path_;};
  int deviationIndex() { return deviationIndex_;};
  bool operator<(const haruki::CandidatePath &rhs) const {return path_ < rhs.path_;};
  bool operator>(const haruki::CandidatePath &rhs) const {return path_ > rhs.path_;};
  bool operator==(const haruki::CandidatePath &rhs) const {return deviationIndex_ == rhs.deviationIndex_ && path_ == rhs.path_;};
};
}
