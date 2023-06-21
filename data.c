#include <math.h>
#include "data.h"

float leftSparator[2][3] = { 0 };
float rightSparator[2][3] = { 0 };
float horiLine[2][3] = { 0 };
float vertLine[2][3] = { 0 };
float firstCircle[360][3] = { 0 };
float secendCircle[360][3] = { 0 };
float thirdCircle[360][3] = { 0 };
float fourthCircle[360][3] = { 0 };
float fifthCircle[360][3] = { 0 };
float sixthCircle[360][3] = { 0 };
float seventhCircle[360][3] = { 0 };
float eigthCircle[360][3] = { 0 };
float leftIndicator[8][3] = { 0 };
float rightIndicator[8][3] = { 0 };

const unsigned int indicatorIndices[7 * 3] = {
    0, 1, 7,
    1, 2, 7,
    2, 3, 7,
    3, 4, 7,
    4, 5, 7,
    5, 6, 7,
    6, 0, 7
};

float topRect[4][3] = { 0 };
float leftRect[4][3] = { 0 };
float rightRect[4][3] = { 0 };
float verspeed[10][3] = { 0 };

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

float circle[362][3] = { 0 };
unsigned int circleIndices[361 * 3] = { 0 };
unsigned int rectIndices[6] = { 0, 1, 2, 0, 2, 3 };
float roundRect[368][3] = { 0 };
unsigned int roundIndices[364 * 3 + 10 * 3] = { 0 };

