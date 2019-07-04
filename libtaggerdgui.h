//
// Created by ivanp on 03.07.19.
//

#ifndef TGD_LIBTAGGERDGUI_H
#define TGD_LIBTAGGERDGUI_H

void tgd_cairo_draw_pixel(cairo_t *cr, double x, double y, double pixel_size);
void tgd_cairo_clear_image(cairo_t *cr, int width, int height);


void tgd_notify_send(const char * name, const char * text, int timeout);
void tgd_notify_send_ext(const char * name, const char * text, int timeout, NotifyUrgency urgency, const char * icon, const char * category);

void tgd_copy_text_to_clipboard(const char * text);

#endif //TGD_LIBTAGGERDGUI_H
