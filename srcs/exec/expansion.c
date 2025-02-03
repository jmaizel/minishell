/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:30:00 by cdedessu          #+#    #+#             */
/*   Updated: 2025/02/03 20:58:53 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"

static char	*get_variable_name(const char *str)
{
	int		i;
	char	*name;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	name = ft_substr(str, 0, i);
	return (name);
}

static char	*expand_exit_status(t_env_manager *env_mgr)
{
	if (!env_mgr || !env_mgr->tools)
		return (ft_strdup("1"));
	return (ft_itoa(env_mgr->tools->exit_code));
}

static char	*handle_variable_expansion(const char *str, t_env_manager *env_mgr)
{
	char	*var_name;
	char	*value;

	if (!str || !env_mgr)
		return (ft_strdup(""));
	if (str[0] == '?')
		return (expand_exit_status(env_mgr));
	var_name = get_variable_name(str);
	if (!var_name)
		return (ft_strdup(""));
	value = get_env_var(var_name, env_mgr);
	free(var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

static char	*append_variable(char *result, const char *str, int *i,
	t_env_manager *env_mgr)
{
	char	*var_value;
	char	*temp;

	(*i)++;
	var_value = handle_variable_expansion(str + *i, env_mgr);
	if (!var_value)
		return (result);
	temp = ft_strjoin(result, var_value);
	free(var_value);
	free(result);
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (temp);
}

static char	*append_character(char *result, char c)
{
	char	*temp;

	temp = ft_charjoin(result, c);
	free(result);
	return (temp);
}

char	*expand_variables(const char *str, t_env_manager *env_mgr)
{
	char	*result;
	bool	in_single_quotes;
	bool	in_double_quotes;
	int		i;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	in_single_quotes = false;
	in_double_quotes = false;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (str[i] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else if (str[i] == '$' && !in_single_quotes && str[i + 1])
			result = append_variable(result, str, &i, env_mgr);
		else
			result = append_character(result, str[i]);
		i++;
	}
	return (result);
}
