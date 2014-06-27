Skullboard v2 (aka NdH2K14 Collector Badge)
===========================================

Cette board a été conçue par mes soins, et est basée sur deux projets formidables que sont USnooBie (http://eleccelerator.com/usnoobie/)
et Arduino (http://www.arduino.cc/fr/). Pour le coup, le design du circuit a permis de jouer sur la forme, et elle arbore donc une belle
tête de mort, logo de l'association HZV.

Ce document décrit les spécifications de la board, ses fonctionnalités ainsi que la manière de l'utiliser.


Spécifications techniques & fonctionnalités
-------------------------------------------

La Skullboard est basée sur un micro-contrôleur ATmega328p, qui est la base même de pas mal de boards Arduino, mais avec quelques ajustements:

* elle peut être programmée sans composant supplémentaire, via le port USB (si le micro-contrôleur possède le bon bootloader)
* elle peut communiquer via USB et émuler des périphériques HID (clavier, souris, joystick)
* elle possède un bon nombre de GPIOs permettant de s'interfacer avec divers composants
* elle possède tout un lot de pads plaqués or, qui peuvent servir à de la soudure ISO 1664
* elle peut être programmée via un connecteur ISP standard (5x2 broches), en cas de problème
* elle possède un connecteur mini-usb

L'alimentation de la board est basée sur 5V, mais cette version n'intègre pas de régulateur/limiteur.


Nomenclature
------------

La nomenclature définit les différents composants ainsi que leurs noms tels que référencés sur le circuit imprimé. Très utile si vous voulez savoir
quoi souder et où le souder.

Nom   |        Desc.
------|-------------------------------------------------------------------------
R1    | Résistance 10 kOhm
R2    | Résistance 1.5 kOhm
R3    | Résistance 68 Ohm
R4    | Résistance 68 Ohm
R5    | Résistance 330 Ohm
Z1    | Diode Zener 3.6V
Z2    | Diode Zener 3.6V
C6    | Condensateur céramique 27 pF
C7    | Condensateur céramique 100nF
C8    | Condensateur céramique 27 pF
XTAL  | Quartz 12 Mhz
PWR   | Led ronde rouge 3mm
SW1   | Bouton tactile noir
RESET | Bouton tactile noir
ISP   | Connecteur male 5x2 broches
USB   | Connecteur mini usb femelle type B
EXT   | Connecteur male 2 broches
D-,D+ | Connecteur male 4 broches (englobe D- et D+)
CONN1 | Connecteur femelle 8 broches
CONN2 | Connecteur femelle 8 broches
CONN3 | Connecteur femelle 6 broches



Guide de montage
----------------

Outils requis:

* 1 fer à souder avec petite panne
* 1 pince coupante
* 1 pince pliante
* 1 éponge
* 1 bobine d'étain
* 1 multimètre

Montage:

1.  Plier chaque résistance (R1 à R5) aux bonnes dimensions, et les souder une à une sur le circuit (ne pas insérer toutes les résistances en une fois).
2.  Plier les diodes zener Z1 et Z2, et les souder l'une après l'autre. Attention à la polarité (marquée par une barre).
3.  Insérer le quartz en place, et le souder.
4.  Monter le connecteur USB, et vérifier son insertion:
     1. Couper légèrement les pattes de fixation si nécessaire, puis les souder. S'assurer de faire de même de l'autre côté du circuit.
     2. Etamer la panne du fer, et toucher rapidement chaque broche du connecteur USB en prenant soin de faire contact avec la piste et la broche. Il faut être minutieux. Vérifier au multimètre.
5.  Monter le support de circuit intégré (28 broches) sur le circuit imprimé, et souder les deux broches opposées en diagonale. Souder les autres broches ensuite.
6.  Monter les boutons tactiles (ne pas trop forcer), et les souder un à un.
7.  Monter et souder la LED 3mm (méplat face au support CI)
8.  Monter et souder les condensateurs C6, C7 et C8
9.  Monter et souder le connecteur ISP (5x2 broches)
10. Monter et souder le connecteur 8 broches CONN2 (D3 à C2)
11. Monter et souder le connecteur 6 broches CONN3
12. Monter et souder le connecteur 8 broches CONN1 (B0 à C1)
13. Monter et souder les connecteurs: découper 2 broches pour EXT, découper 4 broches pour D- et D+.
14. Insérer les cavaliers sur D- et D+.


Vérification du circuit
-----------------------

1. Connecter le montage via le port USB, avec un voltmètre vérifier la tension aux bornes des broches 7 (+5V) et 8 (GND), elle doit être de 5V. Déconnecter l'USB.
2. Insérer le ATmega328p dans son support, tel qu'indiqué sur le circuit (les ergots doivent correspondre)
3. Maintenir appuyé le bouton SW1/BOOT, et connecter l'USB. La led doit s'allumer, et l'ordinateur détecter un périphérique compatible USB.
4. Connecter un programmateur ISP (type USBasp) et tenter de dialoguer avec le micro-contrôleur.



Programmation du micro-contrôleur par USB
-----------------------------------------

Le micro-contrôleur est compatible Arduino, ce logiciel est donc suffisant pour développer des premiers programmes sur la Skullboard. Pour programmer:

1. Sélectionner le programmateur USBasp dans le menu du logiciel Arduino IDE
2. Sélectionner la skullboard comme board cible (cf. configuration du logiciel Arduino IDE)
3. Connecter la skullboard tout en maintenant le bouton SW1/BOOT appuyé, la led s'allume
4. Programmer avec le logiciel Arduino
5. Appuyer sur le bouton RESET pour lancer le programme sur le micro-contrôleur


Programmation du micro-contrôleur par ISP
-----------------------------------------

Si vous possédez un programmateur de micro-contrôleur type USBasp ou similaire, vous pouvez programmer celui-ci via ce connecteur. Attention à ne pas écraser le bootloader.

1. Insérer le cavalier sur EXT si le programmateur fournit l'alimentation
2. Connecter le programmateur
3. Programmer
4. Appuyer sur le bouton RESET





