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

TEST(PATH, CREATION) {
    haruki::Path path1;
}

TEST(PATH, INSERT_VERTEX) {
    haruki::Path path1;
    path1.addEdge(0, 1, 5.7);
    ASSERT_EQ (2, path1.size());
    ASSERT_DOUBLE_EQ (5.7, path1.cost());
}

TEST(PATH, INSERT_VERTEX_ORDER) {
    haruki::Path path1;
    path1.addEdge(0, 10, 5.7);
    path1.addEdge(10, 39, 2.1);
    path1.addEdge(39, 2, 5.5);
    ASSERT_EQ (4, path1.size());

    const std::vector<int>& vertList = path1.getVertList();
    ASSERT_EQ (4, vertList.size());
    ASSERT_EQ (0, vertList[0]);
    ASSERT_EQ (10, vertList[1]);
    ASSERT_EQ (39, vertList[2]);
    ASSERT_EQ (2, vertList[3]);

}

TEST(PATH, INSERT_VERTEX_COST) {
    haruki::Path path1;
    path1.addEdge(0, 1, 5.7);
    path1.addEdge(1, 3, 2.1);
    path1.addEdge(3, 2, 5.5);
    ASSERT_DOUBLE_EQ (13.3, path1.cost());
}

TEST(PATH, SUBPATH) {
    haruki::Path path1;
    path1.addEdge(0, 1, 5.7);
    path1.addEdge(1, 3, 2.1);
    path1.addEdge(3, 2, 5.5);
    path1.addEdge(2, 7, 1.1);
    path1.addEdge(7, 8, 3.6);
    path1.addEdge(8, 5, 1.5);

    haruki::Path sp1 = path1.subpath(0, 2);

    ASSERT_EQ(2, sp1.size());
    ASSERT_EQ(0, sp1.getVertList()[0]);
    ASSERT_EQ(1, sp1.getVertList()[1]);

    ASSERT_DOUBLE_EQ(5.7, sp1.cost());

    haruki::Path sp2 = path1.subpath(2, 5);

    ASSERT_EQ(3, sp2.size());
    ASSERT_EQ(3, sp2.getVertList()[0]);
    ASSERT_EQ(2, sp2.getVertList()[1]);
    ASSERT_EQ(7, sp2.getVertList()[2]);

    ASSERT_DOUBLE_EQ(6.6, sp2.cost());
}

TEST(PATH, INVALID_EDGE) {
    haruki::Path path1;

    path1.addEdge(0, 5, 4.6);
    try {
        path1.addEdge(9, 1, 1.1);
        FAIL() << "Invalid edge was added to path";
    }
    catch(std::invalid_argument expected){}
}

TEST(PATH, COMPARE_SUBPATHS) {
    haruki::Path path1, path2;

    path1.addEdge(0, 2, 1);
    path1.addEdge(2, 3, 1);
    path1.addEdge(3, 6, 1);
    path1.addEdge(6, 7, 1);

    ASSERT_EQ(5, path1.size());

    path2.addEdge(4, 2, 1);
    path2.addEdge(2, 3, 1);
    path2.addEdge(3, 6, 1);
    path2.addEdge(6, 7, 1);
    path2.addEdge(7, 9, 1);

    ASSERT_EQ(6, path2.size());

    ASSERT_FALSE(path1.subpath(0, 1) == path2.subpath(0, 1));
    ASSERT_FALSE(path1.subpath(0, 2) == path2.subpath(0, 2));
    ASSERT_TRUE(path1.subpath(1, 2) == path2.subpath(1, 2));
    ASSERT_TRUE(path1.subpath(1, 3) == path2.subpath(1, 3));
    ASSERT_TRUE(path1.subpath(1, 4) == path2.subpath(1, 4));
    ASSERT_TRUE(path1.subpath(1, 5) == path2.subpath(1, 5));
    ASSERT_FALSE(path1.subpath(1, 6) == path2.subpath(1, 6));
}

TEST(PATH, ADD) {
    haruki::Path path1, path2;

    path1.addEdge(0, 2, 1);
    path1.addEdge(2, 3, 1);

    path2.addEdge(3, 6, 1);
    path2.addEdge(6, 7, 1);
    path2.addEdge(7, 9, 1);

    haruki::Path path3 = path1 + path2;
    ASSERT_EQ(6, path3.size());
    ASSERT_EQ(0, path3.getVertList()[0]);
    ASSERT_EQ(2, path3.getVertList()[1]);
    ASSERT_EQ(3, path3.getVertList()[2]);
    ASSERT_EQ(6, path3.getVertList()[3]);
    ASSERT_EQ(7, path3.getVertList()[4]);
    ASSERT_EQ(9, path3.getVertList()[5]);
}
