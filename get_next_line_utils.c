
#include "get_next_line.h"

char *check_new_line(char *buffer, size_t size)
{
  size_t i;

  i = 0;
  while (i < size)
  {
    if (buffer[i] == '\n')
      return (&buffer[i]);
    i++;
  }
  return (NULL);
}

void    *ft_memcpy(void *dest, const void *src, size_t n)
{
  size_t          i;
  char            *destholder;
  char            *srcholder;

  if (dest == NULL && src == NULL)
    return (NULL);
  i = 0;
  destholder = (char *) dest;
  srcholder = (char *) src;
  while (i < n)
  {
    destholder[i] = srcholder[i];
    i++;
  }
  return (dest);
}

// only the dst that need to be freed
char *ft_realloc(char *dst, size_t dstsize, char *src, size_t srcsize)
{
  char *new;

  new = malloc(sizeof(char) * (dstsize + srcsize + 1));
  if (new == NULL)
  {
    if (dst)
      free(dst);
    return (NULL);
  }
  ft_memcpy(new, dst, dstsize);
  if (dst)
    free(dst);
  ft_memcpy(&new[dstsize], src, srcsize);
  new[dstsize + srcsize] = '\0';
  return (new);
}

t_state *init_state()
{
  t_state *state;

  state = malloc(sizeof(t_state));
  if (state == NULL)
    return (NULL);
  state->remained = malloc(sizeof(char) * BUFFER_SIZE);
  if (state->remained == NULL)
  {
    free(state);
    return (NULL);
  }
  state->remained_size = 0;
  return (state);
}
