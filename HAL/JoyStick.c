
#include <HAL/JoyStick.h>
#include <proj2_app.h>
#define UP_THRESHOLD 8000
#define DOWN_THRESHOLD 7300
#define UP_LIMIT 14000
#define DOWN_LIMIT 2000
#define RIGHT_THRESHOLD 7950
#define LEFT_THRESHOLD 7000

enum _JoystickDebounceState {IS_DOWN, NOT_DOWN, IS_UP, NOT_UP, LEFT, NOT_LEFT, RIGHT, NOT_RIGHT};
typedef enum _JoystickDebounceState JoystickDebounceState;

void initADC()
{
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_SYSOSC,
                     ADC_PREDIVIDER_1,
                     ADC_DIVIDER_1,
                     0);
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);

    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
}

void startADC()
{
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
}

void initJoyStick()
{
    ADC14_configureConversionMemory(ADC_MEM0,
                                    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A15,
                                    ADC_NONDIFFERENTIAL_INPUTS);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN0,
                                               GPIO_TERTIARY_MODULE_FUNCTION);

    ADC14_configureConversionMemory(ADC_MEM1,
                                    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A9,
                                    ADC_NONDIFFERENTIAL_INPUTS);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                               GPIO_PIN4,
                                               GPIO_TERTIARY_MODULE_FUNCTION);
}




Joystick Joystick_construct()
{
    Joystick joystick;

    initADC();
    initJoyStick();
    startADC();

    return joystick;
}

void Joystick_refresh(Joystick* joystick_p)
{

    joystick_p->x = ADC14_getResult(ADC_MEM0);
    joystick_p->y = ADC14_getResult(ADC_MEM1);


}

bool Joystick_isPressedUp(Joystick* joystick_p)
{
    if(joystick_p->y >= UP_THRESHOLD)
        return true;
    return false;
}

bool Joystick_isPressedDown(Joystick* joystick_p)
{
    if(joystick_p->y <= DOWN_THRESHOLD)
        return true;
    return false;
}

bool Joystick_isPressedRight(Joystick* joystick_p)
{
    if(joystick_p->x >= RIGHT_THRESHOLD)
        return true;
    return false;
}

bool Joystick_isPressedLeft(Joystick* joystick_p)
{
    if(joystick_p->x <= LEFT_THRESHOLD)
        return true;
    return false;
}

bool Joystick_isTappedUp(Joystick* joystick_p, App_project2* app_p)
{
    static JoystickDebounceState state = NOT_UP;
    bool output = false;

    switch(state)
    {
    case NOT_UP:
        if (joystick_p->y < UP_THRESHOLD)
        {
            state = IS_UP;
            output = true;
        }
    case IS_UP:
        if (joystick_p->y > UP_LIMIT)
            state = NOT_UP;
    }
    return(output);
}
bool Joystick_isTappedDown(Joystick* joystick_p, App_project2* app_p)
{

    static JoystickDebounceState state = NOT_DOWN;
        bool output = false;

        switch(state)
        {
        case NOT_DOWN:
            if (joystick_p->y > DOWN_THRESHOLD)
            {
                state = IS_DOWN;
                output = true;
            }
        case IS_DOWN:
            if (joystick_p->y < DOWN_LIMIT)
                state = NOT_DOWN;
        }
        return(output);
}


