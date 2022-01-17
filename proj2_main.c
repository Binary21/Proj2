/**
 * Starter code for Project 2. Good luck!
 */
#include <proj2_app.h>
#include <HAL/HAL.h>
#include <HAL/Timer.h>
#include <stdlib.h>


#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void InitNonBlockingLED()
{
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
}

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void PollNonBlockingLED()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0) {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }
}

int main()
{
    WDT_A_holdTimer();
    InitSystemTiming();

    HAL hal = HAL_construct();
    App_project2 app = Application_construct(&app);
    initialize(&app);
    // Do not remove this line. This is your non-blocking check.
    InitNonBlockingLED();
    while (1)
    {
        // Do not remove this line. This is your non-blocking check.
        PollNonBlockingLED();
        Application_loop(&app, &hal);
        HAL_refresh(&hal);
        ;
    }
}
//creates initail values based of application
App_project2 Application_construct(App_project2* app_p)
{
    // declares application
    App_project2 app;
    // declares timers from application
    app.timer = SWTimer_construct(2000);
    app.speedMeteor = SWTimer_construct(50);
    app.speedMeteor2 = SWTimer_construct(30);
    app.speedCharacter = SWTimer_construct(25);
    app.speedBullet = SWTimer_construct(12);
    //starts timers
    SWTimer_start(&app.timer);
    SWTimer_start(&app.speedMeteor);
    SWTimer_start(&app.speedMeteor2);
    SWTimer_start(&app.speedCharacter);
    SWTimer_start(&app.speedBullet);
    return app;
}

void initialize(App_project2* app_p)
{
    // declares inital game values
    app_p->isTappedUP = false;
    app_p->isTappedDOWN = false;
    app_p->gameStart = true;
    //declares initial character
    app_p->Character.xMin = 88;
    app_p->Character.xMax = 94;
    app_p->Character.yMin = 122;
    app_p->Character.yMax = 127;
    // declares inital meteor
    app_p->Meteor.xMin = 57;
    app_p->Meteor.xMax = 67;
    app_p->Meteor.yMin = 0;
    app_p->Meteor.yMax = 10;
    // sets the values for highscore
    app_p->FirstPlace = 0;
    app_p->SecondPlace = 0;
    app_p->ThirdPlace = 0;
    // creates boarder for loading bar
    app_p->LoadingRectangle.xMax = 30;
    app_p->LoadingRectangle.xMin = 90;
    app_p->LoadingRectangle.yMax = 100;
    app_p->LoadingRectangle.yMin = 110;
    // creates loading bars initial values
    app_p->LoadingBar.xMax = 30;
    app_p->LoadingBar.xMin = 30;
    app_p->LoadingBar.yMax = 100;
    app_p->LoadingBar.yMin = 110;

    // initializes random for our meteor
    srand(1);
    srand(rand()%30);


}

