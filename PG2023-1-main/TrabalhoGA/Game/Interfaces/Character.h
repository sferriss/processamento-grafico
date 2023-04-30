#pragma once
#include "Sprite.h" 
#include <string>

using namespace std;

class Character {
  public:
    Character(Sprite* i_sprite, float i_x, float i_y, float i_speed);

    Sprite* sprite;
    float x = 0;
    float y = 0;
    float speed = 0;

    void set_sprite(Sprite* i_sprite);
    void move_right();
    void move_left();
    void move_up();
    void move_down();
    void move_x();
    void move_y();

    static int load_texture(const string& path); 
};