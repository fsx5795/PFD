#include "window.h"
#include "gl.h"
void on_activate(GtkApplication *app, gpointer data)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    GtkWidget *glArea = gtk_gl_area_new();
    gtk_container_add(GTK_CONTAINER(window), glArea);
    g_signal_connect(glArea, "realize", G_CALLBACK(on_realize), NULL);
    g_signal_connect(glArea, "render", G_CALLBACK(on_render), NULL);
    gtk_widget_show_all(window);
    return;
}