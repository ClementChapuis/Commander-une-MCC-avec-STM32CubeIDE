# Commander-une-MCC-avec-STM32CubeIDE

A partir d'un hacheur complet et d'une carte Nucleo-STM32G474RE, nous souhaitons :
- Réaliser un shell pour commander le hacheur
- Réaliser la commande des 4 transistors du hacheur en commande complémentaire décalée
- Faire l'acquisition des différents capteurs
- Réaliser l'asservissement en temps réel

## Réalisation du shell
Comme nous l’avons déjà fait pour d’autres projets, nous avons créé un shell disposant de différentes fonctions d’information et de contrôle pour le moteur.
Ces fonctions sont listées comme suit : 
- une fonction "Help" pour afficher toutes les fonctions, leurs commandes et leur utilité.
- une fonction "Pinout" pour afficher les pinouts actifs, permettant ainsi de réaliser l'ensemble des branchements facilement.
- une fonction "Start" permet de mettre le moteur sous tension.
- une fonction "Stop" pour mettre le moteur hors tension.
- une fonction "Speed" permettant de contrôler le cycle de fonctionnement du PWM entraînant le moteur.
- une fonction "Current" permettant de contrôler la valeur du courant voulu pour l'asservissement en courant de notre MCC.

## Génération des PWM 
Nous avons besoin de fréquence de PWM <= 16 kHz avec une résolution de 10 bits. Pour cela, nous fixons l’ARR à 1023 et PSC à 9. Nous avons réglé DEADTIMES TO 200, pour réaliser une rupture de 2 entre les canaux complémentaires. Nous utilisons TIM1_CH1 et TIM1_CH2 pour réaliser la génération PWM en canal complémentaire.

## Séquence d'initialisation et commande de vitesse
### Séquence d'initialisation
Pour allumer le hacheur, il faut régler le ResetPin bas du hacheur pour nous permettre d’envoyer des signaux au conducteur du moteur. Ensuite, nous devons démarrer les timers qui contrôlent les signaux PWM.

### Commande de vitesse
Nous savons que la CCR maximale est de 1024. En choisissant une CCR de 512, le cycle de service est fixé à 50%, qui est la position d’arrêt du moteur. Si un CCR entre 512 et 1024 est choisi, le moteur tournera dans une direction. Si un CCR entre 0 et 512 est choisi, il tournera dans la direction opposée. 
Tout ce que nous avons à faire est de créer une commande qui prend un nombre compris entre 0 et 1024 comme argument et qui règle le CCR du canal 1 de la minuterie à cette valeur et du canal 2 à 1024 moins la valeur demandée.

## Mesure de la vitesse et du courant
Le but de cette partie est de déterminer les différents paramètres du moteur afin de le contrôler.

### Mesure de la vitesse
Pour mesurer la vitesse du moteur, l’encodeur doit être utilisé. La sortie Z de l’encodeur est 1 lorsque le moteur a fait un tour et 0 le reste du temps.  Avec cette commande et la mesure du temps entre deux transitions de l’état 0 à l’état 1, on peut déduire la vitesse du moteur. Nous utiliserons Z, A et B pour déterminer les impulsions par révolution. Cela servira à normaliser la valeur de la vitesse dans le calcul PID.

### Mesure actuelle
Pour mesurer le courant, nous nous intéressons aux broches de hachoir qui renvoient la rétroaction du capteur de courant de phase jaune et la rétroaction du capteur de courant de phase rouge (broches 16 et 35). Il nous faut aussi un ADC.


## Asservissement du moteur
Nous devons faire deux servocommandes : une en vitesse et une en courant.
### Contrôle de la servocommande de vitesse
Pour contrôler le moteur en vitesse. Nous allons mémoriser les données de positions de l’encodeur et les utiliser pour calculer la vitesse avec les impulsions par valeur de révolution. Ce qu’il faut faire, c’est comparer la valeur réelle de la vitesse à celle ciblée. Nous multiplions l’erreur par un gain proportionnel **KP**, en ajoutant une constante intégrale et dérivée (**KI** et **KD**). Toutes ces valeurs n’ayant pu être déterminées dans de bonnes conditions de travail, elles ne peuvent donc pas être optimisées. La mémoire du système est assurée grâce à des tampons et réutilisée pour calculer la vitesse à n+2 valeurs.
Le principe de fonctionnement de cette commande a été testé sur un moteur beaucoup plus petit. Il est possible qu’une grande quantité de constantes ne soient pas correctes.
Une fois que le PID nous a donné une valeur de vitesse à envoyer, nous devons convertir cette vitesse en un cycle de service. Nous avons utilisé le fait que le cycle d’utilisation de 100 % menait à 3 000 tr/min, et que 50 % mène à 0 tr/min. Le PID n’est probablement pas adapté pour aller en valeurs de régime négatives, nous n’avons pas eu le temps de nous adapter à ce type d’utilisation.

## Conclusion
Le principe de servocommande a été réalisé à une autre échelle et fonctionnait bien, la transposition pour le moteur est faite presque à l’aveugle. Le principe est probablement correct. Nous avons réalisé dans ce cours que le flotteur est parfois un problème puisqu’il utilise une grande quantité de mémoires. Nous avons obtenu le contrôle électronique de la vitesse mais par manque de chance, nous n’avons pas pu réaliser le contrôle du servo courant. Si c’était le cas, nous aurions dû surveiller de près les fréquences d’échantillonnage entre la vitesse et le courant. Si le courant est échantillonné plus lentement que la vitesse, le résultat est assez mauvais en simulation.
