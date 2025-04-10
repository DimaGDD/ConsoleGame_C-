#pragma once

#include <vector>

using namespace std;

void mainGame(); // Основная логика игры
void drawField(int roundScore, int totalScore, int maxScore, vector<int> rolledDice, vector<int> selectedDice, int indexSelectedDice, vector<int> savedIndexDice, bool continueRound, bool canContinue, int computerScore); // Рисуем и обновляем игровое поле
int calculateScore(vector<int> selectedDice, vector<int> rolledDice); // Подсчитываем количество очков
vector<int> addSelectedDice(vector<int> savedIndexDice, vector<int> rolledDice); // Добавляем в массив кости, которые мы откладываем
vector<int> deleteRolledDice(vector<int> savedIndexDice, vector<int> rolledDice); // Убираем кости из основного потока, которые мы отложили