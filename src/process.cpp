#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return _pid; }

float Process::CpuUtilization() const { 
    // ref: https://stackoverflow.com/a/16736599/7059839
    float totalCpuTime = float(LinuxParser::ActiveJiffies(_pid)) / sysconf(_SC_CLK_TCK);
    // percentage 100* will be done when displaying, here we only return the fraction
    return totalCpuTime / LinuxParser::UpTime(_pid);
}

string Process::Command() { return LinuxParser::Command(_pid); }

// in mb
string Process::Ram() { 
    int kbRam = std::stoi(LinuxParser::Ram(_pid));
    return std::to_string(kbRam / 1024); 
}

string Process::User() { return LinuxParser::User(_pid); }

long int Process::UpTime() { return LinuxParser::UpTime(_pid); }

bool Process::operator<(Process const& a[[maybe_unused]]) const { 
    return this->CpuUtilization() < a.CpuUtilization();
}