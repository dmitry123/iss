#include "JsonBuilder.h"

JsonBuilder& JsonBuilder::add(StringC key, StringC value) {
	this->_append(key, value);
	return *this;
}

JsonBuilder& JsonBuilder::add(StringC key, JsonBuilder& json) {
	this->_append(key, json, TRUE);
	return *this;
}

JsonBuilder& JsonBuilder::add(StringC key, BufferRefC value) {
	this->_append(key, value);
	return *this;
}

JsonBuilder& JsonBuilder::add(StringC key, Sint32 value) {
	sprintf(this->numberBuffer, "%d", value);
	this->_append(key, this->numberBuffer, TRUE);
	return *this;
}

JsonBuilder& JsonBuilder::add(StringC key, Sint64 value) {
	sprintf(this->numberBuffer, "%lld", value);
	this->_append(key, this->numberBuffer, TRUE);
	return *this;
}

JsonBuilder& JsonBuilder::add(StringC key, Uint32 value) {
	sprintf(this->numberBuffer, "%u", value);
	this->_append(key, this->numberBuffer, TRUE);
	return *this;
}

JsonBuilder& JsonBuilder::add(StringC key, Uint64 value) {
	sprintf(this->numberBuffer, "%u", value);
	this->_append(key, this->numberBuffer, TRUE);
	return *this;
}

JsonBuilder& JsonBuilder::add(StringC key, Float32 value) {
	sprintf(this->numberBuffer, "%.4f", value);
	this->_append(key, this->numberBuffer, TRUE);
	return *this;
}

JsonBuilder& JsonBuilder::add(StringC key, Float64 value) {
	sprintf(this->numberBuffer, "%.4f", value);
	this->_append(key, this->numberBuffer, TRUE);
	return *this;
}

Buffer JsonBuilder::build(Void) {

	if (this->jsonString.empty()) {
		return "{}";
	}
	else {
		this->jsonString.pop_back();
		this->jsonString.pop_back();
	}

	return this->jsonString + "}";
}

Void JsonBuilder::_append(BufferRefC key, BufferRefC value, Bool isNumber) {

	if (this->jsonString.empty()) {
		this->jsonString += "{";
	}

	if (isNumber) {
		this->jsonString += "\"" + key + "\" : " + value + ", ";
	} else {
		this->jsonString += "\"" + key + "\" : \"" + value + "\", ";
	}
}