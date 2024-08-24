#!/bin/bash

valgrind ./allocate -f cases/task1/simple.txt -s SJF -m infinite -q 1
valgrind ./allocate -f cases/task1/more-processes.txt -s SJF -m infinite -q 3

valgrind ./allocate -f cases/task2/simple.txt -s RR -m infinite -q 3
valgrind ./allocate -f cases/task2/two-processes.txt -s RR -m infinite -q 1
valgrind ./allocate -f cases/task2/two-processes.txt -s RR -m infinite -q 3

valgrind ./allocate -f cases/task3/simple.txt -s SJF -m best-fit -q 3
valgrind ./allocate -f cases/task3/non-fit.txt -s SJF -m best-fit -q 3
valgrind ./allocate -f cases/task3/non-fit.txt -s RR -m best-fit -q 3

valgrind ./allocate -f cases/task4/spec.txt -s SJF -m infinite -q 3
valgrind ./allocate -f cases/task1/more-processes.txt -s SJF -m infinite -q 3
valgrind ./allocate -f cases/task2/simple.txt -s RR -m infinite -q 3

valgrind ./allocate -f cases/task1/simple.txt -s SJF -m infinite -q 1
valgrind ./allocate -f cases/task2/two-processes.txt -s RR -m infinite -q 3

