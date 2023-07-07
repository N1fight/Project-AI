#include "Header.h"
#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
#include <iostream>

void Rand(int ax[4], int ay[4]) {

    int figures[7][4] = // задание фигурок
    {
        1,3,5,7, // I
        2,4,5,7, // Z
        3,5,4,6, // S
        3,5,4,7, // T
        2,3,5,7, // L
        3,5,7,6, // J
        2,3,4,5, // O
    };

    int n = rand() % 7; // тип фигуры тоже случайно
    for (int i = 0; i < 4; i++)
    {
        ax[i] = figures[n][i] % 2;
        ay[i] = figures[n][i] / 2;
    }

}



