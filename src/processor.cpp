#include "processor.h"
#include "linux_parser.h"
#include <iostream>

Processor::Processor() {
    _prevActiveJiffies = LinuxParser::ActiveJiffies();
    _prevIdleJiffies = LinuxParser::IdleJiffies();
}

// ref: https://stackoverflow.com/a/23376195/7059839
float Processor::Utilization() { 
    long currActive = LinuxParser::ActiveJiffies();
    long currIdle = LinuxParser::IdleJiffies();
    long deltaTotal = (currActive + currIdle) - (_prevActiveJiffies + _prevIdleJiffies);
    long deltaIdle = currIdle - _prevIdleJiffies;
    float cpuUtil = 0.0;
    // prevent division by 0
    if (deltaTotal != 0) 
        cpuUtil = (float(deltaTotal) - float(deltaIdle))/float(deltaTotal);
    
    _prevActiveJiffies = currActive;
    _prevIdleJiffies = currIdle;
    return cpuUtil;
}