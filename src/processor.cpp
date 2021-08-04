#include "processor.h"
#include "linux_parser.h"

using LinuxParser::kIdle_;
using LinuxParser::kIOwait_;
using LinuxParser::kUser_;
using LinuxParser::kNice_;
using LinuxParser::kSystem_;
using LinuxParser::kIRQ_;
using LinuxParser::kSoftIRQ_;
using LinuxParser::kSteal_;

//Return the instantaneous CPU utilization
float Processor::Utilization() { 
    std::vector<long> cpu_util_jifs = LinuxParser::CpuUtilization();

    long prev_idle = idle;
    long prev_non_idle = non_idle;
    long prev_total = prev_idle + prev_non_idle;

    idle = cpu_util_jifs[kIdle_] + cpu_util_jifs[kIOwait_];
    non_idle = cpu_util_jifs[kUser_] + cpu_util_jifs[kNice_] + cpu_util_jifs[kSystem_] + 
                cpu_util_jifs[kIRQ_] + cpu_util_jifs[kSoftIRQ_] + cpu_util_jifs[kSteal_];

    long total = idle + non_idle;
    
    float total_delta = total - prev_total;
    float idle_delta = idle - prev_idle;

    return (total_delta-idle_delta)/total_delta; 
}