//     ____  __________     ___   ____ ___  _____ //
//    / __ \/ ____/ __ \   |__ \ / __ \__ \|__  / //
//   / /_/ / /   / / / /   __/ // / / /_/ / /_ <  //
//  / ____/ /___/ /_/ /   / __// /_/ / __/___/ /  //
// /_/    \____/\____/   /____/\____/____/____/   //
// Auteurs : Prénom Nom, Prénom Nom


// A vous de remplir les méthodes, vous pouvez ajouter des attributs ou méthodes pour vous aider
// déclarez les dans ComputationManager.h et définissez les méthodes ici.
// Certaines fonctions ci-dessous ont déjà un peu de code, il est à remplacer, il est là temporairement
// afin de faire attendre les threads appelants et aussi afin que le code compile.

#include "computationmanager.h"


ComputationManager::ComputationManager(int maxQueueSize) : MAX_TOLERATED_QUEUE_SIZE(maxQueueSize) {
    // TODO

    notFull.reserve(NB_COMPUTATION_TYPES);
    notEmpty.reserve(NB_COMPUTATION_TYPES);

    // Initialize the request vector with vectors of the specified size for each ComputationType
    for (int i = 0; i < NB_COMPUTATION_TYPES; ++i) {
        //requests[i] = std::map<int, Request>();
        notFull.push_back(std::make_unique<Condition>());
        notEmpty.push_back(std::make_unique<Condition>());
    }
}

// Client
// Potentiellement bloquante
int ComputationManager::requestComputation(Computation c) {
    // TODO
    monitorIn();

    // Wait while full
    while (requests[static_cast<size_t>(c.computationType)].size() == MAX_TOLERATED_QUEUE_SIZE) {
        wait(*notFull[static_cast<size_t>(c.computationType)]);
    }

    // Add the request to the queue
    int id = nextId++;
    requests[static_cast<size_t>(c.computationType)].emplace(id, Request(c, id));

    // Add a result object to result to ensure correct order
    results.emplace(id, std::nullopt);

    // Signal that the queue is not empty
    signal(*notEmpty[static_cast<size_t>(c.computationType)]);

    monitorOut();
    return id;
}

/*Pas bloquante (pas d'appel à wait). Cette méthode permet d’annuler un calcul en cours grâce à son identifiant.*/
void ComputationManager::abortComputation(int id) {
    // TODO
    monitorIn();
    
    // Si la requête/resultat de calcul se trouve dans le buffer
    for(auto& requestMapPerType : requests){
        ComputationType currentType = static_cast<ComputationType>(&requestMapPerType - &requests[0]);

        auto it = requestMapPerType.find(id);
        if(it != requestMapPerType.end()){
            requestMapPerType.erase(it);
            signal(*notFull[static_cast<size_t>(currentType)]);

            // Il faudra libérer le thread qui attend  sur le résultat de la requete/résultat interrompu
            //bool threadIsWaitingOnResult = (results[id] == results.begin() ? true : false);
            results.erase(id);
            //if(threadIsWaitingOnResult){
            //    signal(newResult);
            //}
        }
    }    

    monitorOut();
}

/*L’appel à getNextResult() est potentiellement bloquant si le prochain
résultat n’est pas encore disponible*/
Result ComputationManager::getNextResult() {
    // TODO
    // Replace all of the code below by your code

    // Filled with some code in order to make the thread in the UI wait
    monitorIn();

    // !results[0].has_value doit retourner faux si 
    while(results.empty() || !results.begin()->second.has_value()){
        //wait(notEmpty); //TODO how to get computation type?
        wait(newResult);
    }

    Result res = results.begin()->second.value();
    results.erase(results.begin());

    //signal(notFull); //TODO how to get computation type?

    monitorOut();

    return res;
}
// End Client

// Calculateur
// Potentiellement bloquante
/* Cette méthode permet au calculateur de demander du travail du type computationType,
qu’il reçoit sous forme d’une requête de calcul. S’il n’y a pas de calcul à effectuer, l’appel de getWork() mettra le thread en attente sur une condition
dans le moniteur (buffer) et il sera réveillé lorsqu’il y en aura. */
Request ComputationManager::getWork(ComputationType computationType) {
    // TODO
    // Replace all of the code below by your code

    // Filled with arbitrary code in order to make the callers wait
    monitorIn();

    // Wait while empty
    while (requests[static_cast<size_t>(computationType)].empty()) {
        wait(*notEmpty[static_cast<size_t>(computationType)]);
    }

    // Get the request for specified type
    // (Gets always first request in the map because request is removed from requests map as it is assigned to a calculator)
    Request request = requests[static_cast<size_t>(computationType)][0];

    // Remove the request from the map
    requests[static_cast<size_t>(computationType)].erase(requests[static_cast<size_t>(computationType)].begin());

    // Signal that the queue is not full
    signal(*notFull[static_cast<size_t>(computationType)]);

    monitorOut();

    return request;
}

/*Pas bloquante (pas d'appel à wait). Si un calculateur travaille déjà sur la requête il devra arrêter le calcul au prochain appel de
continueWork(id) et ne pas retourner le résultat.*/
bool ComputationManager::continueWork(int id) {
    // TODO
    return true;
}

/*Cette méthode permet au calculateur de retourner le résultat du calcul.*/
void ComputationManager::provideResult(Result result) {
    // TODO
    monitorIn();

    // Put result in results map
    results[result.getId()] = result;

    signal(newResult);

    monitorOut();

}
// End Calculateur

void ComputationManager::stop() {
    // TODO
}
