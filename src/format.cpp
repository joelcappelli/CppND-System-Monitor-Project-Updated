#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::Time(long seconds) { 
    string output;
    int HH = int(seconds/(60*60));
    int MM = int(60*(seconds/60.0/60.0 - float(HH)));
    int SS = seconds - (HH*60*60) - MM*60;

    std::string HH_ = std::to_string(HH);
    std::string MM_ = std::to_string(MM);
    std::string SS_ = std::to_string(SS);

    if(HH < 10){ HH_ = "0"+ HH_;}   
    if(MM < 10){ MM_ = "0"+ MM_;}
    if(SS < 10){ SS_ = "0"+ SS_;}

    output = HH_ + ":" + MM_ + ":" + SS_;
    return output; 
}