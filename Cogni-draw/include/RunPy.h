#ifndef RUN_PY_H
#define RUN_PY_H

#include <thread>
#include <cstdlib>
#include <string>
#include <sstream>
#include <functional>
#include <atomic>

class AsyncPy
{
public:
	AsyncPy(const std::string& input_file);
	~AsyncPy() = default;

	inline bool check_done() { return m_Done.test(); }
	inline std::atomic_flag* get_flag() { return &m_Done; }

private:
	std::atomic_flag m_Done;
	
};


#endif