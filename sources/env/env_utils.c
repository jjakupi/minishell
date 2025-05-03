/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julrusse <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:28:27 by julrusse          #+#    #+#             */
/*   Updated: 2025/05/03 14:07:34 by julrusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char **dup_envp(char **envp)
{
    int   i;
    char **copy;

    /* 1) compter */
    i = 0;
    while (envp[i])
        i++;
    /* 2) allouer (i+1) pointeurs */
    copy = malloc((i + 1) * sizeof(*copy));
    if (!copy)
        return (NULL);
    /* 3) dupliquer chaque chaîne */
    for (i = 0; envp[i]; i++)
    {
        copy[i] = strdup(envp[i]);
        if (!copy[i])
        {
            while (i-- > 0)
                free(copy[i]);
            free(copy);
            return (NULL);
        }
    }
    copy[i] = NULL;
    return (copy);
}

/*
** Libère l'envp que vous avez dupliqué avec dup_envp
*/
void free_envp(char **envp)
{
    int i = 0;
    if (!envp)
        return;
    while (envp[i])
    {
        free(envp[i]);
        i++;
    }
    free(envp);
}
