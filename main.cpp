#include <iostream>
#include <windows.h>
#include <iomanip>
#include <random>
#include <vector>
#include <algorithm>

using namespace std;


void mainGame();
void drawField(int roundScore, int totalScore, int maxScore, vector<int> rolledDice, string selectedDice, int indexSelectedDice, vector<int> savedIndexDice);
vector<int> generateRandomDigits(int numDigits);
string keabordInput();
int calculateScore(vector<int> selectedDice, vector<int> rolledDice);

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    mainGame();

    return 0;
}

void mainGame()
{
    bool isStartGame = true;
    
    // Переменные
    int roundScore; // Очки раунда
    int totalScore; // Общее количество очков
    int maxScore = 1000; // Максимальное количество очков
    vector<int> rolledDice; // Кинутые кости
    string selectedDice; // Кости, которые мы отложили

    rolledDice = generateRandomDigits(6);

    int indexSelectedDice = 0; // Индекс кости, на которой остановился курсор
    vector<int> savedIndexDice; // Индекс кости, которые надо отложить

    // Параметры ввода
    bool canInput = true;
    string keabordInputs;
    bool rightButtonPressed = false;
    bool leftButtonPressed = false;
    bool spaceButtonPressed = false;
    bool qButtonPressed = false;
    bool eButtonPresses = false;

    while (true)
    {
        if (rightButtonPressed || leftButtonPressed || spaceButtonPressed || isStartGame)
        {
            drawField(roundScore, totalScore, maxScore, rolledDice, selectedDice, indexSelectedDice, savedIndexDice);
            isStartGame = false;
        }

        if (canInput)
        {
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
            else if (keabordInputs == "SPACE")
            {
                if (!spaceButtonPressed)
                {
                auto it = find(savedIndexDice.begin(), savedIndexDice.end(), indexSelectedDice);
                
                if (it != savedIndexDice.end())
                {
                    savedIndexDice.erase(it);
                }
                else
                {
                    savedIndexDice.push_back(indexSelectedDice);
                }

                spaceButtonPressed = true;
                }
            }
            else if (keabordInputs == "Q")
            {
                if (!qButtonPressed)
                {
                    calculateScore(savedIndexDice, rolledDice);
                    qButtonPressed = true;
                    canInput = false;
                }
            }
            else if (keabordInputs == "E")
            {
                if (!eButtonPresses)
                {
                    eButtonPresses = true;
                    canInput = false;
                }
            }
            else
            {
                leftButtonPressed = false;
                rightButtonPressed = false;
                spaceButtonPressed = false;
                qButtonPressed = false;
                eButtonPresses = false;
            }
        }
    }

    cin.get();
}


void drawField(int roundScore, int totalScore, int maxScore, vector<int> rolledDice, string selectedDice, int indexSelectedDice, vector<int> savedIndexDice)
{
    system("cls");

    string rolledDiceString;

    for (int digit : rolledDice)
    {
        rolledDiceString += to_string(digit);
        rolledDiceString += " ";
    }

    stringstream ss(rolledDiceString);
    string digit;
    int index = 0;
    string result = "";

    while (ss >> digit)
    {
        bool isSaved = find(savedIndexDice.begin(), savedIndexDice.end(), index) != savedIndexDice.end();
        bool isSelected = (index == indexSelectedDice);

        if (isSaved && isSelected)
        {
            result += "[*" + digit + "*] ";
        }
        else if (isSaved)
        {
            result += "*" + digit + "* ";
        }
        else if (isSelected)
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

int calculateScore(vector<int> selectedDice, vector<int> rolledDice)
{
    for (int dice : selectedDice)
    {
        cout << rolledDice[dice] << " ";
    }
    cout << endl;

    return 0;
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