void Application_loop(App_project2* app_p, HAL* hal_p)
{
    // checks if the timer for our screen has expired
    // if expired changes game start to false
    if (SWTimer_expired(&app_p->timer))
    {
        if (app_p->gameStart)
            Graphics_clearDisplay(&hal_p->g_sContext);
        app_p->gameStart = false;

    }
    // if game start is true it displays the title screen

    if (app_p->gameStart)
    {
        app_p->state = TITLE_SCREEN;
        app_p->joystick_flick = UP;
    }
    // FSM for changing between all screen states in the game
    switch (app_p->state){
    case TITLE_SCREEN:
        // displays the inforamation for the title screen
        Graphics_drawString(&hal_p->g_sContext, (int8_t *)"ECE 2564 Project 2", -1, 14, 24, true);
        Graphics_drawString(&hal_p->g_sContext, (int8_t *)"Astro Defenders", -1, 24, 40, true);
        Graphics_drawString(&hal_p->g_sContext, (int8_t *)"Rick Ejizu", -1, 35, 80, true);
        // displays the loading bar
        Graphics_drawRectangle(&hal_p->g_sContext, &app_p->LoadingRectangle);
        Graphics_fillRectangle(&hal_p->g_sContext, &app_p->LoadingBar);
        // incriments the loading bar by two
        app_p->LoadingBar.xMax += 2;
        // switches states when gameStart is false
        if(!app_p->gameStart)
            Graphics_clearDisplay(&hal_p->g_sContext);
            app_p->state = OPTIONS_SCREEN;




        break;
    case OPTIONS_SCREEN:
        // draws options
        Graphics_drawString(&hal_p->g_sContext, (int8_t *)"MENU", -1, 40, 10, true);
        Graphics_drawString(&hal_p->g_sContext, (int8_t *)"Start Game", -1, 35, 20, true);
        Graphics_drawString(&hal_p->g_sContext, (int8_t *)"Instructions", -1, 35, 40, true);
        Graphics_drawString(&hal_p->g_sContext, (int8_t *)"High Score", -1, 35, 80, true);
        // detects joystic movements and changes the registered postion of the cursor
        if(Joystick_isTappedDown(&hal_p->joystick, app_p))
        {
            if(app_p->joystick_flick == UP)
                        {
                // moves cursor if the joystic is tapped
                            Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_BLACK);
                                                    Graphics_drawString(&hal_p->g_sContext, (int8_t *)">", -1, 30, 20, true);
                                                    Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_WHITE);
                                                    //changes the state to the current cursor location
                            app_p->joystick_flick = CENTER;
                        }
            // moves cursor if the joystic is tapped
                        else if(app_p->joystick_flick == CENTER)
                        {
                            Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_BLACK);
                                                    Graphics_drawString(&hal_p->g_sContext, (int8_t *)">", -1, 30, 40, true);
                                                    Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_WHITE);
                                                    //changes the state to the current cursor location
                            app_p->joystick_flick = DOWN;
                        }
            // moves cursor if the joystic is tapped
                        else if(app_p->joystick_flick == DOWN)
                        {
                            Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_BLACK);
                                                    Graphics_drawString(&hal_p->g_sContext, (int8_t *)">", -1, 30, 80, true);
                                                    Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_WHITE);
                                                    //changes the state to the current cursor location
                            app_p->joystick_flick = UP;
                        }
        }


        if(Joystick_isTappedUp(&hal_p->joystick, app_p))
                {
            // moves cursor if the joystic is tapped
                    if(app_p->joystick_flick == UP)
                    {
                        Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_BLACK);
                        Graphics_drawString(&hal_p->g_sContext, (int8_t *)">", -1, 30, 20, true);
                        Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_WHITE);
                        //changes the state to the current cursor location
                        app_p->joystick_flick = DOWN;
                    }
                    // moves cursor if the joystic is tapped
                    else if(app_p->joystick_flick == CENTER)
                    {
                        Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_BLACK);
                        Graphics_drawString(&hal_p->g_sContext, (int8_t *)">", -1, 30, 40, true);
                        Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_WHITE);
                        //changes the state to the current cursor location
                        app_p->joystick_flick = UP;
                    }
                    // moves cursor if the joystic is tapped

                    else if(app_p->joystick_flick == DOWN)
                    {
                        Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_BLACK);
                        Graphics_drawString(&hal_p->g_sContext, (int8_t *)">", -1, 30, 80, true);
                        Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_WHITE);
                        //changes the state to the current cursor location
                        app_p->joystick_flick = CENTER;
                    }

                }
        // FSM for cursor positions
        switch(app_p->joystick_flick)
        {
        case CENTER:
            // changes state if the cursor is pressed
            Graphics_drawString(&hal_p->g_sContext, (int8_t *)">", -1, 30, 40, true);
            if(Button_isTapped(&hal_p->boosterpackS1))
            {

                app_p->state = INSTRUCTIONS_SCREEN;
                app_p->instruction_enter = true;
                Graphics_clearDisplay(&hal_p->g_sContext);
            }

            break;
        case UP:
            // changes state if the cursor is pressed
            Graphics_drawString(&hal_p->g_sContext, (int8_t *)">", -1, 30, 20, true);
            if(Button_isTapped(&hal_p->boosterpackS1))
            {
                app_p->score = 0;
                app_p->lives = 3;
                app_p->Character.xMin = 88;
                app_p->Character.xMax = 94;
                app_p->Character.yMin = 122;
                app_p->Character.yMax = 127;
                app_p->state = GAME_SCREEN;
                Graphics_clearDisplay(&hal_p->g_sContext);
                Application_GameScreen(app_p, hal_p);

            }
            break;
        case DOWN:
            // changes state if the cursor is pressed
            Graphics_drawString(&hal_p->g_sContext, (int8_t *)">", -1, 30, 80, true);
            if(Button_isTapped(&hal_p->boosterpackS1))
            {
                app_p->highscore_enter = true;
                app_p->state = HIGH_SCORE;
                Graphics_clearDisplay(&hal_p->g_sContext);
            }
            break;
        }




        break;
    case INSTRUCTIONS_SCREEN:
        // checks if this is the first time weve cycled through the instruction screen
        // if so it displays the instructions
        if(app_p->instruction_enter)
        {
            Graphics_drawString(&hal_p->g_sContext, (int8_t *)"HOW TO PLAY", -1, 20, 10, true);
                    Graphics_drawString(&hal_p->g_sContext, (int8_t *)"Defend the Earth!", -1, 10, 20, true);
                    Graphics_drawString(&hal_p->g_sContext, (int8_t *)"Score points by", -1, 10, 30, true);
                    Graphics_drawString(&hal_p->g_sContext, (int8_t *)"destroying meteors.", -1, 10, 40, true);
                    Graphics_drawString(&hal_p->g_sContext, (int8_t *)"If you collide with", -1, 10, 50, true);
                    Graphics_drawString(&hal_p->g_sContext, (int8_t *)"debris or let it ", -1, 10, 60, true);
                    Graphics_drawString(&hal_p->g_sContext, (int8_t *)"reach earth, you ", -1, 10, 70, true);
                    Graphics_drawString(&hal_p->g_sContext, (int8_t *)"lose a life.", -1, 10, 80, true);
                    Graphics_drawString(&hal_p->g_sContext, (int8_t *)"Use JOYSTICK to move", -1, 10, 90, true);
                    Graphics_drawString(&hal_p->g_sContext, (int8_t *)"Press BB2 to shoot", -1, 10, 100, true);
                    Graphics_drawString(&hal_p->g_sContext, (int8_t *)"Press BB1 to exit", -1, 10, 110, true);
                    app_p->instruction_enter = false;
        }




        // if BB1 is pressed we return to the menu screen

        if(Button_isTapped(&hal_p->boosterpackS1))
                {
                    Graphics_clearDisplay(&hal_p->g_sContext);
                    app_p->state = OPTIONS_SCREEN;

                }
        break;
    case GAME_SCREEN:
        // checks if we are dead
        // if so we move to the result screen
        if(app_p->lives <= 0)
        {
            Graphics_clearDisplay(&hal_p->g_sContext);
            app_p->results_enter = true;
            app_p->state = RESULT_SCREEN;
        }
        // if we arent dead
        else
        {
            // updates the blast's position
            Update_Blast1(app_p, hal_p);
            // checks if the meteor has collided with anything
                    if(Update_Meteor(app_p, hal_p, Application_Collision(app_p, hal_p)))
                        // spawns new meteor if collision happened
                        Application_Meteor(app_p, hal_p);

                    // calls function that controls the spaships movement
                    Application_CharacterMovement(app_p, hal_p);

                    // checks if BB2 was pressed
                    // if so calls function to fire blast
                    if(Button_isTapped(&hal_p->boosterpackS2))
                    {
                        if(app_p->Blast1.yMax < 0)
                            Applicaton_Blast1(app_p, hal_p);
                    }
        }



        break;
    case RESULT_SCREEN:

        // checks if the score is greater than -1
        // necessary for creating mutable strings in a state
        if(app_p->score > -1)
        {
            // creates empty string
            char scores[16];
            // formats string with integer score value and prints it
                        snprintf(scores, 24, "Score %06d", app_p->score);
                        Graphics_drawString(&hal_p->g_sContext, (int8_t *)scores, -1, 40, 40, true);

        }
        // checks if this is the first time we have calculated the top score
        /** this is to ensure that if someone gets the same score as the high score they get
         * second place. This also shifts all the previous winners down depending on which place
         * the new high score comes in
         */
        if(app_p->results_enter)
        {
            if(app_p->score > app_p->FirstPlace)
                    {
                        app_p->ThirdPlace = app_p->SecondPlace;
                        app_p->SecondPlace = app_p->FirstPlace;
                        app_p->FirstPlace = app_p->score;
                    }
                    else if(app_p->score > app_p->SecondPlace && app_p->score <= app_p->FirstPlace)
                    {
                        app_p->ThirdPlace = app_p->SecondPlace;
                        app_p->SecondPlace = app_p->score;
                    }
                    else if(app_p->score > app_p->ThirdPlace && app_p->score <= app_p->SecondPlace && app_p->score <= app_p->FirstPlace)
                        app_p->ThirdPlace = app_p->score;
            app_p->results_enter = false;
        }



        // prompts the player to return to menu and checks if BB1 was pressed
        // if so we return to menu
        Graphics_drawString(&hal_p->g_sContext, (int8_t *)"press button 1 to return to menu", -1, 10, 80, true);
        if(Button_isTapped(&hal_p->boosterpackS1))
        {
            Graphics_clearDisplay(&hal_p->g_sContext);
            app_p->state = OPTIONS_SCREEN;

        }

        break;
    case HIGH_SCORE:
        // checks if this is the first time we have looped through the high score state
        // this is to save processing time
        if(app_p->highscore_enter)
        {
            // updates highscores
            Application_HighScoreUpdate(app_p, hal_p);

        }
        // checks of BB1 was tapped, if so we return to menu
        if(Button_isTapped(&hal_p->boosterpackS1 ))
                {
                    Graphics_clearDisplay(&hal_p->g_sContext);
                    app_p->state = OPTIONS_SCREEN;

                }

        break;
    }
}
// controls the running game screen
void Application_GameScreen(App_project2* app_p, HAL* hal_p)
{
    // draws game boarder and mostly non moving elements
    Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Character);
    Graphics_drawLine(&hal_p->g_sContext, 55, 0, 55, 128);
    Graphics_drawLine(&hal_p->g_sContext, 125, 0, 125, 128);
    Graphics_drawString(&hal_p->g_sContext, (int8_t *)"ASTRO", -1, 9, 10, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t *)"DEFEN", -1, 5, 25, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t *)"-DERS", -1, 6, 40, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t *)"SCORE", -1, 4, 65, true);
    // calls the game score function which draws the game score
    Application_GameScore(app_p, hal_p);
    Graphics_drawString(&hal_p->g_sContext, (int8_t *)"LIVES", -1, 4, 100, true);
    // calls the game lives function which draws the game lives
    Application_GameLives(app_p, hal_p);

}
// Converts live score from integer to string and draws it in the HIGH SCORE screen
void Application_HighScoreUpdate(App_project2* app_p, HAL* hal_p)
{
    Graphics_drawString(&hal_p->g_sContext, (int8_t *)"HIGH SCORE", -1, 40, 10, true);
    // creates empty strings for the scores
    char FirstPlace[18];
                char SecondPlace[19];
                char ThirdPlace[18];
                // formats the integer values into strings and draws them on the screen
                snprintf(FirstPlace, 24, "First Place: %06d", app_p->FirstPlace);
                Graphics_drawString(&hal_p->g_sContext, (int8_t *)FirstPlace, -1, 10, 40, true);

                snprintf(SecondPlace, 24, "Second Place: %06d", app_p->SecondPlace);
                Graphics_drawString(&hal_p->g_sContext, (int8_t *)SecondPlace, -1, 6, 60, true);

                snprintf(ThirdPlace, 24, "Third Place: %06d", app_p->ThirdPlace);
                Graphics_drawString(&hal_p->g_sContext, (int8_t *)ThirdPlace, -1, 10, 80, true);
                // resets the highscore_enter value, this prevents the HIGH SCORE state from calling this function
                // multiple times
                app_p->highscore_enter = false;
                Graphics_drawString(&hal_p->g_sContext, (int8_t *)"Press BB1 to exit", -1, 10, 90, true);
}

