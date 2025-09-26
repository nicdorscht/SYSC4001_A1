if [ ! -d "bin" ]; then
    mkdir bin
else
	rm bin/*
fi

if [ ! -d "output_files" ]; then
    mkdir output_files
else
	rm output_files/*
fi

g++ -g -O0 -I . -o bin/interrupts interrupts.cpp

for i in 0 1 2 3 4 5 
do
    ./bin/interrupts ./input_files/trace$i.txt vector_table.txt device_table.txt
    mv execution.txt output_files/execution$i.txt
done