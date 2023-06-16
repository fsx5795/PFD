#ifndef GLEXP_H
#define GLEXP_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#ifdef WIN32
#define GLGetProcAddress wglGetProcAddress
#else
#define GLGetProcAddress glXGetProcAddress
#endif

inline void APIENTRY glDeleteProgram(GLuint program)
{
    return ((PFNGLDELETEPROGRAMPROC)GLGetProcAddress("glDeleteProgram"))(program);
}
inline void APIENTRY glDeleteBuffers(GLsizei n, const GLuint *buffers)
{
    return ((PFNGLDELETEBUFFERSPROC)GLGetProcAddress("glDeleteBuffers"))(n, buffers);
}
inline void APIENTRY glDeleteVertexArrays(GLsizei n, const GLuint *arrays)
{
    return ((PFNGLDELETEVERTEXARRAYSPROC)GLGetProcAddress("glDeleteVertexArrays"))(n, arrays);
}
inline GLuint APIENTRY glCreateShader(GLenum type)
{
    return ((PFNGLCREATESHADERPROC)GLGetProcAddress("glCreateShader"))(type);
}
inline void APIENTRY glShaderSource(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length)
{
    return ((PFNGLSHADERSOURCEPROC)GLGetProcAddress("glShaderSource"))(shader, count, string, length);
}
inline void APIENTRY glCompileShader(GLuint shader)
{
    return ((PFNGLCOMPILESHADERPROC)GLGetProcAddress("glCompileShader"))(shader);
}
inline void APIENTRY glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
    return ((PFNGLGETSHADERIVPROC)GLGetProcAddress("glGetShaderiv"))(shader, pname, params);
}
inline void APIENTRY glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    return ((PFNGLGETSHADERINFOLOGPROC)(GLGetProcAddress("glGetShaderInfoLog")))(shader, bufSize, length, infoLog);
}
inline void APIENTRY glGenVertexArrays(GLsizei n, GLuint *arrays)
{
    return ((PFNGLGENVERTEXARRAYSPROC)(GLGetProcAddress("glGenVertexArrays")))(n, arrays);
}
inline void APIENTRY glBindVertexArray(GLuint array)
{
    return ((PFNGLBINDVERTEXARRAYPROC)(GLGetProcAddress("glBindVertexArray")))(array);
}
inline void APIENTRY glGenBuffers(GLsizei n, GLuint *buffers)
{
    return ((PFNGLGENBUFFERSPROC)(GLGetProcAddress("glGenBuffers")))(n, buffers);
}
inline void APIENTRY glBindBuffer(GLenum target, GLuint buffer)
{
    return ((PFNGLBINDBUFFERPROC)(GLGetProcAddress("glBindBuffer")))(target, buffer);
}
inline void APIENTRY glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
{
    return ((PFNGLBUFFERDATAPROC)(GLGetProcAddress("glBufferData")))(target, size, data, usage);
}
inline void APIENTRY glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
{
    return ((PFNGLVERTEXATTRIBPOINTERPROC)(GLGetProcAddress("glVertexAttribPointer")))(index, size, type, normalized, stride, pointer);
}
inline void APIENTRY glEnableVertexAttribArray(GLuint index)
{
    return ((PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLGetProcAddress("glEnableVertexAttribArray")))(index);
}
inline GLuint APIENTRY glCreateProgram(void)
{
    return ((PFNGLCREATEPROGRAMPROC)(GLGetProcAddress("glCreateProgram")))();
}
inline void APIENTRY glAttachShader(GLuint program, GLuint shader)
{
    return ((PFNGLATTACHSHADERPROC)(GLGetProcAddress("glAttachShader")))(program, shader);
}
inline void APIENTRY glLinkProgram(GLuint program)
{
    return ((PFNGLLINKPROGRAMPROC)(GLGetProcAddress("glLinkProgram")))(program);
}
inline void APIENTRY glGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
    return ((PFNGLGETPROGRAMIVPROC)(GLGetProcAddress("glGetProgramiv")))(program, pname, params);
}
inline void APIENTRY glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    return ((PFNGLGETPROGRAMINFOLOGPROC)(GLGetProcAddress("glGetProgramInfoLog")))(program, bufSize, length, infoLog);
}
inline void APIENTRY glDetachShader(GLuint program, GLuint shader)
{
    return ((PFNGLDETACHSHADERPROC)(GLGetProcAddress("glDetachShader")))(program, shader);
}
inline void APIENTRY glDeleteShader(GLuint shader)
{
    return ((PFNGLDELETESHADERPROC)(GLGetProcAddress("glDeleteShader")))(shader);
}
inline void APIENTRY glUseProgram(GLuint program)
{
    return ((PFNGLUSEPROGRAMPROC)(GLGetProcAddress("glUseProgram")))(program);
}
inline void APIENTRY glActiveTexture(GLenum texture)
{
    return ((PFNGLACTIVETEXTUREPROC)(GLGetProcAddress("glActiveTexture")))(texture);
}
inline void APIENTRY glUniform1i(GLint location, GLint v0)
{
    return ((PFNGLUNIFORM1IPROC)(GLGetProcAddress("glUniform1i")))(location, v0);
}
inline GLint APIENTRY glGetUniformLocation(GLuint program, const GLchar *name)
{
    return ((PFNGLGETUNIFORMLOCATIONPROC)(GLGetProcAddress("glGetUniformLocation")))(program, name);
}
inline void APIENTRY glGenerateMipmap(GLenum target)
{
    return ((PFNGLGENERATEMIPMAPPROC)(GLGetProcAddress("glGenerateMipmap")))(target);
}
inline void APIENTRY glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    return ((PFNGLUNIFORMMATRIX4FVPROC)(GLGetProcAddress("glUniformMatrix4fv")))(location, count, transpose, value);
}
inline void APIENTRY glUniform2f(GLint location, GLfloat v0, GLfloat v1)
{
    return ((PFNGLUNIFORM2FPROC)(GLGetProcAddress("glUniform2f")))(location, v0, v1);
}

#endif
