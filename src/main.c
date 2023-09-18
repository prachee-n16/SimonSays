#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "ece198.h"

void beep(int time);
void start_game_sequence();
void win_round_sequence();
void lose_game_sequence();

void display_sequence(int currentround, int *led_array);
bool validate_sequence(int *simon, int *user, int count);

void beep(int time)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, true);
    HAL_Delay(time);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, false);
}

void start_game_sequence()
{
    for (int i = 0; i < 3; i++)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, true);
        HAL_Delay(100);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, false);
        HAL_Delay(100);
    }
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, true);
    HAL_Delay(100);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4 | GPIO_PIN_10 | GPIO_PIN_3 | GPIO_PIN_5, true);
    HAL_Delay(750);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, false);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4 | GPIO_PIN_10 | GPIO_PIN_3 | GPIO_PIN_5, false);
    for (int i = 0; i < 3; i++)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, true);
        HAL_Delay(50);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, false);
        HAL_Delay(50);
    }
}

void win_round_sequence()
{
    HAL_Delay(1000);
    beep(100);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4 | GPIO_PIN_10 | GPIO_PIN_3 | GPIO_PIN_5, true);
    HAL_Delay(750);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4 | GPIO_PIN_10 | GPIO_PIN_3 | GPIO_PIN_5, false);
    HAL_Delay(1000);
}

bool validate_sequence(int *simon, int *user, int count)
{
    for (int i = 0; i <= count; i++)
    {
        if (simon[i] != user[i])
        {
            return true;
        }
    }
    return false;
}

void lose_game_sequence()
{
    HAL_Delay(500);
    for (int i = 0; i < 3; i++)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, true);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4 | GPIO_PIN_10 | GPIO_PIN_3 | GPIO_PIN_5, true);
        HAL_Delay(500);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, false);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4 | GPIO_PIN_10 | GPIO_PIN_3 | GPIO_PIN_5, false);
    }
}

void display_sequence(int currentround, int *led_array)
{
    for (int i = 0; i <= currentround; i++)
    {
        HAL_Delay(750);
        if (led_array[i] == 1)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, true);
            HAL_Delay(50);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, true); // Turn on RED LED
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, false);
            HAL_Delay(750);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, false);
        }
        else if (led_array[i] == 2)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, true);
            HAL_Delay(50);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, true); // Turn on Blue LED
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, false);
            HAL_Delay(750);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, false);
        }
        else if (led_array[i] == 3)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, true);
            HAL_Delay(50);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, true); // Turn on Green LED
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, false);
            HAL_Delay(750);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, false);
        }
        else if (led_array[i] == 4)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, true);
            HAL_Delay(50);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, true); // Turn on Yellow LED
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, false);
            HAL_Delay(750);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, false);
        }
    }
}

int main(void)
{
    HAL_Init();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    //Initialize LEDs
    InitializePin(GPIOB, GPIO_PIN_4, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  //D5, Blue LED
    InitializePin(GPIOB, GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); //D6, Red LED
    InitializePin(GPIOB, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  //D5, Yellow LED
    InitializePin(GPIOB, GPIO_PIN_3, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  //D4, Green LED

    //Initialize Speaker
    InitializePin(GPIOA, GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);

    //Initialize Buttons
    InitializePin(GPIOA, GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_PULLUP, 0); //D12, Red LED
    InitializePin(GPIOA, GPIO_PIN_7, GPIO_MODE_INPUT, GPIO_PULLUP, 0); //D11, Blue LED
    InitializePin(GPIOB, GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_PULLUP, 0); //D10, Green LED
    InitializePin(GPIOC, GPIO_PIN_7, GPIO_MODE_INPUT, GPIO_PULLUP, 0); //D9, Yellow LED

    SerialSetup(9600);

    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
        ; // wait for button press

    start_game_sequence(); //Start the game
    srand(HAL_GetTick());  //New random sequence everytime the program is reset
    int led_sequence[10];  //Create an int array of size 10 to store the random sequence for Simon Says

    for (int i = 0; i < 10; i++) //For loop to iterate through the array and set the values
    {
        led_sequence[i] = (random() % 4) + 1; //Generate a random number between (1 --> 4) inclusive and store it in the sequence array
    }

    //Define variables
    int round_counter = 0;    //Stores which round user is playing in
    int button_presses = 1;   //Number of button's that need to be pressed
    int user_presses = 0;     //Number of button's that the user has pressed
    int user_input = 0;       //Which button did the user pressed
    bool failedRound = false; //Has user failed round? Note: false = continue playing game, true = end game

    while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
        ; // wait for button to be released

    while ((round_counter != 10) && (failedRound == false))
    {
        display_sequence(round_counter, led_sequence); //Display the sequence for round

        //Check for button presses or when user has pressed required number of buttons
        while ((HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) || HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) || HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) || HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)) && (user_presses < button_presses))
        {
            if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == false) //RED LED
            {
                beep(50);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, true);
                HAL_Delay(750);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, false);

                user_input = 1;
                if (user_input != led_sequence[user_presses])
                {
                    failedRound = true;
                    user_presses = button_presses;
                }
                user_presses++;
            }
            else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == false) //BLUE LED
            {
                beep(50);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, true);
                HAL_Delay(750);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, false);
                user_input = 2;
                if (user_input != led_sequence[user_presses])
                {
                    failedRound = true;
                    user_presses = button_presses;
                }
                user_presses++;
            }
            else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == false) //GREEN LED
            {
                beep(50);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, true);
                HAL_Delay(750);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, false);
                user_input = 3;
                if (user_input != led_sequence[user_presses])
                {
                    failedRound = true;
                    user_presses = button_presses;
                }
                user_presses++;
            }
            else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) == false) //YELLOW LED
            {
                beep(50);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, true);
                HAL_Delay(750);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, false);
                user_input = 4;
                if (user_input != led_sequence[user_presses])
                {
                    failedRound = true;
                    user_presses = button_presses;
                }
                user_presses++;
            }
        }

        if (failedRound == false) //If user passes round
        {
            win_round_sequence(); //Play win round sequence
            round_counter++;      //Go to next round
            user_presses = 0;     //Reset number of buttons pressed by user
            button_presses++;     //Increase expected button presses
        }
    }
    if (round_counter == 10) //If user completed 10 rounds
    {
        HAL_Delay(1000);
        //Win game sequence
        for (int i = 0; i < 3; i++)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, true);
            HAL_Delay(100);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, false);
            beep(100);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, true);
            HAL_Delay(100);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, false);
            beep(100);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, true);
            HAL_Delay(100);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, false);
            beep(100);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, true);
            HAL_Delay(100);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, false);
            beep(100);
            beep(100);
        }
    }
    else if (failedRound == true) //If user failed round
    {
        HAL_Delay(500);
        lose_game_sequence(); //Lost game sequence
    }

    return 0;
}

// This function is called by the HAL once every millisecond
void SysTick_Handler(void)
{
    HAL_IncTick(); // tell HAL that a new tick has happened
    // we can do other things in here too if we need to, but be careful
}
