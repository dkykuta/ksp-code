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
#include <vector>
#include <functional>
#include <utility>

TEST(PRIORITY_QUEUE, DEFAULT_MAX_HEAP) {

    std::priority_queue<int> pq;

    pq.push(4);
    pq.push(8);
    pq.push(3);
    pq.push(1);
    pq.push(9);

    ASSERT_FALSE(pq.empty());

    ASSERT_DOUBLE_EQ (9, pq.top());
    pq.pop();
    ASSERT_DOUBLE_EQ (8, pq.top());
    pq.pop();
    ASSERT_DOUBLE_EQ (4, pq.top());
    pq.pop();
    ASSERT_DOUBLE_EQ (3, pq.top());
    pq.pop();
    ASSERT_DOUBLE_EQ (1, pq.top());
    pq.pop();

    ASSERT_TRUE(pq.empty());
}

TEST(PRIORITY_QUEUE, MIN_HEAP) {

    std::priority_queue<int, std::vector<int>, std::greater<int> > pq;

    pq.push(4);
    pq.push(8);
    pq.push(3);
    pq.push(3);
    pq.push(1);
    pq.push(9);

    ASSERT_DOUBLE_EQ (1, pq.top());
    pq.pop();
    ASSERT_DOUBLE_EQ (3, pq.top());
    pq.pop();
    ASSERT_DOUBLE_EQ (3, pq.top());
    pq.pop();
    ASSERT_DOUBLE_EQ (4, pq.top());
    pq.pop();
    ASSERT_DOUBLE_EQ (8, pq.top());
    pq.pop();
    ASSERT_DOUBLE_EQ (9, pq.top());
    pq.pop();
}

typedef std::pair<double, int> pairVertCost;

TEST(PRIORITY_QUEUE, PAIR_VERT_COST) {

    std::priority_queue<pairVertCost, std::vector<pairVertCost>, std::greater<pairVertCost> > pq;

    pq.push(std::make_pair(2.3, 8));
    pq.push(std::make_pair(3.1, 6));
    pq.push(std::make_pair(3.1, 2));
    pq.push(std::make_pair(1.3, 3));

    pairVertCost pr = pq.top();
    ASSERT_DOUBLE_EQ (1.3, pr.first);
    ASSERT_EQ (3, pr.second);
    pq.pop();

    pr = pq.top();
    ASSERT_DOUBLE_EQ (2.3, pr.first);
    ASSERT_EQ (8, pr.second);
    pq.pop();

    pr = pq.top();
    ASSERT_DOUBLE_EQ (3.1, pr.first);
    ASSERT_EQ (2, pr.second);
    pq.pop();

    pr = pq.top();
    ASSERT_DOUBLE_EQ (3.1, pr.first);
    ASSERT_EQ (6, pr.second);
    pq.pop();


}
