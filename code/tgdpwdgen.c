//
// Created by ivanp on 20.06.19.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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

char * splitter_symbol = NULL;

char * get_splitter_symbol(int is_random_symbol_used) {
    if (splitter_symbol == NULL || is_random_symbol_used)
        splitter_symbol = generator_general(charset_simple_separators, 1);
    return splitter_symbol;
}

char * generator_password_simple(const char * charset, int length, int is_random_symbol_used) {
    int check_size = 2 + TGDPWDGEN_DEFAULT_SET_SIZE_1 + TGDPWDGEN_DEFAULT_SET_SIZE_2 + TGDPWDGEN_DEFAULT_SET_SIZE_2;
    char * accu_set = NULL;
    char * res = NULL;

    if (length >= check_size) {
        int remainder = length - check_size;
        int quantity_of_slices = (remainder / (TGDPWDGEN_DEFAULT_SET_SIZE_2 + 1)) + 2;
        int additional_slice_length = remainder % (TGDPWDGEN_DEFAULT_SET_SIZE_2 + 1);

        res = malloc(length + 1);
        accu_set = generator_general(charset_simple_set, TGDPWDGEN_DEFAULT_SET_SIZE_1);
        strcpy(res, accu_set);

        for (int i = 0; i < quantity_of_slices; i++) {
            accu_set = generator_general(charset_simple_set, TGDPWDGEN_DEFAULT_SET_SIZE_2);
            strcat(res, get_splitter_symbol(is_random_symbol_used));
            strcat(res, accu_set);
        }
        if (additional_slice_length != 0) {
            if (additional_slice_length > 1) {
                accu_set = generator_general(charset_simple_set, additional_slice_length - 1);
                strcat(res, get_splitter_symbol(is_random_symbol_used));
            } else
                accu_set = generator_general(charset_simple_set, 1);
            strcat(res, accu_set);
        }

        free(splitter_symbol);
        splitter_symbol = NULL;
        free(accu_set);
    } else
        res = generator_general(charset, length);
    return res;
}
char * generator_pin(int length) { return generator_general(charset_pin, length); }
char * generator_path(int length) { return tgd_concat("tmp_", generator_general(charset_path, length)); }
char * generator_password_hard(int length) { return generator_password_simple(charset_all, length, 1); }
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
        res = generator_password_simple(charset_simple_set, length, 0);

    return res;
}
