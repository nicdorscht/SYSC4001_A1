/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 * @author Eshal Kashif
 * @author Nicholas Dorscht
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
    int context_save_time = 30;
    int isr_activity_time = 200;

    /******************************************************************/

    // parse each line of the input trace file
    while (std::getline(input_file, trace))
    {
        auto [activity, duration_intr] = parse_trace(trace);

        int device_delay = 0;
        if (duration_intr < delays.size()) {
            device_delay = delays[duration_intr];
        }
        
        if(activity =="CPU"){
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", CPU\n";
            current_time += (unsigned long long)duration_intr;
        }
        else if (activity == "SYSCALL"){
            auto pair = intr_boilerplate(current_time, duration_intr, context_save_time, std::get<0>(deviceTable));
            execution += pair.first; 
            current_time = pair.second;

            // Execute ISR body - call device driver
            execution += std::to_string(current_time) + ", " + std::to_string(device_delay) + ", SYSCALL ISR: call device driver\n"; 
            current_time += device_delay;

            // Execute ISR body - isr activity time (for testing)
            execution += std::to_string(current_time) + ", " + std::to_string(isr_activity_time) + ", handle ISR (activity time)\n";
            current_time += isr_activity_time;

        

            
            // Execute IRET (1ms)
            execution += std::to_string(current_time) + ", 1, IRET\n";
            current_time += 1;

            // Restore context
            execution += std::to_string(current_time) + ", " + std::to_string(context_save_time) + ", Restore context\n";
            current_time += context_save_time;

            // Switch to user mode
            execution += std::to_string(current_time) + ", 1, Switch to user mode\n";
            current_time++;           //Added value of duration 
        }
        else if(activity=="END_IO"){
        

            // Get the device delay to calculate the I/O completion time

            auto pair = intr_boilerplate(current_time, duration_intr, context_save_time, std::get<0>(deviceTable));
            execution += pair.first;
            current_time = pair.second;

            // Handles end of I/O (HW interrupt)
            const int device = duration_intr;
            int device_delay = delays.at(device);
            execution += std::to_string(current_time) + ", " + std::to_string(device_delay) + ", END_IO ISR: Handle device\n";
            current_time += device_delay;
           
            // Execute ISR body for I/O completion handling
            // Execute ISR body - isr activity time (for testing)
            execution += std::to_string(current_time) + ", " + std::to_string(isr_activity_time) + ", handle I/O completeion (activity time)\n";
            current_time += isr_activity_time;

            // Execute IRET (1ms)
            execution += std::to_string(current_time) + ", 1, IRET\n";
            current_time += 1;

            // Restore context
            execution += std::to_string(current_time) + ", " + std::to_string(context_save_time) + ", Restore context\n";
            current_time += context_save_time;

            // Switch to user mode
            execution += std::to_string(current_time) + ", 1, Switch to user mode\n";
            current_time++;
        }
        
    }

    input_file.close();

    write_output(execution);

    return 0;
}
