#include <vector>
#include <iostream>
#include <thread>
#include <windows.h>
#include <random>

#include "lera.h"

using namespace std;


vector<int> generateRandomDigits(int numDigits)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 6);

    vector<int> digits;
    for (int i = 0; i < numDigits; ++i) {
        digits.push_back(dis(gen));
    }
    
    return digits;
}

void endGame(bool isWin)
{
    if (isWin)
    {
        cout << endl << endl << setColor("Вы победили!", "\033[0;32m") << endl;
    }
    else
    {
        cout << setColor("Вы проиграли!", "\033[0;31m") << endl;
    }
    
    this_thread::sleep_for(chrono::seconds(3));
    exit(0);
}

string setColor(string text, string colorCode)
{
    return colorCode + text + "\033[0m"; 
}

string keabordInput()
{
    if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41))
    {
        return "LEFT";
    }
    
    if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44))
    {
        return "RIGHT";
    }

    if (GetAsyncKeyState(VK_SPACE))
    {
        return "SPACE";
    }

    if (GetAsyncKeyState(0x51))
    {
        return "Q";
    }

    if (GetAsyncKeyState(0x45))
    {
        return "E";
    }

    return "NONE";
}