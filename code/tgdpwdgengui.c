//
// Created by ivanp on 05.07.19.
//

#include <string.h>
#include <gtk/gtk.h>
#include <libnotify/notify.h>
#include <qrencode.h>
#include "../headers/tgdpwdgengui.h"
#include "../headers/tgdpwdgen.h"
#include "../headers/libtaggerdgui.h"

GtkRevealer * tgd_revealer;
GtkRevealer * tgd_revealer_qr;
GError * tgd_error = NULL;
GtkEntry * tgd_entry;
GtkSpinButton * tgd_min;
GtkSpinButton * tgd_max;
GtkComboBoxText * tgd_pwd_type;
GtkImage * tgd_qr_code_image;
cairo_t * tgd_cairo_cr;
gint tgd_qr_width, tgd_qr_height;
cairo_surface_t * tgd_cairo_surface;
GtkButton * tgd_save_qr_button;
GtkAboutDialog * tgd_about_dialog;

static void qr_generate_new(cairo_t * cr, int width, int height) {
    QRcode * qrcode = QRcode_encodeString(gtk_entry_get_text(tgd_entry), 4, QR_ECLEVEL_H, QR_MODE_8, 1);

    int qr_width = qrcode->width;
    int margin = 3;

    double coefficient = ((float) width - margin * 8) / qr_width;

    tgd_cairo_clear_image(cr, width, height);

    for (int i = 0; i < qr_width; i++) {
        for (int k = 0; k < qr_width; k++) {
            int idx = i * qr_width + k;
            if (qrcode->data[idx] & 1) {
                tgd_cairo_draw_pixel(cr, k + margin, i + margin, coefficient);
            }
        }
    }

    QRcode_free(qrcode);
}

cairo_surface_t * draw_qr_cairo_surface() {
    GdkPixbuf * pixbuf;
    cairo_format_t format;
//    cairo_t * tgd_cairo_cr;

    g_type_init();

    tgd_qr_width = 150;
    tgd_qr_height = tgd_qr_width;
    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, tgd_qr_width, tgd_qr_height);
    g_assert (pixbuf != NULL);
    format = (gdk_pixbuf_get_has_alpha (pixbuf)) ? CAIRO_FORMAT_ARGB32 : CAIRO_FORMAT_RGB24;
    tgd_qr_width = gdk_pixbuf_get_width (pixbuf);
    tgd_qr_height = gdk_pixbuf_get_height (pixbuf);
    tgd_cairo_surface = cairo_image_surface_create (format, tgd_qr_width, tgd_qr_height);
    g_assert (tgd_cairo_surface != NULL);
    tgd_cairo_cr = cairo_create (tgd_cairo_surface);
    /* Draw the pixbuf */
    gdk_cairo_set_source_pixbuf (tgd_cairo_cr, pixbuf, 0, 0);
    cairo_paint(tgd_cairo_cr);

    qr_generate_new(tgd_cairo_cr, tgd_qr_width, tgd_qr_height);

//    cairo_destroy (cr);

    return tgd_cairo_surface;
}///////////////////////////////////////////////////

void gui_do_generate() {
    gchar * res = do_generate(
            gtk_spin_button_get_value_as_int(tgd_min),
            gtk_spin_button_get_value_as_int(tgd_max),
            gtk_combo_box_get_active_id(GTK_COMBO_BOX(tgd_pwd_type))
    );
    gtk_entry_set_text(tgd_entry, res);
    cairo_surface_t * cst = draw_qr_cairo_surface();
    gtk_image_set_from_surface(tgd_qr_code_image, cst);
    g_free(res);
}

void adjustment_min_max_fixer(GtkSpinButton * button) {
    int min_value = gtk_spin_button_get_value_as_int(tgd_min);
    int max_value = gtk_spin_button_get_value_as_int(tgd_max);

    if (min_value != max_value) {
        if (button == tgd_min) {
            if (min_value > max_value) gtk_spin_button_set_value(tgd_max, min_value);
        } else {
            if (min_value > max_value) gtk_spin_button_set_value(tgd_min, max_value);
        }
    }
}

void on_min_change_value(GtkSpinButton * button, gpointer data) {
//    gtk_adjustment_set_lower(gtk_spin_button_get_adjustment(tgd_max), gtk_spin_button_get_value_as_int(button));
    adjustment_min_max_fixer(button);
}

