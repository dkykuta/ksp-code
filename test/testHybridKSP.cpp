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
#include "../src/hybridksp.hpp"
#include "../src/ksp.hpp"
#include "../src/path.hpp"
#include "../src/graph.hpp"
#include "../src/candidatepath.hpp"

TEST(HYBRID_KSP, WRAPPED1) {
    haruki::KSP<haruki::HybridKSP> hybrid;

    haruki::GraphBuilder pg;
    pg.addEdge(0, 2, 1);
    pg.addEdge(0, 3, 1);
    pg.addEdge(0, 5, 1);
    pg.addEdge(0, 6, 1);
    pg.addEdge(1, 2, 1);
    pg.addEdge(1, 3, 1);
    pg.addEdge(2, 0, 1);
    pg.addEdge(2, 3, 1);
    pg.addEdge(2, 5, 1);
    pg.addEdge(2, 6, 1);
    pg.addEdge(3, 2, 1);
    pg.addEdge(3, 4, 1);
    pg.addEdge(3, 6, 1);
    pg.addEdge(4, 1, 1);
    pg.addEdge(4, 3, 1);
    pg.addEdge(5, 0, 1);
    pg.addEdge(5, 4, 1);
    pg.addEdge(5, 6, 1);
    pg.addEdge(6, 3, 1);
    pg.addEdge(6, 5, 1);

    haruki::Graph g(pg);

    std::vector<haruki::Path> result = hybrid.run(g, 0, 6, 10);

    ASSERT_EQ(10, result.size());

    haruki::Path firstMinPath = result[0];

    EXPECT_DOUBLE_EQ(1.0, firstMinPath.cost());
    ASSERT_EQ(2, firstMinPath.size());
    EXPECT_EQ(0, firstMinPath.getVertList()[0]);
    EXPECT_EQ(6, firstMinPath.getVertList()[1]);

    haruki::Path secondMinPath = result[1];

    EXPECT_DOUBLE_EQ(2.0, secondMinPath.cost());
    ASSERT_EQ(3, secondMinPath.size());
    EXPECT_EQ(0, secondMinPath.getVertList()[0]);
    EXPECT_EQ(2, secondMinPath.getVertList()[1]);
    EXPECT_EQ(6, secondMinPath.getVertList()[2]);

    haruki::Path ninethMinPath = result[8];

    EXPECT_DOUBLE_EQ(4.0, ninethMinPath.cost());
    ASSERT_EQ(5, ninethMinPath.size());
    EXPECT_EQ(0, ninethMinPath.getVertList()[0]);
    EXPECT_EQ(5, ninethMinPath.getVertList()[1]);
    EXPECT_EQ(4, ninethMinPath.getVertList()[2]);
    EXPECT_EQ(3, ninethMinPath.getVertList()[3]);
    EXPECT_EQ(6, ninethMinPath.getVertList()[4]);
}

TEST(HYBRID_KSP, WRAPPED2) {
    haruki::KSP<haruki::HybridKSP> hybrid;

    haruki::GraphBuilder pg;
    pg.addEdge(0, 1, 1);
    pg.addEdge(0, 2, 1);
    pg.addEdge(0, 5, 1);
    pg.addEdge(0, 6, 1);
    pg.addEdge(1, 0, 1);
    pg.addEdge(1, 3, 1);
    pg.addEdge(1, 4, 1);
    pg.addEdge(1, 5, 1);
    pg.addEdge(2, 0, 1);
    pg.addEdge(2, 1, 1);
    pg.addEdge(2, 4, 1);
    pg.addEdge(2, 5, 1);
    pg.addEdge(3, 1, 1);
    pg.addEdge(3, 2, 1);
    pg.addEdge(3, 5, 1);
    pg.addEdge(3, 6, 1);
    pg.addEdge(4, 0, 1);
    pg.addEdge(4, 3, 1);
    pg.addEdge(4, 5, 1);
    pg.addEdge(4, 6, 1);
    pg.addEdge(5, 0, 1);
    pg.addEdge(5, 1, 1);
    pg.addEdge(5, 3, 1);
    pg.addEdge(5, 4, 1);
    pg.addEdge(5, 6, 1);
    pg.addEdge(6, 0, 1);
    pg.addEdge(6, 2, 1);
    pg.addEdge(6, 3, 1);
    pg.addEdge(6, 4, 1);
    pg.addEdge(6, 5, 1);

    haruki::Graph g(pg);

    std::vector<haruki::Path> result = hybrid.run(g, 0, 6, 10);

    ASSERT_EQ(10, result.size());

    haruki::Path firstMinPath = result[0];

    EXPECT_DOUBLE_EQ(1.0, firstMinPath.cost());
    ASSERT_EQ(2, firstMinPath.size());
    EXPECT_EQ(0, firstMinPath.getVertList()[0]);
    EXPECT_EQ(6, firstMinPath.getVertList()[1]);

    haruki::Path secondMinPath = result[1];

    EXPECT_DOUBLE_EQ(2.0, secondMinPath.cost());
    ASSERT_EQ(3, secondMinPath.size());
    EXPECT_EQ(0, secondMinPath.getVertList()[0]);
    EXPECT_EQ(5, secondMinPath.getVertList()[1]);
    EXPECT_EQ(6, secondMinPath.getVertList()[2]);

    haruki::Path thirdMinPath = result[2];

    EXPECT_DOUBLE_EQ(3.0, thirdMinPath.cost());
    ASSERT_EQ(4, thirdMinPath.size());
    EXPECT_EQ(0, thirdMinPath.getVertList()[0]);
    EXPECT_EQ(1, thirdMinPath.getVertList()[1]);
    EXPECT_EQ(3, thirdMinPath.getVertList()[2]);
    EXPECT_EQ(6, thirdMinPath.getVertList()[3]);

    haruki::Path ninethMinPath = result[8];

    EXPECT_DOUBLE_EQ(3.0, ninethMinPath.cost());
    ASSERT_EQ(4, ninethMinPath.size());
    EXPECT_EQ(0, ninethMinPath.getVertList()[0]);
    EXPECT_EQ(5, ninethMinPath.getVertList()[1]);
    EXPECT_EQ(4, ninethMinPath.getVertList()[2]);
    EXPECT_EQ(6, ninethMinPath.getVertList()[3]);
}

