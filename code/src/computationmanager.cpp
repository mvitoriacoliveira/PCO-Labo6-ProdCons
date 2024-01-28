//     ____  __________     ___   ____ ___  _____ //
//    / __ \/ ____/ __ \   |__ \ / __ \__ \|__  / //
//   / /_/ / /   / / / /   __/ // / / /_/ / /_ <  //
//  / ____/ /___/ /_/ /   / __// /_/ / __/___/ /  //
// /_/    \____/\____/   /____/\____/____/____/   //
// Auteur·ices: Vitória Oliveira et Samuel Roland
// Date: 28.01.2024


// A vous de remplir les méthodes, vous pouvez ajouter des attributs ou méthodes pour vous aider
// déclarez les dans ComputationManager.h et définissez les méthodes ici.
// Certaines fonctions ci-dessous ont déjà un peu de code, il est à remplacer, il est là temporairement
// afin de faire attendre les threads appelants et aussi afin que le code compile.

#include "computationmanager.h"

ComputationManager::ComputationManager(int maxQueueSize) : MAX_TOLERATED_QUEUE_SIZE((size_t) maxQueueSize) {
    // Initialize the request vector with vectors of the specified size for each ComputationType
    for (size_t i = 0; i < NB_COMPUTATION_TYPES; ++i) {
        notFull.at(i) = (std::make_unique<Condition>());
        notEmpty.at(i) = (std::make_unique<Condition>());
    }
}

// Client
int ComputationManager::requestComputation(Computation c) {
    size_t computationIndex = static_cast<size_t>(c.computationType);

    monitorIn();

    // Wait while the queue of the current computation type is full
    while (!stopped && requests.at(computationIndex).size() == MAX_TOLERATED_QUEUE_SIZE) {
        wait(*notFull.at(computationIndex));
    }

    stopExecutionIfEndOfService(*notFull.at(computationIndex));

    // Add the request to the queue
    int id = nextId++;

    requests.at(computationIndex).emplace(id, Request(c, id));

    // Preallocate a result object in results to ensure following results are not returned before
    results.emplace(id, std::nullopt);

    // Signal that the queue is not empty
    signal(*notEmpty.at(computationIndex));

    monitorOut();
    return id;
}

void ComputationManager::abortComputation(int id) {
    monitorIn();

    size_t computationIndex = 0;

    // Si la requête/resultat de calcul se trouve dans le buffer
    for (auto &requestMapPerType: requests) {
        auto it = requestMapPerType.find(id);
        if (it != requestMapPerType.end()) {
            requestMapPerType.erase(it);

            //Signal the list of the given computation is not full anymore
            signal(*notFull.at(computationIndex));
            break;
        }

        computationIndex++;
    }

    //Delete the result
    if (results.erase(id) > 0) {
        //In case, there was a deletion, signal the first thread waiting
        if (!results.empty() && results.begin()->second.has_value())
            signal(nextResultReady);
    }

    monitorOut();
}

Result ComputationManager::getNextResult() {
    monitorIn();

    //We have to check or recheck that the next result to return is received
    while (!stopped && (results.empty() || !results.begin()->second.has_value())) {
        wait(nextResultReady);
    }

    stopExecutionIfEndOfService(nextResultReady);

    Result res = results.begin()->second.value();
    results.erase(results.begin());

    //Signal the next thread that could be waiting on the next result
    signal(nextResultReady);

    monitorOut();

    return res;
}
// End Client

// Calculateur
Request ComputationManager::getWork(ComputationType computationType) {
    size_t computationIndex = static_cast<size_t>(computationType);

    // Filled with arbitrary code in order to make the callers wait
    monitorIn();

    // Wait while empty
    while (!stopped && requests.at(computationIndex).empty()) {
        wait(*notEmpty.at(computationIndex));
    }

    stopExecutionIfEndOfService(*notEmpty.at(computationIndex));

    // Get the request for specified type
    // We take the element with the smallest id, as a map is indexed by id and ordered by index
    // we can just take the element pointed by begin().
    Request request = requests.at(computationIndex).begin()->second;

    // Remove the request from the map because it is assigned to a calculator
    requests.at(computationIndex).erase(requests.at(computationIndex).begin());

    // Signal that the queue is not full
    signal(*notFull.at(computationIndex));

    monitorOut();

    return request;
}

bool ComputationManager::continueWork(int id) {
    monitorIn();

    if (stopped) {
        return false;
    }

    //If the results still has a pre allocated result with this id, we can continue
    auto found = results.find(id) != results.end();

    monitorOut();

    return found;
}

void ComputationManager::provideResult(Result result) {
    monitorIn();

    //Define the pre allocated result in results map
    results.at(result.getId()) = result;

    //Signal potential waiting clients
    signal(nextResultReady);

    monitorOut();
}
// End Calculateur

void ComputationManager::stop() {
    monitorIn();

    stopped = true;

    //Lets signal all sleeping threads
    signal(nextResultReady);

    for (size_t i = 0; i < NB_COMPUTATION_TYPES; ++i) {
        signal(*notFull.at(i));
        signal(*notEmpty.at(i));
    }

    monitorOut();
}

// When the service is stopped, we want to wake up all sleeping threads in cascade
//and stop with an exception
void ComputationManager::stopExecutionIfEndOfService(Condition &cond) {
    if (stopped) {
        signal(cond);
        monitorOut();
        throwStopException();
    }
}