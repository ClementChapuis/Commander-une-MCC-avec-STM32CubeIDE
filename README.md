# Commander-une-MCC-avec-STM32CubeIDE

A partir d'un hacheur complet et d'une carte Nucleo-STM32G474RE, nous souhaitons :
- Réaliser un shell pour commander le hacheur
- Réaliser la commande des 4 transistors du hacheur en commande complémentaire décalée
- Faire l'acquisition des différents capteurs
- Réaliser l'asservissement en temps réel

<img width="253" alt="Capture d’écran 2023-01-04 224641" src="https://user-images.githubusercontent.com/83455678/210656492-48200264-6c79-4155-9875-4c621a067290.png">

## Réalisation du shell
Comme nous l’avons déjà fait pour d’autres projets, nous avons créé un shell disposant de différentes fonctions d’information et de contrôle pour le moteur.
Ces fonctions sont listées comme suit : 
- une fonction "Help" pour afficher toutes les fonctions, leurs commandes et leur utilité
- une fonction "Pinout" pour afficher les pinouts actifs, permettant ainsi de réaliser l'ensemble des branchements facilement
- une fonction "Start" permet de mettre le moteur sous tension
- une fonction "Stop" pour mettre le moteur hors tension
- une fonction "Speed" permettant de contrôler le cycle de fonctionnement du PWM entraînant le moteur
- une fonction "Current" permettant de contrôler la valeur du courant voulu pour l'asservissement en courant de notre MCC

## Génération des PWM 
Nous avons besoin d'une fréquence de PWM <= 16 kHz avec une résolution de 10 bits. Pour cela, nous fixons l’ARR à 1023 et PSC à 9. Nous avons réglé les temps morts à 200, pour réaliser une rupture de 2 entre les canaux complémentaires. Nous utilisons TIM1_CH1 et TIM1_CH2 pour réaliser la génération PWM en canaux complémentaires.

![2](https://user-images.githubusercontent.com/83455678/210656542-b797ad15-d4c5-42d8-943d-7241e7cbf422.jpg)

## Séquence d'initialisation et commande de vitesse
### Séquence d'initialisation
Pour allumer le hacheur, il faut régler le ResetPin du hacheur pour nous permettre d’envoyer des signaux au conducteur du moteur. Ensuite, nous démarrons le timer (timer 1) qui contrôle les signaux PWM. Tout ceci est réalisé par la commande "Start".

### Commande de vitesse
Nous savons que la CCR maximale est de 1024. En choisissant une CCR de 512, notre alpha est fixé à 50%, qui est la position d’arrêt du moteur. Si un CCR entre 512 et 1024 est choisi, le moteur tournera dans une direction. Si un CCR entre 0 et 512 est choisi, il tournera dans la direction opposée. 
Tout ce que nous avons à faire est de créer une commande qui prend un nombre compris entre 0 et 1024 comme argument et qui règle le CCR du canal 1 du timer à cette valeur et du canal 2 à 1024 moins la valeur demandée. 
Nous réalisons une conversion afin qu'il suffise de taper dans le shell "speed alpha" avec le alpha désiré (en %) pour réaliser la commande de vitesse.
Tout ceci est assez contraignant : nous sommes obligé de monter la vitesse par paliers allant de 2 à 4% en fonction de la vitesse, sinon, nous avons un "Hall over current" au niveau du hacheur. Les vitesses hautes sont difficilement atteignables, en fonction des MCC et des hacheurs (le matériel nous ayant lâché à plusieurs reprises, nous avons eu l'occasion de faire plusieurs tests), une valeur de 70% pour notre alpha était parfois inatteignable.
Nous avons donc besoin de réaliser un asservissement.

## Mesure de la vitesse et du courant
Le but de cette partie est de récupérer les différents paramètres du moteur afin de le contrôler.

### Mesure de la vitesse
Pour mesurer la vitesse du moteur, l’encodeur doit être utilisé. Les sorties A et B de l’encodeur valent 1 pour une impulsion et 0 le reste du temps.  Avec cette commande et la mesure du temps entre deux transitions de l’état 0 à l’état 1, on peut déduire la vitesse du moteur. Nous utiliserons et B pour déterminer les impulsions par révolution. Cela servira à normaliser la valeur de la vitesse dans le calcul des paramètres de notre PI.

### Mesure du courant
Pour mesurer le courant, nous nous intéressons aux broches de hacheur qui renvoient la valeur mesurée du capteur de courant de phase jaune et la valeur mesurée du capteur de courant de phase rouge (broches 16 et 35). Il nous faut aussi un ADC. 
Au cours de la séance, la consigne a été précisée, et seule la valeur mesurée du capteur de courant de phase jaune a été utilisée.

## Asservissement du moteur
Nous devons réaliser deux asservissements : un en courant et un en vitesse.
Malheureusement, nous ne sommes pas parvenus à réaliser le second asservissement. Nous nous contenterons donc de vous présenter notre travail pour l'asservissement en courant.
### Asservissement en courant
Afin de réaliser cet asservissement, nous avons créé un correcteur PI dans un fichier de code à part. Nous initialisons notre correcteur et nous récupérons la valeur du courant mesurée avec l'ADC, ainsi que la valeur de courant voulue par l'utilisateur. C'est l'erreur entre ces deux valeurs qui va permettre de mettre en place les effet proportionnel et intégral de notre correcteur. Pour ces effet, nous choisissons respectivement un gain Kp et un gain Ki, que nous déterminons à tâtons et en plusieurs essais avec notre machine.
Notre correcteur PI est utilisé avec le même timer que celui des PWM (timer 1) : on génère une interruption à l'aide de celui-ci durant laquelle on mesure le courant et on applique notre correction avec une consigne modifiée en PWM.

Cependant, il s'avère qu'il y a un décalage entre le courant que nous mesurons directement avec une sonde de courant et la valeur désirée par l'utilisateur : le moteur est bien asservi, mais pas idéalement. Nous avons cherché à expliquer ce phénomène et avons remarqué que notre mesure du courant avec l'ADC n'était pas totalement exacte. Cela peut s'expliquer par un décalage entre le courant exact et théorique que nous n'avons pas pris en compte lors de nos calculs, ou encore un problème matériel qui impacterait la mesure.

## Conclusion
Nous sommes parvenus à réaliser l'ensemble des consignes de ce TP, exceptée la dernière pour l'asservissement en vitesse. 
Notre asservissement en courant est réussi mais notre courant a des valeurs en décalage avec celles désirées. Nous avons fourni des pistes d'explications de ce phénomène et pensons qu'il conviendrait de les explorer en détail avec plus de temps afin de régler le problème.
