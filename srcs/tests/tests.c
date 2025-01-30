/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:01:44 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/30 16:42:25 by cdedessu         ###   ########.fr       */
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
        t_simple_cmds   cmd;
        t_tools         tools;
        char            *args[] = {"pwd", NULL};

        printf("Running test_builtin_pwd...\n");

        // Initialiser les structures
        ft_memset(&tools, 0, sizeof(t_tools));
        ft_memset(&cmd, 0, sizeof(t_simple_cmds));

        // Configurer la commande
        cmd.str = args;

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


/* =========================== */
/*       Test du Builtin exit  */
/* =========================== */

/* static void test_builtin_exit(void)
{
    t_tools tools;
    t_simple_cmds cmd;
    char *args1[] = {"exit", NULL};
    char *args2[] = {"exit", "42", NULL};
    char *args3[] = {"exit", "abc", NULL};
    char *args4[] = {"exit", "42", "43", NULL};

    printf("Running test_builtin_exit...\n");

    ft_memset(&tools, 0, sizeof(t_tools));
    ft_memset(&cmd, 0, sizeof(t_simple_cmds));
    tools.exit_code = 5;

    printf("Test: exit avec aucun argument...\n");
    cmd.str = args1;
    builtin_exit(&cmd, &tools);
    assert(tools.exit_code == 5);

    printf("Test: exit avec 42...\n");
    cmd.str = args2;
    builtin_exit(&cmd, &tools);
    assert(tools.exit_code == 42);

    printf("Test: exit avec argument non numérique...\n");
    cmd.str = args3;
    builtin_exit(&cmd, &tools);
    assert(tools.exit_code == 2);

    printf("Test: exit avec trop d’arguments...\n");
    cmd.str = args4;
    builtin_exit(&cmd, &tools);
    assert(tools.exit_code == 1);

    printf("Test Passed: builtin_exit\n");
} */

/* =========================== */
/*   Tests supplémentaires    */
/*        pour echo           */
/* =========================== */

static void     test_echo_multiple_args(void)
{
       t_simple_cmds   cmd;
       char            *args[] = {"echo", "Hello", "World", "42", NULL};

       printf("Running test_echo_multiple_args...\n");
       cmd.str = args;
       assert(builtin_echo(&cmd) == 0);
       printf("Test Passed: echo with multiple arguments\n");
}

static void     test_echo_multiple_options(void)
{
       t_simple_cmds   cmd;
       char            *args[] = {"echo", "-n", "-n", "-n", "test", NULL};

       printf("Running test_echo_multiple_options...\n");
       cmd.str = args;
       assert(builtin_echo(&cmd) == 0);
       printf("Test Passed: echo with multiple -n options\n");
}

/* =========================== */
/*   Tests supplémentaires    */
/*        pour pwd            */
/* =========================== */

static void     test_pwd_with_args(void)
{
        t_simple_cmds   cmd;
        t_tools         tools;
        char            *args[] = {"pwd", "--help", NULL};

        printf("Running test_pwd_with_args...\n");

        // Initialiser les structures
        ft_memset(&tools, 0, sizeof(t_tools));
        ft_memset(&cmd, 0, sizeof(t_simple_cmds));

        // Configurer la commande
        cmd.str = args;

        assert(builtin_pwd(&cmd, &tools) == ERR_INVALID_CMD);
        printf("Test Passed: pwd with invalid arguments\n");
}

/* =========================== */
/*   Tests supplémentaires    */
/*        pour env            */
/* =========================== */

static void     test_env_with_args(void)
{
       t_tools         tools;
       t_simple_cmds   cmd;
       char            *args[] = {"env", "something", NULL};
       char            *mock_env[] = {
               "USER=test_user",
               NULL
       };

       printf("Running test_env_with_args...\n");
       ft_memset(&tools, 0, sizeof(t_tools));
       tools.env = duplicate_env(mock_env);
       cmd.str = args;

       assert(builtin_env(&cmd, &tools) == ERR_INVALID_CMD);
       free_env(tools.env);
       printf("Test Passed: env with arguments (should fail)\n");
}