TEST(HYBRID_KSP, PREPROC_DAG_UPSTREAM) {
    haruki::HybridKSP hybridKSP;

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

    hybridKSP.preproc(h, 0, 6, 2);

    ASSERT_EQ(0, hybridKSP.dag_paths_upstream_[0].size());
    ASSERT_EQ(2, hybridKSP.dag_paths_upstream_[1].size());
    ASSERT_EQ(0, hybridKSP.dag_paths_upstream_[2].size());
    ASSERT_EQ(0, hybridKSP.dag_paths_upstream_[3].size());
    ASSERT_EQ(0, hybridKSP.dag_paths_upstream_[4].size());
    ASSERT_EQ(1, hybridKSP.dag_paths_upstream_[5].size());
    ASSERT_EQ(4, hybridKSP.dag_paths_upstream_[6].size());
}

TEST(HYBRID_KSP, CANDIDATES) {
    haruki::HybridKSP hybridKSP;

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
    hybridKSP.preproc(h, 0, 6, 2);
    hybridKSP.initializeYellowGraph(h);
    std::set<haruki::CandidatePath> candidates = hybridKSP.generateCandidates(h, 6, pvec, minPath, 0);
    ASSERT_EQ(2, candidates.size());

    std::set<haruki::Path> fixedCostCandidates;
    for (std::set<haruki::CandidatePath>::iterator it = candidates.begin(); it != candidates.end(); it++) {
        haruki::CandidatePath cpx = *it;
        haruki::Path px = cpx.path();
        haruki::Path p = hybridKSP.fixCosts(g, px);
        fixedCostCandidates.insert(p);
    }

    ASSERT_EQ(rx, fixedCostCandidates);
}

TEST(HYBRID_KSP, CANDIDATES_AT_EDGE) {
    haruki::HybridKSP hybridKSP;

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
    hybridKSP.preproc(h, 0, 6, 2);
    hybridKSP.colors_ = std::vector<int>(h.getNumVert(), 3);
    hybridKSP.initializeYellowGraph(h);
    std::vector<int> newYellowList = hybridKSP.initialColor(h, 6, pvec, paux1, 1);
    hybridKSP.removeEdgesSharedPrefix(h, 6, pvec, paux1, 1);
    hybridKSP.updateArtificialEdges(h, newYellowList, -1, 1);
    haruki::Path candidate = hybridKSP.generateCandidateAtEdge(h, 6, pvec, paux1, 1);

    haruki::Path fixedCostCandidate = hybridKSP.fixCosts(g, candidate);

    ASSERT_EQ(paux2, fixedCostCandidate);
}

