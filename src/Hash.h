#ifndef __ISS_HASH__
#define __ISS_HASH__

#include "Utility.h"

template <class T> class Hash {
public:
	Hash(T t = 0) :
		hash(t)
	{
	}
public:
	Uint32 distance(Hash hash) {

		__int64 dist = 0, val = hash ^ this->hash;

		while (val) {
			++dist; val &= val - 1;
		}

		return Uint32(dist);
	}
public:
	inline operator T& () {
		return this->hash;
	}
	inline T operator = (T t) {
		this->hash = t; return Hash(t);
	}
private:
	T hash;
};

typedef Hash<Uint64> Hash64;
typedef Hash<Uint32> Hash32;
typedef Hash<Uint32> Hash16;

template <class T> class Hashable {
public:
	typedef T HashType;
public:
	static inline Uint32 getHashSize() {
		return sizeof(T);
	}
public:
	virtual HashType getHash() = 0;
};

class Hashable16 : public Hashable < Hash16 > {};
class Hashable32 : public Hashable < Hash32 > {};
class Hashable64 : public Hashable < Hash64 > {};

template <class T> class HashMap16 :
	public Map < Hashable16::HashType, Hashable16 > {};
template <class T> class HashMap32 :
	public Map < Hashable32::HashType, Hashable32 > {};
template <class T> class HashMap64 :
	public Map < Hashable64::HashType, Hashable64 > {};

#endif // ~__ISS_HASH__