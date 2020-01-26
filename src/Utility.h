#ifndef UTILITY_H_
#define UTILITY_H_

#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <bitset>

#define DYNAMIC_ASSERT(boolian,errormsg) \
	if( !boolian ) { \
		std::cerr << "ERROR: " << errormsg << std::endl; \
	}



using bclock = std::chrono::system_clock;
using Typtime = std::chrono::time_point<bclock>;
using Hashtype = size_t;




template<typename Datatype, typename Hashtype>
auto ComputeHash( const unsigned index, const Hashtype PreviousHash, const Typtime timestamp, const Datatype data ) {
	std::cout << "Hash of this Datatype has yet to be implemented.\n";
	return 0;
}

// Working pair of types - std::string and size_t
template<>
auto ComputeHash<std::string,size_t>( const unsigned index,
										const size_t PreviousHash,
										const Typtime timestamp,
										const std::string data ) {
	return std::hash<std::string>()(data);
}


template<typename Type>
unsigned count_leading_zero_1( Type x )   // slowest; one bit at a time
{
  int n;
  if (sizeof(x)==8) {
	  if (x == 0) return 64;
	  for (n = 0; ((x & 0x8000000000000000) == 0); n++, x <<= 1);
  } else if (sizeof(x)==4) {
	  if (x == 0) return 32;
	  for (n = 0; ((x & 0x80000000) == 0); n++, x <<= 1);
  } else {
	  std::cerr << "Can't count leading zeros for this type.\n";
  }
  return n;
}

/*
 * https://stackoverflow.com/questions/11376288/fast-computing-of-log2-for-64-bit-integers
 * http://aggregate.org/MAGIC/#Leading Zero Count
 * https://en.wikipedia.org/wiki/Find_first_set
 * https://medium.com/@lhartikk/a-blockchain-in-200-lines-of-code-963cc1cc0e54
 * https://www.geeksforgeeks.org/top-10-algorithms-in-interview-questions/#algo5
 * 
 */


#endif