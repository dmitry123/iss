#include "Pack.h"

Void PackStream::write(VoidP bufferData, Uint32 size) {

	// check for error
	if (!this->oStream_) {
		throw Exception("PackStream : Output stream closed");
	}

	// write data to output sttream
	this->oStream_.write(String(bufferData), size);
}

Void PackStream::read(VoidP bufferData, Uint32 size) {

	// check for error
	if (!this->iStream_) {
		throw Exception("PackStream : Output stream closed");
	}

	// read data from input stream
	this->iStream_.read(String(bufferData), size);
}

Void PackStream::seek(Uint32 position) {

	// check for error
	if (!this->iStream_) {
		throw Exception("PackStream : Output stream closed");
	}

	// jump to position (only for input stream)
	this->iStream_.seekg(position);
}