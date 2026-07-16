#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

void printUsage(const char *name) {
  std::cerr
      << "Usage: " << name << " -w <size> -range <max_value>\n"
      << "       " << name << " <size> <max_value>\n"
      << "Generates two binary files:\n"
      << "  g-matrices.bin  - header [w,w,w] then first matrix and second "
         "matrix\n"
      << "  g-product.bin   - product of the two matrices\n"
      << "Each matrix element is stored as a 32-bit float. Header values are "
         "32-bit ints.\n"
      << "If <max_value> is <= 0, the value range defaults to [-500, 500].\n";
}

bool parseArgs(int argc, char *argv[], int &size, float &maxValue) {
  if (argc == 3) {
    try {
      size = std::stoi(argv[1]);
      maxValue = static_cast<float>(std::stof(argv[2]));
      return true;
    } catch (...) {
      return false;
    }
  }

  if (argc == 5) {
    std::string a1 = argv[1];
    std::string a2 = argv[2];
    std::string a3 = argv[3];
    std::string a4 = argv[4];
    try {
      if ((a1 == "-w" || a1 == "--w") && (a3 == "-range" || a3 == "--range")) {
        size = std::stoi(a2);
        maxValue = static_cast<float>(std::stof(a4));
        return true;
      }
      if ((a1 == "-range" || a1 == "--range") && (a3 == "-w" || a3 == "--w")) {
        maxValue = static_cast<float>(std::stof(a2));
        size = std::stoi(a4);
        return true;
      }
    } catch (...) {
      return false;
    }
  }

  return false;
}

int main(int argc, char *argv[]) {
  int size = 0;
  float maxValue = 500.0f;

  if (!parseArgs(argc, argv, size, maxValue)) {
    printUsage(argv[0]);
    return 1;
  }

  if (size <= 0) {
    std::cerr << "Matrix size must be a positive integer.\n";
    return 2;
  }

  if (maxValue <= 0.0f) {
    maxValue = 500.0f;
  }

  maxValue = std::min(maxValue, 500.0f);
  const float minValue = -maxValue;

  const std::size_t count =
      static_cast<std::size_t>(size) * static_cast<std::size_t>(size);
  std::vector<float> matrixA(count);
  std::vector<float> matrixB(count);
  std::vector<float> product(count);

  std::random_device rd;
  std::mt19937 rng(static_cast<std::uint32_t>(rd()));
  std::uniform_real_distribution<float> dist(minValue, maxValue);

  for (std::size_t i = 0; i < count; ++i) {
    matrixA[i] = dist(rng);
    matrixB[i] = dist(rng);
  }

  for (int row = 0; row < size; ++row) {
    for (int col = 0; col < size; ++col) {
      double sum = 0.0;
      for (int k = 0; k < size; ++k) {
        sum += static_cast<double>(matrixA[row * size + k]) *
               static_cast<double>(matrixB[k * size + col]);
      }
      product[row * size + col] = static_cast<float>(sum);
    }
  }

  {
    std::ofstream out("g-matrices.bin", std::ios::binary);
    if (!out) {
      std::cerr << "Failed to open g-matrices.bin for writing.\n";
      return 3;
    }
    int32_t header[3] = {size, size, size};
    out.write(reinterpret_cast<const char *>(header), sizeof(header));
    out.write(reinterpret_cast<const char *>(matrixA.data()),
              count * sizeof(float));
    out.write(reinterpret_cast<const char *>(matrixB.data()),
              count * sizeof(float));
  }

  {
    std::ofstream out("g-product.bin", std::ios::binary);
    if (!out) {
      std::cerr << "Failed to open g-product.bin for writing.\n";
      return 4;
    }
    out.write(reinterpret_cast<const char *>(product.data()),
              count * sizeof(float));
  }

  std::cout << "Generated g-matrices.bin and g-product.bin for size " << size
            << " with value range [" << minValue << ", " << maxValue << "]\n";
  return 0;
}
