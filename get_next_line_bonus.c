/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayait-el <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 11:05:59 by ayait-el          #+#    #+#             */
/*   Updated: 2023/11/22 20:36:02 by ayait-el         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static int	check_remained(t_state *state, char *buffer, t_line *line)
{
	char	*tmp;

	if (state->remained_size)
	{
		tmp = check_new_line(state->remained, state->remained_size);
		if (tmp)
		{
			line->line = ft_realloc(NULL, 0, state->remained, tmp
					- state->remained + 1);
			if (line->line == NULL)
			{
				ft_free(&state, buffer, line->line, STATE | BUFFER);
				return (1);
			}
			ft_memcpy(state->remained, tmp + 1, state->remained_size - (tmp
					- state->remained + 1));
			state->remained_size -= (tmp - state->remained + 1);
			ft_free(&state, buffer, line->line, BUFFER);
			return (1);
		}
		line->line = ft_realloc(NULL, 0, state->remained, state->remained_size);
		line->size = state->remained_size;
		state->remained_size = 0;
	}
	return (0);
}

static int	read_line(t_state *state, char *buffer, t_line *line, int fd)
{
	char	*tmp;
	int		readed;

	readed = read(fd, buffer, BUFFER_SIZE);
	while (readed > 0)
	{
		tmp = check_new_line(buffer, readed);
		if (tmp)
		{
			line->line = ft_realloc(line->line, line->size, buffer, tmp - buffer
					+ 1);
			if (line->line == NULL)
			{
				ft_free(&state, buffer, line->line, STATE | BUFFER);
				return (readed);
			}
			ft_memcpy(state->remained, tmp + 1, readed - (tmp - buffer + 1));
			state->remained_size = readed - (tmp - buffer + 1);
			ft_free(&state, buffer, line->line, BUFFER);
			return (readed);
		}
		line->line = ft_realloc(line->line, line->size, buffer, readed);
		if (line->line == NULL)
		{
			ft_free(&state, buffer, line->line, STATE | BUFFER);
			return (readed);
		}
		line->size += readed;
    readed = read(fd, buffer, BUFFER_SIZE);
	}
	return (readed);
}

char	*get_next_line(int fd)
{
	static t_state	*state_arr[OPEN_MAX] = {0};
	char			*buffer;
	int				readed;
	t_line			line;

	if (fd < 0 || fd > OPEN_MAX)
		return (NULL);
	if (!init_state(&state_arr[fd], &line, &buffer))
		return (NULL);
	if (check_remained(state_arr[fd], buffer, &line))
		return (line.line);
	readed = read_line(state_arr[fd], buffer, &line, fd);
	if (readed > 0)
		return (line.line);
	ft_free(&state_arr[fd], buffer, line.line, STATE | BUFFER);
	if (line.line && readed != -1)
		return (line.line);
	ft_free(&state_arr[fd], buffer, line.line, LINE);
	return (NULL);
}

/*
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	char	*name;
	int		fd;
	char	*temp;

  //int fd = open("1char.txt", O_RDONLY);
  //close(fd);
//  char *line;
//
//  while ((line = get_next_line(100)))
//  {
//    printf("%s", line);
//    free(line);
//  }
//  printf("%s\n", line);
  name = "read_error.txt";
  fd = open(name, O_RDONLY);
  printf("expeced: aaaaaaaaaa\ngot: %s", get_next_line(fd));
  printf("expeced: bbbbbbbbbb\ngot: %s", get_next_line(fd));
  // set the next read call to return -1
  //next_read_error = 1;
  if (BUFFER_SIZE >= 100) {
	do {
		temp = get_next_line(fd);
		free(temp);
	} while (temp != NULL);
  }
  printf("expeced null, got %s\n", get_next_line(fd));
}
*/