void on_max_change_value(GtkSpinButton * button, gpointer data) {
//    gtk_adjustment_set_upper(gtk_spin_button_get_adjustment(tgd_min), gtk_spin_button_get_value_as_int(button));
    adjustment_min_max_fixer(button);
}

void on_about_button_clicked(GtkModelButton * button, gpointer data) {
    gtk_widget_show_all(GTK_WIDGET(tgd_about_dialog));
}

int on_about_dialog_deleted(GtkAboutDialog * dialog, gpointer data) {
    gtk_widget_hide(GTK_WIDGET(dialog));
    return TRUE;
}

void on_open_extras_click(GtkToggleButton * button, gpointer data) {
    gtk_revealer_set_reveal_child(tgd_revealer, gtk_toggle_button_get_active(button));
}

void adjust_spin_button_values(GtkSpinButton *btn, int min, int max) {
    GtkAdjustment * adj = gtk_spin_button_get_adjustment(btn);

    if (gtk_spin_button_get_value_as_int(btn) > max) gtk_spin_button_set_value(btn, max);
    if (gtk_spin_button_get_value_as_int(btn) < min) gtk_spin_button_set_value(btn, min);

    gtk_adjustment_set_lower(adj, min);
    gtk_adjustment_set_upper(adj, max);
}

void on_type_changed(GtkComboBoxText * combo, gpointer data) {
    const char * type = gtk_combo_box_get_active_id(GTK_COMBO_BOX(combo));
    gtk_entry_set_text(tgd_entry, "");

    tgd_cairo_clear_image(tgd_cairo_cr, tgd_qr_width, tgd_qr_height);
    gtk_image_set_from_surface(tgd_qr_code_image, tgd_cairo_surface);

    if (strcmp(type, TGDPWDGEN_COMBO_TYPE_ITEM_PIN) == 0) {
        adjust_spin_button_values(tgd_min, 4, 10);
        adjust_spin_button_values(tgd_max, 4, 10);
        gtk_spin_button_set_value(tgd_min, 4);
        gtk_spin_button_set_value(tgd_max, 4);
    } else if (strcmp(type, TGDPWDGEN_COMBO_TYPE_ITEM_PATH) == 0) {
        adjust_spin_button_values(tgd_min, 8, 20);
        adjust_spin_button_values(tgd_max, 8, 20);
        gtk_spin_button_set_value(tgd_min, 10);
        gtk_spin_button_set_value(tgd_max, 10);
    } else if (strcmp(type, TGDPWDGEN_COMBO_TYPE_ITEM_PASSWORD_HARD) == 0) {
        adjust_spin_button_values(tgd_min, 12, 32);
        adjust_spin_button_values(tgd_max, 12, 32);
        gtk_spin_button_set_value(tgd_min, 16);
        gtk_spin_button_set_value(tgd_max, 16);
    } else if (strcmp(type, TGDPWDGEN_COMBO_TYPE_ITEM_PASSWORD_INSANE) == 0) {
        adjust_spin_button_values(tgd_min, 20, 32);
        adjust_spin_button_values(tgd_max, 20, 32);
        gtk_spin_button_set_value(tgd_min, 22);
        gtk_spin_button_set_value(tgd_max, 22);
    } else if (strcmp(type, TGDPWDGEN_COMBO_TYPE_ITEM_PASSWORD_PARANOIA) == 0) {
        adjust_spin_button_values(tgd_min, 32, 32);
        adjust_spin_button_values(tgd_max, 32, 32);
    } else {
        adjust_spin_button_values(tgd_min, 4, 32);
        adjust_spin_button_values(tgd_max, 4, 32);
        gtk_spin_button_set_value(tgd_min, 8);
        gtk_spin_button_set_value(tgd_max, 8);
    }
}

void on_button_qr_click(GtkToggleButton * button, gpointer data) {
    gtk_revealer_set_reveal_child(tgd_revealer_qr, gtk_toggle_button_get_active(button));
}

void on_button_copy_clicked(GtkButton * button, gpointer data) {
    tgd_copy_text_to_clipboard(gtk_entry_get_text(tgd_entry));
    tgd_notify_send_ext(TGDPWDGEN_SOFTWARE_NAME, "Copied!", 1000, NOTIFY_URGENCY_LOW, NULL, "Passwords");
}



void on_generate_password_clicked(GtkButton * button, gpointer data) {
    gui_do_generate();
}

GtkBuilder* tgd_builder_new() {
    GtkBuilder * builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "../resources/window.ui", &tgd_error) == 0) {
        g_printerr("Error loading file: %s\n", tgd_error->message);
        g_clear_error(&tgd_error);
        _exit(0);
    }

    return builder;
}

