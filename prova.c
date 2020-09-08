#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


#define NEW_LINE 1026
#define INPUT_BUFFER_SIZE 100

typedef struct text {
    long long first_line;
    long long second_line;
    long long array_size;
    bool boolean;
    char command;
    char **lines;
}text_editor;

bool insert;
char **lines = NULL;
long long array_size = 0, max_undo = 0, max_redo = 0, complete_states_size = 0;
long long *complete_states;
text_editor *states = NULL;

void free_all(long long version) {
    for(long long i = version; i < max_undo - max_redo; i++) {
        free(&states[i]);
    }
    (&states)[max_undo + 1] = NULL;
}


void insert_at_end(char c, long long a, long long b) {
    long long i = 0, j = 0;
    states = realloc(states, sizeof(text_editor) * max_undo);
    states[max_undo - 1].command = c;
    states[max_undo - 1].first_line = a;
    states[max_undo - 1].second_line = b;
    states[max_undo - 1].array_size = array_size;
    if(c == 'd' || insert == true) {
        complete_states = realloc(complete_states, complete_states_size + 1);
        complete_states_size = max_undo - 1;
        states[max_undo - 1].boolean = true;
        states[max_undo - 1].lines = malloc(sizeof(char *) * array_size);
        for (i = 0; i < array_size; i++) {
            states[max_undo - 1].lines[i] = lines[i];
        }
    }

    else {
        states[max_undo - 1].boolean = false;
        states[max_undo - 1].lines = malloc(sizeof(char *) * (b - a + 1));
        for (i = a - 1; i < b; i++) {
            states[max_undo - 1].lines[j] = lines[i];
            j++;
        }
    }
}


void print(long long first_line, long long second_line) {
    long long i;
    if(first_line == 0 && second_line == 0) {
        putc('.', stdout);
        putc('\n', stdout);
    }
    else {
        if (first_line == 0) {
            putc('.',stdout);
            putc('\n', stdout);
            i = first_line;
        }
        else {
            i = first_line - 1;
        }
        while (i < second_line) {
            if (i < array_size && i >= first_line - 1 && lines[i] != NULL) {
                fputs(lines[i], stdout);
            } else {
                putc('.',stdout);
            }
            putc('\n',stdout);
            i++;
        }
    }
}


void change_after_undo(long long first_line, long long second_line, char **lines_to_change) {
    long long i = 0;
    long long j = 0;
    if (lines == NULL) {
        lines = malloc(sizeof(char *) * second_line);
        array_size = second_line;
        while (i < second_line - first_line + 1) {
            lines[i] =  lines_to_change[i];
            i++;
        }
    } else {
        if (array_size < second_line) {
            lines = realloc(lines, sizeof(char *) * second_line);
            array_size = second_line;
        }
        for (i = first_line - 1; j < second_line - first_line + 1; i++) {
            lines[i] = lines_to_change[j];
            j++;
        }
    }
}


void change (long long first_line, long long second_line) {
    if(max_redo != 0) {
        free_all(max_undo + 1);
    }
    max_undo = max_undo + 1;
    max_redo = 0;
    char *new_line = malloc(sizeof(char) * NEW_LINE);
    long long i = 0;

    if(first_line <= array_size) {
        insert = true;
    }

    if (lines == NULL) {
        lines = malloc(sizeof(char *) * second_line);
        fgets(new_line, NEW_LINE, stdin);
        strtok(new_line, "\n");
        while (strcmp(new_line, ".") != 0) {
            lines[i] = malloc(sizeof(char) * strlen(new_line) + 1);
            strcpy(lines[i], new_line);
            i++;
            fgets(new_line, NEW_LINE, stdin);
            strtok(new_line, "\n");
        }
        array_size = second_line;
    }
    else {
        fgets(new_line, NEW_LINE, stdin);
        strtok(new_line, "\n");
        if (array_size < second_line) {
            lines = realloc(lines, sizeof(char *) * second_line);
            array_size = second_line;
        }
        if (first_line == 0) {
            first_line = 1;
        }
        for (i = first_line - 1; strcmp(new_line, ".") != 0; i++) {
            lines[i] = malloc(sizeof(char) * strlen(new_line) + 1);
            strcpy(lines[i], new_line);
            fgets(new_line, NEW_LINE, stdin);
            strtok(new_line, "\n");
        }
    }
    free(new_line);
}


