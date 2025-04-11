#include <thread>
#include <cstdlib>
#include <string>
#include <sstream>
#include <functional>
#include <atomic>

void run_async_python(const std::string& input_file, std::atomic_flag& is_done);
