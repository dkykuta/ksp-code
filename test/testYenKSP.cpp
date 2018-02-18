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
#include "../src/yenksp.hpp"
#include "../src/ksp.hpp"
#include "../src/path.hpp"
#include "../src/graph.hpp"

TEST(YEN_KSP, WRAPPED) {
    haruki::KSP<haruki::YenKSP> yen;

    haruki::GraphBuilder pg;
    pg.addEdge(0, 1, 1);
    pg.addEdge(0, 2, 2);
    pg.addEdge(1, 3, 2);
    pg.addEdge(1, 4, 2);
    pg.addEdge(1, 5, 1);
    pg.addEdge(2, 1, 2);
    pg.addEdge(2, 4, 2);
    pg.addEdge(3, 2, 2);
    pg.addEdge(3, 6, 2);
    pg.addEdge(4, 6, 2);
    pg.addEdge(5, 6, 1);

    haruki::Graph g(pg);

    std::vector<haruki::Path> result = yen.run(g, 0, 6, 2);

    ASSERT_EQ(2, result.size());

    haruki::Path firstMinPath = result[0];

    ASSERT_DOUBLE_EQ(3.0, firstMinPath.cost());
    ASSERT_EQ(4, firstMinPath.size());
    ASSERT_EQ(0, firstMinPath.getVertList()[0]);
    ASSERT_EQ(1, firstMinPath.getVertList()[1]);
    ASSERT_EQ(5, firstMinPath.getVertList()[2]);
    ASSERT_EQ(6, firstMinPath.getVertList()[3]);

    haruki::Path secondMinPath = result[1];

    ASSERT_DOUBLE_EQ(5.0, secondMinPath.cost());
    ASSERT_EQ(4, secondMinPath.size());
    ASSERT_EQ(0, secondMinPath.getVertList()[0]);
    ASSERT_EQ(1, secondMinPath.getVertList()[1]);
    ASSERT_EQ(3, secondMinPath.getVertList()[2]);
    ASSERT_EQ(6, secondMinPath.getVertList()[3]);

}

TEST(YEN_KSP, CANDIDATES) {
    haruki::YenKSP yenKSP;

    haruki::GraphBuilder pg;
    pg.addEdge(0, 1, 1);
    pg.addEdge(0, 2, 2);
    pg.addEdge(1, 3, 2);
    pg.addEdge(1, 4, 2);
    pg.addEdge(1, 5, 1);
    pg.addEdge(2, 1, 2);
    pg.addEdge(2, 4, 2);
    pg.addEdge(3, 2, 2);
    pg.addEdge(3, 6, 2);
    pg.addEdge(4, 6, 2);
    pg.addEdge(5, 6, 1);

    haruki::Graph g(pg);

    haruki::Path minPath;
    minPath.addEdge(0, 1, 1.0);
    minPath.addEdge(1, 5, 1.0);
    minPath.addEdge(5, 6, 1.0);

    std::set<haruki::Path> rx;
    haruki::Path paux1, paux2;

    paux1.addEdge(0, 1, 1.0);
    paux1.addEdge(1, 3, 2.0);
    paux1.addEdge(3, 6, 2.0);

    paux2.addEdge(0, 2, 2.0);
    paux2.addEdge(2, 4, 2.0);
    paux2.addEdge(4, 6, 2.0);

    rx.insert(paux1);
    rx.insert(paux2);

    std::vector<haruki::Path> pvec;

    std::set<haruki::Path> candidates = yenKSP.generateCandidates(g, 6, pvec, minPath);
    ASSERT_EQ(2, candidates.size());
    ASSERT_EQ(rx, candidates);
}

TEST(YEN_KSP, CANDIDATES_AT_EDGE) {
    haruki::YenKSP yenKSP;

    haruki::GraphBuilder pg;
    pg.addEdge(0, 1, 1);
    pg.addEdge(0, 2, 2);
    pg.addEdge(1, 3, 2);
    pg.addEdge(1, 4, 2);
    pg.addEdge(1, 5, 1);
    pg.addEdge(2, 1, 2);
    pg.addEdge(2, 4, 2);
    pg.addEdge(3, 2, 2);
    pg.addEdge(3, 6, 2);
    pg.addEdge(4, 6, 2);
    pg.addEdge(5, 6, 1);

    haruki::Graph g(pg);

    haruki::Path minPath;
    minPath.addEdge(0, 1, 1.0);
    minPath.addEdge(1, 5, 1.0);
    minPath.addEdge(5, 6, 1.0);

    std::set<haruki::Path> rx;
    haruki::Path paux1, paux2;

    paux1.addEdge(0, 1, 1.0);
    paux1.addEdge(1, 3, 2.0);
    paux1.addEdge(3, 6, 2.0);

    paux2.addEdge(0, 2, 2.0);
    paux2.addEdge(2, 4, 2.0);
    paux2.addEdge(4, 6, 2.0);

    rx.insert(paux1);

    std::vector<haruki::Path> pvec;

    yenKSP.removeEdgesSharedPrefix(g, 6, pvec, minPath, 0);
    haruki::Path candidate1 = yenKSP.generateCandidateAtEdge(g, 6, pvec, minPath, 0);
    ASSERT_EQ(paux2, candidate1);

    g.resetEdgesRemoved();

    yenKSP.removeEdgesSharedPrefix(g, 6, pvec, minPath, 1);
    haruki::Path candidate2 = yenKSP.generateCandidateAtEdge(g, 6, pvec, minPath, 1);
    ASSERT_EQ(paux1, candidate2);
}
