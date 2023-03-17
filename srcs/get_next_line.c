/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bzawko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:59:22 by bzawko            #+#    #+#             */
/*   Updated: 2023/02/14 11:59:38 by bzawko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_statjoin(char *stat_str, char *buffer)
{
	size_t	i;
	size_t	j;
	char	*ret;

	if (!stat_str)
	{
		stat_str = malloc(1);
		stat_str[0] = '\0';
	}
	if (!stat_str || !buffer)
		return (NULL);
	ret = malloc(ft_strlen(stat_str) + ft_strlen(buffer) + 1);
	if (!ret)
		return (NULL);
	i = -1;
	j = 0;
	while (stat_str[++i])
		ret[i] = stat_str[i];
	while (buffer[j])
		ret[i++] = buffer[j++];
	ret[ft_strlen(stat_str) + ft_strlen(buffer)] = '\0';
	free(stat_str);
	return (ret);
}

static char	*remove_start_stat(char *stat_str)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	j = 0;
	while (stat_str[i] && stat_str[i] != '\n')
		++i;
	if (!stat_str[i])
	{
		free(stat_str);
		return (NULL);
	}
	ret = malloc(ft_strlen(stat_str) - i);
	if (!ret)
		return (NULL);
	++i;
	while (stat_str[i])
		ret[j++] = stat_str[i++];
	ret[j] = '\0';
	free(stat_str);
	return (ret);
}

static char	*find_line(char *stat_str)
{
	int		i;
	char	*ret;

	i = 0;
	while (stat_str[i] && stat_str[i] != '\n')
		++i;
	if (stat_str[i] == '\n')
	{
		ret = malloc(i + 2);
		if (!ret)
			return (NULL);
		ret[i + 1] = '\0';
	}
	else
	{
		ret = malloc(i + 1);
		if (!ret)
			return (NULL);
	}
	while (i >= 0)
	{
		ret[i] = stat_str[i];
		--i;
	}
	return (ret);
}

static char	*add_to_static(int fd, char *stat_str)
{
	int		i;
	char	*buffer;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	i = 1;
	while (!ft_isinstring(stat_str, '\n') && i)
	{
		i = read(fd, buffer, BUFFER_SIZE);
		if (i == -1)
		{
			free(buffer);
			return (NULL);
		}
		buffer[i] = '\0';
		stat_str = ft_statjoin(stat_str, buffer);
	}
	free(buffer);
	return (stat_str);
}

char	*get_next_line(int fd)
{
	static char	*stat_str[1027];
	char		*ret;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stat_str[fd] = add_to_static(fd, stat_str[fd]);
	if (!stat_str[fd])
		return (NULL);
	ret = find_line(stat_str[fd]);
	stat_str[fd] = remove_start_stat(stat_str[fd]);
	if (!*ret)
	{
		free(ret);
		return (NULL);
	}
	return (ret);
}
