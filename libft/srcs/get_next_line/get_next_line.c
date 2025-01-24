/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 18:01:01 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/22 16:52:26 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/get_next_line.h"

static char	*extract_line(char *buffer)
{
	size_t	i;
	char	*line;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 1 + (buffer[i] == '\n')));
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
	{
		line[i] = '\n';
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*save_leftover(char *buffer)
{
	size_t	i;
	size_t	j;
	char	*leftover;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	leftover = malloc(sizeof(char) * (ft_strlen(buffer) - i));
	if (!leftover)
	{
		free(buffer);
		return (NULL);
	}
	i++;
	j = 0;
	while (buffer[i])
		leftover[j++] = buffer[i++];
	leftover[j] = '\0';
	free(buffer);
	return (leftover);
}

static int	fill_buffer(int fd, char **static_buffer)
{
	char	buffer[BUFFER_SIZE + 1];
	ssize_t	bytes_read;
	char	*temp;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		temp = *static_buffer;
		*static_buffer = ft_strjoin(temp, buffer);
		free(temp);
		if (!*static_buffer)
			return (-1);
		if (ft_strchr(*static_buffer, '\n'))
			break ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	return (bytes_read);
}

static void	free_buffer(char **static_buffer)
{
	if (*static_buffer)
	{
		free(*static_buffer);
		*static_buffer = NULL;
	}
}

char	*get_next_line(int fd)
{
	static char	*static_buffer = NULL;
	char		*line;
	int			bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX)
		return (NULL);
	bytes_read = fill_buffer(fd, &static_buffer);
	if (bytes_read == -1 || (!static_buffer && bytes_read == 0))
	{
		free_buffer(&static_buffer);
		return (NULL);
	}
	if (ft_strlen(static_buffer) == 0)
	{
		free_buffer(&static_buffer);
		return (NULL);
	}
	line = extract_line(static_buffer);
	if (!line)
	{
		free_buffer(&static_buffer);
		return (NULL);
	}
	static_buffer = save_leftover(static_buffer);
	return (line);
}

/* #include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int fd;
	char *line;

	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			perror("Erreur lors de l'ouverture du fichier");
			return (1);
		}
	}
	else
	{
		fd = 0;
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("Ligne lue : %s\n", line);
		free(line);
	}

	if (argc == 2)
		close(fd);

	return (0);
} */