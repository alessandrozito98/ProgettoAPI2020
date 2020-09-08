#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define NEW_LINE 1026
#define INPUT_BUFFER_SIZE 100

typedef struct text {
    long long first_line;
    long long second_line;
    long long array_size;
    bool boolean;
    char command;
    char **lines;
    struct text *next;
    struct text *prec;
}text_editor;


long long array_size = 0;
long long max_undo = 0;
long long max_redo = 0;
char **lines = NULL;
bool insert;


void free_all(text_editor *list, text_editor *head) {

    if (list == NULL) {
        while (head->next != NULL) {
            head = (head)->next;
            free(head->prec);
        }
        free(head);
        head = NULL;
    } else {
        text_editor *delete;
        delete = list->next;
    while (delete->next != NULL) {
        delete = delete->next;
        free(delete->prec);
    }
    free(delete);
        list->next = NULL;
    }

}


void insert_at_end(text_editor **headref, text_editor **lastref, text_editor **actual_state, char c, long long a, long long b) {
    long long i, j = 0;
    text_editor *temp = malloc(sizeof(text_editor));
    temp->command = c;
    temp->first_line = a;
    temp->second_line = b;
    temp->array_size = array_size;
    temp->next = NULL;
    if(c == 'd' || insert == true) {
        temp->boolean = true;
        temp->lines = malloc(sizeof(char *) * array_size);
        for (i = 0; i < array_size; i++) {
            temp->lines[i] = lines[i];
        }
    }

    else {
        temp->boolean = false;
        temp->lines = malloc(sizeof(char *) * (b - a + 1));
        for (i = a - 1; i < b; i++) {
            temp->lines[j] = lines[i];
            j++;
        }
    }
    if(*headref == NULL) { //Checks if the list is empty.
        temp->prec = NULL;
        *headref = temp; // Places the address of the new node in HEAD pointer.
        *lastref = temp; // Places the address of the new node in LAST pointer.
    }
    else {
        /* If the list is not empty, then make the next pointer of the present last node point to the new node*/
        (*lastref)->next = temp;
        temp->prec = *lastref;
        *lastref = (*lastref)->next;// Increment LAST to point to the new last node.
    }
    *actual_state = *lastref;
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


void change (long long first_line, long long second_line, text_editor **head, text_editor *actual_state, text_editor **lastref) {
    max_undo = max_undo + 1;
    if(max_redo != 0) {
        free_all(actual_state, *head);
        if(actual_state == NULL) {
            *head = actual_state;
        }
        *lastref = actual_state;
    }
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


void delete(long long first_line, long long second_line, text_editor **head, text_editor *actual_state, text_editor **lastref) {
    max_undo = max_undo + 1;
    if (max_redo != 0) {
        free_all(actual_state, *head);
        if(actual_state == NULL) {
            *head = NULL;
        }
        *lastref = actual_state;
    }
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

void undo(long long a, text_editor **actual_state, text_editor *head) {
    text_editor *curr = *actual_state;

    long long count = 0;

    while (count != a) {
        curr = curr->prec;
        count++;
    }

    if(curr == NULL) {
        for(int i = 0; i < array_size; i++) {
            lines[i] = NULL;
        }
    }
    else {
        if(curr->command == 'd' || curr->boolean == true) {
            lines = malloc(sizeof(char *) * curr->array_size);
            for (int i = 0; i < curr->array_size; i++) {
                lines[i] = curr->lines[i];
            }
            array_size = curr->array_size;
        } else {
            while(curr != NULL && curr->command != 'd' && curr->boolean == false) {
                curr = curr->prec;
                count++;
            }
            if(curr == NULL) {
                array_size = 0;
                curr = head;
                for(int i = 0; i < array_size; i++) {
                    lines[i] = NULL;
                }
                lines = NULL;
            }
            else {
                for (int i = 0; i < curr->array_size; i++) {
                    lines[i] = curr->lines[i];
                }
                array_size = curr->array_size;
            }
            while (count != a) {
                change_after_undo(curr->first_line, curr->second_line, curr->lines);
                count--;

            }
        }
    }
    *actual_state = curr;
    max_undo = max_undo - a;
    max_redo = max_redo - a;
}

void redo(long long num_redo, text_editor **actual_state, text_editor *head) {
    text_editor *curr = *actual_state;

    max_redo = max_redo + num_redo;
    max_undo = max_undo + num_redo;

    if (curr == NULL) {
        curr = head;
        lines = malloc(sizeof(char *)* curr->array_size);
        for (int i = 0; i < curr->array_size; ++i) {
            lines[i] = curr->lines[i];
        }
        array_size = curr->array_size;
        num_redo--;
    }

    while(num_redo) {
        curr = curr->next;
        if(curr->command == 'd' || curr->boolean == true) {
            lines = realloc(lines, sizeof(char *) * curr->array_size);
            for (int i = 0; i < curr->array_size; ++i) {
                lines[i] = curr->lines[i];
            }
            array_size = curr->array_size;
        } else {
            change_after_undo(curr->first_line, curr->second_line, curr->lines);
        }

        num_redo--;
    }
    *actual_state = curr;
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
    text_editor *undo_head = NULL, *undo_tail = NULL, *actual_state;
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
                    undo(x, &actual_state, undo_head);
                } else if (x < 0) {
                    x = -x;
                    redo(x, &actual_state, undo_head);
                }
                x = 0;
            }

            if (c == 'q') {
                return 0;
            }
            if (c == 'c') {
                change(a, b, &undo_head, actual_state, &undo_tail);
                insert_at_end(&undo_head, &undo_tail, &actual_state, c, a, b);
                actual_state = undo_tail;
            }
            if (c == 'd') {
                delete(a, b, &undo_head, actual_state, &undo_tail);
                insert_at_end(&undo_head, &undo_tail, &actual_state, c, a, b);
                actual_state = undo_tail;
            }
            if (c == 'p') {
                print(a, b);
            }
        }
    }
}

