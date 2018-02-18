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
#include "../src/pascoalksp.hpp"
#include "../src/ksp.hpp"
#include "../src/path.hpp"
#include "../src/graph.hpp"

TEST(PASCOAL_KSP, WRAPPED) {
    haruki::KSP<haruki::PascoalKSP> pascoal;

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

    std::vector<haruki::Path> result = pascoal.run(g, 0, 6, 2);

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

TEST(PASCOAL_KSP, PREPROC) {
    haruki::PascoalKSP pascoalKSP;

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

    haruki::Graph h = g;

    // TODO: separar o preproc e posproc
    pascoalKSP.preproc(h, 0, 6, 2);

    ASSERT_EQ(0, h.getEdgeCost(0, 1));
    ASSERT_EQ(3, h.getEdgeCost(0, 2));
    ASSERT_EQ(2, h.getEdgeCost(1, 3));
    ASSERT_EQ(2, h.getEdgeCost(1, 4));
    ASSERT_EQ(0, h.getEdgeCost(1, 5));
    ASSERT_EQ(0, h.getEdgeCost(2, 1));
    ASSERT_EQ(0, h.getEdgeCost(2, 4));
    ASSERT_EQ(4, h.getEdgeCost(3, 2));
    ASSERT_EQ(0, h.getEdgeCost(3, 6));
    ASSERT_EQ(0, h.getEdgeCost(4, 6));
    ASSERT_EQ(0, h.getEdgeCost(5, 6));
}

TEST(PASCOAL_KSP, CANDIDATES) {
    haruki::PascoalKSP pascoalKSP;

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

    // paux2.addEdge(0, 2, 2.0);
    // paux2.addEdge(2, 4, 2.0);
    // paux2.addEdge(4, 6, 2.0);

    paux2.addEdge(0, 2, 2.0);
    paux2.addEdge(2, 1, 2.0);
    paux2.addEdge(1, 5, 1.0);
    paux2.addEdge(5, 6, 1.0);

    rx.insert(paux1);
    rx.insert(paux2);

    haruki::Graph h = g;

    std::vector<haruki::Path> pvec;

    // TODO: separar o preproc e posproc
    pascoalKSP.preproc(h, 0, 6, 2);
    std::set<haruki::Path> candidates = pascoalKSP.generateCandidates(h, 6, pvec, minPath);
    ASSERT_EQ(2, candidates.size());

    std::set<haruki::Path> fixedCostCandidates;
    for (std::set<haruki::Path>::iterator it = candidates.begin(); it != candidates.end(); it++) {
        haruki::Path p = pascoalKSP.fixCosts(g, *it);
        fixedCostCandidates.insert(p);
    }

    ASSERT_EQ(rx, fixedCostCandidates);
}


TEST(PASCOAL_KSP, CANDIDATE_AT_EDGE) {
    haruki::PascoalKSP pascoalKSP;

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
    paux2.addEdge(2, 1, 2.0);
    paux2.addEdge(1, 5, 1.0);
    paux2.addEdge(5, 6, 1.0);

    rx.insert(paux1);
    rx.insert(paux2);

    haruki::Graph h(g);

    std::vector<haruki::Path> pvec;

    // TODO: separar o preproc e posproc
    pascoalKSP.preproc(h, 0, 6, 2);

    haruki::Path candidate1 = pascoalKSP.generateCandidateAtEdge(h, 6, pvec, minPath, 0);
    haruki::Path fixedCostCandidate1 = pascoalKSP.fixCosts(g, candidate1);
    ASSERT_EQ(paux2, fixedCostCandidate1);

    haruki::Path candidate2 = pascoalKSP.generateCandidateAtEdge(h, 6, pvec, minPath, 1);
    haruki::Path fixedCostCandidate2 = pascoalKSP.fixCosts(g, candidate2);
    ASSERT_EQ(paux1, fixedCostCandidate2);
}
