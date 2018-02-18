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
#include "../src/graph.hpp"

TEST(GRAPH, CREATION_AND_NUM_NODES) {
    haruki::GraphBuilder pg1, pg2, pg3;
    pg1.addEdge(0, 5, 1.0);
    pg1.addEdge(1, 3, 1.0);
    pg1.addEdge(2, 7, 1.0);
    pg1.addEdge(2, 3, 1.0);

    haruki::Graph g1(pg1);
    ASSERT_EQ(8, g1.getNumVert());

    pg2.addEdge(0, 5, 1.0);
    pg2.addEdge(1, 3, 1.0);
    pg2.addEdge(2, 7, 1.0);
    pg2.addEdge(2, 3, 1.0);
    pg2.addEdge(9, 5, 1.0);

    haruki::Graph g2(pg2);
    ASSERT_EQ(10, g2.getNumVert());

    pg3.setNumVert(19);
    pg3.addEdge(0, 5, 1.0);
    pg3.addEdge(1, 3, 1.0);
    pg3.addEdge(2, 7, 1.0);
    pg3.addEdge(2, 3, 1.0);
    pg3.addEdge(9, 5, 1.0);

    haruki::Graph g3(pg3);
    ASSERT_EQ(19, g3.getNumVert());
}

TEST(GRAPH, GET_EDGE_COST) {
    haruki::GraphBuilder pg1, pg2, pg3;
    pg1.addEdge(0, 5, 1.1);
    pg1.addEdge(1, 3, 1.2);
    pg1.addEdge(2, 7, 1.3);
    pg1.addEdge(2, 3, 1.4);

    haruki::Graph g1(pg1);

    ASSERT_EQ(1.2, g1.getEdgeCost(1, 3));
    ASSERT_EQ(1.3, g1.getEdgeCost(2, 7));
    ASSERT_EQ(1.4, g1.getEdgeCost(2, 3));
    ASSERT_EQ(-1.0, g1.getEdgeCost(2, 1));
}

TEST(GRAPH, REMOVE_EDGE) {
    haruki::GraphBuilder pg1, pg2, pg3;
    pg1.addEdge(0, 5, 1.1);
    pg1.addEdge(1, 3, 1.2);
    pg1.addEdge(2, 7, 1.3);
    pg1.addEdge(2, 3, 1.4);

    haruki::Graph g1(pg1);

    ASSERT_EQ(1.2, g1.getEdgeCost(1, 3));
    g1.removeEdge(1, 3);
    ASSERT_EQ(-1, g1.getEdgeCost(1, 3));
}


TEST(GRAPH, GET_EDGES_OUT_ITER_1) {
    haruki::GraphBuilder pg1, pg2, pg3;
    pg1.addEdge(0, 5, 1.1);
    pg1.addEdge(0, 1, 1.3);
    pg1.addEdge(1, 3, 1.2);
    pg1.addEdge(0, 2, 1.2);
    pg1.addEdge(2, 7, 1.3);
    pg1.addEdge(2, 3, 1.4);

    haruki::Graph g1(pg1);

    haruki::EdgeOut::range it = g1.getEdgesOut(0);
    haruki::EdgeOut::iterator x = it.begin();
    ASSERT_DOUBLE_EQ(1.3, (*x).cost);
    ASSERT_EQ(0, (*x).tail);
    ASSERT_EQ(1, (*x).head);
    ++x;
    ASSERT_DOUBLE_EQ(1.2, (*x).cost);
    ASSERT_EQ(0, (*x).tail);
    ASSERT_EQ(2, (*x).head);
    ++x;
    ASSERT_DOUBLE_EQ(1.1, (*x).cost);
    ASSERT_EQ(0, (*x).tail);
    ASSERT_EQ(5, (*x).head);
    ++x;
    ASSERT_FALSE(it.end() != x);
}

