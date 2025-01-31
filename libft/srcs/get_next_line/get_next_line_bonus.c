/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:16:19 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/27 10:12:32 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/get_next_line_bonus.h"

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
		line[i++] = '\n';
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
	leftover = malloc(sizeof(char) * (gnl_strlen(buffer) - i));
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
		*static_buffer = gnl_strjoin(temp, buffer);
		free(temp);
		if (!*static_buffer)
			return (-1);
		if (gnl_strchr(*static_buffer, '\n'))
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
	static char	*static_buffer[FD_MAX];
	char		*line;
	int			bytes_read;

	if (fd < 0 || fd >= FD_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	bytes_read = fill_buffer(fd, &static_buffer[fd]);
	if (bytes_read == -1 || (!static_buffer[fd] && bytes_read == 0))
	{
		free_buffer(&static_buffer[fd]);
		return (NULL);
	}
	if (gnl_strlen(static_buffer[fd]) == 0)
	{
		free_buffer(&static_buffer[fd]);
		return (NULL);
	}
	line = extract_line(static_buffer[fd]);
	if (!line)
	{
		free_buffer(&static_buffer[fd]);
		return (NULL);
	}
	static_buffer[fd] = save_leftover(static_buffer[fd]);
	return (line);
}

/*
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int fd1, fd2, fd3;
    char *line;

    fd1 = open("file1.txt", O_RDONLY);
    fd2 = open("file2.txt", O_RDONLY);
    fd3 = open("file3.txt", O_RDONLY);

    if (fd1 == -1 || fd2 == -1 || fd3 == -1)
    {
        perror("Erreur lors de l'ouverture des fichiers");
        if (fd1 != -1) close(fd1);
        if (fd2 != -1) close(fd2);
        if (fd3 != -1) close(fd3);
        return (1);
    }
    printf("Contenu de File 1:\n");
    while ((line = get_next_line(fd1)) != NULL)
    {
        printf("%s\n", line);
        free(line);
    }
    close(fd1);
	
    printf("\nContenu de File 2:\n");
    while ((line = get_next_line(fd2)) != NULL)
    {
        printf("%s\n", line);
        free(line);
    }
    close(fd2);

    printf("\nContenu de File 3:\n");
    while ((line = get_next_line(fd3)) != NULL)
    {
        printf("%s\n", line);
        free(line);
    }
    close(fd3);
	
    return (0);
}
*/