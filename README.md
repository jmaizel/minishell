
# 🐚 minishell

> _"As beautiful as a shell."_

---

## 🇫🇷 Version Française

### 🎯 Objectif

`minishell` est une implémentation simplifiée d’un shell Unix. L’objectif est de mieux comprendre le fonctionnement d’un shell, la gestion des processus, des redirections et des pipes.

### 🧱 Fonctionnalités obligatoires

- Affichage d’un **prompt interactif**
- Historique de commandes
- Résolution des chemins avec la variable **PATH**
- Exécution de **commandes avec arguments**
- Redirections :
  - `<` (entrée)
  - `>` (sortie)
  - `<<` (here_doc)
  - `>>` (append)
- **Pipes** entre plusieurs commandes
- Gestion des **variables d’environnement** (`$VAR`, `$?`)
- Gestion des **quotes** simples `'` et doubles `"`
- Comportement interactif :
  - `Ctrl-C` : affiche un nouveau prompt
  - `Ctrl-D` : quitte le shell
  - `Ctrl-\` : ignoré
- Commandes **built-in** :
  - `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`

### 🛠️ Technologies

- **Langage :** C
- **Librairie :** readline (autorisé)
- **Norme :** Norme 42
- **Compilation :** Makefile (`all`, `clean`, `fclean`, `re`, `bonus`)

### ✨ Bonus possibles

- Opérateurs `&&`, `||` avec gestion des priorités via parenthèses
- Expansion des **wildcards `*`** dans le répertoire courant

### 📁 Structure du projet

```
minishell/
├── src/
├── include/
├── libft/
├── Makefile
└── README.md
```

### 🧠 Compétences développées

- Gestion avancée des **processus** et **pipes**
- Implémentation d’un interpréteur de commandes
- Parsing complexe (quotes, variables, redirections)
- Approfondissement des appels système Unix
- Écriture robuste et sans fuite mémoire

---

## 🇬🇧 English Version

### 🎯 Objective

`minishell` is a simplified implementation of a Unix shell. The goal is to understand how a shell works internally, including process handling, parsing, pipes, and redirections.

### 🧱 Mandatory Features

- Interactive **prompt**
- Command **history**
- Path resolution with **PATH**
- Execution of **commands with arguments**
- Redirections:
  - `<` (input)
  - `>` (output)
  - `<<` (here_doc)
  - `>>` (append)
- **Pipes** between multiple commands
- Support for **environment variables** (`$VAR`, `$?`)
- Handling of **quotes**:
  - `'` prevents all interpretation
  - `"` allows only `$` expansion
- Interactive mode behavior:
  - `Ctrl-C`: shows a new prompt
  - `Ctrl-D`: exits the shell
  - `Ctrl-\`: does nothing
- Built-in commands:
  - `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`

### 🛠️ Tech Stack

- **Language:** C
- **Library used:** readline
- **Coding standard:** 42 Norm
- **Build system:** Makefile (`all`, `clean`, `fclean`, `re`, `bonus`)

### ✨ Bonus Features

- Logical operators `&&`, `||` with parentheses
- **Wildcard `*` expansion** in the current directory

### 📁 Project Structure

```
minishell/
├── src/
├── include/
├── libft/
├── Makefile
└── README.md
```

### 🧠 Skills Gained

- Advanced **process and pipe** management
- Shell-like **command parser**
- Handling redirections and quotes
- Deep understanding of Unix syscalls
- Writing robust and memory-safe C code

