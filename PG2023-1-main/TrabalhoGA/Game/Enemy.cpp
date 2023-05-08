#include <math.h>

#include "Interfaces/Enemy.h"

#include "stb_image.h"
#include "Interfaces/Texture.h"

Enemy::Enemy(Sprite* i_sprite, float i_x, float i_y, float i_speedX, float i_speedY, int i_id)
{
    this->sprite = i_sprite;
    this->x = i_x;
    this->y = i_y;
    this->speed_x = i_speedX;
    this->speed_y = i_speedY;
    this->id = i_id;
}

void Enemy::set_sprite(Sprite* i_sprite)
{
    this->sprite = i_sprite;
}

void Enemy::set_speed_x(float i_speed)
{
    this->speed_x = i_speed;
}

void Enemy::set_speed_y(float i_speed)
{
    this->speed_y = i_speed;
}

void Enemy::move_x()
{
    this->x += speed_x;
    this->sprite->set_translation(glm::vec3(this->x, this->y, 0.0));
}

void Enemy::move_y()
{
    this->y += speed_y;
    this->sprite->set_translation(glm::vec3(this->x, this->y, 0.0));
}

int Enemy::load_texture(const string& path)
{
    return Texture().load(path);
}