void init_data(void)
{
    const float center[2] = { -0.1f, 0.0f };
    //姿态仪
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

    //速度
    leftRect[0][0] = -0.8f + center[0];
    leftRect[0][1] = 0.8f + center[1];
    leftRect[1][0] = -0.6f + center[0];
    leftRect[1][1] = 0.8f + center[1];
    leftRect[2][0] = -0.6f + center[0];
    leftRect[2][1] = -0.8f + center[1];
    leftRect[3][0] = -0.8f + center[0];
    leftRect[3][1] = -0.8f + center[1];
    
    //高度
    rightRect[0][0] = 0.6f + center[0];
    rightRect[0][1] = 0.8f + center[1];
    rightRect[1][0] = 0.8f + center[0];
    rightRect[1][1] = 0.8f + center[1];
    rightRect[2][0] = 0.8f + center[0];
    rightRect[2][1] = -0.8f + center[1];
    rightRect[3][0] = 0.6f + center[0];
    rightRect[3][1] = -0.8f + center[1];

    //垂直速度指示器
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

    //方位
    circle[0][0] = 0.5f + center[0];
    circle[0][1] = -1.3f;
    circle[361][0] = center[0];
    circle[361][1] = -1.3f;
    const float r = 0.5f;
    for (int i = 1; i <= 360; ++i) {
        circle[i][0] = cosf(M_PI / 180 * i) * r + center[0];
        circle[i][1] = sinf(M_PI / 180 * i) * r + circle[0][1];
    }

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
    topRect[0][0] = center[0] - 0.6;
    topRect[0][1] = 1;
    topRect[1][0] = center[0] + 0.6;
    topRect[1][1] = 1;
    topRect[2][0] = center[0] + 0.6;
    topRect[2][1] = 0.85;
    topRect[3][0] = center[0] - 0.6;
    topRect[3][1] = 0.85;

    leftSparator[0][0] = topRect[0][0] + (topRect[1][0] - topRect[0][0]) / 3;
    leftSparator[0][1] = 1.0f;
    leftSparator[1][0] = topRect[0][0] + (topRect[1][0] - topRect[0][0]) / 3;
    leftSparator[1][1] = 0.85f;
    rightSparator[0][0] = topRect[0][0] + (topRect[1][0] - topRect[0][0]) / 3 * 2;
    rightSparator[0][1] = 1.0f;
    rightSparator[1][0] = topRect[0][0] + (topRect[1][0] - topRect[0][0]) / 3 * 2;
    rightSparator[1][1] = 0.85f;

    leftIndicator[0][0] = leftRect[0][0] - 0.005f;
    leftIndicator[0][1] = center[1] + 0.07f;
    leftIndicator[1][0] = leftRect[0][0] + 0.12f;
    leftIndicator[1][1] = center[1] + 0.07f;
    leftIndicator[2][0] = leftRect[0][0] + 0.12f;
    leftIndicator[2][1] = center[1] + 0.025f;
    leftIndicator[3][0] = leftRect[0][0] + 0.15f;
    leftIndicator[3][1] = center[1];
    leftIndicator[4][0] = leftRect[0][0] + 0.12f;
    leftIndicator[4][1] = center[1] - 0.025f;
    leftIndicator[5][0] = leftRect[0][0] + 0.12f;
    leftIndicator[5][1] = center[1] - 0.07f;
    leftIndicator[6][0] = leftRect[0][0] - 0.005f;
    leftIndicator[6][1] = center[1] - 0.07f;
    leftIndicator[7][0] = (leftIndicator[1][0] + leftIndicator[0][0]) / 2;
    leftIndicator[7][1] = center[1];

    rightIndicator[0][0] = rightRect[0][0] + 0.08f;
    rightIndicator[0][1] = center[1] + 0.07f;
    rightIndicator[1][0] = verspeed[0][0] - 0.005f;
    rightIndicator[1][1] = center[1] + 0.07f;
    rightIndicator[2][0] = verspeed[0][0] - 0.005f;
    rightIndicator[2][1] = center[1] - 0.07f;
    rightIndicator[3][0] = rightRect[0][0] + 0.08f;
    rightIndicator[3][1] = center[1] - 0.07f;
    rightIndicator[4][0] = rightRect[0][0] + 0.08f;
    rightIndicator[4][1] = center[1] - 0.025f;
    rightIndicator[5][0] = rightRect[0][0] + 0.05f;
    rightIndicator[5][1] = center[1];
    rightIndicator[6][0] = rightRect[0][0] + 0.08f;
    rightIndicator[6][1] = center[1] + 0.025f;
    rightIndicator[7][0] = (rightIndicator[1][0] + rightIndicator[0][0]) / 2;
    rightIndicator[7][1] = center[1];

    horiLine[0][0] = roundRect[91][0] + 0.03;
    horiLine[0][1] = center[1];
    horiLine[1][0] = roundRect[91][0] + 0.07;
    horiLine[1][1] = center[1];
    vertLine[0][0] = (roundRect[363][0] + roundRect[0][0]) / 2;
    vertLine[0][1] = roundRect[182][0] - 0.03;
    vertLine[1][0] = (roundRect[363][0] + roundRect[0][0]) / 2;
    vertLine[1][1] = roundRect[182][0] - 0.07;

    for (int i = 0; i < 360; ++i) {
        firstCircle[i][0] = cosf(M_PI / 180 * i + 1) * 0.015 + (horiLine[1][0] + horiLine[0][0]) / 2;
        firstCircle[i][1] = sinf(M_PI / 180 * i + 1) * 0.015 + (roundRect[90][1] + (roundRect[91][1] - roundRect[90][1]) / 6);
    }
    for (int i = 0; i < 360; ++i) {
        secendCircle[i][0] = cosf(M_PI / 180 * i + 1) * 0.015 + (horiLine[1][0] + horiLine[0][0]) / 2;
        secendCircle[i][1] = sinf(M_PI / 180 * i + 1) * 0.015 + (roundRect[90][1] + (roundRect[91][1] - roundRect[90][1]) / 6 * 2);
    }
    for (int i = 0; i < 360; ++i) {
        thirdCircle[i][0] = cosf(M_PI / 180 * i) * 0.015 + (horiLine[1][0] + horiLine[0][0]) / 2;
        thirdCircle[i][1] = sinf(M_PI / 180 * i) * 0.015 + (roundRect[90][1] + (roundRect[91][1] - roundRect[90][1]) / 6 * 4);
    }
    for (int i = 0; i < 360; ++i) {
        fourthCircle[i][0] = cosf(M_PI / 180 * i) * 0.015 + (horiLine[1][0] + horiLine[0][0]) / 2;
        fourthCircle[i][1] = sinf(M_PI / 180 * i) * 0.015 + (roundRect[90][1] + (roundRect[91][1] - roundRect[90][1]) / 6 * 5);
    }

    for (int i = 0; i < 360; ++i) {
        fifthCircle[i][0] = cosf(M_PI / 180 * i + 1) * 0.015 + (roundRect[363][0] + (roundRect[0][0] - roundRect[363][0]) / 6);
        fifthCircle[i][1] = sinf(M_PI / 180 * i + 1) * 0.015 + (vertLine[1][1] + vertLine[0][1]) / 2;
    }
    for (int i = 0; i < 360; ++i) {
        sixthCircle[i][0] = cosf(M_PI / 180 * i + 1) * 0.015 + (roundRect[363][0] + (roundRect[0][0] - roundRect[363][0]) / 6 * 2);
        sixthCircle[i][1] = sinf(M_PI / 180 * i + 1) * 0.015 + (vertLine[1][1] + vertLine[0][1]) / 2;
    }
    for (int i = 0; i < 360; ++i) {
        seventhCircle[i][0] = cosf(M_PI / 180 * i + 1) * 0.015 + (roundRect[363][0] + (roundRect[0][0] - roundRect[363][0]) / 6 * 4);
        seventhCircle[i][1] = sinf(M_PI / 180 * i + 1) * 0.015 + (vertLine[1][1] + vertLine[0][1]) / 2;
    }
    for (int i = 0; i < 360; ++i) {
        eigthCircle[i][0] = cosf(M_PI / 180 * i + 1) * 0.015 + (roundRect[363][0] + (roundRect[0][0] - roundRect[363][0]) / 6 * 5);
        eigthCircle[i][1] = sinf(M_PI / 180 * i + 1) * 0.015 + (vertLine[1][1] + vertLine[0][1]) / 2;
    }
    return;
}