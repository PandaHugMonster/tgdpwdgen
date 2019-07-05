//
// Created by ivanp on 20.06.19.
//

#ifndef TGDPWDGEN_TGDPWDGEN_H
#define TGDPWDGEN_TGDPWDGEN_H

#define TGDPWDGEN_APPID "org.taggerd.pwdgen"

#define TGDPWDGEN_SOFTWARE_NAME "New Password Generator"
#define TGDPWDGEN_COMBO_TYPE_ITEM_PIN "item_pin"
#define TGDPWDGEN_COMBO_TYPE_ITEM_PATH "item_path"
#define TGDPWDGEN_COMBO_TYPE_ITEM_PASSWORD_HARD "item_password_hard"
#define TGDPWDGEN_COMBO_TYPE_ITEM_PASSWORD_INSANE "item_password_insane"
#define TGDPWDGEN_COMBO_TYPE_ITEM_PASSWORD_PARANOIA "item_password_paranoia"

#define TGDPWDGEN_CHARSET_ALL "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890/?)(!@#$%^&*()_+-><.,";
#define TGDPWDGEN_CHARSET_PIN "1234567890";
#define TGDPWDGEN_CHARSET_PATH "abcdefghijklmnopqrstuvwxyz1234567890";
#define TGDPWDGEN_CHARSET_HEX "0123456789ABCDEF";
#define TGDPWDGEN_CHARSET_SIMPLE_SET "abcdefghikmnpqrstwxyzABCDEFGHJKLMNPQRSTWXYZ234567890";
#define TGDPWDGEN_CHARSET_SIMPLE_SEPARATORS "-+_*/#";

#define TGDPWDGEN_DEFAULT_SET_SIZE_1 2
#define TGDPWDGEN_DEFAULT_SET_SIZE_2 3
#define TGDPWDGEN_DEFAULT_SET_SIZE_3 3

char * do_generate(int minimum, int maximum, const char * type);

#endif //TGDPWDGEN_TGDPWDGEN_H
