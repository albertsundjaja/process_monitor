#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// loop line by line to find the value for the given key, key[space]val
string findValFromKey(string targetKey, string dirName) {
  string key, val;
  string line;
  std::ifstream stream(dirName);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> val;
      if (key == targetKey) {
        return val;
      }
    }
  }
  return ""; 
}

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  int memTotal = std::stoi(findValFromKey("MemTotal:", kProcDirectory + kMeminfoFilename));
  int memFree = std::stoi(findValFromKey("MemFree:", kProcDirectory + kMeminfoFilename));
  int memUsed = memTotal - memFree;
  return float(memUsed) / float(memTotal);
}

long LinuxParser::UpTime() { 
  int uptime, idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
  }
  return uptime;  
}

long LinuxParser::Jiffies() { return UpTime() * sysconf(_SC_CLK_TCK); }

long LinuxParser::ActiveJiffies(int pid) { 
  vector<string> pidStat;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    string tempVal;
    while(linestream >> tempVal) {
      pidStat.emplace_back(tempVal);
    }
  }
  long jiffies = 0;
  for (auto i = 13; i <= 14; i++) {
    jiffies += std::stoi(pidStat[i]);
  }
 
  return jiffies;
}

// System
long LinuxParser::ActiveJiffies() { 
  vector<int> cpuStat = CpuUtilization();
  // user + nice + system + irq + softirq + steal
  return cpuStat[0] + cpuStat[1] + cpuStat[2] + cpuStat[5] + cpuStat[6] + cpuStat[7]; 
}

// System
long LinuxParser::IdleJiffies() { 
  vector<int> cpuStat = CpuUtilization();
  // idle + iowait
  return cpuStat[3] + cpuStat[4];
}

// int from cpu line in /proc/stat
vector<int> LinuxParser::CpuUtilization() { 
  vector<int> cpuStat;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    string cpuId;
    linestream >> cpuId;
    int tempVal;
    while(linestream >> tempVal) {
      cpuStat.emplace_back(tempVal);
    }
  }
  return cpuStat;  
}

int LinuxParser::TotalProcesses() { 
  return std::stoi(findValFromKey("processes", kProcDirectory + kStatFilename)); 
}

int LinuxParser::RunningProcesses() { 
  return std::stoi(findValFromKey("procs_running", kProcDirectory + kStatFilename));    
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

long LinuxParser::UpTime(int pid[[maybe_unused]]) { 
  vector<string> pidStat;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    string tempVal;
    while(linestream >> tempVal) {
      pidStat.emplace_back(tempVal);
    }
  }
  // system up time - process start time converted from clock tick
  long uptime = UpTime() - (std::stol(pidStat[21]) / sysconf(_SC_CLK_TCK));
  return uptime;
}
