#include <iostream>

#include "danil.h"
#include "lera.h"
#include "dima.h"


using namespace std;

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

int computerTurn(int computerTotalScore, int playerScore)
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

        int randomizer;

        if (playerScore - computerTotalScore > 1000)
        {
            randomizer = 1;
        }
        else
        {
            randomizer = 2;
        }
        
        // Решаем, продолжать ли ход
        if (rand() % randomizer == 0 && !rolledDice.empty())
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