TEST(HYBRID_KSP, COLOR_INITIAL) {
    int FENG_COLOR_RED = 1;
    int FENG_COLOR_YELLOW = 2;
    int FENG_COLOR_GREEN = 3;

    haruki::HybridKSP hybridKSP;

    haruki::GraphBuilder pg;
    pg.addEdge(0, 1, 1);
    pg.addEdge(1, 2, 1);
    pg.addEdge(1, 3, 1);
    pg.addEdge(2, 3, 1);
    pg.addEdge(3, 4, 1);
    pg.addEdge(3, 5, 1);
    pg.addEdge(3, 6, 1);
    pg.addEdge(3, 12, 1);
    pg.addEdge(4, 13, 1);
    pg.addEdge(4, 14, 1);
    pg.addEdge(5, 6, 1);
    pg.addEdge(6, 2, 1);
    pg.addEdge(6, 7, 1);
    pg.addEdge(7, 8, 1);
    pg.addEdge(8, 9, 1);
    pg.addEdge(9, 10, 1);
    pg.addEdge(10, 14, 1);
    pg.addEdge(11, 1, 1);
    pg.addEdge(12, 11, 1);
    pg.addEdge(13, 12, 1);
    
    haruki::Graph g(pg);

    hybridKSP.preproc(g, 0, 14, 2);
    hybridKSP.colors_ = std::vector<int>(g.getNumVert(), FENG_COLOR_GREEN);

    g.removeEdge(3, 4);

    haruki::Path minPath;
    minPath.addEdge(0, 1, 1.0);
    minPath.addEdge(1, 3, 1.0);
    minPath.addEdge(3, 4, 1.0);
    minPath.addEdge(4, 14, 1.0);

    std::vector<haruki::Path> pvec;

    std::vector<int> newYellowList(std::move(hybridKSP.initialColor(g, 14, pvec, minPath, 2)));
    std::set<int> newYellow(newYellowList.begin(), newYellowList.end());

    EXPECT_EQ(FENG_COLOR_RED, hybridKSP.colors_[0]);
    EXPECT_EQ(FENG_COLOR_RED, hybridKSP.colors_[1]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[2]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[3]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[4]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[5]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[6]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[7]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[8]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[9]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[10]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[11]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[12]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[13]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[14]);

    ASSERT_EQ(7, newYellow.size());
    EXPECT_TRUE(newYellow.find(2) != newYellow.end());
    EXPECT_TRUE(newYellow.find(3) != newYellow.end());
    EXPECT_TRUE(newYellow.find(5) != newYellow.end());
    EXPECT_TRUE(newYellow.find(6) != newYellow.end());
    EXPECT_TRUE(newYellow.find(11) != newYellow.end());
    EXPECT_TRUE(newYellow.find(12) != newYellow.end());
    EXPECT_TRUE(newYellow.find(13) != newYellow.end());

    hybridKSP.posproc(g, 0, 14, 2, pvec);
}

TEST(HYBRID_KSP, UPDATE_COLOR) {
    int FENG_COLOR_RED = 1;
    int FENG_COLOR_YELLOW = 2;
    int FENG_COLOR_GREEN = 3;

    haruki::HybridKSP hybridKSP;

    haruki::GraphBuilder pg;
    pg.addEdge(0, 1, 1);
    pg.addEdge(1, 2, 1);
    pg.addEdge(1, 3, 1);
    pg.addEdge(2, 3, 1);
    pg.addEdge(3, 4, 1);
    pg.addEdge(3, 5, 1);
    pg.addEdge(3, 6, 1);
    pg.addEdge(3, 12, 1);
    pg.addEdge(4, 13, 1);
    pg.addEdge(4, 14, 1);
    pg.addEdge(5, 6, 1);
    pg.addEdge(6, 2, 1);
    pg.addEdge(6, 7, 1);
    pg.addEdge(7, 8, 1);
    pg.addEdge(8, 9, 1);
    pg.addEdge(9, 10, 1);
    pg.addEdge(10, 14, 1);
    pg.addEdge(11, 1, 1);
    pg.addEdge(12, 11, 1);
    pg.addEdge(13, 12, 1);
    
    haruki::Graph g(pg);

    hybridKSP.preproc(g, 0, 14, 2);
    hybridKSP.colors_ = std::vector<int>(g.getNumVert(), FENG_COLOR_GREEN);

    g.removeEdge(3, 4);

    haruki::Path minPath;
    minPath.addEdge(0, 1, 1.0);
    minPath.addEdge(1, 3, 1.0);
    minPath.addEdge(3, 4, 1.0);
    minPath.addEdge(4, 14, 1.0);

    std::vector<haruki::Path> pvec;
    
    hybridKSP.initialColor(g, 14, pvec, minPath, 2);

    EXPECT_EQ(FENG_COLOR_RED, hybridKSP.colors_[0]);
    EXPECT_EQ(FENG_COLOR_RED, hybridKSP.colors_[1]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[2]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[3]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[4]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[5]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[6]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[7]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[8]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[9]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[10]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[11]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[12]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[13]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[14]);
    
    std::vector<int> newYellowList(std::move(hybridKSP.updateColor(g, 14, pvec, minPath, 3)));
    std::set<int> newYellow(newYellowList.begin(), newYellowList.end());

    EXPECT_EQ(FENG_COLOR_RED, hybridKSP.colors_[0]);
    EXPECT_EQ(FENG_COLOR_RED, hybridKSP.colors_[1]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[2]);
    EXPECT_EQ(FENG_COLOR_RED, hybridKSP.colors_[3]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[4]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[5]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[6]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[7]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[8]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[9]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[10]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[11]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[12]);
    EXPECT_EQ(FENG_COLOR_YELLOW, hybridKSP.colors_[13]);
    EXPECT_EQ(FENG_COLOR_GREEN, hybridKSP.colors_[14]);

    ASSERT_EQ(1, newYellow.size());
    EXPECT_TRUE(newYellow.find(4) != newYellow.end());

    hybridKSP.posproc(g, 0, 14, 2, pvec);
}

