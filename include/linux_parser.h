#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <unistd.h>
#include <map>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

//helpers
void str_to_long(std::vector<std::string>& v_str, 
                              std::vector<long>& v_long);
void split_string(std::string& str, 
                  std::vector<std::string>& vec,
                  char delim,
                  std::size_t start_index);
long HZ_to_secs(long jiffies);
long hsecs_to_HZ(long hsecs);

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_,
  kCpuStates
};
std::vector<long> CpuUtilization();

// Processes
const size_t pid_utime_loc = 14-1;
const size_t pid_stime_loc = 15-1;
const size_t pid_cutime_loc = 16-1;
const size_t pid_cstime_loc = 17-1;
const size_t pid_starttime_loc = 22-1;
const int MB_to_kB = 1024;

// CPU
enum ProcessStates {
  kuTime_ = 0,
  ksTime_,
  kcuTime_,
  kcsTime_,
  kProcessStates
};

const std::map<size_t,ProcessStates> proc_stat_util_map = {{pid_utime_loc,kuTime_},
                                                               {pid_stime_loc,ksTime_},
                                                               {pid_cutime_loc,kcuTime_},
                                                               {pid_cstime_loc,kcsTime_}};

std::vector<long> ProcessUtilization(int pid);
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int StartTime(int pid);

};  // namespace LinuxParser

#endif