#include "remember.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>

void rem_init(struct Remember *rem) {
    rem->pos = 0;
    rem->buffer[0] = 0;
    rem->match_buffer[0] = 0;
    rem->list.next = NULL;
    rem->list.buffer = NULL;
    rem->count = 1;
    rem->word_count = 0;
}

void ListNode_free(struct ListNode *node) {
    if(node->buffer != NULL) {
        free(node->buffer);
    }
    if(node->next != NULL)
        ListNode_free(node->next);

    free(node);
}

void rem_free(struct Remember *rem) {
    if(rem->list.buffer != NULL)
    free(rem->list.buffer); 
    if(rem->list.next != NULL)
    ListNode_free(rem->list.next);
}

void rem_addchar(struct Remember *rem, int code) {
    rem->buffer[rem->pos++] = code;
    rem->buffer[rem->pos] = 0;
}

void rem_delchar(struct Remember *rem) {
    if(rem->pos > 1) {
        rem->buffer[--rem->pos] = 0;
    } else {
        rem->buffer[0] = 0;
    }
}

void rem_printlist(struct Remember *rem) {
    struct ListNode *node = &rem->list;
    while(node != NULL) {
        if(node->buffer != 0)
            printf("%s\n", node->buffer);
        node = node->next;
    }
}

void rem_buildlist(struct Remember *rem, const char *list_file) {
    FILE *fptr = fopen(list_file, "r");
    if(!fptr) {
        fprintf(stderr, "Error opening file: %s\n", list_file);
        exit(EXIT_FAILURE);
    }
    struct ListNode *node = &rem->list;
    while(!feof(fptr)) {
        char buffer[1024];
        if(fgets(buffer, 1024, fptr) != NULL) {
            buffer[strlen(buffer)-1] = 0;
            node->buffer = (char*)malloc(sizeof(char)*strlen(buffer)+1);
            strncpy(node->buffer, buffer, strlen(buffer));
            node->buffer[strlen(buffer)] = 0;
            //node->buffer = strdup(buffer);
            node->next = (struct ListNode*) calloc(1, sizeof(struct ListNode));
            node->next->next = 0;
            node = node->next;
            rem->word_count++;
        }
    }
    fclose(fptr);
}

char *gen_word(struct Remember *rem, int index) {
    struct ListNode *node = &rem->list;
    int i = 0;
    while(node != 0 && i < index) {
        node = node->next;
        ++i;
    }
    return node->buffer;
}

void rem_genwords(struct Remember *rem) {
    for(int i = 0; i < rem->count; ++i) {
        int random = rand()%(rem->word_count-1);
       if(i > 0)
            sprintf(rem->match_buffer, "%s %s", rem->match_buffer, gen_word(rem, random));
        else
            sprintf(rem->match_buffer, "%s",gen_word(rem, random));
    }
    rem->count++;
}

int rem_compare(struct Remember *rem) {
    if(strcmp(rem->buffer, rem->match_buffer) == 0) {
        return 1;
    }
    return 0;
}

void rem_restart(struct Remember *rem) {
    rem->pos = 0;
    rem->buffer[0] = 0;
    rem->match_buffer[0] = 0;
}

void rem_printf(struct Remember *rem, const char *format, ...) {
    char buffer[4096*3];
    va_list args;
    va_start(args, format);
    vsprintf(buffer,format, args);
    perror(buffer);
    va_end(args);
    sprintf(rem->buffer, "%s%s", rem->buffer, buffer);   
}
