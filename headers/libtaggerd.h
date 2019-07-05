//
// Created by ivanp on 03.07.19.
//

#ifndef TGD_LIBTAGGERD_H
#define TGD_LIBTAGGERD_H

#define LIBTGD_VERSION "1.0"

const char * tgd_get_lib_version();
void tgd_init();
char * tgd_concat(const char * str_one, const char * str_two);
int random_length(int min, int max);

#endif //TGD_LIBTAGGERD_H
