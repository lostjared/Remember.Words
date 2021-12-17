#ifndef __REMEMBER_H__
#define __REMEMBER_H__

struct ListNode {
    char *buffer;
    struct ListNode *next;
};

struct Remember {
    char buffer[4096];
    char match_buffer[4096];
    int pos;
    struct ListNode list;
    int count;
    int word_count;
};

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