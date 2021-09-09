#include <stdlib.h>
#include <gtk/gtk.h>
#include "primes2.h"

GtkWidget *win;
GtkWidget *tv;
GtkTextBuffer *tb;
GtkWidget *entry, *button;
GtkEntryBuffer *eb;
GtkWidget *hbox, *vbox;	

static void generate(GtkWidget *widget, gpointer *data) {
	const char *ns = gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(eb));
	puts(ns);
	int n = atoi(ns);
	int *arr = primes(n);
	char *text = itos(arr, n, " "); 
	puts(text);
	gtk_text_buffer_set_text(tb, text, -1);
}

static void activate(GApplication *app, gpointer *data) {
	win = gtk_application_window_new(GTK_APPLICATION(app));
	gtk_window_set_title(GTK_WINDOW(win), "Prime number generator");
	gtk_window_set_default_size(GTK_WINDOW(win), 800, 800);
	tv = gtk_text_view_new();
	tb = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(tv), GTK_WRAP_WORD_CHAR);
	entry = gtk_entry_new();
	eb = gtk_entry_buffer_new(NULL, 0);
	gtk_entry_set_buffer(GTK_ENTRY(entry), eb);
	gtk_entry_set_max_length(GTK_ENTRY(entry), 10);
	button = gtk_button_new_with_label("Generate primes");
	g_signal_connect(button, "clicked", G_CALLBACK(generate), NULL);
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox), entry);
	gtk_box_append(GTK_BOX(hbox), button);
	gtk_box_append(GTK_BOX(vbox), hbox);
	gtk_widget_set_hexpand(tv, TRUE);
	gtk_widget_set_vexpand(tv, TRUE);
	gtk_box_append(GTK_BOX(vbox), tv);
	gtk_window_set_child(GTK_WINDOW(win), vbox);
	gtk_widget_show(win);
}

int main(int argc, char **argv) {
	GtkApplication *app;
	int stat;

	app = gtk_application_new("com.primes", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	stat = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return stat;
}