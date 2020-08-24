#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INPUT_BUFFER_SIZE 15
#define NEW_LINE 1025

typedef struct line {
    char *line;
    struct line *next_line;
}line;

typedef struct text {
    int version;
    struct line *lines;
    struct text *next;
}text_editor;

text_editor *edU;

void print(int first_line, int second_line) {
    if(first_line == 0 && second_line == 0) {
        putc_unlocked('.', stdout);
    }
    else {
        text_editor *temp = edU;
        int count = 1;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        line *temp_line = temp->lines;
        while (first_line != count) {
            temp_line = temp_line->next_line;
            count++;
        }
        while (count != second_line + 1) {
            if (temp_line->line != NULL) {
                fputs(temp_line->line, stdout);
            } else {
                putc_unlocked('.', stdout);
            }
            putc_unlocked('\n', stdout);
            temp_line = temp_line->next_line;
            count++;
        }
    }
}


void change (int first_line, int second_line) {
    char *new_line = malloc(sizeof(char) * NEW_LINE);
    int count = 1;
    line *temp_line;
    if(edU == NULL) {
        // First change
        edU = malloc(sizeof(text_editor));
        edU->next = NULL;
        edU->lines = malloc(sizeof(line));
        edU->lines->next_line = NULL;
        edU->version = 1;
        temp_line = edU->lines;
        if(first_line == 1) {
            fgets(new_line, NEW_LINE, stdin);
            strtok(new_line, "\n");
            edU->lines->line = malloc(sizeof(char) * strlen(new_line) + 1);
            strcpy(edU->lines->line, new_line);
        }
        else {
            while(count != first_line - 1) {
                temp_line->next_line = malloc(sizeof(line));
                temp_line = temp_line->next_line;
                count++;
            }
        }
        fgets(new_line, NEW_LINE, stdin);
        strtok(new_line, "\n");
        while (strcmp(new_line, ".") != 0) {
            temp_line->next_line = malloc(sizeof(line));
            temp_line->next_line->next_line = NULL;
            temp_line->next_line->line = malloc(sizeof(char) * strlen(new_line) + 1);
            strcpy(temp_line->next_line->line, new_line);
            temp_line = temp_line->next_line;
            fgets(new_line, NEW_LINE, stdin);
            strtok(new_line, "\n");
        }
    }
    else {
        text_editor *temp = edU;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = malloc(sizeof(text_editor));
        temp->next->next = NULL;
        temp->next->lines = malloc(sizeof(line));
        temp->next->lines->next_line = NULL;
        memcpy(temp->next->lines, temp->lines, sizeof(line));
        temp->next->version = (temp->version) + 1;
        temp = temp->next;
        temp_line = temp->lines;
        while (count != first_line - 1) {
            temp_line = temp_line->next_line;
            count++;
        }
        fgets(new_line, NEW_LINE, stdin);
        strtok(new_line, "\n");
        while (strcmp(new_line, ".") != 0) {
            temp_line->next_line = malloc(sizeof(line));
            temp_line->next_line->next_line = NULL;
            temp_line->next_line->line = malloc(sizeof(char) * strlen(new_line) + 1);
            strcpy(temp_line->next_line->line, new_line);
            temp_line = temp_line->next_line;
            fgets(new_line, NEW_LINE, stdin);
            strtok(new_line, "\n");
        }
    }
    free(new_line);
}
void delete (int first_line, int second_line) {
    if (first_line == 0 && second_line == 0) {
        return;
    }
    else {

    }
}

void undo(int line) {

}

void redo(int line) {

}

char split_command(char* str, int *num1, int *num2){
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
            int a, b;
            char c = split_command(s, &a, &b);
            //printf("Command: %c on lines from %d to %d\n", c, a, b);
            if (c == 'c') {
                change(a, b);
            }
            if (c == 'd') {
                delete(a, b);
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
        }
    }
}

