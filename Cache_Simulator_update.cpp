#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

ofstream Ofile("output.txt");
ifstream Ifile("input.txt");
struct CacheLine {
  bool valid;       // Valid bit indicating if the cache line is valid or not
  unsigned int tag; // Tag to identify the memory block stored in the cache line
};

void Cache_Simulation(const string &filename, unsigned int S, unsigned int L, unsigned int Time) {
  unsigned int cache_lines = S / L;
  vector<vector<CacheLine>> cache(cache_lines, vector<CacheLine>(L)); // 2D vector representing the cache

  unsigned int accesses = 0;
  unsigned int hits = 0;
  unsigned int misses = 0;

  ifstream inputFile(filename);


  unsigned int memory_address;
  while (inputFile >> memory_address) {
    ++accesses;
    // Calculate the cache index, tag, and offset for the current memory address
    unsigned int index = (memory_address / L) % cache_lines; // Calculate the cache index considering both the cache lines and line size
    unsigned int tag = memory_address / (cache_lines * L); // Calculate the cache tag
    unsigned int offset = memory_address % L; // Calculate the offset within the cache line

    if (cache[index][offset].valid && cache[index][offset].tag == tag) {
      // Cache hit
      ++hits;
    } else {
      // Cache miss
      ++misses;
      // Update the cache line with the new memory block
      cache[index][offset].valid = true;
      cache[index][offset].tag = tag;
    }

    // Output the valid bits and tags of all cache entries
    Ofile << "Valid bits and tags: " << endl;
    for (unsigned int i = 0; i < cache_lines; ++i) {
      for (unsigned int j = 0; j < L; ++j) {
        Ofile << i << ":" << j << " " << cache[i][j].valid << ":" << cache[i][j].tag << endl;
      }
    }
    Ofile << endl;

    // Output the total number of accesses, hit and miss ratios, and AMAT
    Ofile << "Total number of accesses: " << accesses << endl;
    Ofile << "Hit ratio: " << static_cast<double>(hits) / accesses << endl;
    Ofile << "Miss ratio: " << static_cast<double>(misses) / accesses << endl;
    Ofile << "AMAT: " << (hits * Time + misses * (Time + 100)) / static_cast<double>(accesses) << " cycles" << endl;
    Ofile << endl;
  }
  inputFile.close();
}

int main() {
  string filename = "access_sequence.txt";
  unsigned int S; // (8192) 8 KB (Cache Size)
  unsigned int L;    // (64) 64 bytes (Line Size)
  unsigned int Time;   // (5) clock cycles (Access Time)
  Ifile >> S;
  Ifile >> L;
  Ifile >> Time;

  Cache_Simulation(filename, S, L, Time);

  return 0;
}
