#include <chrono>




#define ELAPSED_TIME_MS(x, result) \
	start = std::chrono::high_resolution_clock::now(); \
	x; \
	result = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();

#define ELAPSED_TIME_US(x, result) \
	start = std::chrono::high_resolution_clock::now(); \
	x; \
	result = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();