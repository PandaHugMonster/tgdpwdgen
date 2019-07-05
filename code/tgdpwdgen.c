//
// Created by ivanp on 20.06.19.
//

#include <stdlib.h>
#include <string.h>
#include "../headers/tgdpwdgen.h"
#include "../headers/libtaggerd.h"

const char charset_all[] = TGDPWDGEN_CHARSET_ALL;
const char charset_pin[] = TGDPWDGEN_CHARSET_PIN;
const char charset_path[] = TGDPWDGEN_CHARSET_PATH;
const char charset_hex[] = TGDPWDGEN_CHARSET_HEX;
const char charset_simple_set[] = TGDPWDGEN_CHARSET_SIMPLE_SET;
const char charset_simple_separators[] = TGDPWDGEN_CHARSET_SIMPLE_SEPARATORS;

char * generator_general(const char * charset, int length_new) {
    char * res = NULL;
    res = malloc(sizeof(char) * (length_new + 1));
    if (res) {
        for (int n = 0; n < length_new; ++n) {
            int key = rand() % (int) (strlen(charset));
            res[n] = charset[key];
        }
        res[length_new] = '\0';
    }
    return res;
}

char * generator_password_simple(const char * charset, int length) {
    int check_size = 2 + TGDPWDGEN_DEFAULT_SET_SIZE_1 + TGDPWDGEN_DEFAULT_SET_SIZE_2 + TGDPWDGEN_DEFAULT_SET_SIZE_3;
    char * symbol = NULL; char * set_1 = NULL; char * set_2 = NULL; char * set_3 = NULL;
    char * res = NULL;

    if (length >= check_size) {
        symbol = generator_general(charset_simple_separators, 1);
        set_1 = generator_general(charset_simple_set, TGDPWDGEN_DEFAULT_SET_SIZE_1);
        set_2 = generator_general(charset_simple_set, TGDPWDGEN_DEFAULT_SET_SIZE_2);
        set_3 = generator_general(charset_simple_set, TGDPWDGEN_DEFAULT_SET_SIZE_3);
        res = malloc(check_size + 1);

        strcpy(res, set_1);
        strcat(res, symbol);
        strcat(res, set_2);
        strcat(res, symbol);
        strcat(res, set_3);
        free(symbol);
        free(set_1);
        free(set_2);
        free(set_3);
    } else
        res = generator_general(charset, length);
    return res;
}
char * generator_pin(int length) { return generator_general(charset_pin, length); }
char * generator_path(int length) { return tgd_concat("tmp_", generator_general(charset_path, length)); }
char * generator_password_hard(int length) { return generator_password_simple(charset_all, length); }
char * generator_password_insane(int length) { return generator_general(charset_all, length); }

char * do_generate(int minimum, int maximum, const char * type) {
    char * res;
    int length = random_length(minimum, maximum);

    if (strcmp(type, TGDPWDGEN_COMBO_TYPE_ITEM_PIN) == 0)
        res = generator_pin(length);
    else if (strcmp(type, TGDPWDGEN_COMBO_TYPE_ITEM_PATH) == 0)
        res = generator_path(length);
    else if (strcmp(type, TGDPWDGEN_COMBO_TYPE_ITEM_PASSWORD_HARD) == 0)
        res = generator_password_hard(length);
    else if (strcmp(type, TGDPWDGEN_COMBO_TYPE_ITEM_PASSWORD_INSANE) == 0)
        res = generator_password_insane(length);
    else if (strcmp(type, TGDPWDGEN_COMBO_TYPE_ITEM_PASSWORD_PARANOIA) == 0)
        res = generator_password_insane(length);
    else
        res = generator_password_simple(charset_simple_set, length);

    return res;
}
