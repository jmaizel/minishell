/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdedessu <cdedessu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:42:12 by cdedessu          #+#    #+#             */
/*   Updated: 2025/01/29 13:55:50 by cdedessu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execution.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char     g_current_dir[1024] = "/home/user";
static int      g_chdir_should_fail = 0;

// Mock de chdir pour les tests
int     chdir(const char *path)
{
        if (g_chdir_should_fail)
                return (-1);
        strcpy(g_current_dir, path);
        return (0);
}

char    *getcwd(char *buf, size_t size)
{
        (void)size;
        if (buf)
                strcpy(buf, g_current_dir);
        return (ft_strdup(g_current_dir));
}

static void     setup_test_environment(t_simple_cmds *cmd, t_tools *tools,
        char **args, char **env)
{
        ft_memset(cmd, 0, sizeof(t_simple_cmds));
        ft_memset(tools, 0, sizeof(t_tools));
        cmd->str = args;
        tools->env = duplicate_env(env);
        if (!tools->env)
        {
                printf("Failed to setup test environment\n");
                exit(1);
        }
}

static void     test_builtin_cd(void)
{
        t_simple_cmds   cmd;
        t_tools         tools;
        char            *args[] = {"cd", "/tmp", NULL};
        char            *mock_env[] = {
                "HOME=/home/user",
                "PWD=/home/user",
                "OLDPWD=/home/user",
                NULL
        };

        printf("Running test_builtin_cd...\n");
        g_chdir_should_fail = 0;
        setup_test_environment(&cmd, &tools, args, mock_env);

        int ret = builtin_cd(&cmd, &tools);
        assert(ret == SUCCESS);
        char *pwd = get_env_var("PWD", tools.env);
        assert(pwd && strcmp(pwd, "/tmp") == 0);

        free_env(tools.env);
        printf("Test Passed: cd /tmp\n");
}

static void     test_cd_without_arguments(void)
{
        t_simple_cmds   cmd;
        t_tools         tools;
        char            *args[] = {"cd", NULL};
        char            *mock_env[] = {
                "HOME=/home/user",
                "PWD=/tmp",
                "OLDPWD=/home/user",
                NULL
        };

        printf("Running test_cd_without_arguments...\n");
        g_chdir_should_fail = 0;
        setup_test_environment(&cmd, &tools, args, mock_env);

        int ret = builtin_cd(&cmd, &tools);
        assert(ret == SUCCESS);
        char *pwd = get_env_var("PWD", tools.env);
        assert(pwd && strcmp(pwd, "/home/user") == 0);

        free_env(tools.env);
        printf("Test Passed: cd with no arguments goes to HOME\n");
}

static void     test_cd_to_invalid_directory(void)
{
        t_simple_cmds   cmd;
        t_tools         tools;
        char            *args[] = {"cd", "/nonexistent", NULL};
        char            *mock_env[] = {
                "HOME=/home/user",
                "PWD=/home/user",
                "OLDPWD=/home/user",
                NULL
        };

        printf("Running test_cd_to_invalid_directory...\n");
        g_chdir_should_fail = 1;
        setup_test_environment(&cmd, &tools, args, mock_env);

        int ret = builtin_cd(&cmd, &tools);
        assert(ret == ERR_CHDIR_FAILED);

        free_env(tools.env);
        printf("Test Passed: cd to nonexistent directory\n");
        g_chdir_should_fail = 0;
}

static void     test_cd_without_env(void)
{
        t_simple_cmds   cmd;
        t_tools         tools;
        char            *args[] = {"cd", NULL};

        printf("Running test_cd_without_env...\n");
        ft_memset(&cmd, 0, sizeof(t_simple_cmds));
        ft_memset(&tools, 0, sizeof(t_tools));
        cmd.str = args;
        tools.env = NULL;

        int ret = builtin_cd(&cmd, &tools);
        assert(ret == ERR_INVALID_CMD);
        printf("Test Passed: cd with NULL environment\n");
}

int     main(void)
{
        printf("\nStarting cd builtin tests...\n");
        printf("==============================\n\n");

        test_builtin_cd();
        test_cd_without_arguments();
        test_cd_to_invalid_directory();
        test_cd_without_env();

        printf("\nAll tests passed successfully!\n");
        printf("==============================\n");
        return (0);
}