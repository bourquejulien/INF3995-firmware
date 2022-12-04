# INF3995 - Firmware

Ce dépôt inclut le programme embarqué des drones.

Il est divisé en trois sections :
- Le firmware de Bitcraze présent dans le répertoire [crazyflie-firmware](crazyflie-firmware/README.md).
- Le code source développé dans le cadre du projet présent dans le répertoire [source](source/README.md).
- Le ``remote compiler``, développé dans le cadre du projet, présent dans le répertoire [remote_compiler](remote_compiler/README.md).

Se référer à chaque projet pour les informations les concernant.

## Pour lancer les modules sans les modifier :

### Prérequis
- docker 
- docker compose
- Cloner les submodules avec les commandes ci-dessous:
```bash
git submodule init
git submodule update --recursive --jobs 4
```

### Procédure - Flash

1. Entrer dans le répertoire cloné et lancer la commande ``docker compose build``.
2. Placer le drone à programmer en mode "bootloader" en gardant le bouton de mise en marche appuyé durant 3 secondes.
3. Flasher les drones avec ```docker compose up```.

### Procédure - Remote compiler

Lancer les commandes suivantes :

```bash
docker build --target=final -t remote_compiler .
docker run -p 5003:5003 remote_compiler
```
