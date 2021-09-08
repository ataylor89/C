#include <gtk/gtk.h>

static void activate(GApplication *app, gpointer *data) {
	g_print("GtkApplication activated.\n");
	GtkWidget *window;
	window = gtk_application_window_new(GTK_APPLICATION(app));
	gtk_window_set_title(GTK_WINDOW(window), "GTK4 application");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
	gtk_widget_show(window);
}

int main(int argc, char **argv) {
	GtkApplication *app;
	int stat;
	
	app = gtk_application_new("com.app", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	stat = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return stat;
}	