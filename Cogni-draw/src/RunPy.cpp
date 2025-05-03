#include "RunPy.h"
#include "Macros.h"

#include <iostream>


AsyncPy::AsyncPy(const std::string& input_file)
{
    m_Done.clear();

    std::string sys_cmd("python " + input_file);

    std::function<void()> run_script_fn = [cmd = std::move(sys_cmd), this]
        {
            std::system(cmd.c_str());
            m_Done.test_and_set();
        };

    // detatch to run seperate
    std::thread(run_script_fn).detach();
}