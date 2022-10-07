//
// Created by fanny on 2022-10-06.
//

#ifndef PUISSANCEC_PRINTGRILLE_H
#define PUISSANCEC_PRINTGRILLE_H
#include "user.h"
#include "config.h"

char ** init_grille(gameConfig* config);
void print_all(char ** grille, int rows, int columns);
void print_all_multi(char ** grille, int rows, int columns, char *output);

#endif //PUISSANCEC_PRINTGRILLE_H
