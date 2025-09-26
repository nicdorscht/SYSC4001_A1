/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
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

    /******************************************************************/

    // parse each line of the input trace file
    while (std::getline(input_file, trace))
    {
        auto [activity, duration_intr] = parse_trace(trace);

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
