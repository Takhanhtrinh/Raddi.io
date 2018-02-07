//
//  Command.h
//  Raddi
//
//  Created by Trinh Ta on 2/15/17.
//
//

#ifndef Command_h
#define Command_h
#include <iostream>
#include <stdint.h>
#include "Common.hpp"
struct Command{
    Command(){};
    Command(float dx, float dy, int dtime) :
    x(dx), y(dy),time(dtime){};
    uint16_t x;
    uint16_t y;
    int time;
    float angle;
};

#endif /* Command_h */
