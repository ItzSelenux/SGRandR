#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024

char **resolutions;
char **rates;
char **outputs;

char** get_resolutions() {
    char** resolutions = malloc(BUFFER_SIZE * sizeof(char*));
    FILE* pipe = popen("xrandr", "r");
    char buffer[BUFFER_SIZE];
    int i = 0;
    while (fgets(buffer, BUFFER_SIZE, pipe) != NULL) {
        if (strstr(buffer, "  ") != NULL) {
            char* resolution = strtok(buffer, " ");
            resolutions[i] = strdup(resolution);
            i++;
        }
    }
    resolutions[i] = NULL;
    pclose(pipe);
    return resolutions;
}

    char **get_rates() {
        char buffer[BUFFER_SIZE];
        char **rates = malloc(BUFFER_SIZE * sizeof(char *));
        int count = 0;

        FILE *pipe = popen("xrandr", "r");
        if (!pipe) {
            fprintf(stderr, "Error executing command.\n");
            return NULL;
        }

        int ures = 1;

        while (fgets(buffer, BUFFER_SIZE, pipe)) {
            if (strstr(buffer, "*")) {
                char *rate = strtok(buffer, " ");
                while (rate != NULL && count < BUFFER_SIZE) {  // check if count is less than BUFFER_SIZE before accessing rates array
                    // remove "+" and newlines
                    rate[strcspn(rate, "+\n")] = 0;

                    if (!ures && strcmp(rate, "") != 0) {
                        rates[count] = malloc(strlen(rate) + 1);
                        strcpy(rates[count], rate);

                        count++;
                    }

                    if (ures) {
                        ures = 0;
                    }

                    rate = strtok(NULL, " ");
                }
            }
        }

        pclose(pipe);

        // Add NULL terminator to array
        if (count < BUFFER_SIZE) {  // check if count is less than BUFFER_SIZE before accessing rates array
            rates[count] = NULL;
        } else {
            fprintf(stderr, "Array index out of bounds.\n");
        }

        return rates;
    }


char** get_outputs() {
    char* command = "xrandr";
    char buffer[BUFFER_SIZE];
    FILE* fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to execute command\n");
        exit(EXIT_FAILURE);
    }

    char** outputs = (char**)malloc(sizeof(char*));
    int num_outputs = 0;

    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        if (strstr(buffer, " connected")) {
            char* output_name = strtok(buffer, " ");
            outputs[num_outputs] = (char*)malloc(sizeof(char) * strlen(output_name));
            strcpy(outputs[num_outputs], output_name);
            num_outputs++;
            outputs = (char**)realloc(outputs, sizeof(char*) * (num_outputs + 1));
        }
    }

    outputs[num_outputs] = NULL;
    pclose(fp);

    return outputs;
}

