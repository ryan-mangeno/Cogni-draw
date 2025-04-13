#include "RunPy.h"
#include "Macros.h"

#include <iostream>

void run_async_python(const std::string& input_file, std::atomic_flag& is_done)
{

    static std::stringstream sys_cmd("");

#ifdef _WIN32
    sys_cmd << "python " << input_file;
#elif __linux__
    sys_cmd << "python3 " << input_file;
#endif       


    std::function<void()> run_script_fn = [&] 
        {
            std::system(sys_cmd.str().c_str()); 
            is_done.test_and_set();
        };

    // detatch to run seperate
    std::thread(run_script_fn).detach(); 

}