#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//Classe shader
#include "../../commonfiles/Shader.h"


// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;


// Função MAIN
int main()
{
    // Inicialização da GLFW
    glfwInit();

    //Muita atenção aqui: alguns ambientes não aceitam essas configurações
    //Você deve adaptar para a versão do OpenGL suportada por sua placa
    //Sugestão: comente essas linhas de código para desobrir a versão e
    //depois atualize (por exemplo: 4.5 com 4 e 5)
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

    // Criação da janela GLFW
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 9 - Sandro", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Fazendo o registro da função de callback para a janela GLFW
    glfwSetKeyCallback(window, key_callback);

    // GLAD: carrega todos os ponteiros d funções da OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    // Obtendo as informações de versão
    const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
    const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported " << version << endl;

    // Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);


    // Compilando e buildando o programa de shader
    Shader shader("../../commonfiles/shaders/vertex.vs", "../../commonfiles/shaders/fragment.fs");

    // Gerando um buffer simples, com a geometria das figuras geometricas
    GLuint VAO = setupGeometry();

    GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
    assert(colorLoc > -1);
    glUseProgram(shader.ID);

    // Loop da aplicação - "game loop"
    while (!glfwWindowShouldClose(window))
    {
        // Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
        glfwPollEvents();

        // Limpa o buffer de cor
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // cor de fundo
        glClear(GL_COLOR_BUFFER_BIT);

        glLineWidth(5);
        glPointSize(10);

        glBindVertexArray(VAO);

        // Inicio do exercio

        // Contorno teto
        glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINE_LOOP, 0, 3);

        // preenchimento teto
        glUniform4f(colorLoc, 0.6f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // paredes
        glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINE_LOOP, 3, 4);

        // Contorno porta
        glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINE_LOOP, 7, 4);

        // Preenchimento porta
        glUniform4f(colorLoc, 0.6f, 0.4f, 0.2f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 7, 4);

        // Preenchimento janela
        glUniform4f(colorLoc, 1.0f, 1.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 11, 16);

        // Janela divida em 4 quadrados
        glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINE_LOOP, 11, 4);
        
        glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINE_LOOP, 15, 4);

        glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINE_LOOP, 19, 4);
        
        glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINE_LOOP, 23, 4);

        //chao
        glUniform4f(colorLoc, 0.6f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINE_LOOP, 27, 2);


        // Fim do exercio

        glBindVertexArray(0);

        // Troca os buffers da tela
        glfwSwapBuffers(window);
    }
    // Pede pra OpenGL desalocar os buffers
    glDeleteVertexArrays(1, &VAO);
    // Finaliza a execução da GLFW, limpando os recursos alocados por ela
    glfwTerminate();
    return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a 
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupGeometry()
{
    GLfloat vertices[] = {
        //Telhado
        -0.5f, 0.5f, 0.0f, // canto superior esquerdo
        0.5f, 0.5f, 0.0f, // canto superior direito
        0.0f, 1.0f, 0.0f, // topo do telhado

        // Paredes
        -0.5f, -0.5f, 0.0f, // canto inferior esquerdo
        0.5f, -0.5f, 0.0f, // canto inferior direito
        0.5f, 0.5f, 0.0f, // canto superior direito
        -0.5f, 0.5f, 0.0f, // canto superior esquerdo

        // Porta
        -0.1f, -0.5f, 0.0f, // canto inferior esquerdo
        0.1f, -0.5f, 0.0f, // canto inferior direito
        0.1f, -0.1f, 0.0f, // canto superior direito
        -0.1f, -0.1f, 0.0f, // canto superior esquerdo

        // Janela divido em 4 quadrados
        -0.4f, 0.0f, 0.0f, // canto inferior esquerdo
        -0.25f, 0.0f, 0.0f, // canto inferior direito
        -0.25f, 0.15f, 0.0f, // canto superior direito
        -0.4f, 0.15f, 0.0f, // canto superior esquerdo

        -0.25f, 0.0f, 0.0f, // canto inferior esquerdo
        -0.1f, 0.0f, 0.0f, // canto inferior direito
        -0.1f, 0.15f, 0.0f, // canto superior direito
        -0.25f, 0.15f, 0.0f, // canto superior esquerdo

        -0.25f, 0.15f, 0.0f, // canto inferior esquerdo
        -0.1f, 0.15f, 0.0f, // canto inferior direito
        -0.1f, 0.3f, 0.0f, // canto superior direito
        -0.25f, 0.3f, 0.0f, // canto superior esquerdo

        -0.4f, 0.15f, 0.0f, // canto inferior esquerdo
        -0.25f, 0.15f, 0.0f, // canto inferior direito
        -0.25f, 0.3f, 0.0f, // canto superior direito
        -0.4f, 0.3f, 0.0f, // canto superior esquerdo

        //chao
        -1.0f, -0.5f, 0.0f, // canto inferior esquerdo
        1.0f, -0.5f, 0.0f, // canto inferior direito
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
    //Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
    // Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
    // Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
    // Tipo do dado
    // Se está normalizado (entre zero e um)
    // Tamanho em bytes 
    // Deslocamento a partir do byte zero 

    //Atributo posição
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
    // atualmente vinculado - para que depois possamos desvincular com segurança
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
    glBindVertexArray(0);

    return VAO;
}
