#pragma once
#include "Sprite.h"
#include <string>

using namespace std;

class Enemy
{
public:
    Enemy(Sprite* i_sprite, float i_x, float i_y, float i_speedX, float i_speedY, int id);

    Sprite* sprite;
    float x = 0;
    float y = 0;
    float speed_x = 0;
    float speed_y = 0;
    int id = 0;

    void set_sprite(Sprite* i_sprite);
    void set_speed_x(float i_speed);
    void set_speed_y(float i_speed);
    void move_x();
    void move_y();

    static int load_texture(const string& path);
};
