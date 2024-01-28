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
— `Result getNextResult()`:
— `void provideResult(Result result)`:

**Tests**

## Etape 2
- `
**Tests**
1. 

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

