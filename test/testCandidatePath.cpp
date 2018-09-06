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
#include <gtest/gtest.h>
#include "../src/candidatepath.hpp"

TEST(CANDIDATE_PATH, COMPARE) {
    haruki::Path path1;
    path1.addEdge(0, 10, 5.7);
    path1.addEdge(10, 39, 2.1);
    path1.addEdge(39, 2, 5.5);

    haruki::Path path2;
    path2.addEdge(0, 10, 5.7);
    path2.addEdge(10, 39, 2.1);
    path2.addEdge(39, 2, 5.5);

    haruki::Path path3;
    path3.addEdge(0, 19, 5.7);
    path3.addEdge(19, 29, 2.1);
    path3.addEdge(29, 2, 5.5);

    haruki::Path path4;
    path4.addEdge(0, 11, 9.7);
    path4.addEdge(11, 29, 2.1);
    path4.addEdge(29, 2, 5.5);

    haruki::CandidatePath cp1(0, path1);
    haruki::CandidatePath cp2(0, path2);
    haruki::CandidatePath cp3(0, path3);
    haruki::CandidatePath cp4(0, path4);

    ASSERT_EQ(path1 == path2, cp1 == cp2);
    ASSERT_EQ(path1 < path2, cp1 < cp2);
    ASSERT_EQ(path1 > path2, cp1 > cp2);

    ASSERT_EQ(path1 == path3, cp1 == cp3);
    ASSERT_EQ(path1 < path3, cp1 < cp3);
    ASSERT_EQ(path1 > path3, cp1 > cp3);

    ASSERT_EQ(path1 == path4, cp1 == cp4);
    ASSERT_EQ(path1 < path4, cp1 < cp4);
    ASSERT_EQ(path1 > path4, cp1 > cp4);
}