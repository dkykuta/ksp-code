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
#include "hybridksp.hpp"
#include "ksp.hpp"

using std::string;

#ifdef HRK_COUNT_
int hrk_dijkstra_count;
int hrk_pascoal_shortcuts;
int hrk_pascoal_fallback;
int hrk_feng_yellow_total_size;
int hrk_feng_yellow_graph_count;
int hrk_feng_yellow_path_size;
int hrk_feng_yellow_path_count;
int hrk_response_path_size;
int hrk_response_count;

#ifdef HRK_VERBOSE_
std::vector<int> hrk_feng_yellow_path_list;
#endif
#endif

int main(int argc, char* argv[]) {
  if (argc != 6) {
    std::cout << " Usage: " << argv[0] << "<algorithm> <input_file> <s> <t> <k>" << std::endl;
    exit(0);
  }

  int s, t, k;

#ifdef HRK_COUNT_
  hrk_dijkstra_count = 0;
  hrk_pascoal_shortcuts = 0;
  hrk_pascoal_fallback = 0;
  hrk_feng_yellow_total_size = 0;
  hrk_feng_yellow_graph_count = 0;
  hrk_feng_yellow_path_size = 0;
  hrk_feng_yellow_path_count = 0;
  hrk_response_path_size = 0;
  hrk_response_count = 0;
#endif

  std::stringstream ss1(argv[3]);
  if (!(ss1 >> s)) {
      std::cerr << "Invalid start vertex " << argv[2] << std::endl;
  }
  std::stringstream ss2(argv[4]);
  if (!(ss2 >> t)) {
      std::cerr << "Invalid target vertex " << argv[3] << std::endl;
  }
  std::stringstream ss3(argv[5]);
  if (!(ss3 >> k)) {
      std::cerr << "Invalid number of paths to find " << argv[4] << std::endl;
  }

  haruki::Graph *g = haruki::dimacs::readGrFile(std::string(argv[2]));
  if (g == nullptr) {
    return 0;
  }

  std::string algorithm = std::string(argv[1]);

  if (algorithm == "yen") {
    std::cout << "Algoritmo de Yen" << std::endl << std::endl;
    haruki::KSP<haruki::YenKSP> yen;
    std::vector<haruki::Path> result = yen.run(*g, s, t, k);

    std::cout << "# Paths: " << result.size() << std::endl;

    for(std::vector<haruki::Path>::iterator it = result.begin(); it != result.end(); it++) {
      std::cout << "[" << (*it).cost() << "] ";
      std::vector<int> vertList = (*it).getVertList();
      for (std::vector<int>::iterator it2 = vertList.begin(); it2 != vertList.end(); it2++) {
        std::cout << *it2 << " ";
      }
      std::cout << std::endl;
    }
  } else if (algorithm == "pascoal") {
    std::cout << "Algoritmo de Pascoal" << std::endl << std::endl;
    haruki::KSP<haruki::PascoalKSP> pascoal;
    std::vector<haruki::Path> result2 = pascoal.run(*g, s, t, k);

    std::cout << "# Paths: " << result2.size() << std::endl;

    for(std::vector<haruki::Path>::iterator it = result2.begin(); it != result2.end(); it++) {
      std::cout << "[" << (*it).cost() << "] ";
      std::vector<int> vertList = (*it).getVertList();
      for (std::vector<int>::iterator it2 = vertList.begin(); it2 != vertList.end(); it2++) {
        std::cout << *it2 << " ";
      }
      std::cout << std::endl;
    }
  } else if (algorithm == "feng") {
    std::cout << "Algoritmo de Feng" << std::endl << std::endl;
    haruki::KSP<haruki::FengKSP> feng;
    std::vector<haruki::Path> result3 = feng.run(*g, s, t, k);

    std::cout << "# Paths: " << result3.size() << std::endl;

    for(std::vector<haruki::Path>::iterator it = result3.begin(); it != result3.end(); it++) {
      std::cout << "[" << (*it).cost() << "] ";
      std::vector<int> vertList = (*it).getVertList();
      for (std::vector<int>::iterator it2 = vertList.begin(); it2 != vertList.end(); it2++) {
        std::cout << *it2 << " ";
      }
      std::cout << std::endl;
    }
  } else if (algorithm == "hybrid") {
    std::cout << "Algoritmo Híbrido Proposto" << std::endl << std::endl;
    haruki::KSP<haruki::HybridKSP> hybrid;
    std::vector<haruki::Path> result4 = hybrid.run(*g, s, t, k);

    std::cout << "# Paths: " << result4.size() << std::endl;

    for(std::vector<haruki::Path>::iterator it = result4.begin(); it != result4.end(); it++) {
      std::cout << "[" << (*it).cost() << "] ";
      std::vector<int> vertList = (*it).getVertList();
      for (std::vector<int>::iterator it2 = vertList.begin(); it2 != vertList.end(); it2++) {
        std::cout << *it2 << " ";
      }
      std::cout << std::endl;
    }
  }

  delete g;

#ifdef HRK_COUNT_
  std::cout << "DIJKSTRA_COUNT|" << hrk_dijkstra_count << std::endl;
  std::cout << "PASCOAL_SHORTCUT|" << hrk_pascoal_shortcuts << std::endl;
  std::cout << "PASCOAL_FALLBACK|" << hrk_pascoal_fallback << std::endl;
  double pascoal_success = 0;
  if ((hrk_pascoal_shortcuts + hrk_pascoal_fallback) != 0) {
    pascoal_success = ((double)hrk_pascoal_shortcuts) / (hrk_pascoal_shortcuts + hrk_pascoal_fallback);
  }
  std::cout << "PASCOAL_SUCCESS_RATE|" << pascoal_success << std::endl;
  std::cout << "FENG_YELLOW_PATH_SIZE|" << hrk_feng_yellow_path_size << std::endl;
  std::cout << "FENG_YELLOW_PATH_COUNT|" << hrk_feng_yellow_path_count << std::endl;
  double feng_mean_size = 0;
  if (hrk_feng_yellow_path_count != 0) {
    feng_mean_size = ((double)hrk_feng_yellow_path_size) / hrk_feng_yellow_path_count;
    #ifdef HRK_VERBOSE_
    std::cout << "FENG_YELLOW_PATH_LIST|";
    for (std::vector<int>::iterator it = hrk_feng_yellow_path_list.begin(); it != hrk_feng_yellow_path_list.end(); ++it) {
      std::cout << *it << " ";
    }
    std::cout << std::endl;
    #endif
  }
  std::cout << "FENG_YELLOW_PATH_MEAN_SIZE|" << feng_mean_size << std::endl;
  std::cout << "RESPONSE_PATH_SIZE|" << hrk_response_path_size << std::endl;
  std::cout << "RESPONSE_PATH_COUNT|" << hrk_response_count << std::endl;
  double mean_response_path_size = 0;
  if (hrk_response_count != 0) {
    mean_response_path_size = ((double)hrk_response_path_size) / hrk_response_count;
  }
  std::cout << "RESPONSE_PATH_MEAN_SIZE|" << mean_response_path_size << std::endl;
#endif
}