TEST(GRAPH, GET_EDGES_OUT_ITER_2) {
    haruki::GraphBuilder pg1, pg2, pg3;
    pg1.addEdge(0, 5, 1.1);
    pg1.addEdge(0, 1, 1.3);
    pg1.addEdge(1, 3, 1.2);
    pg1.addEdge(0, 2, 1.2);
    pg1.addEdge(2, 7, 1.3);
    pg1.addEdge(2, 3, 1.4);

    haruki::Graph g1(pg1);

    haruki::EdgeOut::range it = g1.getEdgesOut(1);
    haruki::EdgeOut::iterator x = it.begin();
    ASSERT_DOUBLE_EQ(1.2, (*x).cost);
    ASSERT_EQ(1, (*x).tail);
    ASSERT_EQ(3, (*x).head);
    ++x;
    ASSERT_FALSE(it.end() != x);
}

TEST(GRAPH, GET_EDGES_OUT_ITER_3) {
    haruki::GraphBuilder pg1, pg2, pg3;
    pg1.addEdge(0, 5, 1.1);
    pg1.addEdge(0, 1, 1.3);
    pg1.addEdge(1, 3, 1.2);
    pg1.addEdge(0, 2, 1.2);
    pg1.addEdge(2, 7, 1.3);
    pg1.addEdge(2, 3, 1.4);

    haruki::Graph g1(pg1);

    haruki::EdgeOut::range it = g1.getEdgesOut(3);
    haruki::EdgeOut::iterator x = it.begin();
    ASSERT_FALSE(it.end() != x);
}

TEST(GRAPH, GET_EDGES_OUT_ITER_REMOVED) {
    haruki::GraphBuilder pg1, pg2, pg3;
    pg1.addEdge(0, 5, 1.1);
    pg1.addEdge(0, 1, 1.3);
    pg1.addEdge(1, 3, 1.2);
    pg1.addEdge(0, 2, 1.2);
    pg1.addEdge(0, 7, 1.03);
    pg1.addEdge(2, 7, 1.3);
    pg1.addEdge(2, 3, 1.4);

    haruki::Graph g1(pg1);

    g1.removeEdge(0, 5);

    haruki::EdgeOut::range it = g1.getEdgesOut(0);
    haruki::EdgeOut::iterator x = it.begin();
    ASSERT_DOUBLE_EQ(1.3, (*x).cost);
    ASSERT_EQ(0, (*x).tail);
    ASSERT_EQ(1, (*x).head);
    ++x;
    ASSERT_DOUBLE_EQ(1.2, (*x).cost);
    ASSERT_EQ(0, (*x).tail);
    ASSERT_EQ(2, (*x).head);
    ++x;
    ASSERT_DOUBLE_EQ(1.03, (*x).cost);
    ASSERT_EQ(0, (*x).tail);
    ASSERT_EQ(7, (*x).head);
    ++x;
    ASSERT_FALSE(it.end() != x);

    g1.resetEdgesRemoved();
    g1.removeEdge(0, 1);

    haruki::EdgeOut::range it2 = g1.getEdgesOut(0);
    haruki::EdgeOut::iterator x2 = it2.begin();
    ASSERT_DOUBLE_EQ(1.2, (*x2).cost);
    ASSERT_EQ(0, (*x2).tail);
    ASSERT_EQ(2, (*x2).head);
    ++x2;
    ASSERT_DOUBLE_EQ(1.1, (*x2).cost);
    ASSERT_EQ(0, (*x2).tail);
    ASSERT_EQ(5, (*x2).head);
    ++x2;
    ASSERT_DOUBLE_EQ(1.03, (*x2).cost);
    ASSERT_EQ(0, (*x2).tail);
    ASSERT_EQ(7, (*x2).head);
    ++x2;
    ASSERT_FALSE(it.end() != x2);
}

TEST(GRAPH, GET_EDGES_IN_ITER) {
    haruki::GraphBuilder pg1, pg2, pg3;
    pg1.addEdge(0, 5, 1.1);
    pg1.addEdge(0, 1, 1.3);
    pg1.addEdge(1, 3, 1.2);
    pg1.addEdge(0, 2, 1.2);
    pg1.addEdge(2, 7, 1.3);
    pg1.addEdge(2, 3, 1.4);

    haruki::Graph g1(pg1);

    haruki::EdgeIn::range it = g1.getEdgesIn(3);
    haruki::EdgeIn::iterator x = it.begin();

    ASSERT_DOUBLE_EQ(1.2, (*x).cost);
    ASSERT_EQ(1, (*x).tail);
    ASSERT_EQ(3, (*x).head);
    ++x;
    ASSERT_DOUBLE_EQ(1.4, (*x).cost);
    ASSERT_EQ(2, (*x).tail);
    ASSERT_EQ(3, (*x).head);
    ++x;
    ASSERT_FALSE(it.end() != x);
}

