#ifndef PR_WORLDSESSIONRUNNABLE_H
#define PR_WORLDSESSIONRUNNABLE_H

#include "Common.h"
#include "tbb/concurrent_unordered_map.h"

//#include "tbb/concurrent_unordered_multimap.h"
#include <string>
#include <functional>

namespace MaNGOS
{

	struct PR_Uint8HashCompare {
		static size_t hash(const uint8& x) {
			return std::hash<uint8>{}(x);
		}
		//! True if strings are equal
		static bool equal(const uint8& x, const uint8& y) {
			return x == y;
		}
	};
	struct PR_Int32HashCompare {
		static size_t hash(const int32& x) {
			return std::hash<int32>{}(x);
		}
		//! True if strings are equal
		static bool equal(const int32& x, const int32& y) {
			return x == y;
		}
	};
	struct PR_Uint32HashCompare {
		static size_t hash(const uint32& x) {
			return std::hash<uint32>{}(x);
		}
		//! True if strings are equal
		static bool equal(const uint32& x, const uint32& y) {
			return x == y;
		}
	};


	struct PR_StringHashCompare {
		static size_t hash(const std::string& x) {
			return std::hash<std::string>{}(x);
		}
		//! True if strings are equal
		static bool equal(const std::string& x, const std::string& y) {
			return x == y;
		}
	};
	// A concurrent hash table that maps strings to ints.

	template<typename K, typename T>
	class  MANGOS_DLL_SPEC pr_unordered_map :public tbb::concurrent_unordered_map<K, T>{

	};
}
#endif