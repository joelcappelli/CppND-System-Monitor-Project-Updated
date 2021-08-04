#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key;// MemTotal:, MemFree:
  int mem_val = 0;
  float mem_total = 0.0, mem_free = 0.0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> mem_val) {
        if (key == "MemTotal:") {
          mem_total = float(mem_val);
        }
        else if(key == "MemFree:"){
          mem_free = float(mem_val);
          return (mem_total == 0) ? 0.0 : (mem_total-mem_free)/mem_total;
        } 
      }             
    }
  }
  return (mem_total == 0) ? 0.0 : (mem_total-mem_free)/mem_total;   
}  

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  int up_time = 0, idle_time = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> up_time >> idle_time) {
        return up_time;
      }
    }
  }
  return up_time;   
}

long LinuxParser::HZ_to_secs(long jiffies){
  return jiffies/sysconf(_SC_CLK_TCK);
}

void LinuxParser::str_to_long(std::vector<std::string>& v_str, std::vector<long>& v_long){
  v_long.clear();
  for(std::string str_elem:v_str){
      v_long.push_back(std::stol(str_elem));
  }
}

void LinuxParser::split_string(string& str, vector<string>& vec,char delim = ' ', size_t start_idx = 0){
  vector<string> vec_temp;
  std::istringstream iss(str);
  string item;
  while(std::getline(iss,item,delim)){
    vec_temp.push_back(item);
  }

  vec.clear();
  start_idx = start_idx > (str.length()-1) ? (str.length()-1) : start_idx;
  vec = {vec_temp.begin()+start_idx,vec_temp.end()};
}

// TODO: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization(){
  vector<string> v_line(CPUStates::kCpuStates);
  vector<long> cpu_util_jifs(CPUStates::kCpuStates);  

  string line, key;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      std::size_t found = line.find("cpu",0,3);
      if (found != std::string::npos) {
        LinuxParser::split_string(line, v_line,' ',2);
        LinuxParser::str_to_long(v_line, cpu_util_jifs);

        return cpu_util_jifs;
      }
    }
  }
  return cpu_util_jifs; 
} 

vector<long> LinuxParser::ProcessUtilization(int pid){
  vector<string> v_line(ProcessStates::kProcessStates);
  vector<long> process_util_jifs(ProcessStates::kProcessStates);
  string line;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      split_string(line, v_line,' ');
      process_util_jifs[proc_stat_util_map.at(pid_utime_loc)] = std::stol(v_line[pid_utime_loc]);
      process_util_jifs[proc_stat_util_map.at(pid_stime_loc)] = std::stol(v_line[pid_stime_loc]);
      process_util_jifs[proc_stat_util_map.at(pid_cutime_loc)] = std::stol(v_line[pid_cutime_loc]);
      process_util_jifs[proc_stat_util_map.at(pid_cstime_loc)] = std::stol(v_line[pid_cstime_loc]);
      return process_util_jifs;
    }
  }
  return process_util_jifs;     
}


// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses(){ 
  string line, key;
  int total_processes = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> total_processes) {
        if(key =="processes"){
          return total_processes;
        }
      }
    }
  }
  return total_processes;   
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses(){ 
  string line, key;
  int running_processes = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> running_processes) {
        if(key =="procs_running"){
          return running_processes;
        }
      }
    }
  }
  return running_processes;   
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      return line;
    }
  }
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line, key, buf;// VmSize:
  long int mem_val = 0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> mem_val >> buf) {
        if (key == "VmSize:") {
          return to_string(mem_val/MB_to_kB);
        }
      }             
    }
  }
  return to_string(mem_val/MB_to_kB);
} 

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key;
  string uid = "", temp1, temp2, temp3;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> uid >> temp1 >> temp2 >> temp3) {
        if(key =="Uid:"){
          return uid;
        }
      }
    }
  }
  return uid; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  vector<string> v_line;
  string line,user = "";
  
  string uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      split_string(line, v_line,':');
      if(v_line[2] == uid){
        user = v_line[0];
        return user;
      }
    }
  }
  return user;   
}

// TODO: Read and return the uptime of a process
long LinuxParser::StartTime(int pid) {
  vector<string> v_line;
  string line;
  long start_time = 0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      split_string(line, v_line,' ');
      start_time = HZ_to_secs(std::stol(v_line[pid_starttime_loc]));
    }
  }
  return start_time;
}