// converts game lives from integers to visual values
void Application_GameLives(App_project2* app_p, HAL* hal_p)
{
    if(app_p->lives == 3)
        Graphics_drawString(&hal_p->g_sContext, (int8_t *)"***", -1, 4, 115, true);
    else if(app_p->lives == 2)
        Graphics_drawString(&hal_p->g_sContext, (int8_t *)"** ", -1, 4, 115, true);
    else if(app_p->lives == 1)
        Graphics_drawString(&hal_p->g_sContext, (int8_t *)"*  ", -1, 4, 115, true);



}
// converts game score from integer to string
void Application_GameScore(App_project2* app_p, HAL* hal_p)
{
    char score[6];

    snprintf(score, 24, "%06d", app_p->score);
    Graphics_drawString(&hal_p->g_sContext, (int8_t *)score, -1, 4, 80, true);
}
// spawns the meteor in a random value between the boarders of our screnn
void Application_Meteor(App_project2* app_p, HAL* hal_p)
{
    // creates a random number between 0 and 58
    int startValue = rand() % 58;
    // assigns the meteors values between our screen boarders
    app_p->Meteor.xMin = 57 + startValue;
    app_p->Meteor.xMax = 67 + startValue;
    app_p->Meteor.yMin = 0;
    app_p->Meteor.yMax = 10;
    // draws the meteor
    Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Meteor);
}
// updates the meteors position with time
bool Update_Meteor(App_project2* app_p, HAL* hal_p, bool destroyed)
{
    bool hit = false;
    // checks if player is playing at the easy level
    // if so sets the meteor speed to easy
    if(app_p->score < 1000)
    {
        // switches to easy speed meteor timer
        if(SWTimer_expired(&app_p->speedMeteor))
            {
            // updates the meteors falling position
                Application_DrawMeteor(app_p, hal_p);


            }
    }
    // sets the meteor speed to hard
    else
    {
        // witches to hard speed meteor timer
        if(SWTimer_expired(&app_p->speedMeteor2))
            {
            // updates the meteors falling pisiton
                Application_DrawMeteor(app_p, hal_p);

            }
    }

    // checks if the meteor was destoryed if so erases the meteor
    if(destroyed)
    {
        Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Meteor);
        Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_WHITE);
        // destorys the bullet that destoryed the meteor
        Application_DestroyBullet(app_p, hal_p);
        hit = true;
    }
    // checks if the meteor hit earth
    // removes a life if the meteor hit earth
    if(app_p->Meteor.yMin > 128)
    {
        hit = true;
        app_p->lives -= 1;
        // updates live value on the left of the screen
        Application_GameLives(app_p, hal_p);
    }
    return hit;
}
// creates the bullet
void Applicaton_Blast1(App_project2* app_p, HAL* hal_p)
{
    app_p->Blast1.xMin = app_p->Character.xMin + 3;
    app_p->Blast1.xMax = app_p->Character.xMax - 3;
    app_p->Blast1.yMin = app_p->Character.yMin;
    app_p->Blast1.yMax = app_p->Character.yMax;
    Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Blast1);
}
// updates the bullet position
void Update_Blast1(App_project2* app_p, HAL* hal_p)
{
    //checks if the bullet is within screen range and if the bulltest speed timer has expired
    if(app_p->Blast1.yMax > -100 && SWTimer_expired(&app_p->speedBullet))
    {
        // erases the bullets current position
        Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_BLACK);
            Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Blast1);
            Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_RED);
            // moves the bullet up once
            app_p->Blast1.yMin -= 1;
            app_p->Blast1.yMax -= 1;
            // draws the bullet
            Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Blast1);
            // starts the bullet speed timer
            SWTimer_start(&app_p->speedBullet);
            // resets the foregorund color
            Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_WHITE);
    }
}
// checks for meteor collsions
bool Application_Collision(App_project2* app_p, HAL* hal_p)
{
    // checks if the meteor collided with the bullet on the x axis on the closer side of the bullet
    if(app_p->Blast1.xMin > app_p->Meteor.xMin && app_p->Blast1.xMin < app_p->Meteor.xMax)
    {
        // check if the meteor collided on the y axis
        // if so it adds to the score
        if(app_p->Blast1.yMin > app_p->Meteor.yMax && app_p->Blast1.yMin < app_p->Meteor.yMax)
        {
            app_p->score += 200;
            // updates the visual representation of the game score
            Application_GameScore(app_p, hal_p);
            return true;
        }
        else if(app_p->Blast1.yMax < app_p->Meteor.yMax && app_p->Blast1.yMax > app_p->Meteor.yMin)
        {
            app_p->score += 200;
            // updates the visual representation of the game score
            Application_GameScore(app_p, hal_p);
            return true;
        }
    }
    // checks if the meteor collided with the bullet on the x axis on the farther side of the bullet
    else if(app_p->Blast1.xMax > app_p->Meteor.xMin && app_p->Blast1.xMax < app_p->Meteor.xMax)
        {
            if(app_p->Blast1.yMin > app_p->Meteor.yMax && app_p->Blast1.yMin < app_p->Meteor.yMax)
            {
                app_p->score += 200;
                Application_GameScore(app_p, hal_p);
                return true;
            }
            else if(app_p->Blast1.yMax < app_p->Meteor.yMax && app_p->Blast1.yMax > app_p->Meteor.yMin)
            {
                app_p->score += 200;
                Application_GameScore(app_p, hal_p);
                return true;
            }

        }
    // checks if the meteor collided with the character on the closer side of the x axis
    if(app_p->Character.xMin > app_p->Meteor.xMin && app_p->Character.xMin < app_p->Meteor.xMax)
        {
        //checks if the meteor collided with the character on the y axis
        // if so it sub one from its lives
            if(app_p->Character.yMin < app_p->Meteor.yMax && app_p->Character.yMin > app_p->Meteor.yMin)
            {
                app_p->lives -= 1;
                // updates the visual rep of lives
                Application_GameLives(app_p, hal_p);
                return true;
            }
            else if(app_p->Character.yMax < app_p->Meteor.yMax && app_p->Character.yMax > app_p->Meteor.yMin)
            {
                // updates the visual rep of lives
                app_p->lives -= 1;
                Application_GameLives(app_p, hal_p);
                return true;
            }

        }
    // checks if the meteor collided with the character on the farther side of the x axis
    else if(app_p->Character.xMax > app_p->Meteor.xMin && app_p->Character.xMax < app_p->Meteor.xMax)
            {
                if(app_p->Character.yMin < app_p->Meteor.yMax && app_p->Character.yMin > app_p->Meteor.yMin)
                {

                    app_p->lives -= 1;
                    Application_GameLives(app_p, hal_p);
                    return true;
                }
                else if(app_p->Character.yMax < app_p->Meteor.yMax && app_p->Character.yMax > app_p->Meteor.yMin)
                {
                    app_p->lives -= 1;
                    Application_GameLives(app_p, hal_p);
                    return true;
                }
            }

    return false;
}
// destorys the bullet if it collided with the meteor
void Application_DestroyBullet(App_project2* app_p, HAL* hal_p)
{
    Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Blast1);
    Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_WHITE);
    app_p->Blast1.yMin = -100;
    app_p->Blast1.yMax = -100;
    app_p->Blast1.xMax = -100;
    app_p->Blast1.xMin = -100;
}
// controlls the character movement
void Application_CharacterMovement(App_project2* app_p, HAL* hal_p)
{
    if(SWTimer_expired(&app_p->speedCharacter))
    {
        // recieves the joystic input vertically and horizontally
        Application_JoystickVertical(app_p, hal_p);
        Application_JoystickHorizontal(app_p, hal_p);


                SWTimer_start(&app_p->speedCharacter);
    }
}
// recieves the joystic input horizontally
void Application_JoystickHorizontal(App_project2* app_p, HAL* hal_p)
{
    // checks if the joystick was pressed right
    if(Joystick_isPressedRight(&hal_p->joystick))
                                {
        // checks if the character is within the range of our screens boarders
                                    if(app_p->Character.xMax < 124)
                                    {
                                        // deletes the current character drawing
                                        Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_BLACK);
                                        Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Character);
                                        Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_WHITE);
                                        // moves the character in the joystick direction once
                                        app_p->Character.xMin += 1;
                                        app_p->Character.xMax += 1;
                                        // redraws the charcter
                                        Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Character);
                                    }

                                }
    // check if the joystick was pressed left
                    else if (Joystick_isPressedLeft(&hal_p->joystick))
                                {
                        // checks if the character is within the range of our screens boarders
                                    if(app_p->Character.xMin > 56)
                                    {
                                        // deletes the current character drawing
                                        Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_BLACK);
                                        Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Character);
                                        Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_WHITE);
                                        // moves the character in the joystick direction once
                                        app_p->Character.xMin -= 1;
                                        app_p->Character.xMax -= 1;
                                        // redraws the charcter
                                        Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Character);
                                    }
                                }
}
// recieves the joystic input Vertically
void Application_JoystickVertical(App_project2* app_p, HAL* hal_p)
{
    // check if the joystick was pressed down
    if(Joystick_isPressedDown(&hal_p->joystick))
                    {
        // checks if the character is within the range of our screens boarders
                        if(app_p->Character.yMax < 127)
                        {
                            // deletes the current character drawing
                            Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_BLACK);
                            Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Character);
                            Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_WHITE);
                            // moves the character in the joystick direction once
                            app_p->Character.yMin += 1;
                            app_p->Character.yMax += 1;
                            // redraws the charcter
                            Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Character);
                        }

                    }
    // check if the joystick was pressed up
                    else if (Joystick_isPressedUp(&hal_p->joystick))
                    {
                        // checks if the character is within the range of our screens boarders
                        if(app_p->Character.yMin > 1)
                        {
                            // deletes the current character drawing
                            Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_BLACK);
                            Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Character);
                            Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_WHITE);
                            // moves the character in the joystick direction once
                            app_p->Character.yMin -= 1;
                            app_p->Character.yMax -= 1;
                            // redraws the charcter
                            Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Character);
                        }
                    }
}
// draws the meteor and increments the meteor every time it is called
void Application_DrawMeteor(App_project2* app_p, HAL* hal_p)
{
    // deletes the initial meteor
    Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_BLACK);
                        Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Meteor);
                        Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_WHITE);
                        // increments the meteor
                        app_p->Meteor.yMin += 1;
                        app_p->Meteor.yMax += 1;
                        // draws the meteor
                        Graphics_fillRectangle(&hal_p->g_sContext, &app_p->Meteor);
                        // starts the meteors speed timer
                        SWTimer_start(&app_p->speedMeteor);
}
