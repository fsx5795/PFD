#version 450
layout (location = 0) in vec3 pos;
//指针的旋转矩阵
uniform mat4 rotate;
//指针的偏移量（中心点偏移量）
uniform vec2 origin;
void main()
{
    //还原中心点至屏幕中心点
    vec4 v4 = vec4(pos.x - origin.x, pos.y - origin.y, pos.z, 1.0f);
    //旋转
    v4 = rotate * v4;
    //偏移中心点
    gl_Position = vec4(v4.x + origin.x, v4.y + origin.y, v4.z, 1.0f);
}