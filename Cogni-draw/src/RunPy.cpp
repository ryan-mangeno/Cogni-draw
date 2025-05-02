#include "RunPy.h"
#include "Macros.h"

#include <iostream>

void run_async_python(const std::string& input_file, std::atomic_flag& is_done)
{

    const std::string sys_cmd("python " + input_file);

    std::function<void()> run_script_fn = [cmd = std::move(sys_cmd), &is_done]
        {
            std::system(cmd.c_str());
            is_done.test_and_set();
        };

    // detatch to run seperate
    std::thread(run_script_fn).detach(); 

}