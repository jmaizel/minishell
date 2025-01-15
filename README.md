Minishell - Projet 42

Bienvenue dans le projet Minishell, développé par jmaizel et cdedessu dans le cadre du cursus 42.

📖 Description

Le projet Minishell consiste à recréer un shell minimaliste inspiré de Bash, offrant une meilleure compréhension des mécanismes sous-jacents des systèmes Unix/Linux. Ce shell devra être conforme à certaines spécifications fournies par le projet tout en implémentant des fonctionnalités essentielles telles que :
	•	L’exécution de commandes simples.
	•	La gestion des variables d’environnement.
	•	La gestion des signaux.
	•	La prise en charge des redirections (>, <, >>).
	•	La gestion des pipes (|).
	•	Une implémentation simplifiée de certaines commandes intégrées comme cd, echo, et exit.

Ce projet est une excellente opportunité pour plonger dans le monde fascinant des systèmes Unix/Linux et comprendre les interactions entre le shell, le noyau, et les processus.

📋 Fonctionnalités principales
	•	Commandes simples : Exécution de commandes comme ls, pwd, etc.
	•	Commandes intégrées : Implémentation de commandes comme cd, echo, export, etc.
	•	Gestion des pipes : Chaine de commandes utilisant le caractère |.
	•	Redirections : Gestion des redirections d’entrée/sortie (>, <, >>).
	•	Gestion des signaux : Réactivité aux signaux comme Ctrl+C, Ctrl+D, et Ctrl+Z.
	•	Environnement : Gestion et modification des variables d’environnement.

📚 Liens utiles

Pour mieux comprendre les attentes et trouver des inspirations pour l’implémentation, nous nous appuierons sur les ressources suivantes :
	•	42-minishell par Hqndler (GitHub)

Ces projets peuvent servir d’exemples pour explorer différentes approches d’implémentation et structuration du code.

🚀 Installation et utilisation

Prérequis

Assurez-vous d’avoir les outils suivants installés sur votre machine :
	•	Un compilateur C conforme (GCC ou Clang).
	•	Make.
	•	Un système Unix/Linux.

Étapes d’installation
	1.	Clonez ce dépôt :

git clone https://github.com/votre-utilisateur/minishell.git
cd minishell


	2.	Compilez le projet :

make


	3.	Lancez le shell :

./minishell

📂 Structure du projet

Voici un aperçu de l’organisation des fichiers :

minishell/
├── src/                # Code source principal
├── include/            # Fichiers d'en-tête
├── lib/                # Librairies externes (le cas échéant)
├── Makefile            # Fichier de compilation
└── README.md           # Ce fichier

🛠️ Fonctionnalités à implémenter
	•	Commandes intégrées :
	•	cd
	•	echo
	•	exit
	•	env
	•	export
	•	unset
	•	Signaux : Gestion de Ctrl+C, Ctrl+D, et Ctrl+Z.
	•	Gestion des erreurs : Gestion des erreurs dans l’exécution des commandes, le parsing, etc.

📜 Licence

Ce projet est réalisé dans le cadre du cursus 42 et est soumis à sa politique de collaboration. Veuillez respecter les règles d’intégrité académique lors de l’utilisation ou de la modification de ce code.

💬 Auteurs
	•	jmaizel
	•	cdedessu

N’hésitez pas à nous contacter pour toute question ou suggestion ! 😄

Ce fichier est prêt à être utilisé et peut être adapté selon vos besoins spécifiques. 😊
