/* 绘制实心三角形
 * 顶点数据: 硬编码提供 3 组 (x, y, z) 数据
 * 背景颜色: glClearColor 设置固定颜色
 * 三角形颜色: fragment shader 中设置固定颜色
*/
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLchar *VERTEX_SHADER_SOURCE =
    "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main() {\n"
    "    gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\n"
    ;
const GLchar *FRAGMENT_SHADER_SOURCE =
    "#version 330 core\n"
    "out vec4 color;\n"
    "void main() {\n"
    "    color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n"
    ;

void init_glfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void init_glew() {
    glewExperimental = GL_TRUE;
    glewInit();
}

void exit_by_error() {
    const char *desc;
    int err_code = glfwGetError(&desc);
    std::cout << "Error: code: " << err_code << ", detail: " << desc << std::endl;
    exit(-1);
}

GLFWwindow *create_window() {
    GLFWwindow *win = glfwCreateWindow(800, 600, "Triangle Test", nullptr, nullptr);
    if (win == nullptr) {
        exit_by_error();
    }
    glfwMakeContextCurrent(win);
    return win;
}

GLuint gen_VAO() {
    GLfloat vertices[] {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);                 // 创建 VAO(顶点数组对象)
    glGenBuffers(1, &VBO);                      // 创建 VBO(顶点缓冲对象)

    glBindVertexArray(VAO);                     // bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);         // bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);      // 将数据缓冲到 GPU
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);     // 设置如何解析缓冲的数据(void *)
    glEnableVertexAttribArray(0);               // 启用顶点属性
    glBindBuffer(GL_ARRAY_BUFFER, 0);           // unbind VBO
    glBindVertexArray(0);                       // unbind VAO

    return VAO;
}

void compile_shader(GLuint shader_index) {
    glCompileShader(shader_index);
    GLint success;
    glGetShaderiv(shader_index, GL_COMPILE_STATUS, &success);      // 检查是否编译成功
    if (!success) {
        GLchar fail_log[512];
        glGetShaderInfoLog(shader_index, 512, NULL, fail_log);
        std::cout << "Compile shader error: " << fail_log << std::endl;
    }
}

GLuint create_vertex_shader() {
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);        // 创建
    glShaderSource(vertex_shader, 1, &VERTEX_SHADER_SOURCE, NULL);  // 设置源码
    compile_shader(vertex_shader);                                  // 编译
    return vertex_shader;
}

GLuint create_fragment_shader() {
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &FRAGMENT_SHADER_SOURCE, NULL);
    compile_shader(fragment_shader);
    return fragment_shader;
}

GLuint create_shader_program() {
    GLuint vertex_shader = create_vertex_shader();              // 顶点着色器
    GLuint fragment_shader = create_fragment_shader();          // 片段着色器
    GLuint shader_program = glCreateProgram();                  // 创建着色器程序
    glAttachShader(shader_program, vertex_shader);              // 添加着色器
    glAttachShader(shader_program, fragment_shader);            // 添加着色器
    glLinkProgram(shader_program);                              // 链接着色器
    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);   // 检查是否链接成功
    if (!success) {
        GLchar fail_log[512];
        glGetProgramInfoLog(shader_program, 512, NULL, fail_log);
        std::cout << "Link shader error: " << fail_log << std::endl;
    }

    glDeleteShader(vertex_shader);                              // 链接后即可删除着色器
    glDeleteShader(fragment_shader);
    return shader_program;
}

void draw_triangle(GLFWwindow *win, GLuint VAO, GLuint shader_program) {
    glUseProgram(shader_program);           // 设置着色器程序 (如何绘制数据)
    glBindVertexArray(VAO);                 // 设置顶点数据
    glDrawArrays(GL_TRIANGLES, 0, 3);       // 绘制指定图元
    glBindVertexArray(0);
}

int main(int argc, char **argv) {
    init_glfw();
    GLFWwindow *win = create_window();                  // 创建窗口
    init_glew();                                        // glew 要在设置 context 后初始化
    int width, height;
    glfwGetFramebufferSize(win, &width, &height);
    glViewport(0, 0, width, height);                    // 设置视口

    GLuint VAO = gen_VAO();                             // 创建 VAO (要绘制的顶点信息)
    GLuint shader_program = create_shader_program();    // 创建着色器程序 (如何绘制顶点)

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();                               // 处理系统事件
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);                   // 清屏

        draw_triangle(win, VAO, shader_program);        // 绘制

        glfwSwapBuffers(win);                           // 将缓冲区像素输出到屏幕
    }
    glfwTerminate();
    return 0;
}
