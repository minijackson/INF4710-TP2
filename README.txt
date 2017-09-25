INF4710 A2017 -- Utilisation de la solution

La solution fournie pour les TPs est basée sur CMake, alors vous aurez à générer votre
propre project Makefile/Visual Studio/... tel que désiré. Le but principal de CMake est
de simplifier la gestion des dépendances et les configurations multi-plateforme.

Typiquement, une solution CMake sépare le code source des objets transitoires (e.g. les
éléments compilés ou ayant une configuration liée à l'ordinateur), ce qui aide aussi les
systèmes de contrôle de version. Tout le code source du TP que vous aurez à modifier se
trouve dans ce répertoire, mais les librairies/exécutables produits seront localisés
là ou bon vous semble.

Pour générer votre solution, suivre les étapes suivantes:

 - Démarrez CMake-GUI (on assume que vous ne travaillez pas uniquement en command-line)
 - Spécifiez le répertoire pour le code source (i.e. l'emplacement de ce README.txt)
 - Spécifiez le répertoire pour les fichiers de sortie (e.g. C:/TEMP/inf4710_build)
 - Cliquez sur 'Configure', et spécifiez votre IDE (e.g. Visual Studio 14 2015 Win64)
 - Cliquez sur 'Generate' pour produire le project spécifié

Remarque1: Évitez de mettre les fichiers de sorties sur votre disque réseau!
Remarque2: OpenCV, qui est une dépendance du projet, devrait être trouvé automatiquement
par CMake (les fichiers nécessaires sont fournis avec la solution). Si vous éprouvez des
problèmes avec cette version précompilée de OpenCV (e.g. architecture non supportée), vous
devrez vous procurer une version précompliée compatible de OpenCV en ligne, ou bien 
compiler vous-même le code source disponible sur Github (https://github.com/opencv/opencv).

Pour Visual Studio, vous devriez obtenir un fichier '*.sln' dans votre dossier de sortie
(e.g. là où les objets compilés apparaîtront, e.g. C:/TEMP/inf4710_build).
Ouvrez cette solution, et une fois dans Visual Studio, vous devriez voir 4 projets dans
l'explorateur de solution, i.e.:
 - ALL_BUILD: sert à générer tous les projets d'un coup
 - tpX_lib: contient le code que vous devrez modifier
 - tpX_main: contient un 'main' qui sert à tester vos algos
 - ZERO_CHECK: sera automatiquement exécuté si Visual Studio détecte une modif à CMake

Vous pouvez maintenant vous lancer dans le code, tout devrait fonctionner avec OpenCV!

NB: Pour lancer une application en mode débug, n'oubliez pas d'abord de la sélectionner
comme projet de démarrage dans l'explorateur de solutions (bouton droite, et "Définir 
comme projet de démarrage"). Par défaut, c'est 'ALL_BUILD' qui est sélectionné, et il
ne possède pas d'exécutable...