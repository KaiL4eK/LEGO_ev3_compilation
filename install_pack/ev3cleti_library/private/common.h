#ifndef COMMON_H_
#define COMMON_H_

#include <string>

#define clip_arg(n, lower, upper) {n = n > upper ? upper : n < lower ? lower : n;}

void error_process ( std::string function, std::string msg );

#endif //COMMON_H_
