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
#include "../src/candidateset.hpp"
#include "../src/candidatepath.hpp"
#include "../src/path.hpp"

TEST(CANDIDATE_SET, CREATION) {
    haruki::CandidateSet<int> c_set(10, 0);
}

TEST(CANDIDATE_SET, ADD) {
    haruki::CandidateSet<int> c_set(10, 0);

    ASSERT_EQ(0, c_set.size());

    c_set.addCandidate(3);
    c_set.addCandidate(7);

    ASSERT_EQ(2, c_set.size());
}

TEST(CANDIDATE_SET, ADD_LIMIT) {
    haruki::CandidateSet<int> c_set(3, 0);

    c_set.addCandidate(3);
    c_set.addCandidate(7);

    ASSERT_EQ(2, c_set.size());

    c_set.addCandidate(8);

    ASSERT_EQ(3, c_set.size());

    c_set.addCandidate(9);
    ASSERT_EQ(3, c_set.size());

    c_set.addCandidate(1);
    ASSERT_EQ(4, c_set.size());

    c_set.addCandidate(13);
    ASSERT_EQ(4, c_set.size());
}

TEST(CANDIDATE_SET, POP_FIRST) {
    haruki::CandidateSet<int> c_set(10, 0);

    c_set.addCandidate(7);
    c_set.addCandidate(3);

    ASSERT_EQ(2, c_set.size());
    ASSERT_EQ(3, c_set.popFirst());
    ASSERT_EQ(1, c_set.size());

    c_set.addCandidate(4);
    ASSERT_EQ(2, c_set.size());
}

TEST(CANDIDATE_SET, PATH) {
    haruki::Path path1, path2, path3;
    path1.addEdge(0,10, 5.7);
    path1.addEdge(10, 39, 2.1);
    path1.addEdge(39, 2, 5.5);

    path2.addEdge(0, 12, 99.1);
    path2.addEdge(12, 1, 12.7);
    path2.addEdge(1, 7, 3.5);

    haruki::CandidateSet<haruki::Path> c_set(10, haruki::Path());
    c_set.addCandidate(path1);
    c_set.addCandidate(path2);
    ASSERT_EQ(2, c_set.size());

    haruki::Path pPath = c_set.popFirst();
    ASSERT_DOUBLE_EQ(13.3, pPath.cost());
    ASSERT_EQ(path1, pPath);
    ASSERT_EQ(1, c_set.size());
}

TEST(CANDIDATE_SET, CANDIDATE_PATH) {
    haruki::Path path1, path2, path3;
    path1.addEdge(0,10, 5.7);
    path1.addEdge(10, 39, 2.1);
    path1.addEdge(39, 2, 5.5);

    path2.addEdge(0, 12, 99.1);
    path2.addEdge(12, 1, 12.7);
    path2.addEdge(1, 7, 3.5);

    haruki::CandidatePath cp1(0, path1);

    haruki::CandidateSet<haruki::CandidatePath> c_set(10, haruki::CandidatePath());
    c_set.addCandidate(cp1);
    c_set.addCandidate(haruki::CandidatePath(0, path2));
    ASSERT_EQ(2, c_set.size());

    haruki::Path pPath = c_set.popFirst().path();
    ASSERT_DOUBLE_EQ(13.3, pPath.cost());
    ASSERT_EQ(path1, pPath);
    ASSERT_EQ(1, c_set.size());
}
