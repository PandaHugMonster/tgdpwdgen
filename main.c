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
#include "headers/tgdpwdgengui.h"

int main(int argc, char **argv) {
    int status;
    tgd_init();

    status = run_gui_application(argc, argv);

    return status;
}