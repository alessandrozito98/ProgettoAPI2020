#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFFER_SIZE 1026

typedef struct text {
    char **lines;
    struct text *next;
    struct text *prec;
}text_editor;

long long array_size = 0;
char **lines = NULL;

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


int change (text_editor **head, text_editor **tail, long long first_line, long long second_line) {
    char *new_line = malloc(sizeof(char) * INPUT_BUFFER_SIZE);
    text_editor *new_node = malloc(sizeof(text_editor));
    long i = 0;
    if (lines == NULL) {
        lines = malloc(sizeof(char *) * second_line + 1);
        fgets(new_line, INPUT_BUFFER_SIZE, stdin);
        strtok(new_line, "\n");
        array_size = second_line;
        while (strcmp(new_line, ".") != 0) {
            lines[i] = malloc(sizeof(char) * strlen(new_line) + 1);
            strcpy(lines[i], new_line);
            i++;
            fgets(new_line, INPUT_BUFFER_SIZE, stdin);
            strtok(new_line, "\n");

        }
        new_node->lines = lines;
        new_node->next = NULL;
        new_node->prec = NULL;
        if (*head == NULL) {
            *head = new_node;
            *tail = *head;
        } else {
            (*tail)->next = new_node;
            new_node->prec = *tail;

            *tail = (*tail)->next; // Increment LAST to point to the new last node.
        }
    } else {
        fgets(new_line, INPUT_BUFFER_SIZE, stdin);
        strtok(new_line, "\n");
        if (array_size < second_line) {
            lines = realloc(lines, sizeof(char *) * second_line);
            array_size = second_line;
        }
        for (i = first_line - 1; strcmp(new_line, ".") != 0; i++) {
            lines[i] = malloc(sizeof(char) * strlen(new_line) + 1);
            strcpy(lines[i], new_line);
            fgets(new_line, INPUT_BUFFER_SIZE, stdin);
            strtok(new_line, "\n");
        }

        new_node->lines = lines;
        new_node->next = NULL;
        new_node->prec = NULL;
        if (*head == NULL) {
            *head = new_node;
            *tail = *head;
        } else {
            (*tail)->next = new_node;
            new_node->prec = *tail;
            *tail = (*tail)->next; // Increment LAST to point to the new last node.
        }
    }
    free(new_line);
}


void delete(text_editor **head, text_editor **tail, long long first_line, long long second_line) {
    text_editor *new_node = malloc(sizeof(text_editor));
    if (first_line == 0 && second_line == 0) {
        new_node->lines = lines;
        (*tail)->next = new_node;
        new_node->prec = *tail;
        *tail = (*tail)->next;
    } else {

    }
}

void undo(long long line) {

}

void redo(long long line) {

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
    *num2 = atoi(str+comma+1);
    return command;
}

int main() {
    char *inputBuffer = malloc(sizeof(char) * INPUT_BUFFER_SIZE);
    text_editor *edU = NULL;
    text_editor *tail = NULL;
    while (1) {
        if (fgets(inputBuffer, INPUT_BUFFER_SIZE, stdin)) {
            strtok(inputBuffer, "\n");
            if (*inputBuffer == 'q') {
                free(inputBuffer);
                free(edU->lines);
                free(edU);
                return 0;
            }
            char *s = malloc(sizeof(char) * strlen(inputBuffer) + 1);
            strcpy(s, inputBuffer);
            long long a, b;
            char c = split_command(s, &a, &b);
            //printf("Command: %c on lines from %d to %d\n", c, a, b);
            if (c == 'c') {
                change(&edU, &tail, a, b);
            }
            if (c == 'd') {
                delete(&edU, &tail, a, b);
            }
            if (c == 'u') {
                undo(a);
            }
            if (c == 'r') {
                redo(a);
            }
            if (c == 'p') {
                print(a, b);
            }
            free(s);
        }
    }
}

