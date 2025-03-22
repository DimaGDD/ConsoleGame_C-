#include <iostream>
#include <windows.h>
#include <iomanip>
#include <random>
#include <vector>

using namespace std;


void mainGame();
void drawField(int roundScore, int totalScore, int maxScore, string& rolledDice, string& selectedDice, int indexSelectedDice);
vector<int> generateRandomDigits(int numDigits);
string keabordInput();

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    mainGame();

    return 0;
}

void mainGame()
{
    bool isStartGame = true;

    vector<int> digits = generateRandomDigits(6);

    int roundScore;
    int totalScore;
    int maxScore = 1000;
    string rolledDice;
    string selectedDice;

    for (int digit : digits)
    {
        rolledDice += to_string(digit);
        rolledDice += " ";
    }

    int indexSelectedDice = 0;
    string keabordInputs;
    bool rightButtonPressed = false;
    bool leftButtonPressed = false;

    while (true)
    {
        if (rightButtonPressed || leftButtonPressed || isStartGame)
        {
            drawField(roundScore, totalScore, maxScore, rolledDice, selectedDice, indexSelectedDice);
            isStartGame = false;
        }

        keabordInputs = keabordInput();
        if (keabordInputs == "LEFT")
        {
            if (indexSelectedDice != 0 && !leftButtonPressed)
            {
                indexSelectedDice -= 1;
                leftButtonPressed = true;
            }
        }
        else if (keabordInputs == "RIGHT")
        {
            if (indexSelectedDice != 5 && !rightButtonPressed)
            {
                indexSelectedDice += 1;
                rightButtonPressed = true;
            }
        }
        else
        {
            leftButtonPressed = false;
            rightButtonPressed = false;
        }
    }

    cin.get();
}


void drawField(int roundScore, int totalScore, int maxScore, string& rolledDice, string& selectedDice, int indexSelectedDice)
{
    system("cls");

    stringstream ss(rolledDice);
    string digit;
    int index = 0;
    string result = "";

    while (ss >> digit)
    {
        if (index == indexSelectedDice)
        {
            result += "[" + digit + "] ";
        }
        else
        {
            result += " " + digit + " ";
        }
        index++;
    }

    cout << setw(3) << totalScore << " / " << setw(4) << maxScore << " | " << result << endl;
    cout << "-----------|----------" << endl;
    
    cout << setw(10) << roundScore << " | " << selectedDice << endl;
}

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

string keabordInput()
{
    if (GetAsyncKeyState(VK_LEFT))
    {
        return "LEFT";
    }
    
    if (GetAsyncKeyState(VK_RIGHT))
    {
        return "RIGHT";
    }

    if (GetAsyncKeyState(VK_SPACE))
    {
        return "SPACE";
    }

    return "NONE";
}