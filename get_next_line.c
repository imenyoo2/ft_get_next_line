/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayait-el <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 11:05:59 by ayait-el          #+#    #+#             */
/*   Updated: 2023/11/23 19:55:14 by ayait-el         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int alloc_new_line(t_state **state, t_line *line, char *buffer)
{
  line->line = ft_realloc(NULL, 0, (*state)->remained, (*state)->remained_size);
  if (line->line == NULL)
  {
    ft_free(state, buffer, line->line, BUFFER | STATE);
    return (1);
  }
  line->size = (*state)->remained_size;
  return (0);
}

static int	check_remained(t_state **state, char *buffer, t_line *line)
{
	char	*tmp;

	if ((*state)->remained_size)
	{
		tmp = check_new_line((*state)->remained, (*state)->remained_size);
		if (tmp)
		{
			line->line = ft_realloc(NULL, 0, (*state)->remained, tmp
					- (*state)->remained + 1);
			if (line->line == NULL)
			{
				ft_free(state, buffer, line->line, STATE | BUFFER);
				return (1);
			}
			ft_memcpy((*state)->remained, tmp + 1, (*state)->remained_size - (tmp
					- (*state)->remained + 1));
			(*state)->remained_size -= (tmp - (*state)->remained + 1);
			ft_free(state, buffer, line->line, BUFFER);
			return (1);
		}
    if (alloc_new_line(state, line, buffer))
      return (1);
		(*state)->remained_size = 0;
	}
	return (0);
}

static int	realloc_line(t_state **state, t_line *line, int readed, char *buffer)
{
	line->line = ft_realloc(line->line, line->size, buffer, readed);
	if (line->line == NULL)
	{
		ft_free(state, buffer, line->line, STATE | BUFFER);
		return (readed);
	}
	line->size += readed;
	return (0);
}

static int	read_line(t_state **state, char *buffer, t_line *line, int fd)
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
				ft_free(state, buffer, line->line, STATE | BUFFER);
				return (readed);
			}
			ft_memcpy((*state)->remained, tmp + 1, readed - (tmp - buffer + 1));
			(*state)->remained_size = readed - (tmp - buffer + 1);
			ft_free(state, buffer, line->line, BUFFER);
			return (readed);
		}
		if (realloc_line(state, line, readed, buffer))
			return (readed);
		readed = read(fd, buffer, BUFFER_SIZE);
	}
	return (readed);
}

char	*get_next_line(int fd)
{
	static t_state	*state = NULL;
	char			*buffer;
	int				readed;
	t_line			line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!init_state(&state, &line, &buffer))
		return (NULL);
	if (check_remained(&state, buffer, &line))
		return (line.line);
	readed = read_line(&state, buffer, &line, fd);
	if (readed > 0)
		return (line.line);
	ft_free(&state, buffer, line.line, STATE | BUFFER);
	if (line.line && readed != -1)
		return (line.line);
	if (line.line)
		ft_free(&state, buffer, line.line, LINE);
	return (NULL);
}

/*
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int fd = open("variable_nls.txt", O_RDONLY);
	printf("expected \n, got %s\n", get_next_line(fd));
	printf("expected: 0123456789012345678901234567890123456789x2\n, got: %s", get_next_line(fd));
	printf("expected: 0123456789012345678901234567890123456789x3\n, got %s", get_next_line(fd));
	printf("expected: \n, got %s\n", get_next_line(fd));
	printf("expected: 0123456789012345678901234567890123456789x5\n, got: %s", get_next_line(fd));
	printf("expected \n, got %s\n", get_next_line(fd));
	printf("expected \n, got %s\n", get_next_line(fd));
	printf("expected: 0123456789012345678901234567890123456789x8\n, got: %s", get_next_line(fd));
	printf("expected: \n, got %s\n", get_next_line(fd));
	printf("expected: \n, got %s\n", get_next_line(fd));
	printf("expected: \n, got %s\n", get_next_line(fd));
	printf("expected: 0123456789012345678901234567890123456789x12, got: %s\n", get_next_line(fd));
	printf("expected: NULL, got %s\n", get_next_line(fd));
}
*/
