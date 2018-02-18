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
#include "../src/hybridfengksp.hpp"
#include "../src/ksp.hpp"
#include "../src/path.hpp"
#include "../src/graph.hpp"

TEST(HYBRIDFENG_KSP, WRAPPED) {
    haruki::KSP<haruki::HybridFengKSP> hybridFeng;

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

    std::vector<haruki::Path> result = hybridFeng.run(g, 0, 6, 2);

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

TEST(HYBRIDFENG_KSP, PREPROC_DAG_UPSTREAM) {
    haruki::HybridFengKSP hybridFengKSP;

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

    hybridFengKSP.preproc(h, 0, 6, 2);

    ASSERT_EQ(0, hybridFengKSP.dag_paths_upstream_[0].size());
    ASSERT_EQ(2, hybridFengKSP.dag_paths_upstream_[1].size());
    ASSERT_EQ(0, hybridFengKSP.dag_paths_upstream_[2].size());
    ASSERT_EQ(0, hybridFengKSP.dag_paths_upstream_[3].size());
    ASSERT_EQ(0, hybridFengKSP.dag_paths_upstream_[4].size());
    ASSERT_EQ(1, hybridFengKSP.dag_paths_upstream_[5].size());
    ASSERT_EQ(4, hybridFengKSP.dag_paths_upstream_[6].size());
}

TEST(HYBRIDFENG_KSP, CANDIDATES) {
    haruki::HybridFengKSP hybridFengKSP;

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
    hybridFengKSP.preproc(h, 0, 6, 2);
    std::set<haruki::Path> candidates = hybridFengKSP.generateCandidates(h, 6, pvec, minPath);
    ASSERT_EQ(2, candidates.size());

    std::set<haruki::Path> fixedCostCandidates;
    for (std::set<haruki::Path>::iterator it = candidates.begin(); it != candidates.end(); it++) {
        haruki::Path p = hybridFengKSP.fixCosts(g, *it);
        fixedCostCandidates.insert(p);
    }

    ASSERT_EQ(rx, fixedCostCandidates);
}

TEST(HYBRIDFENG_KSP, CANDIDATES_AT_EDGE) {
    haruki::HybridFengKSP hybridFengKSP;

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

    haruki::Path paux1, paux2;

    paux1.addEdge(0, 1, 1.0);
    paux1.addEdge(1, 3, 2.0);
    paux1.addEdge(3, 6, 2.0);

    paux2.addEdge(0, 1, 1.0);
    paux2.addEdge(1, 4, 2.0);
    paux2.addEdge(4, 6, 2.0);

    haruki::Graph h = g;

    std::vector<haruki::Path> pvec;
    pvec.push_back(minPath);

    // TODO: separar o preproc e posproc
    hybridFengKSP.preproc(h, 0, 6, 2);
    hybridFengKSP.colors_ = std::vector<int>(h.getNumVert(), 3);
    hybridFengKSP.initializeYellowGraph(h);
    hybridFengKSP.fixColorsNewDeviationVertex(h, 6, pvec, paux1, 0);
    hybridFengKSP.removeEdgesSharedPrefix(h, 6, pvec, paux1, 1);
    hybridFengKSP.fixColorsNewDeviationVertex(h, 6, pvec, paux1, 1);
    haruki::Path candidate = hybridFengKSP.generateCandidateAtEdge(h, 6, pvec, paux1, 1);

    haruki::Path fixedCostCandidate = hybridFengKSP.fixCosts(g, candidate);

    ASSERT_EQ(paux2, fixedCostCandidate);
}

TEST(HYBRIDFENG_KSP, COLORS) {
    haruki::HybridFengKSP hybridFengKSP;

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

    hybridFengKSP.colors_ = std::vector<int>(g.getNumVert(), 3);

    haruki::Path minPath;
    minPath.addEdge(0, 1, 1.0);
    minPath.addEdge(1, 5, 1.0);
    minPath.addEdge(5, 6, 1.0);

    hybridFengKSP.preproc(g, 0, 6, 2);
    std::vector<haruki::Path> pvec;

    hybridFengKSP.initializeYellowGraph(g);
    hybridFengKSP.fixColorsNewDeviationVertex(g, 6, pvec, minPath, 1);

    ASSERT_EQ(1, hybridFengKSP.colors_[0]);
    ASSERT_EQ(2, hybridFengKSP.colors_[1]);
    ASSERT_EQ(2, hybridFengKSP.colors_[2]);
    ASSERT_EQ(3, hybridFengKSP.colors_[3]);
    ASSERT_EQ(3, hybridFengKSP.colors_[4]);
    ASSERT_EQ(3, hybridFengKSP.colors_[5]);
    ASSERT_EQ(3, hybridFengKSP.colors_[6]);
}