/* =========================== */
/*   Tests supplémentaires    */
/*        pour export         */
/* =========================== */

static void     test_export_empty(void)
{
       t_tools         tools;
       t_simple_cmds   cmd;
       char            *args[] = {"export", NULL};
       char            *mock_env[] = {
               "USER=test_user",
               "PATH=/bin",
               NULL
       };

       printf("Running test_export_empty...\n");
       ft_memset(&tools, 0, sizeof(t_tools));
       tools.env = duplicate_env(mock_env);
       cmd.str = args;

       assert(builtin_export(&cmd, &tools) == SUCCESS);
       free_env(tools.env);
       printf("Test Passed: export without arguments\n");
}

static void     test_export_invalid_name(void)
{
       t_tools         tools;
       t_simple_cmds   cmd;
       char            *args[] = {"export", "2VAR=test", NULL};
       char            *mock_env[] = {"PATH=/bin", NULL};

       printf("Running test_export_invalid_name...\n");
       ft_memset(&tools, 0, sizeof(t_tools));
       tools.env = duplicate_env(mock_env);
       cmd.str = args;

       assert(builtin_export(&cmd, &tools) == ERR_INVALID_CMD);
       free_env(tools.env);
       printf("Test Passed: export with invalid variable name\n");
}

/* =========================== */
/*       Test du Builtin unset */
/* =========================== */

static void test_builtin_unset(void)
{
    t_tools tools;
    t_simple_cmds cmd;
    char *args1[] = {"unset", "TEST_VAR", NULL};
    char *args2[] = {"unset", "NON_EXISTENT_VAR", NULL};
    char *args3[] = {"unset", "VAR1", "VAR2", NULL};
    char *args4[] = {"unset", "PATH", NULL};
    char *mock_env[] = {
        "TEST_VAR=value",
        "USER=test_user",
        "PATH=/bin",
        "VAR1=one",
        "VAR2=two",
        NULL
    };

    printf("Running test_builtin_unset...\n");

    tools.env = duplicate_env(mock_env);
    if (!tools.env)
    {
        printf("Error: failed to duplicate environment\n");
        return;
    }

    // Supprimer une variable existante
    cmd.str = args1;
    assert(builtin_unset(&cmd, &tools) == 0);
    assert(get_env_var("TEST_VAR", tools.env) == NULL);
    printf("Test Passed: unset TEST_VAR\n");

    // Supprimer une variable inexistante (ne doit pas provoquer d'erreur)
    cmd.str = args2;
    assert(builtin_unset(&cmd, &tools) == 0);
    printf("Test Passed: unset NON_EXISTENT_VAR (should do nothing)\n");

    // Supprimer plusieurs variables
    cmd.str = args3;
    assert(builtin_unset(&cmd, &tools) == 0);
    assert(get_env_var("VAR1", tools.env) == NULL);
    assert(get_env_var("VAR2", tools.env) == NULL);
    printf("Test Passed: unset multiple variables (VAR1, VAR2)\n");

    // Supprimer une variable système (PATH)
    cmd.str = args4;
    assert(builtin_unset(&cmd, &tools) == 0);
    assert(get_env_var("PATH", tools.env) == NULL);
    printf("Test Passed: unset PATH (system variable)\n");

    free_env(tools.env);
}


/* ============================ */
/*     Fonction principale      */
/* ============================ */

int     main(void)
{
        printf("\nStarting builtin tests...\n");
        printf("==============================\n\n");

        // Tests originaux
        test_builtin_echo();
        test_builtin_pwd();
        test_builtin_env();
        test_builtin_export();
		test_builtin_unset();
		//test_builtin_exit();

        // Tests supplémentaires
        test_echo_multiple_args();
        test_echo_multiple_options();
        test_pwd_with_args();
        test_env_with_args();
        test_export_empty();
        test_export_invalid_name();

        printf("\nAll tests passed successfully!\n");
        printf("==============================\n");
        return (0);
}
