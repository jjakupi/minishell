#include "../include/minishell.h"

/*
** builtin_env:
**   Affiche toutes les variables d'environnement, une par ligne.
**   Si la commande 'env' reçoit un argument (autre que "env"), affiche une
**   erreur et retourne EXIT_FAILURE.
**
** Paramètres :
**   - cmd : structure t_command contenant la commande et ses arguments.
**           (cmd->args[0] est "env"; aucune autre argument n'est attendu)
**   - env : tableau de chaînes représentant l'environnement.
**
** Retourne :
**   - EXIT_SUCCESS si l'affichage a réussi,
**   - EXIT_FAILURE si un argument inattendu est présent.
*/
int	builtin_env(t_command *cmd, char **env)
{
	int	i;

	i = 0;
	if (cmd->arg_count > 0)
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		return (EXIT_FAILURE);
	}
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}
