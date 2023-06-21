#ifndef DATA_H
#define DATA_H

extern float roundRect[368][3];
extern unsigned int roundIndices[364 * 3 + 10 * 3];
extern float leftRect[4][3];
extern float rightRect[4][3];
extern unsigned int rectIndices[6];
extern float circle[362][3];
extern unsigned int circleIndices[361 * 3];
extern float verspeed[10][3];
extern unsigned int verIndices[9 * 3];
extern float topRect[4][3];
extern float leftIndicator[8][3];
extern float rightIndicator[8][3];
extern const unsigned int indicatorIndices[7 * 3];
extern float leftSparator[2][3];
extern float rightSparator[2][3];
extern float horiLine[2][3];
extern float vertLine[2][3];
extern float firstCircle[360][3];
extern float secendCircle[360][3];
extern float thirdCircle[360][3];
extern float fourthCircle[360][3];
extern float fifthCircle[360][3];
extern float sixthCircle[360][3];
extern float seventhCircle[360][3];
extern float eigthCircle[360][3];
 
extern void init_data(void);

#endif