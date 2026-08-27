#include "window.h"
#include "gl.h"
void on_activate(GtkApplication *app, gpointer data)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    GtkWidget *glArea = gtk_gl_area_new();
    /*
#if GTK_CHECK_VERSION(4, 12, 0)
    // 强制使用桌面 OpenGL 上下文（而非 GLES），匹配 #version 450 着色器
    gtk_gl_area_set_allowed_apis(GTK_GL_AREA(glArea), GDK_GL_API_GL);
#endif
*/
    gtk_window_set_child(GTK_WINDOW(window), glArea);
    g_signal_connect(glArea, "realize", G_CALLBACK(on_realize), NULL);
    g_signal_connect(glArea, "render", G_CALLBACK(on_render), NULL);
    gtk_window_present(GTK_WINDOW(window));
    return;
}