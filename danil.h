#pragma once

#include <vector>

using namespace std;

bool checkRolledDiceCombination(vector<int> rolledDice); // Проверка выпавших костей на то, может ли игрок продолжить ход дальше
bool checkCombination(vector<int> savedIndexDice, vector<int> rolledDice); // Проверяем валидность комбинаций
int computerTurn(int computerTotalScore, int playerScore); // Имитация хода компьютера