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
void drawField(int roundScore, int totalScore, int maxScore, vector<int> rolledDice, vector<int> selectedDice, int indexSelectedDice, vector<int> savedIndexDice, bool continueRound, bool canContinue, int computerScore); // Рисуем и обновляем игровое поле
vector<int> generateRandomDigits(int numDigits); // Генерируем броски костей
string keabordInput(); // Отслеживание нажатия клавиатуры
int calculateScore(vector<int> selectedDice, vector<int> rolledDice); // Подсчитываем количество очков
bool checkRolledDiceCombination(vector<int> rolledDice); // Проверка выпавших костей на то, может ли игрок продолжить ход дальше
bool checkCombination(vector<int> savedIndexDice, vector<int> rolledDice); // Проверяем валидность комбинаций
vector<int> addSelectedDice(vector<int> savedIndexDice, vector<int> rolledDice); // Добавляем в массив кости, которые мы откладываем
vector<int> deleteRolledDice(vector<int> savedIndexDice, vector<int> rolledDice); // Убираем кости из основного потока, которые мы отложили
void endGame(bool isWin); // Конец игры
string setColor(string text, string colorCode); // Изменение цвета текста
int computerTurn(int computerTotalScore); // Имитация хода компьютера

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
    int maxScore = 4000; // Максимальное количество очков
    vector<int> rolledDice; // Кинутые кости
    vector<int> selectedDice; // Кости, которые мы отложили

    int computerTotalScor = 0;

    rolledDice = generateRandomDigits(6);

    int indexSelectedDice = 0; // Индекс кости, на которой остановился курсор
    vector<int> savedIndexDice; // Индекс кости, которые надо отложить

    // Параметры ввода
    string keabordInputs;
    bool rightButtonPressed = false;
    bool leftButtonPressed = false;
    bool spaceButtonPressed = false;
    bool qButtonPressed = false;
    bool eButtonPressed = false;

    // Параметры, которые позволят продолжить ход
    bool canContinue = false; // Можем ли мы отлжожить выбранные кости

    while (true)
    {
        if (rightButtonPressed || leftButtonPressed || spaceButtonPressed || qButtonPressed || eButtonPressed || isStartGame)
        {
            drawField(roundScore, totalScore, maxScore, rolledDice, selectedDice, indexSelectedDice, savedIndexDice, checkRolledDiceCombination(rolledDice), canContinue, computerTotalScor);

            isStartGame = false;
        }

        if (!checkRolledDiceCombination(rolledDice))
        {
            computerTotalScor = computerTurn(computerTotalScor);

            if (computerTotalScor >= maxScore)
            {
                drawField(roundScore, totalScore, maxScore, rolledDice, selectedDice, indexSelectedDice, savedIndexDice, checkRolledDiceCombination(rolledDice), canContinue, computerTotalScor);
                endGame(false);
            }
            else
            {
                roundScore = 0;
                rolledDice = generateRandomDigits(6);
                selectedDice.clear();

                continue;
            }
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

                canContinue = checkCombination(savedIndexDice, rolledDice);

                qButtonPressed = true;
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
                    drawField(roundScore, totalScore, maxScore, rolledDice, selectedDice, indexSelectedDice, savedIndexDice, checkRolledDiceCombination(rolledDice), canContinue, computerTotalScor);
                    endGame(true);
                }
                else
                {
                    computerTotalScor = computerTurn(computerTotalScor);

                    if (computerTotalScor >= maxScore)
                    {
                        drawField(roundScore, totalScore, maxScore, rolledDice, selectedDice, indexSelectedDice, savedIndexDice, checkRolledDiceCombination(rolledDice), canContinue, computerTotalScor);
                        endGame(false);
                    }
                    else
                    {
                        roundScore = 0;

                        rolledDice = generateRandomDigits(6);
                        selectedDice.clear();
                        savedIndexDice.clear();
                        indexSelectedDice = 0;

                        canContinue = checkCombination(savedIndexDice, rolledDice);
                    }
                }

                eButtonPressed = true;
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


void drawField(int roundScore, int totalScore, int maxScore, vector<int> rolledDice, vector<int> selectedDice, int indexSelectedDice, vector<int> savedIndexDice, bool continueRound, bool canContinue, int computerScore)
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
            result += setColor("[", "\033[1;35m") + setColor("*", "\033[1;34m") + digit + setColor("*", "\033[1;34m") + setColor("]", "\033[1;35m") + " ";
            // result += "[*" + digit + "*] ";
        }
        else if (isSaved)
        {
            result += setColor("*", "\033[1;34m") + digit + setColor("*", "\033[1;34m") + " ";
            // result += "*" + digit + "* ";
        }
        else if (isSelected)
        {
            result += setColor("[", "\033[1;35m") + digit + setColor("]", "\033[1;35m") + " ";
            // result += "[" + digit + "] ";
        }
        else
        {
            result += " " + digit + " ";
        }

        index++;
    }

    // setColor(15);
    cout << "Счет противника >> " << computerScore << endl << endl; 
    cout << setw(3) << totalScore << " / " << setw(4) << maxScore << " | " << result << endl;
    cout << "-----------|----------" << endl;
    
    cout << setw(10) << roundScore << " | " << selectedDiceString << endl;

    cout << endl << endl << "---------- Раскладка ----------" << endl;
    cout << "'<' / '>' - передвигаться по костям!" << endl;
    cout << "'SPACE' - выбрать кость!" << endl << endl;

    if (canContinue)
    {
        cout << setColor("'Q' - отложить выбранные кости и продолжить свой ход!", "\033[0;32m") << endl;
        cout << setColor("'E' - отложить выбранные кости и передать свой ход!", "\033[0;32m") << endl;
    }
    else
    {
        cout << setColor("'Q' - отложить выбранные кости и продолжить свой ход!", "\033[0;31m") << endl;
        cout << setColor("'E' - отложить выбранные кости и передать свой ход!", "\033[0;31m") << endl;
    }

    if (!continueRound)
    {
        cout << endl << endl << setColor("Не выпала нужная комбинация!", "\033[0;31m") << endl << endl;
        this_thread::sleep_for(chrono::seconds(1));
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
            case 1: one.push_back(dice); break;
            case 2: two.push_back(dice); break;
            case 3: three.push_back(dice); break;
            case 4: four.push_back(dice); break;
            case 5: five.push_back(dice); break;
            case 6: six.push_back(dice); break;
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
            case 1: one.push_back(dice); break;
            case 2: two.push_back(dice); break;
            case 3: three.push_back(dice); break;
            case 4: four.push_back(dice); break;
            case 5: five.push_back(dice); break;
            case 6: six.push_back(dice); break;
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

int computerTurn(int computerTotalScore)
{
    int computerRoundScore = 0;
    vector<int> rolledDice = generateRandomDigits(6);
    vector<int> selectedDice;
    bool canContinue = true;

    while (canContinue && !rolledDice.empty())
    {
        // Проверяем, есть ли комбинация для набора очков
        if (!checkRolledDiceCombination(rolledDice))
        {
            break;
        }

        // Ищем лучшую комбинацию
        vector<int> bestCombination;
        int bestScore = 0;

        // Перебираем все возможные комбинации
        for (int i = 0; i < static_cast<int>(rolledDice.size()); i++)
        {
            vector<int> currentCombination = {i};
            if (checkCombination(currentCombination, rolledDice))
            {
                int currentScore = calculateScore(currentCombination, rolledDice);
                if (currentScore > bestScore)
                {
                    bestScore = currentScore;
                    bestCombination = currentCombination;
                }
            }
        }

        if (bestCombination.empty())
        {
            break;
        }

        // Добавляем очки и обновляем кости
        computerRoundScore += bestScore;
        selectedDice = addSelectedDice(bestCombination, rolledDice);
        rolledDice = deleteRolledDice(bestCombination, rolledDice);

        // Решаем, продолжать ли ход
        if (rand() % 2 == 0 && !rolledDice.empty())
        {
            rolledDice = generateRandomDigits(rolledDice.size());
        }
        else
        {
            canContinue = false;
        }
    }

    computerTotalScore += computerRoundScore;

    return computerTotalScore;
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