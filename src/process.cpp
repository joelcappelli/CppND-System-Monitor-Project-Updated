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
using LinuxParser::kuTime_;
using LinuxParser::ksTime_;
using LinuxParser::kcuTime_;
using LinuxParser::kcsTime_;

Process::Process(int pid):
pid_(pid),
user_(LinuxParser::User(pid_)),
command_(LinuxParser::Command(pid_)),
ram_reserved_(LinuxParser::Ram(pid_)),
start_time_(LinuxParser::StartTime(pid_))
{}

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::UpdateCpuUtilization() { 
    std::vector<long> cpu_util_jifs = LinuxParser::ProcessUtilization(pid_);

    long prev_proc_time_secs = proc_time_secs;
    long prev_time_since_start_secs = time_since_start_secs;

    proc_time_secs = LinuxParser::HZ_to_secs(cpu_util_jifs[kuTime_] + cpu_util_jifs[ksTime_] + cpu_util_jifs[kcuTime_] + cpu_util_jifs[kcsTime_]);

    time_since_start_secs = LinuxParser::UpTime() - start_time_;
    
    float proc_time_delta = proc_time_secs - prev_proc_time_secs;
    float time_since_start_delta = time_since_start_secs - prev_time_since_start_secs;

    recent_cpu_usage_ = time_since_start_delta == 0 ? 0: proc_time_delta/time_since_start_delta;
    return recent_cpu_usage_; 
}

float Process::GetCpuUsage() const { return recent_cpu_usage_;}

// TODO: Return the command that generated this process
string Process::Command() const{ return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() const{ return ram_reserved_; }

// TODO: Return the user (name) that generated this process
string Process::User() const{ return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::ProcessTime() const{ return proc_time_secs; }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& other) const { return GetCpuUsage() < other.GetCpuUsage(); }