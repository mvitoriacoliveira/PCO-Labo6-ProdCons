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
    /*
    requests.resize(ComputationType.values().length);
    // Initialize the request vector with vectors of the specified size for each ComputationType
    for (ComputationType type: {ComputationType::A, ComputationType::B, ComputationType::C}) {
        requests[type] = std::vector<Request>(maxQueueSize);
    }

    notFull.resize(ComputationType.values().length);
    */
}

// Client
// Potentiellement bloquante
int ComputationManager::requestComputation(Computation c) {
    // TODO
    monitorIn();
    // Wait while full
    /*
    while (requests[c.computationType].size() == MAX_TOLERATED_QUEUE_SIZE) {
        wait(notFull[c.computationType]);
    }
    */

    // TODO continuer...


    monitorOut();
    return -1;
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
    auto c = Condition();
    wait(c);
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
