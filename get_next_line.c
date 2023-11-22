/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayait-el <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 11:05:59 by ayait-el          #+#    #+#             */
/*   Updated: 2023/11/22 10:30:53 by ayait-el         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "get_next_line.h"

char *get_next_line(int fd)
{
  static t_state *state = NULL;
  char *buffer;
  int readed;
  char *tmp;
  t_line line;

  if (fd < 0)
    return (NULL);
  if (state == NULL)
  {
    if (!(state = init_state()))
      return (NULL);
  }
  line.line = NULL;
  line.size = 0;
  if (!(buffer = malloc(BUFFER_SIZE * sizeof(char))))
  {
    free(state->remained);
    free(state);
    state = NULL;
    return (NULL);
  }
  if (state->remained_size)
  {
    if ((tmp = check_new_line(state->remained, state->remained_size)))
    {
      line.line = ft_realloc(NULL, 0, state->remained, tmp - state->remained + 1);
      if (line.line == NULL)
      {
        free(state->remained);
        free(state);
        state = NULL;
        free(buffer);
        return (NULL);
      }
      ft_memcpy(state->remained, tmp + 1, state->remained_size - (tmp - state->remained + 1));
      state->remained_size -= (tmp - state->remained + 1);
      free(buffer);
      return (line.line);
    }
    line.line = ft_realloc(NULL, 0, state->remained, state->remained_size);
    line.size = state->remained_size;
    state->remained_size = 0;
  }
  while ((readed = read(fd, buffer, BUFFER_SIZE)) > 0)
  {
    if ((tmp = check_new_line(buffer, readed)))
    {
      line.line = ft_realloc(line.line, line.size, buffer, tmp - buffer  + 1);
      if (line.line == NULL)
      {
        free(state->remained);
        free(state);
        state = NULL;
        free(buffer);
        return (NULL);
      }
      ft_memcpy(state->remained, tmp + 1, readed - (tmp - buffer + 1));
      state->remained_size = readed - (tmp - buffer + 1);
      free(buffer);
      return (line.line);
    }
    line.line = ft_realloc(line.line, line.size, buffer, readed);
    if (line.line == NULL)
    {
        free(state->remained);
        free(state);
        state = NULL;
        free(buffer);
        return (NULL);
    }
    line.size += readed;
  }
  free(buffer);
  free(state->remained);
  free(state);
  if (line.line)
    return (line.line);
  return (NULL);
}

/*
#include <fcntl.h>
#include <stdio.h>
int main(void)
{
  int fd = open("1char.txt", O_RDONLY);
  //close(fd);
  char *line;

  while ((line = get_next_line(fd)))
  {
    printf("%s", line);
    free(line);
  }
  printf("%s\n", line);
}
*/
