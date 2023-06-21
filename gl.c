#include <math.h>
#include "gl.h"
#include "data.h"
#include "glexp.h"
static unsigned int vao[22], program[4];
static unsigned int init_shader(GLenum type, const char *path)
{
    unsigned int shader = glCreateShader(type);
    FILE *fp;
    if ((fp = fopen(path, "r")) != NULL) {
        fseek(fp, 0, SEEK_END);
        long leng = ftell(fp);
        char data[leng];
        char *p = data;
        memset(data, '\0', leng);
        rewind(fp);
        fread(data, 1, leng, fp);
        fclose(fp);
        glShaderSource(shader, 1, (const char * const*)&p, NULL);
        glCompileShader(shader);
        int status, logLen = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
            if (logLen > 0) {
                char *log = alloca(logLen);
                glGetShaderInfoLog(shader, logLen, NULL, log);
                puts(log);
            }
            return -1;
        }
        return shader;
    }
    return -1;
}
static void bind_vao_vbo_ebo(unsigned int vao, unsigned int vbo, unsigned int ebo, size_t dataSize, void *data, size_t indicesSize, const void *indices)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return;
}
static void bind_vao_vbo(unsigned int vao, unsigned int vbo, size_t size, void *data)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return;
}
static bool init_program(unsigned int *program, unsigned int vertshader, unsigned int fragshader)
{
    *program = glCreateProgram();
    glAttachShader(*program, vertshader);
    glAttachShader(*program, fragshader);
    glLinkProgram(*program);
    int status, logLen = 0;
    glGetProgramiv(*program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            char *log = alloca(logLen);
            glGetProgramInfoLog(*program, logLen, NULL, log);
            puts(log);
        }
        return false;
    }
    return true;
}
void on_realize(GtkGLArea *area)
{
    gtk_gl_area_make_current(area);
    if (gtk_gl_area_get_error(area) != NULL)
        return;

    init_data();
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH, GL_NICEST);
    glLineWidth(0.2);

    unsigned int roundShader = init_shader(GL_VERTEX_SHADER, "./round.glsl");
    if (roundShader == -1)
        return;
    unsigned int fragShader = init_shader(GL_FRAGMENT_SHADER, "./frag.glsl");
    if (fragShader == -1)
        return;
    unsigned int greyShader = init_shader(GL_FRAGMENT_SHADER, "./grey.glsl");
    if (greyShader == -1)
        return;
    unsigned int whiteShader = init_shader(GL_FRAGMENT_SHADER, "./white.glsl");
    if (whiteShader == -1)
        return;
    unsigned int blackShader = init_shader(GL_FRAGMENT_SHADER, "./black.glsl");
    if (blackShader == -1)
        return;
    
    unsigned int vbo[22], ebo[6];
    glGenVertexArrays(22, vao);
    glGenBuffers(22, vbo);
    glGenBuffers(6, ebo);

    bind_vao_vbo_ebo(vao[0], vbo[0], ebo[0], sizeof roundRect, (void*)roundRect, sizeof roundIndices, roundIndices);
    bind_vao_vbo_ebo(vao[1], vbo[1], ebo[1], sizeof leftRect, (void*)leftRect, sizeof rectIndices, (void*)rectIndices);
    bind_vao_vbo_ebo(vao[2], vbo[2], ebo[1], sizeof rightRect, (void*)rightRect, sizeof rectIndices, rectIndices);
    bind_vao_vbo_ebo(vao[3], vbo[3], ebo[2], sizeof circle, circle, sizeof circleIndices, circleIndices);
    bind_vao_vbo_ebo(vao[4], vbo[4], ebo[3], sizeof verspeed, verspeed, sizeof verIndices, verIndices);
    bind_vao_vbo_ebo(vao[5], vbo[5], ebo[1], sizeof topRect, (void*)topRect, sizeof rectIndices, rectIndices);
    bind_vao_vbo_ebo(vao[6], vbo[6], ebo[4], sizeof leftIndicator, (void*)leftIndicator, sizeof indicatorIndices, indicatorIndices);
    bind_vao_vbo_ebo(vao[7], vbo[7], ebo[4], sizeof rightIndicator, (void*)rightIndicator, sizeof indicatorIndices, indicatorIndices);

    bind_vao_vbo(vao[8], vbo[8], sizeof leftIndicator, (void*)leftIndicator);
    bind_vao_vbo(vao[9], vbo[9], sizeof rightIndicator, (void*)rightIndicator);
    bind_vao_vbo(vao[10], vbo[10], sizeof leftSparator, (void*)leftSparator);
    bind_vao_vbo(vao[11], vbo[11], sizeof rightSparator, (void*)rightSparator);
    bind_vao_vbo(vao[12], vbo[12], sizeof horiLine, (void*)horiLine);
    bind_vao_vbo(vao[13], vbo[13], sizeof vertLine, (void*)vertLine);
    bind_vao_vbo(vao[14], vbo[14], sizeof firstCircle, (void*)firstCircle);
    bind_vao_vbo(vao[15], vbo[15], sizeof secendCircle, (void*)secendCircle);
    bind_vao_vbo(vao[16], vbo[16], sizeof thirdCircle, (void*)thirdCircle);
    bind_vao_vbo(vao[17], vbo[17], sizeof fourthCircle, (void*)fourthCircle);
    bind_vao_vbo(vao[18], vbo[18], sizeof fifthCircle, (void*)fifthCircle);
    bind_vao_vbo(vao[19], vbo[19], sizeof sixthCircle, (void*)sixthCircle);
    bind_vao_vbo(vao[20], vbo[20], sizeof seventhCircle, (void*)seventhCircle);
    bind_vao_vbo(vao[21], vbo[21], sizeof eigthCircle, (void*)eigthCircle);

    if (!init_program(&program[0], roundShader, fragShader))
        return;
    if (!init_program(&program[1], roundShader, greyShader))
        return;
    if (!init_program(&program[2], roundShader, blackShader))
        return;
    if (!init_program(&program[3], roundShader, whiteShader))
        return;

    glDetachShader(program[0], roundShader);
    glDetachShader(program[0], fragShader);
    glDetachShader(program[1], roundShader);
    glDetachShader(program[1], greyShader);
    glDetachShader(program[2], roundShader);
    glDetachShader(program[2], blackShader);
    glDetachShader(program[3], roundShader);
    glDetachShader(program[3], whiteShader);
    return;
}
bool on_render(GtkGLArea *area, GdkGLContext *context)
{
    glViewport(0, 0, gtk_widget_get_allocated_width(GTK_WIDGET(area)), gtk_widget_get_allocated_height(GTK_WIDGET(area)));
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program[0]);

    glBindVertexArray(vao[0]);
    glDrawElements(GL_TRIANGLES, 364 * 3 + 3 * 10, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glUseProgram(program[1]);
    glBindVertexArray(vao[1]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindVertexArray(vao[2]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindVertexArray(vao[3]);
    glDrawElements(GL_TRIANGLES, 361 * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindVertexArray(vao[4]);
    glDrawElements(GL_TRIANGLES, 9 * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindVertexArray(vao[5]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glUseProgram(program[2]);
    glBindVertexArray(vao[6]);
    glDrawElements(GL_TRIANGLES, 7 * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindVertexArray(vao[7]);
    glDrawElements(GL_TRIANGLES, 7 * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glUseProgram(program[3]);
    glBindVertexArray(vao[8]);
    glDrawArrays(GL_LINE_LOOP, 0, 7);
    glBindVertexArray(0);
    glBindVertexArray(vao[9]);
    glDrawArrays(GL_LINE_LOOP, 0, 7);
    glBindVertexArray(0);

    glBindVertexArray(vao[10]);
    glDrawArrays(GL_LINE_STRIP, 0, 2);
    glBindVertexArray(0);
    glBindVertexArray(vao[11]);
    glDrawArrays(GL_LINE_STRIP, 0, 2);
    glBindVertexArray(0);

    glBindVertexArray(vao[12]);
    glDrawArrays(GL_LINE_STRIP, 0, 2);
    glBindVertexArray(0);
    glBindVertexArray(vao[13]);
    glDrawArrays(GL_LINE_STRIP, 0, 2);
    glBindVertexArray(0);

    glBindVertexArray(vao[14]);
    glDrawArrays(GL_LINE_LOOP, 0, 360);
    glBindVertexArray(0);
    glBindVertexArray(vao[15]);
    glDrawArrays(GL_LINE_LOOP, 0, 360);
    glBindVertexArray(0);
    glBindVertexArray(vao[16]);
    glDrawArrays(GL_LINE_LOOP, 0, 360);
    glBindVertexArray(0);
    glBindVertexArray(vao[17]);
    glDrawArrays(GL_LINE_LOOP, 0, 360);
    glBindVertexArray(0);
    glBindVertexArray(vao[18]);
    glDrawArrays(GL_LINE_LOOP, 0, 360);
    glBindVertexArray(0);
    glBindVertexArray(vao[19]);
    glDrawArrays(GL_LINE_LOOP, 0, 360);
    glBindVertexArray(0);
    glBindVertexArray(vao[20]);
    glDrawArrays(GL_LINE_LOOP, 0, 360);
    glBindVertexArray(0);
    glBindVertexArray(vao[21]);
    glDrawArrays(GL_LINE_LOOP, 0, 360);
    glBindVertexArray(0);

    glFlush();
    return true;
}
