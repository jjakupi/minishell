/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:13:17 by julrusse          #+#    #+#             */
/*   Updated: 2025/04/21 16:05:06 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	update_directories(const char *old_pwd, char ***env)
{
	char	new_cwd[MAX_PATH];
	char	assign[MAX_PATH + 5];

	if (getcwd(new_cwd, sizeof(new_cwd)) == NULL)
	{
		perror("cd: getcwd error after chdir");
		return (EXIT_FAILURE);
	}
	ft_strlcpy(assign, "PWD=", sizeof(assign));
	ft_strlcat(assign, new_cwd, sizeof(assign));
	if (set_env_var(env, assign) != 0)
		return (EXIT_FAILURE);
	ft_strlcpy(assign, "OLDPWD=", sizeof(assign));
	ft_strlcat(assign, old_pwd, sizeof(assign));
	if (set_env_var(env, assign) != 0)
		ft_putstr_fd("cd: could not update OLDPWD\n", 2);
	return (EXIT_SUCCESS);
}

int	change_dir_home(char *cur_pwd, char ***env)
{
	char	*home;

	home = getenv("HOME");
	if (home == NULL)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		return (EXIT_FAILURE);
	}
	if (chdir(home) != 0)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}
	return (update_directories(cur_pwd, env));
}

int	change_to_old_dir(char *cur_pwd, char ***env)
{
	char	*old;

	old = getenv("OLDPWD");
	if (old == NULL)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		return (EXIT_FAILURE);
	}
	if (chdir(old) != 0)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}
	printf("%s\n", old);
	return (update_directories(cur_pwd, env));
}

int	execute_cd_path(char *cur_pwd, char *path, char ***env)
{
	if (chdir(path) != 0)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}
	return (update_directories(cur_pwd, env));
}
