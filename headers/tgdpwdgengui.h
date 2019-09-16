//
// Created by ivanp on 05.07.19.
//

#ifndef TGDPWDGEN_TGDPWDGENGUI_H
#define TGDPWDGEN_TGDPWDGENGUI_H

#define TGDPWDGEN_QR_SIZE 150
#define TGDPWDGEN_QR_MARGIN 3

int run_gui_application(int argc, char **argv);
void activate(GtkApplication *app, gpointer user_data, GtkWindow *window);

#endif //TGDPWDGEN_TGDPWDGENGUI_H
