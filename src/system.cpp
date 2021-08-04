#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

#include <iostream>

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System():
 kernel_(LinuxParser::Kernel()),
 os_(LinuxParser::OperatingSystem()),
 cpu_(Processor())
{}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

vector<int>& System::ProcessIds() { return pids_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> latest_pids = LinuxParser::Pids();

    for(auto it = processes_.begin();it != processes_.end();++it){
       if(std::find(latest_pids.begin(),latest_pids.end(),it->Pid())== latest_pids.end()){
             processes_.erase(it);
         }
     }

    for(auto id:latest_pids){
        auto p_found = std::find_if(processes_.begin(),processes_.end(),
        [&id](const Process& p){
            return p.Pid()== id;
        });

        if(p_found == processes_.end()){
            Process proc = Process(id);
            if(!proc.Command().empty()){
                if(proc.Command().compare(0,4,"htop")==0){
                    processes_.push_back(Process(id));
                }
            }
        }
    }

    std::sort(processes_.rbegin(),processes_.rend());

    return processes_;     
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return os_; }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); 
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }