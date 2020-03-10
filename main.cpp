/**
 * COPY CAT GAME FOR MICROBIT
 * Author: Sam Sloman
 * Date: 09/02/2020
 * Description: IoT Challenge 1
 */


#include <string> 
#include "MicroBit.h"
#define MAX 20
#define SLEEP 500


int sequence[MAX]; // Array in which its size is limited by global variable MAX
int userInputs;
int currentLevel;
int isGameOver;
int buttonPressed;


MicroBitImage image(5,5);
MicroBit uBit;
MicroBitImage leftTilt("255,100,20,0,0\n255,100,20,0,0\n255,100,20,0,0\n255,100,20,0,0\n255,100,20,0,0\n");
MicroBitImage rightTilt("0,0,20,100,255\n0,0,20,100,255\n0,0,20,100,255\n,0,0,20,100,255\n0,0,20,100,255\n");
MicroBitImage downTilt("255,255,255,255,255\n100,100,100,100,100\n20,20,20,20,20\n\n\n");
MicroBitImage upTilt("0,0,0,0,0\n0,0,0,0,0\n20,20,20,20,20\n100,100,100,100,100\n255,255,255,255,255\n");
MicroBitImage center("0,0,0,0,0\n0,0,255,0,0\n0,255,50,255,0\n0,0,255,0,0\n0,0,0,0,0\n");
MicroBitImage incorrect("0,255,0,255,0\n0,0,0,0,0\n0,255,255,255,0\n255,0,0,0,255\n");

// Prototype
void copyCat();


/*
* Function: randomNum
* Purpose: Uses the microbits random number generator
* Returns: Random number between 0-5
*/
int randomNum()
{
    return uBit.random(6);
}

/*
* Function: randomSequence
* Purpose: Stores sequence of random generated numbers
* Returns: N/A
*/
void randomSequence()
{ 
    for(int i = 0; i < MAX; i++)
    {
        sequence[i] = randomNum();
    }
}


void buttonEvent(MicroBitEvent)
{
    buttonPressed = 1;
}

/*
* Function: displaySequence
* Purpose: Pastes images to MicroBit. Each image has a variable between 0-5 which is associated with a mircobit gesture
* Returns: N/A
*/
void displaySequence()
{
    for (int i = 0; i < currentLevel; i++)
    {
        if (sequence[i] == 0)
        {
            uBit.display.image.paste(leftTilt);
        }
        else if (sequence[i] == 1)
        {
            uBit.display.image.paste(rightTilt);
        }
        else if (sequence[i] == 2)
        {
            uBit.display.image.paste(upTilt);
        }
        else if (sequence[i] == 3)
        {
            uBit.display.image.paste(downTilt);
        }
        else if (sequence[i] == 4)
        {
            uBit.display.print("A");
        }
        else if (sequence[i] == 5)
        {
           uBit.display.print("B");
        }

        uBit.sleep(SLEEP*2);
        uBit.display.image.paste(center);
        uBit.sleep(SLEEP*2);

    }

    uBit.display.clear();
}


/*
* Function: countDown
* Purpose: Simple for loop to count down from 3 to 1
* Returns: N/A
*/
void countDown()
{
    for(int i = 3; i > 0; i--)
    {
        uBit.display.print(i);
        uBit.sleep(SLEEP*2);
    }
}


/*
* Function: correctGesture
* Purpose: Releases fibre, displays center image and increments user inputs
* Returns: N/A
*/
void correctGesture()
{
    uBit.sleep(SLEEP*2);
    uBit.display.image.paste(center);
    userInputs++;
}


/*
* Function: levelUp
* Purpose: Increments level, resets user inputs, displays current level
* Returns: N/A
*/
void levelUp()
{
    if(!isGameOver)
    {
        uBit.display.image.paste(center);
        currentLevel++;
        userInputs = 0;
        string str= to_string(currentLevel);
        uBit.display.scroll("Level");
        uBit.display.scroll(currentLevel);
        uBit.sleep(SLEEP);
        uBit.display.clear();
    }  
}



/*
* Function: getGesture
* Purpose: Takes advantage of the microBits built in gestures
* Returns: Accelerometer gesture using X and Y positions
*/
int getGesture()
{
    uBit.accelerometer.getX();
    uBit.accelerometer.getY();

    return uBit.accelerometer.getGesture();
}



/*
* Function: gameOver
* Purpose: Player can shake to replay and level is reset, Press A or B to Quit and release fibre
* Returns: N/A
*/
void gameOver()
{
    uBit.sleep(SLEEP);
    uBit.display.image.paste(incorrect);
    uBit.sleep(SLEEP*2);
    uBit.display.scroll("...SHAKE");

    
    while(1)
    {
        // Shake to play again
        if(getGesture() == MICROBIT_ACCELEROMETER_EVT_SHAKE)
        {
            countDown();
            uBit.sleep(SLEEP);
            break;
        }
        // Ends game and displays level
        if (uBit.buttonA.isPressed() || uBit.buttonB.isPressed())
        {
            string str= to_string(currentLevel);
            uBit.display.scroll("REACHED LEVEL");
            uBit.display.scroll(currentLevel);
            release_fiber();
       }
    }

    isGameOver = 0;
    currentLevel = 1;
}


/*
* Function: startUp
* Purpose: Starts game on button press to make sure player is ready
* Returns: NA
*/
void startUp()
{
    uBit.display.scroll("COPY CAT");
    uBit.display.scroll("PRESS A OR B TO START");
    uBit.sleep(SLEEP);
    
    while(!buttonPressed)
    {
        if(uBit.buttonA.isPressed() || uBit.buttonB.isPressed())
        {
            countDown();
            uBit.display.image.paste(center);
            uBit.sleep(SLEEP * 2);
            break;
        }
    }
}



/*
* Function: recordPlayerInputs
* Purpose: Records the player inputs and checks if inputs and gestures are correct
* Returns: N/A
*/
void recordPlayerInputs()
{
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, buttonEvent);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, buttonEvent);
    

    uBit.sleep(SLEEP);

    while(userInputs < currentLevel && !isGameOver)
    {
        if(getGesture() == MICROBIT_ACCELEROMETER_EVT_TILT_LEFT)
        {
            uBit.display.image.paste(leftTilt);
            if(sequence[userInputs] != 0)
            {
                // Player failed
                isGameOver = 1;
                break;
            }

            // The player is correct
            correctGesture();

        }
        else if(getGesture() == MICROBIT_ACCELEROMETER_EVT_TILT_RIGHT)
        {
            uBit.display.image.paste(rightTilt);
            if(sequence[userInputs] != 1)
            {
                isGameOver = 1;
                break;
            }

            correctGesture();

        }
        else if(getGesture() == MICROBIT_ACCELEROMETER_EVT_TILT_UP)
        {
            uBit.display.image.paste(upTilt);
            if(sequence[userInputs] != 2)
            {
                isGameOver = 1;
                break;
            }

            correctGesture();

        }
        else if(getGesture() == MICROBIT_ACCELEROMETER_EVT_TILT_DOWN)
        {
            uBit.display.image.paste(downTilt);
            if(sequence[userInputs] != 3)
            {
                isGameOver = 1;
                break;
            }

            correctGesture();

        } 
        else if (uBit.buttonA.isPressed())
        {
            uBit.display.print("A");
            if(sequence[userInputs] != 4)
            {
                isGameOver = 1;
                break;
            }

            correctGesture();

        }
        else if(uBit.buttonB.isPressed())
        {
            uBit.display.print("B");
            if(sequence[userInputs] != 5)
            {
                isGameOver = 1;
                break;
            }

            correctGesture();

        }
        else
        {
            uBit.display.image.paste(center);
        }
           
    }
    // Player failed
    if(isGameOver == 1) 
    {   
        gameOver(); 
        copyCat();  
    }
    // Player successful  
    else 
    {
        levelUp();
        copyCat();
    }    
}

/*
* Function: copyCat
* Purpose: Calls functions that play the game
* Returns: N/A
*/
void copyCat()
{
    randomSequence();
    displaySequence();
    recordPlayerInputs();
}


int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    // LED Brightness 
    uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE);

    // Start from level 1
    currentLevel = 1;

    startUp();
    copyCat();

    // Release fibre and enter power efficient sleep
    release_fiber();
}

