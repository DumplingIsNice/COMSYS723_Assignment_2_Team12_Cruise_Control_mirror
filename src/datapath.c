/* Created by Hao Lin and Chamith Nanayakkara
    this file implements the datapath component of the cruise control
    for example:
    - Speed
    - CruiseSpeed
    - CruiseState
    - ThrottleCmd
    should all have an corresponding variable here. Inc intermediate signals such as:
    - isAccel
    - isBrake
*/

#include<stdio.h>
#include<stdlib.h>
#include "datapath.h"