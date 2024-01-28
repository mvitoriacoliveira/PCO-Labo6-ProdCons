# Labo 6 - Producteur-Consommateur pour calcul différé (Hoare)

Auteur·ices: Vitória Oliveira et Samuel Roland

## Attributs partagées
```c++
constexpr static std::size_t NB_COMPUTATION_TYPES = static_cast<std ::size_t>(ComputationType::Count);
// Où count est le dernier élément de l'enum ComputationType permettant ainsi de récupérer le nombre de types différents. 
```
Cette constante nous permet de facilement dimensionner et parcourir nos conteneurs.

```c++
// Buffer structure
std::array<std::map<int, Request>, NB_COMPUTATION_TYPES> requests;
std::map<int, std::optional<Result>> results;
```
Ces deux structures représentent notre buffer. 
La première sert à stocker les requêtes de calculs qui n'ont pas encore été attribuées à des calculateurs. Chaque type de computation (ComputationType) possède sa propre map indexée par id. 
Le deuxième conteneur permet de stocker les résultats des calculs en attente d'être renvoyés au client. Le type optional nous permet d'allouer notre résultat dans la map avant d'avoir le vrai résultat du calculateur afin d'assurer l'ordre, l'attente et l'annulation des retours des calculs.   

L'utilisation des mapes nous permet de facilement accéder et retirer un élément par son id, mais aussi en utilisant l'id comme clé, cela nous assure que les requêtes et les résultats sont toujours ordonnés et nous pouvons facilement accéder à l'élément avec le plus petit id via l'itérateur begin. 

```c++
// Conditions
std::array<std::unique_ptr<Condition>, NB_COMPUTATION_TYPES> notFull;
std::array<std::unique_ptr<Condition>, NB_COMPUTATION_TYPES> notEmpty;
Condition nextResultReady;
```
1. `notFull`
    Permet de faire attendre les clients lorsque la file des requêtes de calculs est pleine (cad elle a atteint `MAX_TOLERATED_QUEUE_SIZE`).
2. `notEmpty`
    Permet de faire attendre les calculateurs lorsque la file de requêtes de calculs est vide. 
3. `nextResultReady`
   Permet de rêlacher le thread attendant sur le prochain résultat. 


## Etape 1
— `int requestComputation(Computation c)`
    Elle bloque si le conteneur du type de computation demandé est plein. 
    Cette méthode crée une requête de calcul et l'insère dans `requests` du type de computation passé en paramètre. En plus, elle place à la fin de la map `results` un nouvel résultat nul.
    Elle permet de réveiller un thread qui attend sur la condition `notEmpty` du type de computation donné. 
    Finalement, elle retourne l'id assigné à la demande de calcul. 
— `Request getWork(ComputationType computationType)`
    Cette méthode bloque lorsque `requests` est vide. Sinon, elle accède au premier élément de celui-ci et récupère la requête qu'y est stockée. Une fois ceci effectué, la requête est rétirée du conteneur. 
    Elle permet de réveiller un thread qui attend sur la condition `notFull` du type de computation de la requête demandée. 

**Tests**
En plus des tests fournis, nous avons vérifié le bon fonctionnement de ces méthodes en effectuant des tests manuels via l'interface et en consultant les journaux d'exécution (logs). Nous avons initié plusieurs demandes de calcul sur les différents calculateurs et nous avons vérifié que les identifiants étaient attribués dans le bon ordre. Nous nous sommes assurés que les demandes de calcul étaient également traitées dans le bon ordre, que les requêtes étaient récupérées par les calculateurs appropriés et dans le bon ordre. De plus, nous avons confirmé que chaque calculateur obtenait une requête de calcul lorsqu'il était effectivement disponible

## Etape 2
— `Result getNextResult()`
    Cette méthode bloque si `results` est vide ou si le premier `Result` dans ce conteneur n'a pas encore de valeur (cad le calcul n'est pas encore fini et retourné). Autrement, elle copie le premier résultat qu'y est stocké, l'efface du conteneur et le retourne. 
    Elle permet de réveiller un thread qui attend sur le prochain résultat à retourner. 

— `void provideResult(Result result)`
    Cette méthode insère le resultat passé en paramètre dans `results` à la position de l'id de `result`. 
    Elle permet de réveiller un thread qui attend sur le prochain résultat à retourner. 

**Tests**
Autre que les tests déjà fournis, nous avons effectués plusieurs tests via l'interface. Nous avons vérifié que les résultats étaient retournés dans le bon ordre et le cas où `getNextResult()` est censé bloquer. 

## Etape 3
1. 
**Tests**
1. 

## Etape 4

```c++
bool stopped = false;
void stopExecutionIfEndOfService(Condition &cond);
```

**Tests**
1. 

