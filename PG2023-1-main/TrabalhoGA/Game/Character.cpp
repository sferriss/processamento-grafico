#include <math.h>

#include "Interfaces/Character.h"

#include "stb_image.h"
#include "Interfaces/Texture.h"

Character::Character(Sprite* i_sprite, float i_x, float i_y, float i_speed)
{
    this->sprite = i_sprite;
    this->x = i_x;
    this->y = i_y;
    this->speed = i_speed;
}

void Character::set_sprite(Sprite* i_sprite)
{
    this->sprite = i_sprite;
}

void Character::move_right()
{
    this->x += speed;
    this->sprite->set_translation(glm::vec3(this->x, this->y, 0.0));
}

void Character::move_left()
{
    this->x -= speed;
    this->sprite->set_translation(glm::vec3(this->x, this->y, 0.0));
}

void Character::move_up()
{
    this->y += speed;
    this->sprite->set_translation(glm::vec3(this->x, this->y, 0.0));
}

void Character::move_down()
{
    this->y -= speed;
    this->sprite->set_translation(glm::vec3(this->x, this->y, 0.0));
}

void Character::move_x()
{
    this->x += speed;
    this->sprite->set_translation(glm::vec3(this->x, this->y, 0.0));
}

void Character::move_y()
{
    this->y += speed;
    this->sprite->set_translation(glm::vec3(this->x, this->y, 0.0));
}

int Character::load_texture(const string& path)
{
    return Texture().load(path);
}
