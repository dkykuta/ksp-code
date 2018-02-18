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
#include <queue>
#include <iostream>
#include <functional>
#include <utility>
#include <set>
#include <vector>
#include <../src/path.hpp>

TEST(SET, DEFAULT_SET) {

    std::set<int> st;

    ASSERT_TRUE(st.empty());

    st.insert(4);

    ASSERT_FALSE(st.empty());

    st.insert(6);

    ASSERT_EQ(2, st.size());

    ASSERT_NE(st.end(), st.find(4));
    ASSERT_EQ(st.end(), st.find(9));

    st.insert(4);

    ASSERT_EQ(2, st.size());
    ASSERT_NE(st.end(), st.find(4));
    ASSERT_EQ(st.end(), st.find(9));

    st.insert(9);
    ASSERT_NE(st.end(), st.find(9));

    st.erase(4);
    ASSERT_EQ(st.end(), st.find(4));
}

TEST(SET, PATH_SET) {
    std::set<haruki::Path> path_set;

    ASSERT_TRUE(path_set.empty());

    haruki::Path path1, path2, path3;
    path1.addEdge(0, 10, 5.7);
    path1.addEdge(10, 39, 2.1);
    path1.addEdge(39, 2, 5.5);

    path2.addEdge(0, 12, 99.1);
    path2.addEdge(12, 1, 12.7);
    path2.addEdge(1, 7, 3.5);

    path3.addEdge(0, 10, 5.7);
    path3.addEdge(10, 39, 2.1);
    path3.addEdge(39, 2, 5.5);

    path_set.insert(path1);

    ASSERT_FALSE(path_set.empty());

    ASSERT_NE(path_set.end(), path_set.find(path1));
    ASSERT_EQ(path_set.end(), path_set.find(path2));
    ASSERT_NE(path_set.end(), path_set.find(path3));

    path_set.insert(path2);
    ASSERT_EQ(2, path_set.size());

    path_set.insert(path3);
    ASSERT_EQ(2, path_set.size());

    path_set.erase(path3);
    ASSERT_EQ(1, path_set.size());
    ASSERT_EQ(path_set.end(), path_set.find(path1));
    ASSERT_EQ(path_set.end(), path_set.find(path3));
}

TEST(SET, SET_FROM_VECTOR) {
    std::vector<int> v;
    v.push_back(1);
    v.push_back(4);
    v.push_back(6);
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    std::set<int> s(v.begin(), v.end());

    ASSERT_EQ(7, v.size());
    ASSERT_EQ(5, s.size());
    ASSERT_NE(s.end(), s.find(1));
    ASSERT_NE(s.end(), s.find(2));
    ASSERT_NE(s.end(), s.find(3));
    ASSERT_NE(s.end(), s.find(4));
    ASSERT_NE(s.end(), s.find(6));
}
