# INF3995 - Firmware - Remote compiler

Ce répertoire contient le code du "remote compiler" développé dans le cadre du projet.

Le petit module permet de compiler le code du firmware à l'aide de commandes gRPC. Les fichiers peuvent également être modifiés.

Note : Les fichiers sont d'abord copiés dans un répertoire temporaire avant d'être modifiés, les modifications sont donc uniquement préservées durant l'exécution du programme.


## Prérequis

Il est nécessaire d'installer les dépendances à l'aide des instructions suivantes :

```bash
python3 -m venv venv
source venv/bin/activate
python3 requirements.py
```

## Exécution

Le programme peut être lancé à l'aide de la commande suivante :
```
python3 app.py
```

La variable d'environnement ``REMOTE_COMPILER_PORT`` est optionnelle et permet d'indiquer le port du serveur lancé.

## Formatage

En raison de la petite taille de ce module, le formatage a été réalisé manuellement en suivant le standard [**PEP8**](https://peps.python.org/pep-0008/).
