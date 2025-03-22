#include <iostream>
#include <windows.h>
#include <iomanip>
#include <random>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;


void mainGame();
void drawField(int roundScore, int totalScore, int maxScore, vector<int> rolledDice, vector<int> selectedDice, int indexSelectedDice, vector<int> savedIndexDice);
vector<int> generateRandomDigits(int numDigits);
string keabordInput();
int calculateScore(vector<int> selectedDice, vector<int> rolledDice);
vector<int> addSelectedDice(vector<int> savedIndexDice, vector<int> rolledDice);
vector<int> deleteRolledDice(vector<int> savedIndexDice, vector<int> rolledDice);



map<string, int> diceScores;

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    diceScores["1"] = 100;
    diceScores["11"] = 200;
    diceScores["111"] = 1000;
    diceScores["1111"] = 2000;
    diceScores["11111"] = 4000;
    diceScores["111111"] = 8000;
    diceScores["222"] = 200;
    diceScores["2222"] = 400;
    diceScores["22222"] = 800;
    diceScores["222222"] = 1600;
    diceScores["333"] = 300;
    diceScores["3333"] = 600;
    diceScores["33333"] = 1200;
    diceScores["333333"] = 2400;
    diceScores["444"] = 400;
    diceScores["4444"] = 800;
    diceScores["44444"] = 1600;
    diceScores["444444"] = 3200;
    diceScores["5"] = 50;
    diceScores["55"] = 100;
    diceScores["555"] = 500;
    diceScores["5555"] = 1000;
    diceScores["55555"] = 2000;
    diceScores["555555"] = 4000;
    diceScores["666"] = 600;
    diceScores["6666"] = 1200;
    diceScores["66666"] = 2400;
    diceScores["666666"] = 4800;
    diceScores["123456"] = 1500;
    diceScores["23456"] = 750;
    diceScores["12345"] = 500;

    mainGame();

    return 0;
}

void mainGame()
{
    bool isStartGame = true;
    
    // Переменные
    int roundScore = 0; // Очки раунда
    int totalScore = 0; // Общее количество очков
    int maxScore = 1000; // Максимальное количество очков
    vector<int> rolledDice; // Кинутые кости
    vector<int> selectedDice; // Кости, которые мы отложили

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
        if (rightButtonPressed || leftButtonPressed || spaceButtonPressed || qButtonPressed || eButtonPresses || isStartGame)
        {
            drawField(roundScore, totalScore, maxScore, rolledDice, selectedDice, indexSelectedDice, savedIndexDice);
            isStartGame = false;
        }

        if (true)
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
                    roundScore += calculateScore(savedIndexDice, rolledDice);
                    vector<int> newDice = addSelectedDice(savedIndexDice, rolledDice);
                    selectedDice.insert(selectedDice.end(), newDice.begin(), newDice.end());
                    rolledDice = deleteRolledDice(savedIndexDice, rolledDice);

                    savedIndexDice.clear();
                    indexSelectedDice = 0;

                    rolledDice = generateRandomDigits(rolledDice.size());

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


void drawField(int roundScore, int totalScore, int maxScore, vector<int> rolledDice, vector<int> selectedDice, int indexSelectedDice, vector<int> savedIndexDice)
{
    system("cls");

    string rolledDiceString;
    string selectedDiceString;

    for (int digit : rolledDice)
    {
        rolledDiceString += to_string(digit);
        rolledDiceString += " ";
    }

    for (int digit : selectedDice)
    {
        selectedDiceString += to_string(digit);
        selectedDiceString += " ";
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
    
    cout << setw(10) << roundScore << " | " << selectedDiceString << endl;
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

vector<int> addSelectedDice(vector<int> savedIndexDice, vector<int> rolledDice)
{
    vector<int> selectedDice;

    for (int dice : savedIndexDice)
    {
        selectedDice.push_back(rolledDice[dice]);
    }

    return selectedDice;
}

vector<int> deleteRolledDice(vector<int> savedIndexDice, vector<int> rolledDice)
{
    for (int dice : savedIndexDice)
    {
        rolledDice.erase(rolledDice.begin() + dice);
    }

    return rolledDice;
}

int calculateScore(vector<int> savedIndexDice, vector<int> rolledDice)
{
    string combination;

    for (int dice : savedIndexDice)
    {
        combination += to_string(rolledDice[dice]);
    }

    if (diceScores[combination])
    {
        return diceScores[combination];
    }
    else
    {
        int score = 0;

        for (int dice : savedIndexDice)
        {
            score += diceScores[to_string(rolledDice[dice])];
        }

        return score;
    }
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