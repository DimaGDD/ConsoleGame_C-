#include <iostream>
#include <windows.h>
#include <thread>

#include "dima.h"

using namespace std;





void typewriterEffect();
void setCursorPos(int x, int y);
void clearConsole();
void printWithDelay(const string& text, int delayMs);
void eraseWithDelay(int lines, int delayMs);

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    system("cls");
    // typewriterEffect();

    mainGame();

    return 0;
}

// Установка позиции курсора
void setCursorPos(int x, int y) {
    COORD coord = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Очистка консоли
void clearConsole() {
    system("cls");
}

// Постепенный вывод текста
void printWithDelay(const string& text, int delayMs = 50) {
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
}

// Эффект стирания текста
void eraseWithDelay(int startY, int lines, int delayMs = 30) {
    for (int i = 0; i < lines; i++) {
        setCursorPos(5, startY + i);
        cout << string(40, ' '); // Заполняем строку пробелами
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
}

// Эффект печатной машинки для логотипа
void typewriterEffect() {
    const string logo =
        "      ██████╗ ██╗ ██████╗███████╗\n"
        "     ██╔═══██╗██║██╔════╝██╔════╝\n"
        "     ██║   ██║██║██║     █████╗  \n"
        "     ██║   ██║██║██║     ██╔══╝  \n"
        "     ╚██████╔╝██║╚██████╗███████╗\n"
        "      ╚═════╝ ╚═╝ ╚═════╝╚══════╝\n";

    clearConsole();
    int x = 5, y = 2;
    setCursorPos(x, y);

    for (char c : logo) {
        if (c == '\n') {
            y++;
            setCursorPos(x, y);
        } else {
            cout << c << flush;
            this_thread::sleep_for(chrono::milliseconds(1));
        }
    }

    // Эффект мигания "DICE MASTER Y"
    setCursorPos(12, y + 2);
    for (int i = 0; i < 3; i++) {
        cout << "DICE MASTER Y";
        this_thread::sleep_for(chrono::milliseconds(300));
        setCursorPos(12, y + 2);
        cout << "             "; // Стираем текст
        this_thread::sleep_for(chrono::milliseconds(200));
    }
    setCursorPos(12, y + 2);
    cout << "DICE MASTER Y"; // Финальное отображение

    // Эффект волнового стирания
    this_thread::sleep_for(chrono::milliseconds(1000));
    eraseWithDelay(2, 8, 80);
}