#include "../include/minishell.h"

/*
** update_directories
**   Récupère le nouveau répertoire après chdir, met à jour PWD et OLDPWD
**   Retourne EXIT_SUCCESS si tout est OK, sinon EXIT_FAILURE.
*/
int	update_directories(const char *old_pwd)
{
	char	new_cwd[MAX_PATH];

	if (getcwd(new_cwd, sizeof(new_cwd)) == NULL)
	{
		perror("cd: getcwd error after chdir");
		return (EXIT_FAILURE);
	}
	if (setenv("PWD", new_cwd, 1) != 0)
	{
		perror("cd: setenv PWD");
		return (EXIT_FAILURE);
	}
	if (setenv("OLDPWD", old_pwd, 1) != 0)
		perror("cd: setenv OLDPWD");
	return (EXIT_SUCCESS);
}

/*
** change_dir_home
**   Change le répertoire vers HOME.
*/
int	change_dir_home(char *cur_pwd)
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
	return (update_directories(cur_pwd));
}

/*
** change_to_old_dir
**   Change le répertoire vers OLDPWD et affiche le chemin.
*/
int	change_to_old_dir(char *cur_pwd)
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
	return (update_directories(cur_pwd));
}

/*
** execute_cd_path
**   Change le répertoire vers le chemin passé en argument.
*/
int	execute_cd_path(char *cur_pwd, char *path)
{
	if (chdir(path) != 0)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}
	return (update_directories(cur_pwd));
}
