# Système général
But: mise en place d’un système permettant l’exécution
concurrente et différée de calculs. 

- Distribuer les calculs à faire.
- Réaliser ces calculs de manière concurrente.
- Client/Calculateur sont des threads
- Buffer est un moniteur de Hoare, gère la synchronisation des
accès concurrents du client et des calculateurs ainsi que les attentes. 
- 2 buffers circulaires:
    B1: client est prod & calculateur est lecteur
    Client met des calculs à réaliser dans le buffer
    Calculateur lit un calcul dans le buffer.

    B2: calculateur est prod & client est lecteur
    Calculateur met le calcul réalisé dans le buffer
    Client lit le calcul dans le buffer

# Client

- Donne un calcul à effectuer et récupère l'id de ce calcul. 
- Annule un calcul grâce à un id. 
- Récupère un résultat (quelconque.. donc il doit être dans le bon ordre) 

# Calculateur
Effectuent un seul type de calcul (computationType)

- Récupère le calcul à réaliser. S’il n’y a pas de calcul à effectuer, l’appel de getWork() mettra le thread en attente sur une condition dans le moniteur (buffer) et il sera  réveillé lorsqu’il y en aura. 
- Réalise le calcul et le l'écrit dans le buffer
- Doit vérifier à chaque tour si le calcul est annulé, si c'est le cas il arrête le calcul, ne renvoie pas de résultat et demande à nouveau un travail. 
- 

# Buffers

- Nombre de requêtes de calcul en attente de chaque type est limité. 
- Requêtes conservées par type de requête. 
- Il faut réordonnancer leurs résultats afin que le client puisse récupérer les résultats dans l'ordre de demande. Il ne faut pas oublier que le réordonnacement doit prendre en compte si des requêtes ont été annulées ou pas. 
    — Si la requête de calcul se trouve dans le buffer lors de l’annulation elle devra être éliminée.
    — Si un calculateur travaille déjà sur la requête il devra arrêter le calcul au prochain appel de
    continueWork(id) et ne pas retourner le résultat.
    — Si le résultat est déjà parvenu au buffer il devra être éliminé.
    — Si le client annule un calcul dont le résultat a déjà été consommé par getNextResult() il ne doit
    rien se passer. De même s’il demande d’annuler un identifiant inconnu.

- Doit libérer les threads clients/calculateurs lors d'un arrêt de système. 
