#pragma once

#include <vector>
#include <iostream>

using namespace std;

vector<int> generateRandomDigits(int numDigits); // Генерируем броски костей
string keabordInput(); // Отслеживание нажатия клавиатуры
void endGame(bool isWin); // Конец игры
string setColor(string text, string colorCode); // Изменение цвета текста