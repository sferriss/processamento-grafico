#include <math.h>

#include "Interfaces/Character.h"

#include "stb_image.h"

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
