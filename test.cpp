#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
using std::string;

int TotalProcesses() { 
  //return int(findValFromKey("processes", kProcDirectory + kStatFilename)); 
  string key;
  int val;
  string line;
  std::ifstream stream("/proc/stat");
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
        std::istringstream linestream(line);
        linestream >> key >> val;
        std::cout << key << val;
        if (key == "processes") {
        return val;
        }
    }
    
  }
  return 0;  
}

int main() {
    TotalProcesses();
}