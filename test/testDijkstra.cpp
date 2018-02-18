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
#include "../src/path.hpp"
#include "../src/dijkstra.hpp"

TEST(DIJKSTRA, FUNCTION) {
    haruki::GraphBuilder pg;
    pg.addEdge(0, 1, 2);
    pg.addEdge(0, 2, 999);
    pg.addEdge(1, 2, 2);
    pg.addEdge(1, 4, 999);
    pg.addEdge(2, 3, 2);
    pg.addEdge(3, 4, 2);

    haruki::Graph g(pg);

    haruki::Path path1 = haruki::dijkstra::minPath(g, 0, 4);

    EXPECT_EQ (5, path1.size());
    EXPECT_EQ (0, path1.getVertList()[0]);
    EXPECT_EQ (1, path1.getVertList()[1]);
    EXPECT_EQ (2, path1.getVertList()[2]);
    EXPECT_EQ (3, path1.getVertList()[3]);
    EXPECT_EQ (4, path1.getVertList()[4]);

    EXPECT_DOUBLE_EQ(8.0, path1.cost());
}

TEST(DIJKSTRA, BEST_PATH_ZERO_COST) {
    haruki::GraphBuilder pg;
    pg.addEdge(0, 1, 0);
    pg.addEdge(0, 2, 1);
    pg.addEdge(1, 2, 0);
    pg.addEdge(1, 4, 1);
    pg.addEdge(2, 3, 0);
    pg.addEdge(3, 4, 0);

    haruki::Graph g(pg);

    haruki::Path path1 = haruki::dijkstra::minPath(g, 0, 4);

    EXPECT_EQ (5, path1.size());
    EXPECT_EQ (0, path1.getVertList()[0]);
    EXPECT_EQ (1, path1.getVertList()[1]);
    EXPECT_EQ (2, path1.getVertList()[2]);
    EXPECT_EQ (3, path1.getVertList()[3]);
    EXPECT_EQ (4, path1.getVertList()[4]);

    EXPECT_DOUBLE_EQ(0.0, path1.cost());
}

TEST(DIJKSTRA, PARENTS) {
    haruki::GraphBuilder pg;
    pg.addEdge(0, 1, 2);
    pg.addEdge(0, 2, 999);
    pg.addEdge(1, 2, 2);
    pg.addEdge(1, 4, 999);
    pg.addEdge(2, 3, 2);
    pg.addEdge(3, 4, 2);

    haruki::Graph g(pg);

    std::vector<int> parents;
    std::vector<double> distances;

    haruki::dijkstra::dijkstra_parents(g, 0, 3, false, parents, distances);
    ASSERT_EQ(5, parents.size());
    ASSERT_EQ(0, parents[0]);
    ASSERT_EQ(0, parents[1]);
    ASSERT_EQ(1, parents[2]);
    ASSERT_EQ(2, parents[3]);
    ASSERT_EQ(3, parents[4]);
}

TEST(DIJKSTRA, FUNCTION_EDGE_CASE) {
    haruki::GraphBuilder pg;
    pg.setNumVert(3200);
    pg.addEdge(1, 12, 1182);
    pg.addEdge(1, 47, 1234);
    pg.addEdge(12, 1, 0);
    pg.addEdge(47, 1, 0);

    haruki::Graph g(pg);

    haruki::Path path1 = haruki::dijkstra::minPath(g, 1, 3000);

    EXPECT_EQ (0, path1.size());

    EXPECT_DOUBLE_EQ(0.0, path1.cost());
}

TEST(DIJKSTRA, WITH_EDGE_REMOVED) {
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

    g.removeEdge(0, 1);

    haruki::Path path1 = haruki::dijkstra::minPath(g, 0, 6);

    EXPECT_EQ (4, path1.size());
    EXPECT_EQ (0, path1.getVertList()[0]);
    EXPECT_EQ (2, path1.getVertList()[1]);
    EXPECT_EQ (4, path1.getVertList()[2]);
    EXPECT_EQ (6, path1.getVertList()[3]);
}
