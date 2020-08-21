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
    text_editor *temp = edU;
    int count = 1;
    while(temp->next != NULL) {
        temp = temp->next;
    }
    line *temp_line = temp->lines;
    while (first_line != count) {
        temp_line= temp_line->next_line;
        count++;
    }
    while(count != second_line + 1) {
        fputs(temp_line->line, stdout);
        fputs("\n", stdout);
        temp_line = temp_line->next_line;
        count++;
    }
}


void change (int first_line, int second_line) {
    char *new_line = malloc(NEW_LINE);
    int count = 1;
    if(edU->next == NULL) {
        edU->version = 1;
        if(first_line == 1) {
            fgets(new_line, NEW_LINE, stdin);
            strtok(new_line, "\n");
            edU->lines->line = realloc(edU->lines->line, strlen(new_line) + 1);
            strcpy(edU->lines->line, new_line);
            count++;
        }
        line *temp_line = edU->lines;
        while (second_line + 1 != count) {
            fgets(new_line, NEW_LINE, stdin);
            strtok(new_line, "\n");
            temp_line->next_line = realloc(temp_line->next_line, sizeof(line));
            temp_line->next_line->line = realloc(temp_line->next_line->line, strlen(new_line) + 1);
            strcpy(temp_line->next_line->line, new_line);
            temp_line = temp_line->next_line;
            count++;
        }
    }
    else {
        text_editor *temp = edU;
        while (temp->next != NULL) {
            temp = temp->next;
            count++;
        }
        temp->next = malloc(sizeof(text_editor));
        temp->next->version = temp->version + 1;
        line *temp_line = temp->next->lines;
        while (count != first_line) {
            temp_line = temp_line->next_line;
            count++;
        }
        while (second_line + 1 != count) {
            fgets(new_line, NEW_LINE, stdin);
            strtok(new_line, "\n");
            temp_line->line = realloc(temp->lines->line, strlen(new_line) + 1);
            strcpy(temp_line->line, new_line);
            temp_line = temp_line->next_line;
            count++;
        }
    }
}
void delete (int first_line, int second_line) {

}

void undo(int line) {

}

void redo(int line) {

}

char split_command(char* str, int* num1, int* num2){
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
    char command = *(c-1);
    *(c-1) = '\0';
    *num2 = atoi(str+comma+1);
    return command;
}


int main() {
    edU = malloc(sizeof(text_editor));
    edU->next = NULL;
    edU->lines = malloc(sizeof(line));
    edU->lines->next_line = NULL;
    char *inputBuffer = malloc(INPUT_BUFFER_SIZE);
    while (1) {
        if (fgets(inputBuffer, INPUT_BUFFER_SIZE, stdin)) {
            strtok(inputBuffer, "\n");
            if (inputBuffer[0] == 'q') {
                return 0;
            }
            char *s = malloc(strlen(inputBuffer) + 1);
            strcpy(s, inputBuffer);
            int a, b;
            char c = split_command(s, &a, &b);
            if (c == 'q') {
                return 0;
            }
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
    //printf("Command: %c on lines from %d to %d\n", c, a, b);
    return 0;
}

