#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INPUT_BUFFER_SIZE 15
#define NEWLINE 1025

typedef struct line {
    char *line;
    struct line *next_line;
}line;

typedef struct text {
    int n_lines;
    struct line *lines;
    struct text *next;
}text;

text *text_editor;

void insert_line(char *new_line, text *temp) {
    temp->lines->line = malloc(strlen(new_line) + 1);
    strcpy(temp->lines->line, new_line);
}

void print(int first_line, int second_line) {
    text *temp = text_editor;
    int count = 1;
    while (first_line != count) {
        temp->lines = temp->lines->next_line;
        count++;
    }
    while(count != second_line + 1) {
        fputs(temp->lines->line, stdout);
        temp->lines = temp->lines->next_line;
        count++;
    }
}


void change (int first_line, int second_line) {
    char *new_line = NULL;
    text *temp = text_editor;
    int count = 1;
    while (count != first_line) {
        temp = temp->next;
        count++;
    }
    while (second_line != count + 1) {
        fgets(new_line, NEWLINE, stdin);
        insert_line(new_line, temp);
        temp->lines = temp->lines->next_line;
        count++;
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
    text_editor = malloc(sizeof(text));
    text_editor->lines = malloc(sizeof(line));
    char *inputBuffer = malloc(INPUT_BUFFER_SIZE);
    while (1) {
        if (fgets(inputBuffer, INPUT_BUFFER_SIZE, stdin)) {
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
                //change(a, b);
            }
            if (c == 'd') {
                //delete(a, b);
            }
            if (c == 'u') {
                undo(a);
            }
            if (c == 'r') {
                redo(a);
            }
        }
    }
    //printf("Command: %c on lines from %d to %d\n", c, a, b);
    return 0;
}

