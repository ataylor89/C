#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>

GtkWidget *win;
GtkWidget *tv;
GtkTextBuffer *tb;
GtkTextIter iter;
GtkWidget *entry, *button;
GtkEntryBuffer *eb;
GtkWidget *hbox, *vbox;	
int fd = -1;
pthread_t pingthread;
pthread_t readthread;

typedef struct {
	char *hostname;
	int port;
} cl_args;

int connect_to_server(char *hostname, int port) {
	int fd;
	struct hostent *hp;
	struct sockaddr_in serveraddr;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;

	if ((hp = gethostbyname(hostname)) == NULL)
		return -2;

	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)hp->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
	serveraddr.sin_port = htons(port);

	if (connect(fd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
		return -1;

	return fd;
}

void *ping_loop(void *ptr) {
	cl_args *myargs = (cl_args *) ptr;
	int size = strlen(myargs->hostname) + 6;
	char ping_msg[size];
	sprintf(ping_msg, "PING %s\n", myargs->hostname);
	while (true) {
		if (write(fd, ping_msg, size) < 0)
			return NULL;
		gtk_text_buffer_insert(tb, &iter, ping_msg, size);
		sleep(10);
	}
	return NULL; 
}

char readline(char *buf, int maxlen) {
	int n;
	char c;
	for (n=0; n < maxlen; n++) {
		read(fd, &c, 1);
		buf[n] = c;
		if (c == '\n')
			break;
	}
	return n;
}

void *read_loop(void *ptr) {
	int maxlen = 100;
	char buf[maxlen];
	while (true) {
		readline(buf, maxlen);
		gtk_text_buffer_insert(tb, &iter, buf, strlen(buf));
	}
}

void send_message(GtkWidget *widget, gpointer *data) {
	const char *text = gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(eb));
	char msg[strlen(text) + 1];
	sprintf(msg, "%s\n", text);
	gtk_text_buffer_insert(tb, &iter, text, -1);
	gtk_text_buffer_insert(tb, &iter, "\n", -1);
	if (fd > 0) {
		write(fd, msg, strlen(msg));
	}
}

static void activate(GApplication *app, gpointer *data) {	
	cl_args *myargs = (cl_args *) data;
	win = gtk_application_window_new(GTK_APPLICATION(app));
	gtk_window_set_title(GTK_WINDOW(win), "IRC Client");
	gtk_window_set_default_size(GTK_WINDOW(win), 800, 800);
	tv = gtk_text_view_new();
	tb = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));
	gtk_text_buffer_get_start_iter(tb, &iter);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(tv), GTK_WRAP_WORD_CHAR);
	entry = gtk_entry_new();
	eb = gtk_entry_buffer_new(NULL, 0);
	gtk_entry_set_buffer(GTK_ENTRY(entry), eb);
	gtk_widget_set_hexpand(entry, TRUE);
	button = gtk_button_new_with_label("Send");
	g_signal_connect(button, "clicked", G_CALLBACK(send_message), NULL);
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(hbox), entry);
	gtk_box_append(GTK_BOX(hbox), button);
	gtk_box_append(GTK_BOX(vbox), tv);
	gtk_box_append(GTK_BOX(vbox), hbox);
	gtk_widget_set_hexpand(tv, TRUE);
	gtk_widget_set_vexpand(tv, TRUE);
	gtk_window_set_child(GTK_WINDOW(win), vbox);
	gtk_widget_show(win);
	fd = connect_to_server(myargs->hostname, myargs->port);
	if (fd > 0) {
		pthread_create(&pingthread, NULL, ping_loop, (void *) myargs);
		pthread_create(&readthread, NULL, read_loop, NULL);
	}
}

int main(int argc, char **argv) {
	/*
	if (argc != 3) {
		fprintf(stderr, "usage: %s <hostname> <port>\n", argv[0]);
		return 0;
	}
	*/

	cl_args myargs;
	myargs.hostname = "irc.dal.net";
	myargs.port = 6667;

	GtkApplication *app;
	int stat;

	app = gtk_application_new("com.ircclient", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), &myargs);
	stat = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return stat;
}