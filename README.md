# INF3995 - Firmware

Ce repos inclus le programme embarqué des drones

Il est divisé en deux sections :
- Le firmware de Bitcraze présent dans le répertoire ``/crazyflie-firmware``
- Le code source développé dans le cadre du projet présent dans le répertoire ``/source``

## Comment utiliser le repo

### Prérequis
- docker 
- docker compose

### Procédure - Docker
1. Cloner les submodules avec les commandes ci-dessous:
```bash
git submodule init
git submodule update --recursive --jobs 3
```

2. Entrer dans le répertoire cloné et lancer la commande ``docker compose build``
3. Placer le drone à programme en mode "booloader" en gardant le bouton de mise en marche appuyé durant 3 secondes
4. Flasher les drones avec "docker compose up"
