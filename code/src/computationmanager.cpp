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

    // Signal that the queue is not empty
    signal(*notEmpty[static_cast<size_t>(c.computationType)]);

    monitorOut();
    return id;
}

void ComputationManager::abortComputation(int id) {
    // TODO
}

Result ComputationManager::getNextResult() {
    // TODO
    // Replace all of the code below by your code

    // Filled with some code in order to make the thread in the UI wait
    monitorIn();
    auto c = Condition();
    wait(c);
    monitorOut();

    return Result(-1, 0.0);
}
// End Client

// Calculateur
// Potentiellement bloquante
Request ComputationManager::getWork(ComputationType computationType) {
    // TODO
    // Replace all of the code below by your code

    // Filled with arbitrary code in order to make the callers wait
    monitorIn();

    // Wait while empty
    while (requests[static_cast<size_t>(computationType)].empty()) {
        wait(*notEmpty[static_cast<size_t>(computationType)]);
    }

    // Get the request
    Request request = requests[static_cast<size_t>(computationType)][0];

    // Remove the request from the queue
    requests[static_cast<size_t>(computationType)].erase(requests[static_cast<size_t>(computationType)].begin());

    // Signal that the queue is not full
    signal(*notFull[static_cast<size_t>(computationType)]);

    /*
    auto c = Condition();
    wait(c);
    */
    monitorOut();

    return Request(Computation(computationType), -1);
}

bool ComputationManager::continueWork(int id) {
    // TODO
    return true;
}

void ComputationManager::provideResult(Result result) {
    // TODO
}
// End Calculateur

void ComputationManager::stop() {
    // TODO
}
