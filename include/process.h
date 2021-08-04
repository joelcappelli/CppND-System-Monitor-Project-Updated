#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid() const;                         // TODO: See src/process.cpp
  std::string User() const;                      // TODO: See src/process.cpp
  std::string Command() const;                   // TODO: See src/process.cpp
  float UpdateCpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram() const;                       // TODO: See src/process.cpp
  long int ProcessTime() const;                       // TODO: See src/process.cpp
  float GetCpuUsage() const;
  bool operator<(Process const& other) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string user_;
  std::string command_;
  std::string ram_reserved_;
  long start_time_;
  long proc_time_secs = 0;
  long time_since_start_secs = 0;
  float recent_cpu_usage_ = 0.0;
};

#endif