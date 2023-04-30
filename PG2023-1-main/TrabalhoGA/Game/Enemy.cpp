#include <math.h>

#include "Interfaces/Enemy.h"

#include "stb_image.h"

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
    GLuint tex_id;

    // Gera o identificador da textura na mem�ria 
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    //Ajusta os par�metros de wrapping e filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Carregamento da imagem
    int width, height, nr_channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nr_channels, 0);

    if (data)
    {
        if (nr_channels == 3) //jpg, bmp
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else //png
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return tex_id;
}