void delete(long long first_line, long long second_line) {

    if (max_redo != 0) {
        free_all(max_undo + 1);
    }
    max_undo = max_undo + 1;
    max_redo = 0;
    if (lines == NULL || first_line == 0 && second_line == 0 || first_line > array_size && second_line >= array_size) {
        return;
    }
    else {
        long long i;
        if (second_line >= array_size) {
            if(first_line == 1 || first_line == 0) {
                for(i = 0; i < array_size; i++) {
                    lines[i] = NULL;
                }
                array_size = 0;
            }
            else {
                array_size = first_line;
            }
        }
        else {
            if (first_line == second_line) {
                for (i = first_line - 1; i + 1 < array_size; i++) {
                    lines[i]  = lines[i + 1];
                }
                array_size = array_size - 1;
            }
            else {
                if (first_line == 0) {
                    first_line = 1;
                }
                for (i = first_line - 1; i + (second_line - first_line + 1) < array_size; i++) {
                    lines[i] = lines[i + (second_line - first_line + 1)];
                }
                array_size = array_size - (second_line - first_line + 1);
            }
        }
    }
}

void undo(long long a) {
    long long i;

    if(a == max_undo) {
        for(i = 0; i < array_size; i++) {
            lines[i] = NULL;
        }
    }
    else {

        if(states[max_undo - a].command == 'd' || states[max_undo - a].boolean == true) {
            lines = malloc(sizeof(char *) * states[max_undo - a].array_size);
            for (i = 0; i < states[max_undo - 1].array_size; i++) {
                lines[i] = states[max_undo - 1].lines[i];
            }
            array_size = states[max_undo - 1].array_size;
        } else {
            long long count = max_undo - a;
            while (count >= 0 && states[count].command != 'd' && states[count].boolean != true) {
                count--;
            }
            if (count < 0) {
                for (i = 0; i < states[max_undo - 1].array_size; i++) {
                    lines[i] = states[max_undo - 1].lines[i];
                }
                array_size = 0;
            }
            else {
                for (i = 0; i < states[max_undo - 1].array_size; i++) {
                    lines[i] = states[count].lines[i];
                }
                while(count != max_undo - a) {
                    change_after_undo(states[count].first_line, states[count].second_line, states[count].lines);
                    count++;
                }
            }
        }
    }
    max_undo = max_undo - a;
    max_redo = max_redo - a;
}

void redo(long long num_redo) {
    long long i = 0;

    max_redo = max_redo + num_redo;
    max_undo = max_undo + num_redo;

    if (states[num_redo - 1].boolean == true) {
        lines = realloc(lines, sizeof(char *) * states[num_redo - 1].array_size);
        for (i = 0; i < states[num_redo - 1].array_size; ++i) {
            lines[i] = states[num_redo - 1].lines[i];
        }
    } else {
        while (!(complete_states[i] < num_redo - 1 && complete_states[i + 1] > num_redo - 1)) {
            i++;
        }
        for (long long j = 0; j < states[i - 1].array_size; ++j) {
            lines[j] = states[i].lines[j];
        }
    }
}


char split_command(char* str, long long *num1, long long *num2){
    char* c = str;
    int idx = 0;
    int comma;
    while(*c != '\0'){
        if(*c==','){
            *c = '\0';
            *num1 = atoi(str);
            comma = idx;
        }
        c++;
        idx++;
    }
    *c = '\0';
    char command = *(c-1);
    *(c-1) = '\0';
    if(command == 'q') {
        return command;
    }
    if(command == 'u' || command == 'r') {
        *num1 = atoi(str);
    }
    else {
        *num2 = atoi(str + comma + 1);
    }
    return command;
}

int main() {
    char *inputBuffer = malloc(sizeof(char) * NEW_LINE);
    long long x = 0;
    while (1) {
        if (fgets(inputBuffer, INPUT_BUFFER_SIZE, stdin)) {
            strtok(inputBuffer, "\n");
            long long a, b;
            char *s = malloc(sizeof(char) * strlen(inputBuffer) + 1);
            strcpy(s, inputBuffer);
            char c = split_command(s, &a, &b);
            if (c == 'u' || c == 'r') {
                while (c == 'u' || c == 'r') {
                    if (c == 'u') {
                        if (max_undo <= x + a) {
                            x = max_undo;
                        } else {
                            x = x + a;
                        }
                    }
                    if (c == 'r') {
                        if (x - a > max_redo) {
                            x = x - a;
                        } else {
                            x = max_redo;
                        }
                    }
                    fgets(inputBuffer, INPUT_BUFFER_SIZE, stdin);
                    strtok(inputBuffer, "\n");
                    s = malloc(sizeof(char) * strlen(inputBuffer) + 1);
                    strcpy(s, inputBuffer);
                    c = split_command(s, &a, &b);
                }
                if (x > 0) {
                    undo(x);
                } else if (x < 0) {
                    x = -x;
                    redo(x);
                }
                x = 0;
            }

            if (c == 'q') {
                free(s);
                free(inputBuffer);
                free(lines);
                return 0;
            }
            if (c == 'c') {
                change(a, b);
                insert_at_end(c, a, b);
            }
            if (c == 'd') {
                delete(a, b);
                insert_at_end(c, a, b);

            }
            if (c == 'p') {
                print(a, b);
            }
            free(s);
        }
    }
}

