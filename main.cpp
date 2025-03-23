#include <iostream>
#include <windows.h>
#include <iomanip>
#include <random>
#include <vector>
#include <algorithm>
#include <map>
#include <thread>
#include <chrono>

using namespace std;


void mainGame(); // Основная логика игры
void drawField(int roundScore, int totalScore, int maxScore, vector<int> rolledDice, vector<int> selectedDice, int indexSelectedDice, vector<int> savedIndexDice, bool continueRound, bool canContinue); // Рисуем и обновляем игровое поле
vector<int> generateRandomDigits(int numDigits); // Генерируем броски костей
string keabordInput(); // Отслеживание нажатия клавиатуры
int calculateScore(vector<int> selectedDice, vector<int> rolledDice); // Подсчитываем количество очков
bool checkRolledDiceCombination(vector<int> rolledDice); // Проверка выпавших костей на то, может ли игрок продолжить ход дальше
bool checkCombination(vector<int> savedIndexDice, vector<int> rolledDice); // Проверяем валидность комбинаций
vector<int> addSelectedDice(vector<int> savedIndexDice, vector<int> rolledDice); // Добавляем в массив кости, которые мы откладываем
vector<int> deleteRolledDice(vector<int> savedIndexDice, vector<int> rolledDice); // Убираем кости из основного потока, которые мы отложили
void endGame(bool isWin); // Конец игры
void setColor(int color); // Изменение цвета текста

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
    bool eButtonPressed = false;

    // Параметры, которые позволят продолжить ход
    bool canContinue = false; // Можем ли мы отлжожить выбранные кости
    bool characterStep = true; // Ход игрока

    while (characterStep)
    {
        if (rightButtonPressed || leftButtonPressed || spaceButtonPressed || qButtonPressed || eButtonPressed || isStartGame)
        {
            drawField(roundScore, totalScore, maxScore, rolledDice, selectedDice, indexSelectedDice, savedIndexDice, checkRolledDiceCombination(rolledDice), canContinue);

            isStartGame = false;
        }

        if (eButtonPressed)
        {
            characterStep = false;
            break;
        }

        if (!checkRolledDiceCombination(rolledDice))
        {
            characterStep = false;
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
                if (indexSelectedDice != rolledDice.size() - 1 && !rightButtonPressed)
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

                canContinue = checkCombination(savedIndexDice, rolledDice);

                spaceButtonPressed = true;
                }
            }
            else if (keabordInputs == "Q")
            {
                if (!qButtonPressed && canContinue)
                {
                    roundScore += calculateScore(savedIndexDice, rolledDice);
                    vector<int> newDice = addSelectedDice(savedIndexDice, rolledDice);
                    selectedDice.insert(selectedDice.end(), newDice.begin(), newDice.end());
                    rolledDice = deleteRolledDice(savedIndexDice, rolledDice);

                    savedIndexDice.clear();
                    indexSelectedDice = 0;

                    if (rolledDice.empty())
                    {
                        rolledDice = generateRandomDigits(6);
                        selectedDice.clear();
                    }
                    else
                    {
                        rolledDice = generateRandomDigits(rolledDice.size());
                    }

                    qButtonPressed = true;
                    canInput = false;
                }
            }
            else if (keabordInputs == "E" && canContinue)
            {
                if (!eButtonPressed && canContinue)
                {
                    roundScore += calculateScore(savedIndexDice, rolledDice);

                    totalScore += roundScore;
                    roundScore = 0;

                    if (totalScore >= maxScore)
                    {
                        endGame(true);
                    }

                    eButtonPressed = true;
                    canInput = false;
                }
            }
            else
            {
                leftButtonPressed = false;
                rightButtonPressed = false;
                spaceButtonPressed = false;
                qButtonPressed = false;
                eButtonPressed = false;
            }
        }
    }

    cin.get();
}


void drawField(int roundScore, int totalScore, int maxScore, vector<int> rolledDice, vector<int> selectedDice, int indexSelectedDice, vector<int> savedIndexDice, bool continueRound, bool canContinue)
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

    setColor(15);
    cout << setw(3) << totalScore << " / " << setw(4) << maxScore << " | " << result << endl;
    cout << "-----------|----------" << endl;
    
    cout << setw(10) << roundScore << " | " << selectedDiceString << endl;

    cout << endl << endl << "---------- Раскладка ----------" << endl;
    cout << "'<' / '>' - передвигаться по костям!" << endl;
    cout << "'SPACE' - выбрать кость!" << endl << endl;

    if (canContinue)
    {
        setColor(10);
    }
    else
    {
        setColor(12);
    }

    cout << "'Q' - отложить выбранные кости и продолжить свой ход!" << endl;
    cout << "'E' - отложить выбранные кости и передать свой ход!" << endl;

    if (!continueRound)
    {
        setColor(12);
        cout << endl << "Не выпала нужная комбинация. Попробуйте снова..." << endl;
        this_thread::sleep_for(chrono::seconds(1));

        endGame(false);
    }
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

