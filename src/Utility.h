#ifndef __ISS_UTILITY__
#define __ISS_UTILITY__

#include "Define.h"

#include <list>
#include <vector>
#include <map>
#include <stack>
#include <queue>
#include <set>
#include <utility>

template <class T> using Vector = std::vector <T>;
template <class T> using Deque = std::deque <T>;
template <class T> using List = std::list <T>;
template <class T> using Stack = std::stack <T, std::deque<T>>;
template <class T> using Queue = std::queue <T, std::deque<T>>;
template <class T> using Set = std::set <T>;
template <class K, class T> using Map = std::map <K, T>;
template <class L, class R> using Pair = std::pair <L, R>;

class ISS_API Exception {
public:
	Exception(StringC message, ...) {
		va_list vaList;
		va_start(vaList, message);
		vsprintf(this->errorBuffer, message, vaList);
		va_end(vaList);
	}
public:
	inline StringC what() {
		return &errorBuffer[0];
	}
private:
	static Sint8 errorBuffer[1024];
};

typedef std::string Buffer;

typedef class PackLoader* PackLoaderPtr;
typedef class Image* ImagePtr;
typedef class PackStream* PackStreamPtr;
typedef class ImageExtension* ImageExtensionPtr;

#endif // ~__ISS_UTILITY__