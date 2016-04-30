#ifndef __ISS_JSON_BUILDER__
#define __ISS_JSON_BUILDER__

#include "Utility.h"

class ISS_API JsonBuilder {
	typedef const Buffer& BufferRefC;
public:
	JsonBuilder& add(StringC key, StringC value);
	JsonBuilder& add(StringC key, JsonBuilder& json);
	JsonBuilder& add(StringC key, BufferRefC value);
	JsonBuilder& add(StringC key, Sint32 value);
	JsonBuilder& add(StringC key, Sint64 value);
	JsonBuilder& add(StringC key, Uint32 value);
	JsonBuilder& add(StringC key, Uint64 value);
	JsonBuilder& add(StringC key, Float32 value);
	JsonBuilder& add(StringC key, Float64 value);
public:
	template <class C> Void addContainer(StringC key, C& container) {
		if (this->jsonString.empty()) {
			this->jsonString += "{";
		}
		this->jsonString += "\"" + Buffer(key) + "\" : [";
		for (auto& i : container) {
			if (typeid(i) == typeid(Buffer) ||
				typeid(i) == typeid(BufferRefC) ||
				typeid(i) == typeid(String) ||
				typeid(i) == typeid(StringC)
			) {
				this->jsonString += "\"" + Buffer(i) + "\", ";
			}
			else {
				this->jsonString += Buffer(i) + ", ";
			}
		}
		if (!container.empty()) {
			this->jsonString.pop_back();
			this->jsonString.pop_back();
		}
		this->jsonString += "], ";
	}
public:
	Buffer build(Void);
public:
	inline operator Buffer () {
		return this->build();
	}
private:
	Void _append(BufferRefC key, BufferRefC value, Bool isNumber = FALSE);
private:
	Sint8 numberBuffer[0xFF];
	Buffer jsonString;
};

#endif // ~__ISS_JSON_BUILDER__