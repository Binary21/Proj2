/*
 * proj2_app.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 */

#ifndef PROJ2_APP_H_
#define PROJ2_APP_H_

#include <HAL/HAL.h>
#include <HAL/Graphics.h>
#include <HAL/Timer.h>

#define TITLE_SCREEN_WAIT   3000  //3 seconds
#define RESULT_SCREEN_WAIT  3000  // 3 seconds

#define NUM_TEST_OPTIONS    4
#define NUM_RANDOM_NUMBERS  5


void Init_LCD();
void InitGraphics(Graphics_Context *g_sContext_p);
// all the fsm states
enum _GameState
{
    TITLE_SCREEN, OPTIONS_SCREEN, INSTRUCTIONS_SCREEN, GAME_SCREEN, RESULT_SCREEN, HIGH_SCORE
};
typedef enum _GameState GameState;

enum _JoyStick
{
    CENTER, UP, DOWN
};
typedef enum _JoyStick JoyStick;
/**
 * The top-level Application object, initialized in the main() entry point and
 * passed around to basically every other function. Holds state variables
 * which YOU define as part of your application's state.
 */
struct _App_project2
{
    JoyStick joystick_flick;
    GameState state;  // Determines which screen is currently shown
    SWTimer timer;    // General-purpose timer for when screens must disappear
    SWTimer speedMeteor, speedCharacter, speedMeteor2, speedBullet;
    // creates all the rectangles used in the game
    Graphics_Rectangle Character, Meteor, Blast1, LoadingBar, LoadingRectangle;

    // Booleans to determine which colors the user has picked
    bool redSelected;
    bool greenSelected;
    bool blueSelected;
    // holds the value for if the joystick was tapped up or down
    bool isTappedUP;
    bool isTappedDOWN;
    // checks if its the first time we looped through the FSM
    bool instruction_enter;
    bool highscore_enter;
    bool results_enter;


    // variables from the lives and score
    int score;
    int lives;
    // variables for the first second and third place
    int FirstPlace;
    int SecondPlace;
    int ThirdPlace;

    // Cursor tracker variable

    // List of predetermined random numbers
    int randomNumbers[NUM_RANDOM_NUMBERS];
    int randomNumberChoice;


    bool gameStart;

};
typedef struct _App_project2 App_project2;

App_project2 Application_construct();
// loops the entire game
void Application_loop(App_project2* app_p, HAL* hal_p);
// asings values needed at the begining of the game
void initialize();
// initializes the ADC
void initADC();
// Starts the ADC
void startADC();
// Initializes the joystic
void initJoyStick();
// samples the joystic values
void getSampleJoyStick(unsigned *X, unsigned *Y);
// checks if the joystick is tapped up
bool Joystick_isTappedUp(Joystick* joystick_p, App_project2* app_p);
// checks if the joystick is tapped down
bool Joystick_isTappedDown(Joystick* joystick_p, App_project2* app_p);
// checks if the joystic is pressed up
bool Joystick_isPressedUp(Joystick* joystick_p);
// chceks if the joystick is pressed down
bool Joystick_isPressedDown(Joystick* joystick_p);
// checks if the joystick is pressed right
bool Joystick_isPressedRight(Joystick* joystick_p);
// checks if the joystick is pressed left
bool Joystick_isPressedLeft(Joystick* joystick_p);
// displays the gamescreen strings
void Application_GameScreen(App_project2* app_p, HAL* hal_p);
// creates a new meteor
void Application_Meteor(App_project2* app_p, HAL* hal_p);
// updates the meteor's positon
bool Update_Meteor(App_project2* app_p, HAL* hal_p, bool destroyed);
// creates a new blast
void Applicaton_Blast1(App_project2* app_p, HAL* hal_p);
// updates the blasts position
void Update_Blast1(App_project2* app_p, HAL* hal_p);
// chceks for meteor collision
bool Application_Collision(App_project2* app_p, HAL* hal_p);
// destroys bullet if it collides with the meteor
void Application_DestroyBullet(App_project2* app_p, HAL* hal_p);
// updates visiaul representation of the score
void Application_GameScore(App_project2* app_p, HAL* hal_p);
// updates visual representation of the lives
void Application_GameLives(App_project2* app_p, HAL* hal_p);
// controlls the character movement
void Application_CharacterMovement(App_project2* app_p, HAL* hal_p);
// updates all the high scores
void Application_HighScoreUpdate(App_project2* app_p, HAL* hal_p);
// gets the joysticks horizontal movements
void Application_JoystickHorizontal(App_project2* app_p, HAL* hal_p);
// gets the joystics vertical movements
void Application_JoystickVertical(App_project2* app_p, HAL* hal_p);
// draws the meteor
void Application_DrawMeteor(App_project2* app_p, HAL* hal_p);
#endif /* PROJ2_APP_H_ */

