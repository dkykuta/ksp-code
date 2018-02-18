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
#include "dimacsreader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>

#include "graph.hpp"

namespace haruki
{
namespace dimacs
{

using std::string;

Graph *readGrFile(std::string filepath)
{
  int n, m;
  string line;
  std::ifstream myfile;
  Graph *g = nullptr;
  GraphBuilder pg;

  std::set<std::pair<int,int> > edgesAdded;

  myfile.open(filepath);

  if (myfile.is_open())
  {
    while (getline(myfile, line))
    {
      std::stringstream lstream(line);
      char id;
      int u, v, w;
      string aux;
      lstream >> id;
      switch (id)
      {
      case 'p':
        lstream >> aux >> n >> m;
        pg.setNumVert(n);
        pg.setNumEdges(m);
        break;
      case 'a':
        lstream >> u >> v >> w;
        if (edgesAdded.find(std::pair<int,int>(u, v)) != edgesAdded.end()) {
          continue;
        }
        pg.addEdge(u-1, v-1, w);
        edgesAdded.insert(std::pair<int,int>(u, v));
      case 'c':
      default:
        continue;
      }
    }
    myfile.close();
    g = new Graph(pg);
  }
  else
  {
    std::cout << "it's closed" << std::endl;
  }

  return g;
}
}
}
