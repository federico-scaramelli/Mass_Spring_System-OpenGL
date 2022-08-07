#pragma once
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#ifndef ENGINE_DIR
  #ifdef NDEBUG
	  #define ENGINE_DIR "../../../src/"
	#else
	  #define ENGINE_DIR ""
  #endif
#endif

namespace utils
{
	inline std::string readFile(const std::string& filepath) {
		std::string content;

		std::string enginePath = ENGINE_DIR + filepath;
		std::ifstream file{enginePath, std::ios::in};

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file: " + enginePath);
		}

		std::ostringstream stream;
		stream << file.rdbuf();

		file.close();
		return stream.str();
	}
}