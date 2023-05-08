#include "Interfaces/Sprite.h"

#include <glm/gtc/matrix_transform.hpp>

Sprite::Sprite()
{
    setup();
}

void Sprite::setup()
{
    GLuint vbo, ebo;

    constexpr float vertices[] = {
        // posicoes          // cores          // coordenadas de textura
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0, // superior direito
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // inferior direito
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // inferior esquerdo
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0 // superior esquerdo
    };
    unsigned int indices[] = {
        0, 1, 3, // primeiro triangulo
        1, 2, 3 // segundo triangulo
    };

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    tex_id_ = -1;
    model_ = glm::mat4(1);
    scale_ = glm::vec3(1.0f, 1.0f, 1.0f);
    translation_ = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation_ = 0.0f;
    shader_ = nullptr;
}

void Sprite::set_texture(const int tex_id)
{
    this->tex_id_ = tex_id;
}

void Sprite::handle_rotation(const float angle, const glm::vec3& axis, const bool reset)
{
    if (reset) model_ = glm::mat4(1);
    model_ = rotate(model_, angle, axis);
}

void Sprite::handle_translation(const glm::vec3& displacements, const bool reset)
{
    if (reset) model_ = glm::mat4(1);
    model_ = translate(model_, displacements);
}

void Sprite::handle_scale(const glm::vec3& scale_factors, const bool reset)
{
    if (reset) model_ = glm::mat4(1);
    model_ = glm::scale(model_, scale_factors);
    scale_ = scale_factors;
}

void Sprite::draw() const
{
    glBindTexture(GL_TEXTURE_2D, tex_id_);
    glUniform1i(glGetUniformLocation(shader_->id, "tex_buffer"), 0);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Sprite::update()
{
    handle_translation(translation_);
    handle_rotation(rotation_, glm::vec3(0.0f, 0.0f, 1.0f), false);
    handle_scale(scale_, false);

    const GLint model_loc = glGetUniformLocation(shader_->id, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, value_ptr(model_));
}
