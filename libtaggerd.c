/**
 * Default Taggerd Library.
 * Library is developed to simplify creation of the new software for the Taggerd OS (OS that I develop based on LFS).
 * In addition the library can be used not only for the corresponding OS.
 *
 * @author Ivan Ponomarev <ivan.ponomarev.pi@gmail.com>
 * @version 1.0
 * @date 2019-07-03
 * @license GPL-3
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "libtaggerd.h"

const char * tgd_get_lib_version() {
    return LIBTGD_VERSION;
}

void tgd_init() {
    srand(time(NULL));
}

char * tgd_concat(const char * str_one, const char * str_two) {
    int length = strlen(str_one) + strlen(str_two);
    char * res = NULL;
    res = malloc(sizeof(char) * (length + 1));

    if (res) {
        for (int n = 0; n < strlen(str_one); n++) {
            int key = str_one[n];
            res[n] = key;
        }
        for (int n = strlen(str_one); n < (strlen(str_two) + strlen(str_one)); n++) {
            int key = str_two[n];
            res[n] = key;
        }

        res[length] = '\0';
    }

    return res;
}

int random_length(int min, int max) {
    int res = min;
    int difference = max - min;
    if (difference) res = (rand() % difference) + min;
    return res;
}