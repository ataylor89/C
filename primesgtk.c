#include <gtk/gtk.h>
#include "primes2.h"

static void activate(GApplication *app, gpointer *data) {
	GtkWidget *win;
	GtkWidget *tv;
	GtkTextBuffer *tb;	
	char *text = (char *) data;
	win = gtk_application_window_new(GTK_APPLICATION(app));
	gtk_window_set_title(GTK_WINDOW(win), "Prime number generator");
	gtk_window_set_default_size(GTK_WINDOW(win), 500, 400);
	tv = gtk_text_view_new();
	tb = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));
	gtk_text_buffer_set_text(tb, text, -1);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(tv), GTK_WRAP_WORD_CHAR);
	gtk_window_set_child(GTK_WINDOW(win), tv);
	gtk_widget_show(win);
}

int main(int argc, char **argv) {
	int n = 200;
	int *arr = primes(n);
	char *text = itos(arr, n, " "); 
	puts(text);

	GtkApplication *app;
	int stat;

	app = gtk_application_new("com.primes", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), text);
	stat = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return stat;
}