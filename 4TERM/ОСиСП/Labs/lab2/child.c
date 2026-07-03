#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>

extern char **environ;

#define VAR_NUM 9

char** getShortVariables(const char* fname) {
    FILE *f = NULL;
    if ((f = fopen(fname, "r")) != NULL) {
        char **result = (char **)calloc(VAR_NUM, sizeof(char *));
        for (int i = 0; i < VAR_NUM; i++) {
            result[i] = (char*)calloc(80, sizeof(char));
            fgets(result[i], 80, f);
            result[i][strcspn(result[i], "\n")] = '\0';
            //result[i][strlen(result[i])-1] = '\0';
        }
        fclose(f);
        return result;
    }
    printf("Error while open file.\n");
    return NULL;
}

int includeString(const char* str1, const char* str2) {
    for(size_t i = 0; i<strlen(str2); i++)
        if(str1[i]!=str2[i])
            return 0;
    return 1;
}

char* findEnvpVariable(char* envp[], const char* variable) {
    int i = 0;
    while (envp[i]) {
        if (includeString(envp[i], variable))
            break;
        i++;
    }
    if (envp[i] == NULL) {
        return "(null)";
    }
    return envp[i];
}

int main(int argc, char* argv[], char* envp[]) {

    printf("Process name: %s\nProcess pid: %d\nProcess ppdid: %d\n", argv[0], (int) getpid(), (int) getppid());

    char **variables = getShortVariables(argv[1]);
    if (variables != NULL) {
        switch (argv[2][0]) {
            case '+':
                for (int i = 0; i < VAR_NUM; i++)
                    printf("%s=%s\n", variables[i], getenv(variables[i]));
                break;
            case '*':
                for (int i = 0; i < VAR_NUM; i++) {
                    char* envpVariable = findEnvpVariable(envp, variables[i]);
                    if (strcmp(envpVariable, "(null)") != 0) {
                        printf("%s\n", envpVariable);
                    } else {
                        printf("%s=%s\n", variables[i], envpVariable);
                    }
                }
                break;
            case '&':
                for (int i = 0; i < VAR_NUM; i++) {
                    char* envpVariable = findEnvpVariable(environ, variables[i]);
                    if (strcmp(envpVariable, "(null)") != 0) {
                        printf("%s\n", envpVariable);
                    } else {
                        printf("%s=%s\n", variables[i], envpVariable);
                    }
                }
                break;
        }
        for (int i = 0; i < VAR_NUM; i++)
            free(variables[i]);
    }
    free(variables);
    return 0;
}