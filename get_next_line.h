

#ifndef GET_NEXT_LINE_H
#define GET_NEXT_LINE_H

#include <stdlib.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

typedef struct list {
  char *content;
  struct list *next;
} t_list;

typedef struct state {
  char *remained;
  size_t remained_size;
} t_state;

typedef struct line {
  char *line;
  size_t size;
} t_line;

char *get_next_line(int fd);


// utils
char *check_new_line(char *buffer, size_t size);
void    *ft_memcpy(void *dest, const void *src, size_t n);
char *ft_realloc(char *dst, size_t dstsize, char *src, size_t srcsize);
t_state *init_state();

#endif
