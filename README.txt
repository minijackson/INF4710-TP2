INF4710 A2017 -- Utilisation de la solution

La solution fournie pour les TPs est bas�e sur CMake, alors vous aurez � g�n�rer votre
propre project Makefile/Visual Studio/... tel que d�sir�. Le but principal de CMake est
de simplifier la gestion des d�pendances et les configurations multi-plateforme.

Typiquement, une solution CMake s�pare le code source des objets transitoires (e.g. les
�l�ments compil�s ou ayant une configuration li�e � l'ordinateur), ce qui aide aussi les
syst�mes de contr�le de version. Tout le code source du TP que vous aurez � modifier se
trouve dans ce r�pertoire, mais les librairies/ex�cutables produits seront localis�s
l� ou bon vous semble.

Pour g�n�rer votre solution, suivre les �tapes suivantes:

 - D�marrez CMake-GUI (on assume que vous ne travaillez pas uniquement en command-line)
 - Sp�cifiez le r�pertoire pour le code source (i.e. l'emplacement de ce README.txt)
 - Sp�cifiez le r�pertoire pour les fichiers de sortie (e.g. C:/TEMP/inf4710_build)
 - Cliquez sur 'Configure', et sp�cifiez votre IDE (e.g. Visual Studio 14 2015 Win64)
 - Cliquez sur 'Generate' pour produire le project sp�cifi�

Remarque1: �vitez de mettre les fichiers de sorties sur votre disque r�seau!
Remarque2: OpenCV, qui est une d�pendance du projet, devrait �tre trouv� automatiquement
par CMake (les fichiers n�cessaires sont fournis avec la solution). Si vous �prouvez des
probl�mes avec cette version pr�compil�e de OpenCV (e.g. architecture non support�e), vous
devrez vous procurer une version pr�compli�e compatible de OpenCV en ligne, ou bien 
compiler vous-m�me le code source disponible sur Github (https://github.com/opencv/opencv).

Pour Visual Studio, vous devriez obtenir un fichier '*.sln' dans votre dossier de sortie
(e.g. l� o� les objets compil�s appara�tront, e.g. C:/TEMP/inf4710_build).
Ouvrez cette solution, et une fois dans Visual Studio, vous devriez voir 4 projets dans
l'explorateur de solution, i.e.:
 - ALL_BUILD: sert � g�n�rer tous les projets d'un coup
 - tpX_lib: contient le code que vous devrez modifier
 - tpX_main: contient un 'main' qui sert � tester vos algos
 - ZERO_CHECK: sera automatiquement ex�cut� si Visual Studio d�tecte une modif � CMake

Vous pouvez maintenant vous lancer dans le code, tout devrait fonctionner avec OpenCV!

NB: Pour lancer une application en mode d�bug, n'oubliez pas d'abord de la s�lectionner
comme projet de d�marrage dans l'explorateur de solutions (bouton droite, et "D�finir 
comme projet de d�marrage"). Par d�faut, c'est 'ALL_BUILD' qui est s�lectionn�, et il
ne poss�de pas d'ex�cutable...