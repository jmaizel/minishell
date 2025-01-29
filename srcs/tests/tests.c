/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:01:44 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/29 16:15:56 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"
#include <assert.h>

/* =========================== */
/*       Test du Builtin echo  */
/* =========================== */

static void test_builtin_echo(void)
{
    t_simple_cmds cmd;
    char *args1[] = {"echo", "Hello, world!", NULL};
    char *args2[] = {"echo", "-n", "No newline", NULL};
    char *args3[] = {"echo", NULL};

    printf("Running test_builtin_echo...\n");

    cmd.str = args1;
    assert(builtin_echo(&cmd) == 0);
    printf("Test Passed: echo \"Hello, world!\"\n");

    cmd.str = args2;
    assert(builtin_echo(&cmd) == 0);
    printf("Test Passed: echo -n \"No newline\"\n");

    cmd.str = args3;
    assert(builtin_echo(&cmd) == 0);
    printf("Test Passed: echo with no args\n");
}

/* =========================== */
/*       Test du Builtin pwd   */
/* =========================== */

static void test_builtin_pwd(void)
{
    t_simple_cmds cmd;
    t_tools tools;

    printf("Running test_builtin_pwd...\n");

    assert(builtin_pwd(&cmd, &tools) == 0);
    printf("Test Passed: pwd\n");
}

/* =========================== */
/*       Test du Builtin env   */
/* =========================== */

static void     test_builtin_env(void)
{
        t_tools         tools;
        t_simple_cmds   cmd;
        char            *mock_env[] = {
                "USER=test_user",
                "SHELL=/bin/bash",
                "PWD=/home/user",
                NULL
        };
        char            **cmd_str;

        printf("Running test_builtin_env...\n");

        // Initialiser les structures
        ft_memset(&tools, 0, sizeof(t_tools));
        ft_memset(&cmd, 0, sizeof(t_simple_cmds));

        // Préparer la commande
        cmd_str = malloc(sizeof(char *) * 2);  // Juste "env" + NULL
        if (!cmd_str)
        {
                printf("Error: Failed to allocate command string\n");
                return;
        }
        cmd_str[0] = ft_strdup("env");
        cmd_str[1] = NULL;  // Important ! Votre builtin_env vérifie cmd->str[1]
        cmd.str = cmd_str;

        // Préparer l'environnement
        tools.env = duplicate_env(mock_env);
        if (!tools.env)
        {
                free(cmd.str[0]);
                free(cmd.str);
                printf("Error: Failed to allocate mock environment\n");
                return;
        }

        // Exécuter le test
        assert(builtin_env(&cmd, &tools) == 0);
        printf("Test Passed: env with mock environment\n");

        // Nettoyer
        free(cmd.str[0]);
        free(cmd.str);
        free_env(tools.env);
}

/* ============================ */
/*      Test du Builtin export  */
/* ============================ */

static void test_builtin_export(void)
{
    t_tools tools;
    t_simple_cmds cmd;
    char *args1[] = {"export", "NEW_VAR=test", NULL};
    char *args2[] = {"export", "USER=new_user", NULL};
    char *mock_env[] = {
        "USER=old_user",
        "PWD=/home/user",
        NULL
    };

    printf("Running test_builtin_export...\n");

    tools.env = duplicate_env(mock_env);
    if (!tools.env)
    {
        printf("Error: failed to duplicate environment\n");
        return;
    }

    cmd.str = args1;
    assert(builtin_export(&cmd, &tools) == 0);
    assert(get_env_var("NEW_VAR", tools.env) && strcmp(get_env_var("NEW_VAR", tools.env), "test") == 0);
    printf("Test Passed: export NEW_VAR=test\n");

    cmd.str = args2;
    assert(builtin_export(&cmd, &tools) == 0);
    assert(get_env_var("USER", tools.env) && strcmp(get_env_var("USER", tools.env), "new_user") == 0);
    printf("Test Passed: export USER=new_user\n");

    free_env(tools.env);
}

/* ============================ */
/*     Fonction principale      */
/* ============================ */

int main(void)
{
    printf("\nStarting builtin tests...\n");
    printf("==============================\n\n");

    test_builtin_echo();
    test_builtin_pwd();
    test_builtin_env();
    test_builtin_export();

    printf("\nAll tests passed successfully!\n");
    printf("==============================\n");
    return (0);
}
