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

/* Note: */
//The type 'string' is defined as 'char*' in Esterel, hence passing a string by referenc is 'char**'
//Aviod naming a function 'recv'

/* Libarires */
// #include<stdio.h>
// #include<stdlib.h>

/* PI Controller */
float regulateThrottle(int isGoingOn, float cruiseSpeed, float vehicleSpeed);
