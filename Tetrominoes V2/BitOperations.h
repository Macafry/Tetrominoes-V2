#pragma once
#include <bitset>
using std::bitset;


//for reference
template <typename T>
T setPos1(T x, int pos) {
	return (1 << pos) | x;
}

template <typename T>
T setPos0(T x, int pos) {
	return ~(1 << pos) & x;
}

template <typename T> 
T flipPos(T x, int pos) {
	return (1 << pos) ^ x;
}

template <typename T>
T swapBits(T x, int a, int b) {
	//they are equal	
	T P = ((x >> a) ^ (x >> b)) & 1;
	x ^= (P << a);
	x ^= (P << b);
	return x;
}

//auxiliary stuff
template <int bsize>
bitset<bsize> one(bitset<bsize> x) {
	bitset<bsize> b(1);
	return b;
}

template <int bsize>
bitset<bsize> smallAdd(bitset<bsize> a, bitset<bsize> b) {

	auto intA = (long int)a.to_ulong();
	auto intB = (long int)b.to_ulong();
	auto intX = intA + intB;

	return bitset<bsize>(intX);
}

template <int bsize>
bitset<bsize> negate(bitset<bsize> x) {
	return smallAdd(~x, one(x));
}


//real stuff
template <int bsize>
bitset<bsize> trailing1s(bitset<bsize> x) {
	return smallAdd(x, negative(one(x))) | x;
}

template <int bsize>
bitset<bsize> leastSignificantBit(bitset<bsize> x) {
	return x & negate(x);
}

template <int bsize>
bitset<bsize> maskedCopy(bitset<bsize> A, bitset<bsize> B, bitset<bsize> M) {
	return (B & M) | (A & negate(M));
}


template <int bsize>
bitset<bsize> swapBits(bitset<bsize> x, int a, int b) {
	//they are equal	
	auto temp = x[a];
	x[a] = x[b];
	x[b] = temp;
	return x;
}

