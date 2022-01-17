/*
 * JoyStick.h
 *
 *  Created on: Nov 1, 2021
 *      Author: Rick
 */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#ifndef HAL_JOYSTICK_H_
#define HAL_JOYSTICK_H_

#define DEBOUNCE_TIME_MS    5
#define PRESSED             0
#define RELEASED            1






struct _Joystick
{
    uint_fast16_t x;
    uint_fast16_t y;
};

typedef struct _Joystick Joystick;
Joystick Joystick_construct();

void Joystick_refresh(Joystick* Joystick);

bool Joystick_isPressedUp(Joystick* Joystick);

bool Joystick_isPressedDown(Joystick* Joystick);

bool Joystick_isTapped(Joystick* Joystick);

bool Joystick_isTappedDOWN(Joystick* Joystick);

bool Joystick_isTappedUP(Joystick* Joystick);

bool Joystick_isPressedUp(Joystick* Joystick);
bool Joystick_isPressedDown(Joystick* Joystick);
bool Joystick_isPressedRight(Joystick* Joystick);
bool Joystick_isPressedLeft(Joystick* Joystick);


#endif /* HAL_JOYSTICK_H_ */
