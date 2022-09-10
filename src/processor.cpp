#include "processor.h"
#include "linux_parser.h"
#include <iostream>

Processor::Processor() {
    _prevUpTime = LinuxParser::UpTime();
    _prevActiveJiffies = LinuxParser::ActiveJiffies();
    _prevIdleJiffies = LinuxParser::IdleJiffies();
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    long currActive[[maybe_unused]] = LinuxParser::ActiveJiffies();
    long currIdle[[maybe_unused]] = LinuxParser::IdleJiffies();
    long deltaTotal[[maybe_unused]] = (currActive + currIdle) - (_prevActiveJiffies + _prevIdleJiffies);
    long deltaIdle[[maybe_unused]] = currIdle - _prevIdleJiffies;
    std::cout << "test";
    if (deltaTotal == 0) 
        return 0;
    return (deltaTotal - deltaIdle) / deltaTotal;
}