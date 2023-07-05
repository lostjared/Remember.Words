/*
 
 Remember Words, memory game
 
 I used to play this game with my speech therapist after being in a coma
 and having to learn how to speak again. Helps with memory recall
 so I decided to code it into a computer program.
 
 Hope this can help anyone
 
 - Jared
 
*/
#include "remember.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>

/* initalize Rmember structure */
void rem_init(struct Remember *rem) {
    rem->pos = 0;
    rem->buffer[0] = 0;
    rem->match_buffer[0] = 0;
    rem->list.next = NULL;
    rem->list.buffer = NULL;
    rem->count = 1;
    rem->word_count = 0;
}

/* free linked list node */
void ListNode_free(struct ListNode *node) {
    if(node->buffer != NULL) {
        free(node->buffer);
    }
    if(node->next != NULL)
        ListNode_free(node->next);
    
    free(node);
}

/* free Remember structure */
void rem_free(struct Remember *rem) {
    if(rem->list.buffer != NULL)
        free(rem->list.buffer); 
    if(rem->list.next != NULL)
        ListNode_free(rem->list.next);
}

/* add cahracter to buffer */
void rem_addchar(struct Remember *rem, int code) {
    rem->buffer[rem->pos++] = code;
    rem->buffer[rem->pos] = 0;
}

/* delete a character from the buffer */
void rem_delchar(struct Remember *rem) {
    if(rem->pos > 1) {
        rem->buffer[--rem->pos] = 0;
    } else {
        rem->buffer[0] = 0;
    }
}

/* print the list to stdout */
void rem_printlist(struct Remember *rem) {
    struct ListNode *node = &rem->list;
    while(node != NULL) {
        if(node->buffer != 0)
            printf("%s\n", node->buffer);
        node = node->next;
    }
}
/* build the list from a text file one word per line */
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
            node->next = (struct ListNode*) calloc(1, sizeof(struct ListNode));
            node->next->next = 0;
            node = node->next;
            rem->word_count++;
        }
    }
    fclose(fptr);
}

/* gen a word from the linked list */
char *gen_word(struct Remember *rem, int index) {
    struct ListNode *node = &rem->list;
    int i = 0;
    while(node != 0 && i < index) {
        node = node->next;
        ++i;
    }
    return node->buffer;
}
/* gen words for comparaision */
void rem_genwords(struct Remember *rem) {
    for(int i = 0; i < rem->count; ++i) {
        int random = rand()%(rem->word_count-1);
        if(i > 0) {
            strncat(rem->match_buffer, " ", BUFFER_MAX-1);
            strncat(rem->match_buffer, gen_word(rem, random), BUFFER_MAX-1);
        }
        else
            snprintf(rem->match_buffer, BUFFER_MAX-1, "%s",gen_word(rem, random));
    }
    rem->count++;
}
/* compare words */
int rem_compare(struct Remember *rem) {
    if(strcmp(rem->buffer, rem->match_buffer) == 0) {
        return 1;
    }
    return 0;
}
/* restart */
void rem_restart(struct Remember *rem) {
    rem->pos = 0;
    rem->buffer[0] = 0;
    rem->match_buffer[0] = 0;
}
/* varaiable argument printf to on screen buffer */
void rem_printf(struct Remember *rem, const char *format, ...) {
    char buffer[BUFFER_MAX];
    va_list args;
    va_start(args, format);
    vsprintf(buffer,format, args);
    perror(buffer);
    va_end(args);
    snprintf(rem->buffer, BUFFER_MAX, "%s%s", rem->buffer, buffer);
}
