# Module du kernel pour une utilisatioh facile des servos moteurs avec le Raspberry Pi


## Fonctionnement

### 1- lancer le module
Executer la commande en superutilisateur :

```
sudo ./servod --min=50 --max=50
```

On indique les valeurs min et max pour faciliter les choses, vous allez comprendre au prochain point.

### 2 - Contrôler le servos

Pour contrôler le servo il faut écrire dans le fichier /dev/servoblaster par l'intermédiaire d'un echo par exemple.

```
sudo echo P1-12=50 > /dev/servoblaster
```

Décomposont se code, il écrit dans le fichier /dev/servoblaster, que le Raspberry Pi, au schéma de Pin P1, le pin 12, on envoit 50, alors le service servod en background traite le fichier et envoie la commande.

Voyons maintenant les différentes valeurs :

0° :
```
sudo echo P1-12=50 > /dev/servoblaster
```

90° :
```
sudo echo P1-12=150 > /dev/servoblaster
```
180° : 
```
sudo echo P1-12=225 > /dev/servoblaster
```

### 3 - Eteindre tout

Lancer la commande suivante : 
killall servod