TEST(HYBRID_KSP, UPDATE_ARTIFICIAL_EDGES) {
    int FENG_COLOR_GREEN = 3;

    haruki::HybridKSP hybridKSP;

    haruki::GraphBuilder pg;
    pg.addEdge(0, 1, 1);
    pg.addEdge(1, 2, 1);
    pg.addEdge(1, 3, 1);
    pg.addEdge(2, 3, 1);
    pg.addEdge(3, 4, 1);
    pg.addEdge(3, 5, 1);
    pg.addEdge(3, 6, 1);
    pg.addEdge(3, 12, 1);
    pg.addEdge(4, 13, 1);
    pg.addEdge(4, 14, 1);
    pg.addEdge(5, 6, 1);
    pg.addEdge(6, 2, 1);
    pg.addEdge(6, 7, 1);
    pg.addEdge(7, 8, 1);
    pg.addEdge(8, 9, 1);
    pg.addEdge(9, 10, 1);
    pg.addEdge(10, 14, 1);
    pg.addEdge(11, 1, 1);
    pg.addEdge(12, 11, 1);
    pg.addEdge(13, 12, 1);
    
    haruki::Graph g(pg);

    hybridKSP.preproc(g, 0, 14, 2);
    hybridKSP.colors_ = std::vector<int>(g.getNumVert(), FENG_COLOR_GREEN);

    g.removeEdge(3, 4);

    haruki::Path minPath;
    minPath.addEdge(0, 1, 1.0);
    minPath.addEdge(1, 3, 1.0);
    minPath.addEdge(3, 4, 1.0);
    minPath.addEdge(4, 14, 1.0);

    std::vector<haruki::Path> pvec;
       
    std::vector<int> newYellowList = hybridKSP.initialColor(g, 14, pvec, minPath, 2);

    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(0, 1));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(1, 2));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(1, 3));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(2, 3));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(3, 4));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(3, 5));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(3, 6));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(3, 12));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(4, 13));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(4, 14));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(5, 6));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(6, 2));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(6, 7));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(7, 8));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(8, 9));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(9, 10));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(10, 14));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(11, 1));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(12, 11));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(13, 12));

    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(0, 15));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(1, 15));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(2, 15));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(3, 15));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(4, 15));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(5, 15));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(6, 15));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(7, 15));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(8, 15));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(9, 15));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(10, 15));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(11, 15));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(12, 15));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(13, 15));
    ASSERT_EQ(true, hybridKSP.yellowGraph_->isRemoved(14, 15));

    hybridKSP.updateArtificialEdges(g, newYellowList, -1, 3);

    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(0, 1));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(1, 2));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(1, 3));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(2, 3));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(3, 4));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(3, 5));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(3, 6));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(3, 12));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(4, 13));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(4, 14));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(5, 6));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(6, 2));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(6, 7));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(7, 8));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(8, 9));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(9, 10));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(10, 14));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(11, 1));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(12, 11));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(13, 12));

    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(0, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(1, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(2, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(3, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(4, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(5, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(6, 15));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(7, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(8, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(9, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(10, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(11, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(12, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(13, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(14, 15));

    std::vector<int> newYellowList2 = hybridKSP.updateColor(g, 14, pvec, minPath, 3); // indice 3 no path
    hybridKSP.updateArtificialEdges(g, newYellowList2, 3, 4); // vertice 3

    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(0, 1));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(1, 2));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(1, 3));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(2, 3));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(3, 4));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(3, 5));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(3, 6));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(3, 12));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(4, 13));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(4, 14));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(5, 6));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(6, 2));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(6, 7));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(7, 8));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(8, 9));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(9, 10));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(10, 14));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(11, 1));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(12, 11));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(13, 12));

    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(0, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(1, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(2, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(3, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(4, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(5, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(6, 15));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(7, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(8, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(9, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(10, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(11, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(12, 15));
    EXPECT_EQ(true, hybridKSP.yellowGraph_->isRemoved(13, 15));
    EXPECT_EQ(false, hybridKSP.yellowGraph_->isRemoved(14, 15));

    hybridKSP.posproc(g, 0, 14, 2, pvec);
}

