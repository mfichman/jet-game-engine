/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <Jet/Utility.hpp>

using namespace Jet;




//-----------------------------------------------------------------------------
// MurmurHash2, by Austin Appleby

// Note - This code makes a few assumptions about how your machine behaves -

// 1. We can read a 4-byte value from any address without crashing
// 2. sizeof(int) == 4

// And it has a few limitations -

// 1. It will not work incrementally.
// 2. It will not produce the same results on little-endian and big-endian
//    machines.

static unsigned int murmur_hash2(const void* key, int len, unsigned int seed) {
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	// Initialize the hash to a 'random' value

	unsigned int h = seed ^ len;

	// Mix 4 bytes at a time into the hash

	const unsigned char * data = (const unsigned char *)key;

	while(len >= 4) {
		unsigned int k = *(unsigned int *)data;

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h *= m; 
		h ^= k;

		data += 4;
		len -= 4;
	}
	
	// Handle the last few bytes of the input array

	switch(len) {
		case 3: h ^= data[2] << 16;
		case 2: h ^= data[1] << 8;
		case 1: h ^= data[0];
				h *= m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
} 

uint32_t Jet::hash(const std::string& key) {
	return murmur_hash2(key.c_str(), key.length(), 0);
}

/*
static int seed_ = 3;
static int x_ = 3;
uint32_t Jet::rand() {
	// This is a hacked version of Blum Blum Shub.  I don't take care of
	// the case where the x^2 value overflows, but it still seems to work
	// well enough.  Also the performance isn't great.  One day I will switch
	// this to boost::random.

	// These two primes are specially chosen to the have following properties:
	// 1. They are large
	// 2. GCD(phi(p-1), phi(q-1)) is small
	// 3. They are congruent to 3 (mod 4)
	static const uint32_t p = 49031;
	static const uint32_t q = 49043;
	static const uint32_t m = p*q;

	uint32_t result = 0;
	for (int i = 0; i < 32; i++) {
		// Compute the next value in the sequence
		x_ = (x_*x_) % m;

		// Compute the parity of the value and shift
		// it into the random number
		uint32_t parity = x_;
		parity ^= parity >> 16;
		parity ^= parity >> 8;
		parity ^= parity >> 4;
		parity &= 0xF;
		parity = (0x6996 >> parity) & 1; 
			
		result &= (parity << i); 
	}

	return result;
}

void Jet::srand(uint32_t seed) {
	seed_ = seed;
	x_ = seed;
}*/
