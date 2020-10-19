#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ERR -1


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

    /* Parameter options*/
    parameters_afin options;

    /* Required parameters */
    bool ciphertext_size_flag = false;
    bool afin_mult = false;
    bool afin_const = false;

    /* Error controller*/
    bool doDefault = false;

    /* PROGRAM OPTIONS SELECTION */

    /* Variable declaration */
    int c;
    opterr = 0;

    /* Argument parsing with common arguments and list of possible options */
    while ((c = getopt (argc, argv, "CDm:a:b:i::o::")) != -1)
    {
        /* Checks if the selected option is in our list*/
        switch (c)
        {
            /* ENCRYPTION */
            case 'C':
                options.mode = false;
                break;

            /* DECRYPTION */
            case 'D':
                options.mode = true;
                break;

            /* CIPHERTEXT SPACE SITE */
            case 'm':
                if (isdigit(*optarg) == 0){
                    doDefault = true;
                    break;
                }
                options.ciphertext_size = atoi(optarg);
                ciphertext_size_flag = true;
                break;
            
            /* MULTIPLICATIVE COEFFICIENT (AFIN) */
            case 'a':
                if (!isdigit(*optarg)){
                        doDefault = true;
                        break;
                }
                options.afin_mult = atoi(optarg);
                afin_mult = true;
                break;

            /* CONSTANT TERM (AFIN) */
            case 'b':
                if (!isdigit(*optarg)){
                        doDefault = true;
                        break;
                }
                options.afin_const = atoi(optarg);
                afin_const = true;
                break;
            
            /* INPUT FILE */
            case 'i':
                options.input = stdin;
                break;
            
            /* OUTPUT FILE */
            case 'o':
                options.output = stdout;
                break;
            
            /* ERROR CONTROL */
            case '?':
                doDefault = true;
            default:
                break;
        }

        /* ERROR CONTROL FUNCTION FOR ARGUMENT PARSING */
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
    /* ERROR CONTROL, REQUIRED ARGUMENTS: e/d, m, a, b */
        if( !ciphertext_size_flag || !afin_mult || !afin_const ){
            fprintf (stderr, "REQUIRED ARGUMENTS:\n");
            fprintf (stderr, "\t-m <tamaño del espacio de texto cifrado (value)>\n");
            fprintf (stderr, "\t-a <coeficiente multiplicativo de la función afín (value)>\n");
            fprintf (stderr, "\t-b <término constante de la función afín (value)>\n");
        }

        /* COMPROBAR ENTRE ENCRYPT Y DECRYPT MEDIANTE LA BANDERA options.mode */
        /* TODO */

        /* UTILIZAR LOS VALORES */
}