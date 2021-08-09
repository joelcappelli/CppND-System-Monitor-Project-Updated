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
start_time_jifs(LinuxParser::StartTime(pid_))
{
   UpdateCpuUtilization(); 
}

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::UpdateCpuUtilization() { 
    std::vector<long> cpu_util_jifs = LinuxParser::ProcessUtilization(Pid());

    long prev_proc_jifs = proc_jifs;
    long prev_jifs_since_start = jifs_since_start;

    proc_jifs = cpu_util_jifs[kuTime_] + cpu_util_jifs[ksTime_] + cpu_util_jifs[kcuTime_] + cpu_util_jifs[kcsTime_];

    jifs_since_start = LinuxParser::hsecs_to_HZ(LinuxParser::UpTime()) - start_time_jifs;
    
    float proc_time_delta = proc_jifs - prev_proc_jifs;
    float time_since_start_delta = jifs_since_start - prev_jifs_since_start;

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
long int Process::ProcessTime() const{ return LinuxParser::HZ_to_secs(jifs_since_start); }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& other) const { return GetCpuUsage() < other.GetCpuUsage(); }