void activate(GtkApplication *app, gpointer user_data, GtkWindow *window) {
    GtkBuilder * tgd_builder;
    GtkHeaderBar * bar;
    GtkToggleButton * extras1;
    GtkToggleButton * button_qr;
    GtkButton * button_gen;
    GtkButton * button_copy;

    tgd_builder = tgd_builder_new();

    notify_init(TGDPWDGEN_SOFTWARE_NAME);

    window = GTK_WINDOW(gtk_builder_get_object(tgd_builder, "window1"));
    bar = GTK_HEADER_BAR(gtk_builder_get_object(tgd_builder, "headerbar1"));
    extras1 = GTK_TOGGLE_BUTTON(gtk_builder_get_object(tgd_builder, "extras1"));
    button_qr = GTK_TOGGLE_BUTTON(gtk_builder_get_object(tgd_builder, "button_qr"));
    tgd_revealer = GTK_REVEALER(gtk_builder_get_object(tgd_builder, "revealer1"));
    tgd_revealer_qr = GTK_REVEALER(gtk_builder_get_object(tgd_builder, "revealer2"));
    tgd_entry = GTK_ENTRY(gtk_builder_get_object(tgd_builder, "entry_new_password"));
    button_gen = GTK_BUTTON(gtk_builder_get_object(tgd_builder, "button_gen_new_password"));
    button_copy = GTK_BUTTON(gtk_builder_get_object(tgd_builder, "button_copy"));
    tgd_min = GTK_SPIN_BUTTON(gtk_builder_get_object(tgd_builder, "entry_min_symbols"));
    tgd_max = GTK_SPIN_BUTTON(gtk_builder_get_object(tgd_builder, "entry_max_symbols"));
    tgd_pwd_type = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(tgd_builder, "password_type"));
    tgd_qr_code_image = GTK_IMAGE(gtk_builder_get_object(tgd_builder, "qr_code_image"));
    tgd_save_qr_button = GTK_BUTTON(gtk_builder_get_object(tgd_builder, "save_qr_button"));
    GtkMenuButton * _menu_btn = GTK_MENU_BUTTON(gtk_builder_get_object(tgd_builder, "menu_button"));
    GtkPopoverMenu * _menu_ctx = GTK_POPOVER_MENU(gtk_builder_get_object(tgd_builder, "menu_context"));
    GtkModelButton * about_button = GTK_MODEL_BUTTON(gtk_builder_get_object(tgd_builder, "about_button"));
    tgd_about_dialog = GTK_ABOUT_DIALOG(gtk_builder_get_object(tgd_builder, "about_dialog"));

    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider), "style.css", NULL);
    GtkStyleContext *styleContext = gtk_widget_get_style_context(GTK_WIDGET(tgd_entry));
    gtk_style_context_add_class(styleContext, "password_entry_field");

    gtk_header_bar_set_title(bar, TGDPWDGEN_SOFTWARE_NAME);

    gtk_menu_button_set_popover(_menu_btn, GTK_WIDGET(_menu_ctx));

    gtk_window_set_titlebar(window, GTK_WIDGET(bar));
    gtk_application_add_window(app, window);


    g_signal_connect(extras1, "clicked", G_CALLBACK(on_open_extras_click), NULL);
    g_signal_connect(button_qr, "clicked", G_CALLBACK(on_button_qr_click), NULL);
    g_signal_connect(button_gen, "clicked", G_CALLBACK(on_generate_password_clicked), NULL);
    g_signal_connect(button_copy, "clicked", G_CALLBACK(on_button_copy_clicked), NULL);
    g_signal_connect(tgd_pwd_type, "changed", G_CALLBACK(on_type_changed), NULL);
    g_signal_connect(tgd_min, "value-changed", G_CALLBACK(on_min_change_value), NULL);
    g_signal_connect(tgd_max, "value-changed", G_CALLBACK(on_max_change_value), NULL);
    g_signal_connect(about_button, "clicked", G_CALLBACK(on_about_button_clicked), NULL);
    g_signal_connect(tgd_about_dialog, "delete-event", G_CALLBACK(on_about_dialog_deleted), NULL);

    gui_do_generate();

    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_widget_hide(GTK_WIDGET(tgd_save_qr_button));

}

int run_gui_application(int argc, char **argv) {
    GtkApplication * app;
    int status;

    app = gtk_application_new(TGDPWDGEN_APPID, G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}