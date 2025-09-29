/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 * @author Eshal Kashif
 */

#include <interrupts.hpp>

int main(int argc, char **argv)
{

    // vectors is a C++ std::vector of strings that contain the address of the ISR
    // delays  is a C++ std::vector of ints that contain the delays of each device
    // the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;     //!< string to store single line of trace file
    std::string execution; //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    int current_time = 0;
    // Store device table as a tuple
    std::tuple<std::vector<std::string>, std::vector<int>> deviceTable = parse_args(argc, argv);
    int context_save_time = 10;
    int isr_activity_time = 40;

    /******************************************************************/

    // parse each line of the input trace file
    while (std::getline(input_file, trace))
    {
        auto [activity, duration_intr] = parse_trace(trace);

        if(activity =="CPU"){
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", CPU\n";
            current_time += (unsigned long long)duration_intr;
        }
        else if (activity == "SYSCALL"){
            auto pair = intr_boilerplate(current_time, duration_intr, context_save_time, std::get<0>(deviceTable));
            execution += pair.first;
            current_time = pair.second;

            // Execute ISR body - call device driver
            execution += std::to_string(current_time) + ", " + std::to_string(isr_activity_time) + 
                        ", call device driver\n";
            current_time += isr_activity_time;

            // Execute IRET (1ms)
            execution += std::to_string(current_time) + ", 1, IRET\n";
            current_time += 1;

            
        }
        else if(activity=="END_IO"){
            // Handles end of I/O (HW interrupt)

            // Get the device delay to calculate the I/O completion time

            /*
            int device_delay=0;
            if (duration_intr < delays.size()) {
                device_delay = delays[duration_intr];
            }
            execution += std::to_string(current_time) + ", " + std::to_string(device_delay) + 
                        ", end of I/O " + std::to_string(duration_intr) + ": interrupt\n";
            current_time += device_delay;
            */
            auto pair = intr_boilerplate(current_time, duration_intr, context_save_time, std::get<0>(deviceTable));
            execution += pair.first;
            current_time = pair.second;

            // Execute ISR body for I/O completion handling
            execution += std::to_string(current_time) + ", " + std::to_string(isr_activity_time) + 
                        ", handle I/O completion\n";
            current_time += isr_activity_time;

            // Execute IRET (1ms)
            execution += std::to_string(current_time) + ", 1, IRET\n";
            current_time += 1;
        }
        


        /******************ADD YOUR SIMULATION CODE HERE*************************/
        /*
        1. Check activity type, if its a CPU burst, skip step 2
        2. Use intr_boilerplate to switch to kernal mode, save context, lookup vector in table, put vector address in PC
        3. Lookup device in device table and add execution time
        4. Switch to user mode, restore context
         */
        /************************************************************************/
    }

    input_file.close();

    write_output(execution);

    return 0;
}
