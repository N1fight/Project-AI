#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
#include <iostream>
#include "Header.h"

using namespace std;
using namespace sf;

const int M = 22; // высота игрового поля
const int N = 10; // ширина игрового поля

int field[M][N] = { 0 }; // игровое поле

int ax[4], ay[4], bx[4], by[4];


bool check() // Проверка соблюдения границ и занятости ячеек
{
    for (int i = 0; i < 4; i++)
        if (ax[i] < 0 || ax[i] >= N || ay[i] >= M) return 0;
        else if (field[ay[i]][ax[i]]) return 0;

    return 1;
};

class rotatetetromino {
    public:
        void rot(int ax[4], int ay[4], int bx[4], int by[4]) {
            int px = ax[1], py = ay[1]; // центр вращение
            for (int i = 0; i < 4; i++)
            {
                int x = ay[i] - py; int y = ax[i] - px;
                ax[i] = px - x; ay[i] = py + y;
            }
            if (!check())
                for (int i = 0; i < 4; i++) {
                    ax[i] = bx[i]; ay[i] = by[i];
                }
        }
};

int main()
{
    srand(time(0));

    rotatetetromino rotate;

    // Создание окна
    RenderWindow window(VideoMode(320, 480), "The Game!");

    // текстуры
    Texture t1, t2_1, t3_1, t2_2, t3_2, t2_3, t3_3;
    t1.loadFromFile("..\\TetrisCMakeDebug\\images\\tiles.png");
    t2_1.loadFromFile("..\\TetrisCMakeDebug\\images\\background lvl 1.png");
    t3_1.loadFromFile("..\\TetrisCMakeDebug\\images\\frame lvl 1.png");
    t2_2.loadFromFile("..\\TetrisCMakeDebug\\images\\background lvl 2.png");
    t3_2.loadFromFile("..\\TetrisCMakeDebug\\images\\frame lvl 2.png");
    t2_3.loadFromFile("..\\TetrisCMakeDebug\\images\\background lvl 3.png");
    t3_3.loadFromFile("..\\TetrisCMakeDebug\\images\\frame lvl 3.png");


    Sprite s(t1), background_1(t2_1), frame_1(t3_1); // создание спрайтов

    int dx = 0; // горизонтальное смещение
    int colorNum = 1; // номер цвета
    float timer = 0; // счетчик таймера
    float delay = 0.3; // время смешения
    bool beginGame = true; // флаг начала
    sf::Text text; sf::Font font;
    font.loadFromFile("..\\TetrisCMakeDebug\\sfns-display-bold.ttf");
    string points = "0";

    Clock clock; // часы

    while (window.isOpen())
    {
        // Время с начала в секундах
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        // для первого тетрамино
        if (beginGame) { beginGame = false; Rand(ax, ay); }

        Event e;
        while (window.pollEvent(e))

        {
            if (e.type == Event::Closed) // пользователь нажал на крестик
                window.close();

            if (e.type == Event::KeyPressed) // нажатие клавиш на клавиатуре
                if (e.key.code == Keyboard::Up) rotate.rot(ax, ay, bx, by); //вращаем
                else if (e.key.code == Keyboard::Left) dx = -1; // влево
                else if (e.key.code == Keyboard::Right) dx = 1; // вправо
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05; // ускаряем падение

        //// <- Смещение -> ///
        for (int i = 0; i < 4; i++)
        { 
            bx[i] = ax[i]; // запоминаем изначальное положение
            by[i] = ay[i];
            ax[i] += dx; // смещаем
        }
        if (!check()) // если границы не соблюдаются 
            for (int i = 0; i < 4; i++) {
                ax[i] = bx[i]; // возвращаем обратно
                ay[i] = by[i];
            }       

        ///////Падение//////
        if (timer > delay)
        {
            for (int i = 0; i < 4; i++) //смещаем вниз
            { bx[i] = ax[i]; by[i] = ay[i]; ay[i] += 1; }

            if (!check())
            {
                for (int i = 0; i < 4; i++) // закрашиваем ячейки
                    field[by[i]][bx[i]] = colorNum;

                colorNum = 1 + rand() % 7; // меняем цвет случайно
                Rand(ax, ay);
            }

            timer = 0; // зануляем счетчик
        }

        ///Проверка на проигрыш
        for (int i = 0; i < N; i++) {
            if (field[1][i] != 0) {
                window.close();
            }
        }

        ///////уничтожение строк//////////
        int k = M - 1; // коодината нижней строки
        for (int i = M - 1; i > 0; i--) // снизу вверх проверяет строки
        {
            int count = 0; // счетчик закрашенных квадратов
            for (int j = 0; j < N; j++)
            {
                if (field[i][j]) count++; // провекра на пустотоу 
                field[k][j] = field[i][j];
            }
            if (count < N) {
                k--;
            }
            if (count == 10) {
                points = to_string(stoi(points) + 1);
            }
        }

        // Количество очков
        text.setFont(font); text.setString("Points: " + points);
        text.setCharacterSize(20); text.setPosition(12, 0);
        text.setColor(sf::Color::Color(255, 215, 0));
        text.setStyle(sf::Text::Bold | sf::Text::Underlined);



        /////////Окно и скорость//////////

        dx = 0;

        window.clear(Color::White);

        if (stoi(points) < 1) {
            delay = 0.3; // возвращаем значения
            window.draw(background_1); // на фон картинку
        }

        else if (stoi(points) < 3) {
            delay = 0.16; // возвращаем значения
            Sprite background_2(t2_2);
            window.draw(background_2); // на фон картинку
        }
       
        else {
            delay = 0.09; // возвращаем значения
            Sprite background_3(t2_3);
            window.draw(background_3); // на фон картинку
        }


        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (field[i][j] == 0) continue;
                s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18)); // создание плиток
                s.setPosition(j * 18, i * 18); // позиция каждоя ячейки
                s.move(28, -5); // смещаем поле
                window.draw(s); // отрисовка
            }

        for (int i = 0; i < 4; i++) // отрисовка тетрамино
        {
            s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
            s.setPosition(ax[i] * 18, ay[i] * 18);
            s.move(28, -5); 
            window.draw(s);
        }

        if (stoi(points) < 1) {
            window.draw(frame_1);
        }

        else if (stoi(points) < 3) {
     
            Sprite frame_2(t3_2);
            window.draw(frame_2);
        }

        else {
            Sprite frame_3(t3_3);
            window.draw(frame_3); // на фон картинку
        }

        window.draw(text);
        window.display();

    }

    return 0;
}
