#include "Interfaces/Game.h"

#include "stb_image.h"

static constexpr float width_default = 1300.0f, height_default = 900.0f, height_car = 90.0f, width_car = 45.0f;
static float width = width_default, height = height_default;
static bool resized, left_pressed, right_pressed, up_pressed, down_pressed, started, game_over, ready_to_start = true;

Game::Game() = default;

Game::~Game() = default;

void Game::start()
{
    initialize_graphics();
}

void Game::initialize_graphics()
{
    glfwInit();

    window_ = glfwCreateWindow(width, height, "Trabalho GA - Sandro Ferri", nullptr, nullptr);
    glfwMakeContextCurrent(window_);
    glfwSetKeyCallback(window_, key_callback);
    glfwSetWindowSizeCallback(window_, resize);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported " << version << endl;

    add_shader("Shaders/sprite.vs", "Shaders/sprite.fs");

    resized = true;
}

void Game::add_shader(const string& v_filename, const string& f_filename)
{
    shader_ = new Shader(v_filename.c_str(), f_filename.c_str());
}

void Game::key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        right_pressed = true;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        left_pressed = true;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        up_pressed = true;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        down_pressed = true;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
        right_pressed = false;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
        left_pressed = false;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
        up_pressed = false;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
        down_pressed = false;

    if (!started && !left_pressed && !right_pressed && !up_pressed && !down_pressed)
        ready_to_start = true;

    if (ready_to_start && (left_pressed || right_pressed || up_pressed || down_pressed))
    {
        started = true;
        ready_to_start = false;
    }
}

void Game::resize(GLFWwindow* window, const int w, const int h)
{
    width = w;
    height = h;
    resized = true;

    glViewport(0, 0, width, height);
}

void Game::run()
{
    clock_t start = 0;
    int id_enemy = 0;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    create_scene();

    while (!glfwWindowShouldClose(window_))
    {
        glfwPollEvents();

        if (started)
        {
            if (game_over)
                break;

            if ((clock() - start) / (CLOCKS_PER_SEC / 4) > 3 && id_enemy < 194)
            {
                id_enemy++;
                start = clock();
                create_enemy(id_enemy);
            }

            update_character();
            update_enemies();
            check_collision();
        }

        render();

        glfwSwapBuffers(window_);
    }

    if (started && game_over)
        reset();
}

void Game::create_scene()
{
    create_background();
    create_character();
}

void Game::create_background()
{
    auto* sprite = new Sprite;
    const unsigned int tex_id = load_texture("Textures/Background/city.png");

    sprite->set_texture(tex_id);
    sprite->set_translation(glm::vec3(width / 2, height / 2, 0.0));
    sprite->set_scale(glm::vec3(width * 1.25, height * 1.25, 1.0f));
    sprite->set_shader(shader_);
    objects_.push_back(sprite);

    background_ = new Character(sprite, width / 2, height / 2, 0.05f);
}

void Game::create_character()
{
    auto* sprite = new Sprite;
    const int tex_id = load_texture("Textures/Cars/117.png");
    sprite->set_texture(tex_id);
    sprite->set_translation(glm::vec3(width / 2, height / 2, 0));
    sprite->set_scale(glm::vec3(height_car, width_car, 1.0));
    sprite->set_shader(shader_);
    objects_.push_back(sprite);

    character_ = new Character(sprite, width / 2, height / 2, 0.5f);
}

void Game::create_enemy(int id)
{
    constexpr float x_initial = height_car;
    const float y_initial = rand() % static_cast<int>(height) - width_car / 2;

    auto* sprite = new Sprite;
    const int tex_id = load_texture("Textures/Cars/" + std::to_string(id) + ".png");
    sprite->set_texture(tex_id);
    sprite->set_translation(glm::vec3(x_initial, y_initial, 0));
    sprite->set_scale(glm::vec3(height_car, width_car, 1.0));
    sprite->set_shader(shader_);
    objects_.push_back(sprite);
    const float speed = 0.05f * id;

    auto* enemy = new Enemy(sprite, x_initial, y_initial, speed, speed, id);
    enemies_.push_back(enemy);
}

void Game::update_character() const
{
    if (left_pressed && character_->x > height_car / 2)
    {
        character_->move_left();
        background_->move_right();
    }

    if (right_pressed && character_->x < width_default - height_car / 2)
    {
        character_->move_right();
        background_->move_left();
    }

    if (up_pressed && character_->y < height_default - width_car / 2)
    {
        character_->move_up();
        background_->move_down();
    }

    if (down_pressed && character_->y > width_car / 2)
    {
        character_->move_down();
        background_->move_up();
    }
}

void Game::update_enemies() const
{
    for (const auto enemy : enemies_)
    {
        detect_and_handle_edge_collision(enemy);

        enemy->move_x();
        enemy->move_y();
    }
}

void Game::detect_and_handle_edge_collision(Enemy* enemy)
{
    if (enemy->x > width_default - width_car / 2 || enemy->x < width_car / 2)
    {
        enemy->set_speed_x(-enemy->speed_x);
    }
    if (enemy->y > height_default - width_car / 2 || enemy->y < width_car / 2)
    {
        enemy->set_speed_y(-enemy->speed_y);
    }
}

void Game::check_collision() const
{
    for (const auto enemy : enemies_)
    {
        if (is_collision(character_, enemy))
        {
            end_game();
            break;
        }
    }
}

bool Game::is_collision(const Character* character, const Enemy* enemy)
{
    const float enemy_left = enemy->x;
    const float enemy_right = enemy->x + width_car * 2;
    const float enemy_top = enemy->y;
    const float enemy_bottom = enemy->y + height_car / 2;

    const float character_left = character->x;
    const float character_right = character->x + width_car * 2;
    const float character_top = character->y;
    const float character_bottom = character->y + height_car / 2;

    return (character_right >= enemy_left
        && character_left <= enemy_right
        && character_bottom >= enemy_top
        && character_top <= enemy_bottom);
}

void Game::end_game()
{
    game_over = true;
    started = false;
    ready_to_start = !left_pressed && !right_pressed && !up_pressed && !down_pressed;
}


void Game::render()
{
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (resized)
    {
        setup_camera_2d();
        resized = false;
    }

    for (const auto& object : objects_)
    {
        object->update();
        object->draw();
    }
}

void Game::reset()
{
    game_over = false;
    objects_.clear();
    enemies_.clear();
    run();
}

int Game::load_texture(const string& path)
{
    GLuint tex_id;

    // Gera o identificador da textura na mem�ria 
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    //Ajusta os parametros de wrapping e filtering
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

void Game::setup_camera_2d()
{
    projection_ = glm::ortho(0.0f, width_default, 0.0f, height_default, -1.0f, 1.0f);
    const GLint proj_loc = glGetUniformLocation(shader_->id, "projection");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, value_ptr(projection_));
}
