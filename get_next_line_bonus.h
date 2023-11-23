/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayait-el <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 17:41:57 by ayait-el          #+#    #+#             */
/*   Updated: 2023/11/22 17:42:17 by ayait-el         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <limits.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef enum mode
{
	STATE = 1 << 0,
	BUFFER = 1 << 1,
	LINE = 1 << 2
}				t_mode;

typedef struct list
{
	char		*content;
	struct list	*next;
}				t_list;

typedef struct state
{
	char		*remained;
	size_t		remained_size;
}				t_state;

typedef struct line
{
	char		*line;
	size_t		size;
}				t_line;

char			*get_next_line(int fd);

// utils
char			*check_new_line(char *buffer, size_t size);
void			*ft_memcpy(void *dest, const void *src, size_t n);
char			*ft_realloc(char *dst, size_t dstsize, char *src,
					size_t srcsize);
int				init_state(t_state **state, t_line *line, char **buffer);
void			ft_free(t_state **state, char *buffer, char *line, int mode);

#endif
