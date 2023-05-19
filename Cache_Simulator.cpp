#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

struct CacheLine{
  bool valid;       // Valid bit indicating if the cache line is valid or not
  unsigned int tag; // Tag to identify the memory block stored in the cache line
};

void Cache_Simulation(const string &filename, unsigned int S, unsigned int L, unsigned int Time){
  unsigned int cache_lines = S / L;
  vector<CacheLine> cache(cache_lines); // Vector representing the cache

  unsigned int accesses = 0;
  unsigned int hits = 0;
  unsigned int misses = 0;

  ifstream inputFile(filename);
  if (!inputFile) {
    cerr << "Error opening input file." << endl;
    return;
  }

  unsigned int memory_address;
  while (inputFile >> memory_address) {
    ++accesses;
    // Calculate the cache index and tag for the current memory address
    unsigned int index = (memory_address / L) % cache_lines;
    unsigned int tag = memory_address / (cache_lines * L);

    if (cache[index].valid && cache[index].tag == tag) {
      // Cache hit
      ++hits;
    }

    else {
      // Cache miss
      ++misses;
      // Update the cache line with the new memory block
      cache[index].valid = true;
      cache[index].tag = tag;
    }
    // Output the valid bits and tags of all cache entries
    cout << "Valid bits and tags: ";
    int i = 0;
    for (const auto &line : cache) {
      cout << i << " " << line.valid << ":" << line.tag << endl;
      i++;
    }
    cout << endl;
    // Output the total number of accesses, hit and miss ratios, and AMAT
    cout << "Total number of accesses: " << accesses << endl;
    cout << "Hit ratio: " << static_cast<double>(hits) / accesses << endl;
    cout << "Miss ratio: " << static_cast<double>(misses) / accesses << endl;
    cout << "AMAT: " << (hits * Time + misses * (Time + 100)) / static_cast<double>(accesses) << " cycles" << endl;
    cout << endl;
  }
  inputFile.close();
}

int main() {
  string filename = "access_sequence.txt";
  unsigned int S; // (8192) 8 KB (Cache Size)
  unsigned int L;    // (64) 64 bytes (Line Size)
  unsigned int Time;   // (5) clock cycles (Access Time)
  cout << "Enter Cache Size: "; 
  cin >> S;
  cout << "Enter Line Size: "; 
  cin >> L;
  cout << "Enter Clock Cycles: "; 
  cin >> Time;
  while(Time < 1 && Time > 10){
      cout << "Error! Please enter a valid cycles between 1-10: ";
      cin >> Time;
  }
  
  Cache_Simulation(filename, S, L, Time);

  return 0;
}
