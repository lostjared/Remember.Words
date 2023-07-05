/*
 
 Remember Words, memory game
 
 I used to play this game with my speech therapist after being in a coma
 and having to learn how to speak again. Helps with memory recall
 so I decided to code it into a computer program.
 
 Hope this can help anyone
 
 - Jared
 
*/

#ifndef __REMEMBER_H__
#define __REMEMBER_H__
#define BUFFER_MAX 10000

/* linked list structure to contain each word */
struct ListNode {
    char *buffer;
    struct ListNode *next;
};

/* remember structure to contain typed data and the built word to match against */
struct Remember {
    char buffer[BUFFER_MAX];
    char match_buffer[BUFFER_MAX];
    int pos;
    struct ListNode list;
    int count;
    int word_count;
};

/*
 functions to manipulate the Remember structure
 initalize, free, add a character, delete a char, build the list
 print the list, gen words, compare, start over, and print
 */
extern void rem_init(struct Remember *rem);
extern void rem_free(struct Remember *rem);
extern void rem_addchar(struct Remember *rem, int code);
extern void rem_delchar(struct Remember *rem);
extern void rem_buildlist(struct Remember *rem, const char *list_file);
extern void rem_printlist(struct Remember *rem);
extern void rem_genwords(struct Remember *rem);
extern int rem_compare(struct Remember *rem);
extern void rem_restart(struct Remember *rem);
extern void rem_printf(struct Remember *rem, const char *format, ...);
#endif
