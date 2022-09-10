#include <string>

#include "format.h"

using std::string;

string padWithZero(int desiredLength, string text) {
    int diff = desiredLength - text.size();
    if (diff > 0) 
        text = text.insert(0, diff, '0');
    return text;
}
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    int h = seconds / 3600;
    seconds -= h * 3600;

    int m = seconds / 60;
    seconds -= m * 60;

    string hh, mm, ss;
    hh = padWithZero(2, std::to_string(h));
    mm = padWithZero(2, std::to_string(m));
    ss = padWithZero(2, std::to_string(seconds));
    return hh + ":" + mm + ":" + ss;
}

