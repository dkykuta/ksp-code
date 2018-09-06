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
#include <iostream>
#include <sstream>
#include "graph.hpp"
#include "dimacsreader.hpp"
#include "yenksp.hpp"
#include "pascoalksp.hpp"
#include "fengksp.hpp"
#include "hybridfengksp.hpp"
#include "humanfriendlyksp.hpp"

using std::string;

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cout << " Usage: " << argv[0] << " <input_file> <s> <t> <k>" << std::endl;
    exit(0);
  }

  int s, t, k;
  std::stringstream ss1(argv[2]);
  if (!(ss1 >> s)) {
      std::cerr << "Invalid start vertex " << argv[2] << std::endl;
  }
  std::stringstream ss2(argv[3]);
  if (!(ss2 >> t)) {
      std::cerr << "Invalid target vertex " << argv[3] << std::endl;
  }
  std::stringstream ss3(argv[4]);
  if (!(ss3 >> k)) {
      std::cerr << "Invalid number of paths to find " << argv[4] << std::endl;
  }

  haruki::Graph *g = haruki::dimacs::readGrFile(std::string(argv[1]));
  if (g == nullptr) {
    return 0;
  }
  // std::cout << g->getEdgeCost(0, 2) << std::endl;
  // 0 5


  std::cout << "Algoritmo de Yen" << std::endl << std::endl;
  haruki::HumanFriendlyKSP<haruki::YenKSP> yen;
  std::vector<haruki::Path> result = yen.run(*g, s, t, k);

  std::cout << "# Paths: " << result.size() << std::endl;

  for(std::vector<haruki::Path>::iterator it = result.begin(); it != result.end(); it++) {
    std::cout << "  [cost: " << (*it).cost() << "]" << std::endl;
    std::vector<int> vertList = (*it).getVertList();
    for (std::vector<int>::iterator it2 = vertList.begin(); it2 != vertList.end(); it2++) {
      std::cout << *it2 << " ";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl;

  std::cout << "Algoritmo de Pascoal" << std::endl << std::endl;
  haruki::HumanFriendlyKSP<haruki::PascoalKSP> pascoal;
  std::vector<haruki::Path> result2 = pascoal.run(*g, s, t, k);

  std::cout << "# Paths: " << result.size() << std::endl;

  for(std::vector<haruki::Path>::iterator it = result2.begin(); it != result2.end(); it++) {
    std::cout << "  [cost: " << (*it).cost() << "]" << std::endl;
    std::vector<int> vertList = (*it).getVertList();
    for (std::vector<int>::iterator it2 = vertList.begin(); it2 != vertList.end(); it2++) {
      std::cout << *it2 << " ";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl;

  std::cout << "Algoritmo de Feng" << std::endl << std::endl;
  haruki::HumanFriendlyKSP<haruki::FengKSP> feng;
  std::vector<haruki::Path> result3 = feng.run(*g, s, t, k);

  std::cout << "# Paths: " << result3.size() << std::endl;

  for(std::vector<haruki::Path>::iterator it = result3.begin(); it != result3.end(); it++) {
    std::cout << "  [cost: " << (*it).cost() << "]" << std::endl;
    std::vector<int> vertList = (*it).getVertList();
    for (std::vector<int>::iterator it2 = vertList.begin(); it2 != vertList.end(); it2++) {
      std::cout << *it2 << " ";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl;

  std::cout << "Algoritmo de Feng Alterado (hibrido)" << std::endl << std::endl;
  haruki::HumanFriendlyKSP<haruki::HybridFengKSP> fengAlterado;
  std::vector<haruki::Path> result5 = fengAlterado.run(*g, s, t, k);

  std::cout << "# Paths: " << result5.size() << std::endl;

  for(std::vector<haruki::Path>::iterator it = result5.begin(); it != result5.end(); it++) {
    std::cout << "  [cost: " << (*it).cost() << "]" << std::endl;
    std::vector<int> vertList = (*it).getVertList();
    for (std::vector<int>::iterator it2 = vertList.begin(); it2 != vertList.end(); it2++) {
      std::cout << *it2 << " ";
    }
    std::cout << std::endl;
  }

  delete g;
}
