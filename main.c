#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "printGrille.h"
#include <string.h>
#include "utils.h"
#include "config.h"
#include "user.h"
#include "game.h"
#include "colors.h"

int verifChara(char cara){
    if(!(isalpha(cara))){
        puts("Characteres spéciaux non autorisés");
        return 0;
    }
    Users *users = getUsers();

    for(int k = 0 ; k<users->size;k++){
        if(users->users[k].token == cara){
            puts("Symbole déjà utilisé");
            return 0;
        }
    }

    return 1;
}

int main(int argc, char **argv) {

    // fill gameConfig using config file
    gameConfig config;
    defaultConfig(&config);
    if (readConfig(&config) == 1) {
        fprintf(stderr, "Erreur lors de la lecture du fichier de configuration");
        return EXIT_FAILURE;
    }

    int multiGame = 0;
    int ipExiste = 0;
    int portExiste = 0;

    char *ip=calloc(16,sizeof(char));
    char *port= calloc(6,sizeof(char));

    //BOUCLE SUR LES PARAMETRES DES EXECUTABLES
    for(int i = 1; i < argc ; i++){
        if(strcmp(argv[i],"--help")==0 || strcmp(argv[i],"-?")==0){
            //affiche HELP
            puts("-h, --host              Start a game as host");
            puts("-i, --ip=127.0.0.1      IP to use");
            puts("-j, --join              Join a game");
            puts("-l, --local             Start a local game (default)");
            puts("-p, --port=3000         Port to use");
            puts("-?, --help              Give this help list");
            puts("  , --usage             Give a short usage message");
        }

        //TAILLE CHAINE CHARA DE L'ARGUMENT
        unsigned long long argSize = strlen(argv[i]);

        if(argSize>2){
            for(int j = 0;j<argSize;j++) {
                //TROUVER SI IL Y A IP
                if (argv[i][j] == 'i' && argv[i][j + 1] == 'p') {
                    if(ipExiste != 0){
                        puts("VOUS CONFIGUREZ DEJA L'IP");
                        exit;
                    }
                    ipExiste = 1;
                    int findEg = 0;

                    //CHERCHE LA VALEUR '=' ET LA POSITION
                    while (argv[i][findEg] != '=' && findEg < argSize) {
                        findEg++;
                    }

                    //REMPLIS LE TABLEAU IP
                    for (int k = findEg; k < argSize; k++) {
                        config.globalConfig.ip[k - findEg] = argv[i][k + 1];
                    }

                    puts(ip);
                }

                //TROUVER SI IL Y A PORT
                if (argv[i][j] == 'p' && argv[i][j + 1] == 'o') {
                    if(portExiste != 0){
                        puts("VOUS CONFIGUREZ DEJA LE PORT");
                        exit;
                    }
                    portExiste = 1;
                    int findEg = 0;

                    //CHERCHE LA VALEUR '=' ET LA POSITION
                    while (argv[i][findEg] != '=' && findEg < argSize) {
                        findEg++;
                    }

                    //REMPLIS LE TABLEAU PORT
                    for (int k = findEg; k < argSize; k++) {
                        config.globalConfig.port[k - findEg] = argv[i][k + 1];
                    }

                    puts(port);
                }
            }
        }

        //VERIFIE SI ARGV EST I
        if(strcmp(argv[i],"-i")==0){
            //range dans ip l'adresse IP
            if(ipExiste != 0){
                puts("VOUS CONFIGUREZ DEJA L'IP");
                exit;
            }
            ipExiste = 1;
            i++;
            strcpy(config.globalConfig.ip,argv[i]);
        }

        //VERIFIE SI ARGV EST join
        if(strcmp(argv[i],"-h")==0 || strcmp(argv[i],"--host")==0){
            //Rejoindre le jeu en multi
            config.globalConfig.host = 1;

            puts("Combien de joueur voulez-vous ?");
            int nbJoueurs = 0;
            while(nbJoueurs<2 || nbJoueurs>4){
                puts("Le nombre de joueur doit être supérieur à 2 mais inférieur à 4");
                nbJoueurs = fgetc(stdin);
            }
            config.globalConfig.users = nbJoueurs;
        }

        //VERIFIE SI ARGV EST join
        if(strcmp(argv[i],"-j")==0 || strcmp(argv[i],"--join")==0){
            //Rejoindre le jeu en multi
            config.globalConfig.join = 1;
        }

        //VERIFIE SI ARGV EST local
        if(strcmp(argv[i],"-l")==0 || strcmp(argv[i],"--local")==0){
            //Rejoindre le jeu en local
            config.globalConfig.join = 0;
            config.globalConfig.host = 0;
        }

        //VERIFIE SI ARGV EST P
        if(strcmp(argv[i],"-p")==0){
            //range dans ip l'adresse IP
            if(portExiste != 0){
                puts("VOUS CONFIGUREZ DEJA LE PORT");
                exit;
            }
            portExiste = 1;
            i++;
            strcpy(config.globalConfig.port,argv[i]);
        }

        //VERIFIE SI ARGV EST USAGE
        if(strcmp(argv[i],"--usage")==0){
            //Mettre une introduction pour l'usage de notre executable
            puts("Mettre une introduction pour l'usage de notre executable");
            exit;
        }
    }

    printf("Host: %d\n", config.globalConfig.host);
    printf("Join: %d\n", config.globalConfig.join);
    printf("Ip: %s\n", config.globalConfig.ip);
    printf("Port: %s\n", config.globalConfig.port);

    puts("\033[1;31mHello World!\033[0m");
    // for (int i = BLACK; i < COLOR_COUNT; i++)
    // {
    //     printColoredChar(i, 'A' + i);
    // }

    Users *users = getUsers();

    char name[SIZE_DATA];
    char token1;
    char token2;
    puts("Entrer le nom du joueur 1");
    fgets(name, SIZE_DATA, stdin);
    removeLineFeed(name);
    puts("Entrer le token du joueur 1");
    token1 = (char)fgetc(stdin);
    fflush(stdin);
    while(verifChara(token1) == 0){
        puts("Entrer le token du joueur 1");
        token1 = (char)fgetc(stdin);
        fflush(stdin);
    }

    //TODO check si username valide
    //TODO check si username deja pris
    createUser(name,token1);

    puts("Entrer le nom du joueur 2");
    fgets(name, SIZE_DATA, stdin);
    fflush(stdin);
    removeLineFeed(name);
    puts("Entrer le token du joueur 2");
    token2 = (char)fgetc(stdin);
    fflush(stdin);

    while(verifChara(token2) == 0){
        puts("Entrer le token du joueur 2");
        token2 = (char)fgetc(stdin);
        fflush(stdin);
    }

    createUser(name,token2);

    for (int i = 0; i < users->size; i++)
    {
        puts(users->users[i].name);
    }


    int rows = 6;
    int columns = 7;

    char ** grille = NULL;

    grille = init_grille(rows, columns);
    print_all(grille, rows, columns);


    //tmp
    startGame(users, &config);
}
