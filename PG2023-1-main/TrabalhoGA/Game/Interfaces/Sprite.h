#pragma once

#include "../../commonFiles/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sprite
{
public:
    Sprite();

    ~Sprite()
    = default;

    void setup();

    void set_texture(int tex_id);

    void set_shader(Shader* shader)
    {
        this->shader_ = shader;
        shader->use();
    }

    void set_translation(const glm::vec3& translation) { this->translation_ = translation; }
    void set_scale(const glm::vec3& scale) { this->scale_ = scale; }
    void set_rotation(const float rotation) { this->rotation_ = rotation; }

    void handle_translation(const glm::vec3& displacements, bool reset = true);
    void handle_scale(const glm::vec3& scale_factors, bool reset = true);
    void handle_rotation(float angle, const glm::vec3& axis, bool reset = true);

    void draw() const;
    void update();

protected:
    int tex_id_{};
    glm::mat4 model_;
    glm::vec3 scale_, translation_;
    float rotation_{};
    Shader* shader_{};
    GLuint vao_{};
};
