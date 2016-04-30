#include "User.h"
#include "md5/md5.hh"

static Uint32 _murmurHash2(StringC key, Uint32 len)
{
	const Uint32 m = 0x5bd1e995;
	const Uint32 seed = 0;
	const Uint32 r = 24;

	Uint32 h = seed ^ len;
	Sint8P data = Sint8P(key);

	while (len >= 4) {

		Uint32 k;

		k  = (data[0]) << 0;
		k |= (data[1]) << 8;
		k |= (data[2]) << 16;
		k |= (data[3]) << 24;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	switch (len)
	{
	case 3:
		h ^= (data[2]) << 16;
	case 2:
		h ^= (data[1]) << 8;
	case 1:
		h ^= data[0];
		h *= m;
	};

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

Void User::set(StringC userName, StringC userPassword) {

	this->userName = userName;

	this->nameHash =
		_murmurHash2(userName, strlen(userName));

	strcpy(this->userPassword, MD5(Uint8P(userPassword)).hex_digest());
}