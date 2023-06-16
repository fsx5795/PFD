#ifndef GL_H
#define GL_H

#include <stdbool.h>
#include <gtk/gtk.h>
extern void on_realize(GtkGLArea*);
extern bool on_render(GtkGLArea*, GdkGLContext*);

#endif