TEST(GRAPH, GET_EDGES_IN_ITER_REMOVE_ARTIFICIAL_FENG) {
    haruki::GraphBuilder pg1, pg2, pg3;
    pg1.setNumVert(10);
    pg1.addEdge(0, 5, 1.1);
    pg1.addEdge(0, 1, 1.3);
    pg1.addEdge(1, 3, 1.2);
    pg1.addEdge(0, 2, 1.2);
    pg1.addEdge(2, 7, 1.3);
    pg1.addEdge(2, 3, 1.4);

    pg1.addEdge(1, 9, 0.0);
    pg1.addEdge(3, 9, 0.0);
    pg1.addEdge(8, 9, 0.0);

    haruki::Graph g1(pg1);

    haruki::EdgeIn::range it = g1.getEdgesIn(9);
    haruki::EdgeIn::iterator x = it.begin();

    ASSERT_DOUBLE_EQ(0.0, (*x).cost);
    ASSERT_EQ(1, (*x).tail);
    ASSERT_EQ(9, (*x).head);
    ++x;
    ASSERT_DOUBLE_EQ(0.0, (*x).cost);
    ASSERT_EQ(3, (*x).tail);
    ASSERT_EQ(9, (*x).head);
    ++x;
    ASSERT_DOUBLE_EQ(0.0, (*x).cost);
    ASSERT_EQ(8, (*x).tail);
    ASSERT_EQ(9, (*x).head);
    ++x;
    ASSERT_FALSE(it.end() != x);

    std::vector<int> v;
    v.push_back(3);
    g1.fengRemoveArtificialEdges(v);

    haruki::EdgeIn::range it2 = g1.getEdgesIn(9);
    haruki::EdgeIn::iterator x2 = it2.begin();

    ASSERT_DOUBLE_EQ(0.0, (*x2).cost);
    ASSERT_EQ(1, (*x2).tail);
    ASSERT_EQ(9, (*x2).head);
    ++x2;
    ASSERT_DOUBLE_EQ(0.0, (*x2).cost);
    ASSERT_EQ(8, (*x2).tail);
    ASSERT_EQ(9, (*x2).head);
    ++x2;
    ASSERT_FALSE(it2.end() != x2);

    g1.setRemovedForIncomingEdges(9, false);

    haruki::EdgeIn::range it3 = g1.getEdgesIn(9);
    haruki::EdgeIn::iterator x3 = it3.begin();

    ASSERT_DOUBLE_EQ(0.0, (*x3).cost);
    ASSERT_EQ(1, (*x3).tail);
    ASSERT_EQ(9, (*x3).head);
    ++x3;
    ASSERT_DOUBLE_EQ(0.0, (*x3).cost);
    ASSERT_EQ(3, (*x3).tail);
    ASSERT_EQ(9, (*x3).head);
    ++x3;
    ASSERT_DOUBLE_EQ(0.0, (*x3).cost);
    ASSERT_EQ(8, (*x3).tail);
    ASSERT_EQ(9, (*x3).head);
    ++x3;
    ASSERT_FALSE(it3.end() != x3);

    v.push_back(8);

    g1.fengRemoveArtificialEdges(v);

    haruki::EdgeIn::range it4 = g1.getEdgesIn(9);
    haruki::EdgeIn::iterator x4 = it2.begin();

    ASSERT_DOUBLE_EQ(0.0, (*x4).cost);
    ASSERT_EQ(1, (*x4).tail);
    ASSERT_EQ(9, (*x4).head);
    ++x4;
    ASSERT_FALSE(it4.end() != x4);
}
