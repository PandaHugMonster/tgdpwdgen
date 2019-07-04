//
// Created by ivanp on 03.07.19.
//

#include <cairo.h>
#include <libnotify/notification.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "libtaggerdgui.h"

void tgd_cairo_draw_pixel(cairo_t *cr, double x, double y, double pixel_size) {
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, x * pixel_size, y * pixel_size, pixel_size, pixel_size);
    cairo_fill(cr);
}

void tgd_cairo_clear_image(cairo_t *cr, int width, int height) {
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);
}

void tgd_notify_send(const char * name, const char * text, int timeout) {
    NotifyNotification * notification = notify_notification_new(name, text, NULL);
    notify_notification_set_timeout(notification, timeout);
    notify_notification_set_urgency(notification, NOTIFY_URGENCY_NORMAL);
    notify_notification_show(notification, NULL);
}

void tgd_notify_send_ext(const char * name, const char * text, int timeout, NotifyUrgency urgency, const char * icon, const char * category) {
    NotifyNotification * notification = notify_notification_new(name, text, icon);
    notify_notification_set_timeout(notification, timeout);
    notify_notification_set_category(notification, category);
    notify_notification_set_urgency(notification, urgency);
    notify_notification_show(notification, NULL);
}

void tgd_copy_text_to_clipboard(const char * text) {
    GtkClipboard * tgd_clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_text(tgd_clipboard, text, -1);
}