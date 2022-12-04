# INF3995 - Firmware - Source

Ce répertoire contient le code embarqué développé dans le cadre du projet.


## Prérequis

Il est nécessaire d'installer les dépendances à l'aide des instructions suivantes :
```bash
apt update
apt install -y make gcc-arm-none-eabi
```

## Compilation

La compilation se lance à l'aide d'un simple ```make```.

## Envoi du firmware au drone

- Compiler le firmware
- Placer le drone à programmer en mode "bootloader" en gardant le bouton de mise en marche appuyé durant 3 secondes.
- Envoyer le firmware à l'aide de ``make cload``.

## Formatage

Le formatage est exécuté à l'aide de [*clang-format*](https://clang.llvm.org/docs/ClangFormat.html) qui suit le
standard [**LLVM**](https://llvm.org/docs/CodingStandards.html).

Afin de valider le formatage, il est possible de lancer :
```bash
./format.sh
```

L'ajout de l'option ``-f`` permet de formater les fichiers.
