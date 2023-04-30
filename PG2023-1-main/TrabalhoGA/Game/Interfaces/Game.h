#pragma once

#include "../../commonFiles/Shader.h"
#include "Sprite.h"
#include "Character.h"
#include <glad/glad.h>
#include "Enemy.h"
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace std;

class Game
{
public:
    Game();
    ~Game();

    void start();
    void run();

    void create_scene();
    void create_background();
    void update_character() const;
    void update_enemies() const;
    void check_conflict() const;
    void render();
    void reset();

    static int load_texture(const string& path);

private:
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void resize(GLFWwindow* window, int width, int height);
    void initialize_graphics();
    void setup_camera_2d();
    void add_shader(const string& v_filename, const string& f_filename);
    void create_character();
    void create_enemy(int id);

    GLFWwindow* window_{};
    Shader* shader_{};
    glm::mat4 projection_;

    vector<Sprite*> objects_;
    Character* character_{};
    Character* background_;
    vector<Enemy*> enemies_;
};
