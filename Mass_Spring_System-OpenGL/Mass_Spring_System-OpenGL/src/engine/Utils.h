#pragma once
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

//TODO: check NDEBUG path with shaders
#ifndef ENGINE_DIR
  #ifdef NDEBUG
	  #define ENGINE_DIR "../../../Mass_Spring_System-OpenGL/src/"
	#else
	  #define ENGINE_DIR ""
  #endif
#endif

namespace Utils
{
    template <class... E>
	constexpr bool falseTemplate = false;

	inline std::string readFile(const std::string& filepath) {
		std::string content;

		std::string enginePath = ENGINE_DIR + filepath;
		std::ifstream file{enginePath, std::ios::in};

		if (!file.is_open()) {
			std::cout << enginePath;

			std::string s;
			std::cin >> s;
			
			throw std::runtime_error("failed to open file: " + enginePath);
		}

		std::ostringstream stream;
		stream << file.rdbuf();

		file.close();
		return stream.str();
	}
}
