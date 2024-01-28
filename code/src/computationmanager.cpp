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
#include <iostream>

ComputationManager::ComputationManager(int maxQueueSize) : MAX_TOLERATED_QUEUE_SIZE((size_t) maxQueueSize) {
    // TODO

    notFull.reserve(NB_COMPUTATION_TYPES);
    notEmpty.reserve(NB_COMPUTATION_TYPES);

    // Initialize the request vector with vectors of the specified size for each ComputationType
    for (size_t i = 0; i < NB_COMPUTATION_TYPES; ++i) {
        //requests.at(i] = std::map<int, Request>();
        notFull.push_back(std::make_unique<Condition>());
        notEmpty.push_back(std::make_unique<Condition>());
    }
}

//TODO: should we keep this ??
void ComputationManager::dumpRequests() {
    unsigned type = 0;
    std::cout << "Dumping requests maps" << std::endl;
    for (auto requestMap: requests) {
        std::cout << "map n: " << type << std::endl;
        for (auto request: requestMap) {
            std::cout << "request id:" << request.second.getId() << " and data: " << request.second.data << std::endl;
        }
        type++;
    }
}
// Client
// Potentiellement bloquante
int ComputationManager::requestComputation(Computation c) {
    // TODO
    size_t computationIndex = static_cast<size_t>(c.computationType);

    monitorIn();
    std::cout << "requestComputation computationIndex" << computationIndex << std::endl;

    // Wait while the queue of the current computation type is full
    while (requests.at(computationIndex).size() == MAX_TOLERATED_QUEUE_SIZE) {
        wait(*notFull.at(computationIndex));
    }

    // Add the request to the queue
    int id = nextId++;

    requests.at(computationIndex).emplace(id, Request(c, id));
    // std::cout << "id " << id << std::endl;
    // std::cout << "notEmpty " << notEmpty.size() << std::endl;
    // std::cout << "computationIndex " << computationIndex << std::endl;

    // Add a result object to result to ensure correct order
    results.emplace(id, std::nullopt);

    // Signal that the queue is not empty
    signal(*notEmpty.at(computationIndex));

    monitorOut();
    return id;
}

/*Pas bloquante (pas d'appel à wait). Cette méthode permet d’annuler un calcul en cours grâce à son identifiant.*/
void ComputationManager::abortComputation(int id) {
    // TODO
    monitorIn();

    size_t computationIndex = 0;

    // Si la requête/resultat de calcul se trouve dans le buffer
    for (auto &requestMapPerType: requests) {
        auto it = requestMapPerType.find(id);
        if (it != requestMapPerType.end()) {
            requestMapPerType.erase(it);

			//Signal the list of the given computation is not full anymore
            signal(*notFull.at(computationIndex));
        }

        computationIndex++;
    }
    results.erase(id);

    //Signal the thread waiting on the result that was aborted
    if (results.empty() || !results.begin()->second.has_value())
        signal(nextResultReady);

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

    //We have to check or recheck that we have the next result to use is received
    while (results.empty() || !results.begin()->second.has_value()) {
        wait(nextResultReady);
    }

    Result res = results.begin()->second.value();
    results.erase(results.begin());

    //Signal the next thread that could be waiting on the next result
    signal(nextResultReady);

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
    size_t computationIndex = static_cast<size_t>(computationType);

    // Filled with arbitrary code in order to make the callers wait
    monitorIn();

    // Wait while empty
    while (requests.at(computationIndex).empty()) {
        wait(*notEmpty.at(computationIndex));
    }

    // Get the request for specified type
    //We take the element with the smallest id, as a map is indexed by id and ordered by index
    //we can just take the element pointed by begin().
    Request request = requests.at(computationIndex).begin()->second;

    // Remove the request from the map because it is assigned to a calculator
    requests.at(computationIndex).erase(requests.at(computationIndex).begin());

    // Signal that the queue is not full
    signal(*notFull.at(computationIndex));

    monitorOut();

    return request;
}

/*Pas bloquante (pas d'appel à wait). Si un calculateur travaille déjà sur la requête il devra arrêter le calcul au prochain appel de
continueWork(id) et ne pas retourner le résultat.*/
bool ComputationManager::continueWork(int id) {
    return results.find(id) != results.end();
}

/*Cette méthode permet au calculateur de retourner le résultat du calcul.*/
void ComputationManager::provideResult(Result result) {
    // TODO
    monitorIn();

    // Put result in results map
    results.at(result.getId()) = result;

    signal(nextResultReady);

    monitorOut();
}
// End Calculateur

void ComputationManager::stop() {
    // TODO
}
