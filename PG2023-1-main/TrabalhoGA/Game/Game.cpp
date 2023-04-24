#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//STB IMAGE
#include "../../dependencies/stb_image/stb_image.h"

//Classe shader
#include "../../commonFiles/Shader.h"


// Protótipos das funções
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//int setup_geometry();
int setup_sprite();
GLuint generate_texture(string file_path, int& width, int& height);
void configure_drawing(Shader shader, int width, int height, GLuint tex_id, glm::mat4& model, GLint& model_loc,
                       glm::vec3 char_pos);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint width = 1000, height = 800;
const float radius = 35;

glm::vec3 char_pos;

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(width, height, "Game", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Fazendo o registro da função de callback para a janela GLFW
    glfwSetKeyCallback(window, key_callback);

    // GLAD: carrega todos os ponteiros d funções da OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    // Compilando e buildando o programa de shader
    Shader shader("./Shaders/Game.vs", "./Shaders/Game.fs");

    int bgwidth, bgheight;
    int charwidth, charheight;
    char_pos.x = 100;
    char_pos.y = 110;
    char_pos.z = 0;

    GLuint texID = generate_texture("./Textures/Background/city.png", bgwidth, bgheight);
    GLuint texID2 = generate_texture("./Textures/Cars/132.png", charwidth, charheight);

    GLuint VAO_background = setup_sprite();
    GLuint VAO_character = setup_sprite();

    glUseProgram(shader.ID);

    glm::mat4 projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);

    GLint projLoc = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

    //Ativar o buffer de textura
    glActiveTexture(GL_TEXTURE0);

    //Habilitar teste de profundidade
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    //Habilitar a transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Loop da aplicação - "game loop"
    while (!glfwWindowShouldClose(window))
    {
        // Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
        glfwPollEvents();

        // Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // Limpa o buffer de cor
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
        glClear(GL_COLOR_BUFFER_BIT);

        glLineWidth(1);
        glPointSize(5);

        glBindVertexArray(VAO_background); //Conectando ao buffer de geometria

        //Matriz de modelo para desenho do background
        glm::mat4 model;
        GLint modelLoc;
        configure_drawing(shader, bgwidth * 0.6, bgheight * 0.6, texID, model, modelLoc, glm::vec3(400.0, 300.0, 0.0));


        configure_drawing(shader, 75, 37.5, texID2, model, modelLoc, char_pos);

        glBindVertexArray(0); //Desconectando o buffer de geometria

        // Troca os buffers da tela
        glfwSwapBuffers(window);
    }
    // Pede pra OpenGL desalocar os buffers
    glDeleteVertexArrays(1, &VAO_background);
    // Finaliza a execução da GLFW, limpando os recursos alocados por ela
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    //if (char_pos.x < radius || char_pos.x > width - radius || char_pos.y < radius || char_pos.y > height - radius)
    
    if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
    {
        if (char_pos.x < 765 || char_pos.x > width - radius)
        {
            char_pos.x += 5.0;
        }
    }
    if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
    {
        if (char_pos.x > radius)
        {
            char_pos.x -= 5.0;
        }
    }
    if (key == GLFW_KEY_W || key == GLFW_KEY_UP)
    {
        if (char_pos.y < 580 || char_pos.y > height - radius)
        {
            char_pos.y += 5.0;
        }
    }
    if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN)
    {
        if (char_pos.y > radius - 20)
        {
            char_pos.y -= 5.0;
        }
    }
}

/*int setup_geometry()
{
	
	GLfloat vertices[] = {
		//x   y     z    r    g    b     s     t
		-0.5 , -0.5 , 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, //v0
		 0.5 , -0.5 , 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, //v1
		 0.0 , 0.5 ,  0.0, 0.0, 0.0, 1.0, 0.5, 1.0  //v2
	};

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Atributo posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo coordenada de textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	
	glBindVertexArray(0); 

	return VAO;
}*/

int setup_sprite()
{
    GLfloat vertices[] = {
        //x   y     z    r    g    b     s     t
        //T1
        -0.5, 0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, //v0
        -0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, //v1
        0.5, 0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, //v2
        //T2
        -0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, //v1
        0.5, 0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, //v2
        0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 //v3
    };

    GLuint VBO, VAO;

    //Geração do identificador do VBO
    glGenBuffers(1, &VBO);
    //Faz a conexão (vincula) do buffer como um buffer de array
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Envia os dados do array de floats para o buffer da OpenGl
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Geração do identificador do VAO (Vertex Array Object)
    glGenVertexArrays(1, &VAO);
    // Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
    // e os ponteiros para os atributos 
    glBindVertexArray(VAO);

    //Atributo posição
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //Atributo cor
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //Atributo coordenada de textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
    // atualmente vinculado - para que depois possamos desvincular com segurança
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
    glBindVertexArray(0);

    return VAO;
}

GLuint generate_texture(string file_path, int& width, int& height)
{
    GLuint texID;
    // Gera o identificador da textura na memória 
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    //Definindo o método de wrapping e de filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Carregando a imagen da textura
    int nrChannels;
    unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0);

    //Manda para OpenGL armazenar a textura e gerar o mipmap
    if (data)
    {
        if (nrChannels == 3) //jpg, bmp
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

    return texID;
}

void configure_drawing(Shader shader, int width, int height, GLuint tex_id, glm::mat4& model, GLint& model_loc,
                       glm::vec3 char_pos)
{
    model = glm::mat4(1);

    model = translate(model, char_pos);
    model = scale(model, glm::vec3(width, height, 1.0));
    model_loc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(model_loc, 1, FALSE, value_ptr(model));

    // Chamada de desenho do background
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
