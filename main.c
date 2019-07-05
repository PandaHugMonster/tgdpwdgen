/**
 * @author PandaHugMonster <ivan.ponomarev.pi@gmail.com>
 * @version 1.0
 *
 * This file is an entry point.
 * Requirements are:
 *      * gtk+-3.0
 *      * libnotify
 *      * libqrencode
 */

#include <stdio.h>
#include <gtk/gtk.h>
#include "headers/libtaggerd.h"
#include "headers/tgdpwdgen.h"

int main(int argc, char **argv) {
    GtkApplication * app;
    int status;

    tgd_init();

    app = gtk_application_new("org.taggerd.pwdgen", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}