/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayait-el <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 11:47:35 by ayait-el          #+#    #+#             */
/*   Updated: 2023/11/23 15:25:04 by ayait-el         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*check_new_line(char *buffer, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		if (buffer[i] == '\n')
			return (&buffer[i]);
		i++;
	}
	return (NULL);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*destholder;
	char	*srcholder;

	if (dest == NULL && src == NULL)
		return (NULL);
	i = 0;
	destholder = (char *)dest;
	srcholder = (char *)src;
	while (i < n)
	{
		destholder[i] = srcholder[i];
		i++;
	}
	return (dest);
}

// only the dst that need to be freed
char	*ft_realloc(char *dst, size_t dstsize, char *src, size_t srcsize)
{
	char	*new;

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

int	init_state(t_state **state, t_line *line, char **buffer)
{
	if (*state == NULL)
	{
		*state = malloc(sizeof(t_state));
		if (*state == NULL)
			return (0);
		(*state)->remained = malloc(sizeof(char) * BUFFER_SIZE);
		if ((*state)->remained == NULL)
		{
			free(*state);
			*state = NULL;
			return (0);
		}
		(*state)->remained_size = 0;
	}
	line->line = NULL;
	line->size = 0;
	*buffer = malloc(BUFFER_SIZE * sizeof(char));
	if (!(*buffer))
	{
		ft_free(state, *buffer, line->line, STATE);
		return (0);
	}
	return (1);
}

// mode should only be in t_mode or the result of oring t_mode elements
void	ft_free(t_state **state, char *buffer, char *line, int mode)
{
	if (mode & STATE)
	{
		free((*state)->remained);
		(*state)->remained = NULL;
		free(*state);
		*state = NULL;
	}
	if (mode & BUFFER)
		free(buffer);
	if (mode & LINE)
		free(line);
}
