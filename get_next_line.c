/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayait-el <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 11:05:59 by ayait-el          #+#    #+#             */
/*   Updated: 2023/11/23 20:26:40 by ayait-el         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	alloc_new_line(t_state **state, t_line *line, char *buffer,
		int size)
{
	line->line = ft_realloc(NULL, 0, (*state)->remained, size);
	if (line->line == NULL)
	{
		ft_free(state, buffer, line->line, BUFFER | STATE);
		return (1);
	}
	line->size = size;
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
			if (alloc_new_line(state, line, buffer, tmp - (*state)->remained
					+ 1))
				return (1);
			ft_memcpy((*state)->remained, tmp + 1, (*state)->remained_size
				- (tmp - (*state)->remained + 1));
			(*state)->remained_size -= (tmp - (*state)->remained + 1);
			ft_free(state, buffer, line->line, BUFFER);
			return (1);
		}
		if (alloc_new_line(state, line, buffer, (*state)->remained_size))
			return (1);
		(*state)->remained_size = 0;
	}
	return (0);
}

static int	realloc_line(t_state **state, t_line *line, int readed,
		char *buffer)
{
	line->line = ft_realloc(line->line, line->size, buffer, readed);
	if (line->line == NULL)
	{
		ft_free(state, buffer, line->line, STATE | BUFFER);
		return (1);
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
			if (realloc_line(state, line, tmp - buffer + 1, buffer))
				return (readed);
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
