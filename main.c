#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFFER_SIZE 1026

typedef struct text {
    long long first_line;
    long long second_line;
    char command;
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


void change (text_editor **head, text_editor **tail, long long first_line, long long second_line) {
    char *new_line = malloc(sizeof(char) * INPUT_BUFFER_SIZE);
    text_editor *new_node = malloc(sizeof(text_editor));
    new_node->first_line = first_line;
    new_node->second_line = second_line;
    new_node->command = 'c';
    new_node->lines = malloc(sizeof(char *) * second_line);
    long long i = 0;
    if (lines == NULL) {
        lines = malloc(sizeof(char *) * second_line);
        fgets(new_line, INPUT_BUFFER_SIZE, stdin);
        strtok(new_line, "\n");
        array_size = second_line;
        while (strcmp(new_line, ".") != 0) {
            lines[i] = malloc(sizeof(char) * strlen(new_line) + 1);
            strcpy(lines[i], new_line);
            new_node->lines[i] = lines[i];
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
        long long number_of_lines = second_line - first_line + 1;
        new_node->lines = malloc(sizeof(char *) * number_of_lines);
        fgets(new_line, INPUT_BUFFER_SIZE, stdin);
        strtok(new_line, "\n");
        if (array_size < second_line) {
            lines = realloc(lines, sizeof(char *) * second_line);
            array_size = second_line;
        }
        int j = 0;
        for (i = first_line - 1; strcmp(new_line, ".") != 0; i++) {
            lines[i] = malloc(sizeof(char) * strlen(new_line) + 1);
            strcpy(lines[i], new_line);
            new_node->lines[j] = lines[i];
            j++;
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
    free(new_node);
}


void delete(text_editor **head, text_editor **tail, long long first_line, long long second_line) {
    text_editor *new_node = malloc(sizeof(text_editor));
    new_node->second_line = second_line;
    new_node->first_line = first_line;
    new_node->command = 'd';
    if (lines == NULL || first_line == 0 && second_line == 0 || first_line > array_size && second_line >= array_size) {
        new_node->lines = malloc(sizeof(char *) * array_size);
        memcpy(new_node->lines, lines, sizeof(char *) * array_size);
        new_node->next = NULL;
        new_node = *tail;
        if (*head == NULL) {
            *head = new_node;
            *tail = *head;
        } else {
            (*tail)->next = new_node;
            new_node->prec = *tail;
            *tail = (*tail)->next; // Increment LAST to point to the new last node.
        }
    }
    else {
        long long i;
        if (second_line >= array_size) {
            if(first_line == 1 || first_line == 0) {
                array_size = 0;
            }
            else {
                array_size = first_line;
            }
        }
        else {
            if (first_line == second_line) {
                for (i = first_line - 1; i + 1 < array_size; i++) {
                    lines[i] = lines[i + 1];
                }
                array_size = array_size - 1;
            }
            else {
                for (i = first_line - 1; i + (second_line - first_line + 1) < array_size; i++) {
                    lines[i] = lines[i + (second_line - first_line + 1)];
                }

                array_size = array_size - (second_line - first_line + 1);
            }

        }
        new_node->lines = malloc(sizeof(char *) * array_size);
        memcpy(new_node->lines, lines, sizeof(char *) * array_size);
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
}

void undo(text_editor **tail, long long num_undo) {
    text_editor *temp = *tail;
    long long count = 0;
        while(temp->command != 'd') {
            temp = temp->prec;
            count++;
        }

}

void redo(text_editor **tail, long long num_redo) {

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
                free(edU);
                free(lines);
                free(tail);
                return 0;
            }
            long long a, b;
            if (inputBuffer[1] == 'u') {
                a = atoi(&inputBuffer[0]);
                undo (&tail, a);
            }
            else if (inputBuffer[1] == 'r') {
                a = atoi(&inputBuffer[0]);
                redo(&tail, a);
            }
            else {
                char *s = malloc(sizeof(char) * strlen(inputBuffer) + 1);
                strcpy(s, inputBuffer);
                char c = split_command(s, &a, &b);
                //printf("Command: %c on lines from %d to %d\n", c, a, b);
                if (c == 'c') {
                    change(&edU, &tail, a, b);
                }
                if (c == 'd') {
                    delete(&edU, &tail, a, b);
                }
                if (c == 'p') {
                    print(a, b);
                }
                free(s);
            }
        }
    }
}

