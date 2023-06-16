#include <math.h>
#include "gl.h"
#include "glexp.h"
static unsigned int vao[22], program[4];
static void mat_identity(float *matrix)
{
    matrix[0] = 1;
    matrix[1] = 0;
    matrix[2] = 0;
    matrix[3] = 0;
    matrix[4] = 0;
    matrix[5] = 1;
    matrix[6] = 0;
    matrix[7] = 0;
    matrix[8] = 0;
    matrix[9] = 0;
    matrix[10] = 1;
    matrix[11] = 0;
    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
    return;
}
static void mat_rotate(float *matrix, float x, float y, float z, int angle)
{
    float radian = angle * M_PI / 180;
    float d = sqrtf(x * x + y * y + z * z);
    x /= d;
    y /= d;
    z /= d;
    float s = sinf(radian);
    float c = cosf(radian);
    float m = 1 - c;
    matrix[0] = m * x * x + c;
    matrix[1] = m * x * y - z * s;
    matrix[2] = m * z * x + y * s;
    matrix[3] = 0;
    matrix[4] = m * x * y + z * s;
    matrix[5] = m * y * y + c;
    matrix[6] = m * y * z - x * s;
    matrix[7] = 0;
    matrix[8] = m * z * x - y * s;
    matrix[9] = m * y * z + x * s;
    matrix[10] = m * z * z + c;
    matrix[11] = 0;
    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
    return;
}
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

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH, GL_NICEST);
    glLineWidth(0.2);

    float center[2] = { -0.1f, 0.0f };
    //姿态仪
    float roundRect[368][3] = { 0 };
    roundRect[0][0] = 0.4f + center[0];
    roundRect[0][1] = 0.5f + center[1];
    for (int i = 1; i <= 90; ++i) {
        roundRect[i][0] = sinf(M_PI / 180 * i) * 0.1 + roundRect[0][0];
        roundRect[i][1] = cosf(M_PI / 180 * i) * 0.1 + roundRect[0][1] - 0.1;
    }
    roundRect[91][0] = 0.5f + center[0];
    roundRect[91][1] = -0.4f + center[1];
    for (int i = 1; i <= 90; ++i) {
        roundRect[91 + i][0] = cosf(M_PI / 180 * i) * 0.1 + roundRect[91][0] - 0.1;
        roundRect[91 + i][1] = -sinf(M_PI / 180 * i) * 0.1 + roundRect[91][1];
    }
    roundRect[182][0] = -0.4f + center[0];
    roundRect[182][1] = -0.5f + center[1];
    for (int i = 1; i <= 90; ++i) {
        roundRect[182 + i][0] = -sinf(M_PI / 180 * i) * 0.1 + roundRect[182][0];
        roundRect[182 + i][1] = -cosf(M_PI / 180 * i) * 0.1 + roundRect[182][1] + 0.1;
    }
    roundRect[273][0] = -0.5 + center[0];
    roundRect[273][1] = 0.4 + center[1];
    for (int i = 1; i <= 90; ++i) {
        roundRect[273 + i][0] = -cosf(M_PI / 180 * i) * 0.1 + roundRect[273][0] + 0.1;
        roundRect[273 + i][1] = sinf(M_PI / 180 * i) * 0.1 + roundRect[273][1];
    }
    roundRect[364][0] = roundRect[0][0];
    roundRect[364][1] = roundRect[0][1] - 0.1;
    roundRect[365][0] = roundRect[91][0] - 0.1;
    roundRect[365][1] = roundRect[91][1];
    roundRect[366][0] = roundRect[182][0];
    roundRect[366][1] = roundRect[182][1] + 0.1;
    roundRect[367][0] = roundRect[273][0] + 0.1;
    roundRect[367][1] = roundRect[273][1];

    unsigned int roundIndices[364 * 3 + 10 * 3] = { 0 };
    for (int i = 0; i < 360 * 3 + 4 * 3; ++i) {
        switch (i % 3) {
        case 0:
            roundIndices[i] = i / 3;
            break;
        case 1:
            roundIndices[i] = roundIndices[i - 1] + 1;
            break;
        default:
            if (i < 90 * 3 + 1)
                roundIndices[i] = 364;
            else if (i < 181 * 3)
                roundIndices[i] = 365;
            else if (i < 272 * 3)
                roundIndices[i] = 366;
            else
                roundIndices[i] = 367;
            break;
        }
    }
    
    roundIndices[364 * 3] = 90;
    roundIndices[364 * 3 + 1] = 91;
    roundIndices[364 * 3 + 2] = 364;
    roundIndices[364 * 3 + 3] = 91;
    roundIndices[364 * 3 + 4] = 365;
    roundIndices[364 * 3 + 5] = 364;

    roundIndices[364 * 3 + 6] = 181;
    roundIndices[364 * 3 + 7] = 182;
    roundIndices[364 * 3 + 8] = 365;
    roundIndices[364 * 3 + 9] = 182;
    roundIndices[364 * 3 + 10] = 366;
    roundIndices[364 * 3 + 11] = 365;

    roundIndices[364 * 3 + 12] = 272;
    roundIndices[364 * 3 + 13] = 273;
    roundIndices[364 * 3 + 14] = 366;
    roundIndices[364 * 3 + 15] = 273;
    roundIndices[364 * 3 + 16] = 367;
    roundIndices[364 * 3 + 17] = 366;
    
    roundIndices[364 * 3 + 18] = 363;
    roundIndices[364 * 3 + 19] = 0;
    roundIndices[364 * 3 + 20] = 367;
    roundIndices[364 * 3 + 21] = 0;
    roundIndices[364 * 3 + 22] = 364;
    roundIndices[364 * 3 + 23] = 367;

    roundIndices[364 * 3 + 24] = 364;
    roundIndices[364 * 3 + 25] = 365;
    roundIndices[364 * 3 + 26] = 367;
    roundIndices[364 * 3 + 27] = 365;
    roundIndices[364 * 3 + 28] = 366;
    roundIndices[364 * 3 + 29] = 367;

    const float leftRect[4][3] = {
        { -0.8f + center[0], 0.8f + center[1], 0.0f },
        { -0.6f + center[0], 0.8f + center[1], 0.0f },
        { -0.6f + center[0], -0.8f + center[1], 0.0f },
        { -0.8f + center[0], -0.8f + center[1], 0.0f }
    };
    const float rightRect[4][3] = { { 0.6f + center[0], 0.8f + center[1], 0.0f }, { 0.8f + center[0], 0.8f + center[1], 0.0f }, { 0.8f + center[0], -0.8f + center[1], 0.0f }, { 0.6f + center[0], -0.8f + center[1], 0.0f } };
    //垂直速度指示器
    float verspeed[10][3] = { 0 };
    verspeed[0][0] = rightRect[1][0] + 0.1;
    verspeed[0][1] = center[1] + 0.1;
    verspeed[1][0] = rightRect[1][0] + 0.1;
    verspeed[1][1] = center[1] - 0.1;
    verspeed[2][0] = rightRect[1][0] + 0.05;
    verspeed[2][1] = center[1] - 0.15;
    verspeed[3][0] = rightRect[1][0] + 0.05;
    verspeed[3][1] = center[1] - 0.5;
    verspeed[4][0] = rightRect[1][0] + 0.1;
    verspeed[4][1] = center[1] - 0.5;
    verspeed[5][0] = rightRect[1][0] + 0.2;
    verspeed[5][1] = center[1] - 0.2;
    verspeed[6][0] = rightRect[1][0] + 0.2;
    verspeed[6][1] = center[1] + 0.2;
    verspeed[7][0] = rightRect[1][0] + 0.1;
    verspeed[7][1] = center[1] + 0.5;
    verspeed[8][0] = rightRect[1][0] + 0.05;
    verspeed[8][1] = center[1] + 0.5;
    verspeed[9][0] = rightRect[1][0] + 0.05;
    verspeed[9][1] = center[1] + 0.15;

    unsigned int verIndices[9 * 3] = {
        0, 1, 5,
        2, 3, 1,
        3, 4, 1,
        4, 5, 1,
        5, 6, 1,
        5, 6, 0,
        6, 7, 0,
        7, 8, 0,
        8, 9, 0
    };

    float circle[362][3] = { { 0.5f + center[0], -1.3f, 0.0f } };
    circle[361][0] = center[0];
    circle[361][1] = -1.3f;
    const float r = 0.5f;
    for (int i = 1; i <= 360; ++i) {
        circle[i][0] = cosf(M_PI / 180 * i) * r + center[0];
        circle[i][1] = sinf(M_PI / 180 * i) * r + circle[0][1];
    }

    unsigned int rectIndices[6] = { 0, 1, 2, 0, 2, 3 };

    unsigned int circleIndices[361 * 3] = { 0 };
    for (int i = 0; i < 361 * 3; ++i) {
        switch (i % 3) {
        case 0:
            circleIndices[i] = i / 3;
            break;
        case 1:
            circleIndices[i] = circleIndices[i - 1] + 1;
            break;
        default:
            circleIndices[i] = 361;
            break;
        }
    }
    //自动飞行模式指示器
    const float topRect[4][3] = { { center[0] - 0.6, 1, 0 }, { center[0] + 0.6, 1, 0 }, { center[0] + 0.6, 0.85, 0 }, { center[0] - 0.6, 0.85, 0 } };
    const float leftSparator[2][3] = {
        { topRect[0][0] + (topRect[1][0] - topRect[0][0]) / 3, 1.0f, 0.0f },
        { topRect[0][0] + (topRect[1][0] - topRect[0][0]) / 3, 0.85f, 0.0f }
    };
    const float rightSparator[2][3] = {
        { topRect[0][0] + (topRect[1][0] - topRect[0][0]) / 3 * 2, 1.0f, 0.0f },
        { topRect[0][0] + (topRect[1][0] - topRect[0][0]) / 3 * 2, 0.85f, 0.0f }
    };

    const float leftIndicator[8][3] = {
        { leftRect[0][0] - 0.005f, center[1] + 0.07f, 0.0f },
        { leftRect[0][0] + 0.12f, center[1] + 0.07f, 0.0f },
        { leftRect[0][0] + 0.12f, center[1] + 0.025f, 0.0f },
        { leftRect[0][0] + 0.15f, center[1], 0.0f },
        { leftRect[0][0] + 0.12f, center[1] - 0.025f, 0.0f },
        { leftRect[0][0] + 0.12f, center[1] - 0.07f, 0.0f },
        { leftRect[0][0] - 0.005f, center[1] - 0.07f, 0.0f },
        { (leftIndicator[1][0] + leftIndicator[0][0]) / 2, center[1] }
    };

    const float rightIndicator[8][3] = {
        { rightRect[0][0] + 0.08f, center[1] + 0.07f, 0.0f },
        { verspeed[0][0] - 0.005f, center[1] + 0.07f, 0.0f },
        { verspeed[0][0] - 0.005f, center[1] - 0.07f, 0.0f },
        { rightRect[0][0] + 0.08f, center[1] - 0.07f, 0.0f },
        { rightRect[0][0] + 0.08f, center[1] - 0.025f, 0.0f },
        { rightRect[0][0] + 0.05f, center[1], 0.0f },
        { rightRect[0][0] + 0.08f, center[1] + 0.025f, 0.0f },
        { (rightIndicator[1][0] + rightIndicator[0][0]) / 2, center[1] }
    };

    const unsigned int indicatorIndices[] = {
        0, 1, 7,
        1, 2, 7,
        2, 3, 7,
        3, 4, 7,
        4, 5, 7,
        5, 6, 7,
        6, 0, 7
    };

    const float horiLine[2][3] = {
        { roundRect[91][0] + 0.03, center[1], 0.0f },
        { roundRect[91][0] + 0.07, center[1], 0.0f },
    };
    const float vertLine[2][3] = {
        { (roundRect[363][0] + roundRect[0][0]) / 2, roundRect[182][0] - 0.03, 0.0f },
        { (roundRect[363][0] + roundRect[0][0]) / 2, roundRect[182][0] - 0.07, 0.0f },
    };

    float firstCircle[360][3] = { 0 };
    for (int i = 0; i < 360; ++i) {
        firstCircle[i][0] = cosf(M_PI / 180 * i + 1) * 0.015 + (horiLine[1][0] + horiLine[0][0]) / 2;
        firstCircle[i][1] = sinf(M_PI / 180 * i + 1) * 0.015 + (roundRect[90][1] + (roundRect[91][1] - roundRect[90][1]) / 6);
    }
    float secendCircle[360][3] = { 0 };
    for (int i = 0; i < 360; ++i) {
        secendCircle[i][0] = cosf(M_PI / 180 * i + 1) * 0.015 + (horiLine[1][0] + horiLine[0][0]) / 2;
        secendCircle[i][1] = sinf(M_PI / 180 * i + 1) * 0.015 + (roundRect[90][1] + (roundRect[91][1] - roundRect[90][1]) / 6 * 2);
    }
    float thirdCircle[360][3] = { 0 };
    for (int i = 0; i < 360; ++i) {
        thirdCircle[i][0] = cosf(M_PI / 180 * i) * 0.015 + (horiLine[1][0] + horiLine[0][0]) / 2;
        thirdCircle[i][1] = sinf(M_PI / 180 * i) * 0.015 + (roundRect[90][1] + (roundRect[91][1] - roundRect[90][1]) / 6 * 4);
    }
    float fourthCircle[360][3] = { 0 };
    for (int i = 0; i < 360; ++i) {
        fourthCircle[i][0] = cosf(M_PI / 180 * i) * 0.015 + (horiLine[1][0] + horiLine[0][0]) / 2;
        fourthCircle[i][1] = sinf(M_PI / 180 * i) * 0.015 + (roundRect[90][1] + (roundRect[91][1] - roundRect[90][1]) / 6 * 5);
    }

    float fifthCircle[360][3] = { 0 };
    for (int i = 0; i < 360; ++i) {
        fifthCircle[i][0] = cosf(M_PI / 180 * i + 1) * 0.015 + (roundRect[363][0] + (roundRect[0][0] - roundRect[363][0]) / 6);
        fifthCircle[i][1] = sinf(M_PI / 180 * i + 1) * 0.015 + (vertLine[1][1] + vertLine[0][1]) / 2;
    }
    float sixthCircle[360][3] = { 0 };
    for (int i = 0; i < 360; ++i) {
        sixthCircle[i][0] = cosf(M_PI / 180 * i + 1) * 0.015 + (roundRect[363][0] + (roundRect[0][0] - roundRect[363][0]) / 6 * 2);
        sixthCircle[i][1] = sinf(M_PI / 180 * i + 1) * 0.015 + (vertLine[1][1] + vertLine[0][1]) / 2;
    }
    float seventhCircle[360][3] = { 0 };
    for (int i = 0; i < 360; ++i) {
        seventhCircle[i][0] = cosf(M_PI / 180 * i + 1) * 0.015 + (roundRect[363][0] + (roundRect[0][0] - roundRect[363][0]) / 6 * 4);
        seventhCircle[i][1] = sinf(M_PI / 180 * i + 1) * 0.015 + (vertLine[1][1] + vertLine[0][1]) / 2;
    }
    float eigthCircle[360][3] = { 0 };
    for (int i = 0; i < 360; ++i) {
        eigthCircle[i][0] = cosf(M_PI / 180 * i + 1) * 0.015 + (roundRect[363][0] + (roundRect[0][0] - roundRect[363][0]) / 6 * 5);
        eigthCircle[i][1] = sinf(M_PI / 180 * i + 1) * 0.015 + (vertLine[1][1] + vertLine[0][1]) / 2;
    }

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