bool checkRolledDiceCombination(vector<int> rolledDice)
{
    string rolledCombination;

    for (int digit : rolledDice)
    {
        rolledCombination += to_string(digit);
    }

    if (rolledCombination == "123456" || rolledCombination == "23456" || rolledCombination == "12345")
    {
        return true;
    }

    vector<int> one, two, three, four, five, six;

    for (int dice : rolledDice)
    {
        switch (dice)
        {
            case 1:
                one.push_back(dice);
                break;
            case 2:
                two.push_back(dice);
                break;
            case 3:
                three.push_back(dice);
                break;
            case 4:
                four.push_back(dice);
                break;
            case 5:
                five.push_back(dice);
                break;
            case 6:
                six.push_back(dice);
                break;
        }
    }

    if (one.size() != 0 || five.size() != 0 || two.size() >= 3 || three.size() >= 3 || four.size() >= 3 || six.size() >= 3)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool checkCombination(vector<int> savedIndexDice, vector<int> rolledDice)
{
    if (savedIndexDice.empty())
    {
        return false;
    }
    // Проверяем наличие нестандартной комбинации
    string rolledCombination;

    for (int index : savedIndexDice)
    {
        rolledCombination += to_string(rolledDice[index]);
    }

    if (rolledCombination == "123456" || rolledCombination == "23456" || rolledCombination == "12345")
    {
        return true;
    }

    // Если нестандартной комбанации нет, то ищем обычные комбинации
    vector<int> one, two, three, four, five, six;

    for (int index : savedIndexDice)
    {
        int dice = rolledDice[index];

        switch (dice)
        {
            case 1:
                one.push_back(dice);
                break;
            case 2:
                two.push_back(dice);
                break;
            case 3:
                three.push_back(dice);
                break;
            case 4:
                four.push_back(dice);
                break;
            case 5:
                five.push_back(dice);
                break;
            case 6:
                six.push_back(dice);
                break;
        }
    }

    if ((two.size() < 3 && two.size() > 0) || (three.size() < 3 && three.size() > 0) || (four.size() < 3 && four.size() > 0) || (six.size() < 3 && six.size() > 0))
    {
        return false;
    }
    else
    {
        return true;
    }
    
    if (one.size() != 0 || five.size() != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
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
    vector<int> one, two, three, four, five, six;

    for (int index : savedIndexDice)
    {
        int dice = rolledDice[index];

        switch (dice)
        {
            case 1: one.push_back(dice); break;
            case 2: two.push_back(dice); break;
            case 3: three.push_back(dice); break;
            case 4: four.push_back(dice); break;
            case 5: five.push_back(dice); break;
            case 6: six.push_back(dice); break;
        }
    }

    int score = 0;

    // Проверяем комбинации с удиницой
    if (one.size() <= 2)
    {
        score += one.size() * 100;
    }
    else
    {
        score += 1000;
        for (size_t i = 3; i < one.size(); ++i)
        {
            score *= 2;
        }
    }

    // Проверяем кобинации с пятеркой
    if (five.size() <= 2)
    {
        score += five.size() * 50;
    }
    else
    {
        score += 500;
        for (size_t i = 3; i < five.size(); ++i)
        {
            score *= 2;
        }
    }

    // Проверяем комбинации с оставшимися цифрами
    vector<vector<int>> groups = {two, three, four, six};
    vector<int> values = {2, 3, 4, 6};

    for (size_t i = 0; i < groups.size(); ++i)
    {
        int diceValue = values[i];
        if (groups[i].size() >= 3)
        {
            int baseScore = diceValue * 100;
            score += baseScore;

            for (size_t j = 3; j < groups[i].size(); ++j)
            {
                score *= 2;
            }
        }
    }

    // Проверяем комбинации 1-2-3-4-5-6 (1500 очков)
    if (!one.empty() && !two.empty() && !three.empty() &&
        !four.empty() && !five.empty() && !six.empty()) {
        return 1500;
    }

    // Проверяем комбинации без 1 (750 очков) или без 6 (500 очков)
    if (two.size() > 0 && three.size() > 0 && four.size() > 0 && five.size() > 0) {
        if (!one.empty()) return 750;
        if (!six.empty()) return 500;
    }

    return score;
}

void endGame(bool isWin)
{
    if (isWin)
    {
        setColor(10);
        cout << "Вы победили!" << endl;
    }
    else
    {
        setColor(12);
        cout << "Вы проиграли!" << endl;
    }
    
    this_thread::sleep_for(chrono::seconds(3));
    exit(0);
}

void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
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