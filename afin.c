#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/* Boolean data type*/
typedef enum { false, true } bool;

/* Parameters data type*/
typedef struct {
    bool mode;
    int ciphertext_size;
    int afin_mult;
    int afin_const;
    FILE *input;
    FILE *output;

} parameters_afin;

/* MAIN FUNCTION */
int main(int argc, char *argv[]) {

    parameters_afin options;
    bool doDefault = false;
    int c;
    opterr = 0;

    while ((c = getopt (argc, argv, "CDm:a:b:i::o::")) != -1)
    {
        switch (c)
        {
            case 'C':
                options.mode = false;
                break;
            case 'D':
                options.mode = true;
                break;
            case 'm':
            printf("m is %s\n", optarg);
            fflush(stdout);
                if (isdigit(*optarg) == 0){
                    doDefault = true;
                    break;
                }
                printf("m is %s\n", optarg);
                fflush(stdout);
                options.ciphertext_size = atoi(optarg);
                printf("m value is %d\n", options.ciphertext_size);
                break;
            case 'a':
                if (!isdigit(*optarg)){
                        doDefault = true;
                        break;
                }
                options.afin_mult = atoi(optarg);
                printf("a value is %d\n", options.afin_mult);
                break;
            case 'b':
                if (!isdigit(*optarg)){
                        doDefault = true;
                        break;
                }
                options.afin_const = atoi(optarg);
                printf("b value is %d\n", options.afin_const);
                break;
            case 'i':
                if (!isdigit(*optarg)){
                        doDefault = true;
                        break;
                }
                options.input = atoi(optarg);
                printf("i value is %d\n", options.input);
                break;
            case 'o':
                if (!isdigit(optarg)){
                        doDefault = true;
                        break;
                }
                options.output = atoi(optarg);
                printf("o value is %d\n", options.output);
                break;
            case '?':
                doDefault = true;
                break;
            default:
                printf("hola has llegado.\n");
        }

        if (doDefault){
            if (optopt == 'm')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 'a')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 'b')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 'i')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 'o')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option '-%c'.\n", optopt);
            else
                fprintf (stderr, "Wrong value for -%c.\n", c);
                return 1;
        }
    }
}