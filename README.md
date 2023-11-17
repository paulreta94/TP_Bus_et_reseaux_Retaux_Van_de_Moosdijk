# TP_Bus_et_reseaux_Retaux_Van_de_Moosdijk
Auteurs : Martina Van De Moosdijk et Paul-Etienne Rétaux
Date : 17.11.2023
Ecole : ENSEA

Ce projet consiste à asservir une vanne de radiateur à la température et la pression via une interface web.

On distingue dans le dossier deux parties :

	- une partie software, codée en python : **interface_without_test.py** (nous n'avons pas pu tester les dernières modifications). Ce fichier, stocké dans le dossier "seance3_python" de la Raspberry, permet de définir les pages web qui sont stockées sur un serveur (la Raspberry). Nous commençons par quelques pages basiques pour nous familiariser avec la méthode REST puis codons des pages qui permettent d'envoyer des requêtes au STM32 via le port UART de la Raspberry qui ordonnent au choix :

		- Une récupération de la température 
		- Une récupération de la pression
		- Un réglage du facteur de précision K
		- Une récupération de ce facteur
		- Une récupération de la valeur de l'angle de la vanne.

	- le reste est une partie hardware, codée en C et téléchargée dans un microcontrôleur STM32. Dans un premier fichier **BMP280simple.c**, on récupère et traite les données de pression et de température issues du capteur BMP280. Dans un second fichier **interface_stm32_raspberrypi.c**, on établit la récupération des requêtes de la Raspberry (cf. les cinq tirets ci-dessus) et on appelle une fonction qui ouvre ou ferme la vanne selon que la température est inférieure à 16 ou supérieure à 22 °C.
