#include "RunPy.h"

void run_async_python(const std::string& input_file, std::atomic_flag& is_done)
{

    std::stringstream sys_cmd("");
    sys_cmd << "python3 " << input_file;

    std::function<void()> run_script_fn = [&] 
        {
            std::system(sys_cmd.str().c_str()); 
            is_done.test_and_set();
        };

    // detatch to run seperate
    std::thread(run_script_fn).detach(); 

}