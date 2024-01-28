En plus des tests fournis, nous avons effectué les tests suivants: 

| Test | Statut |
| ---- | ----- |


Liste: (à vérifié si déjà fait ou si coherent)
    Multiple Types of Computation:
        Request computations of types A, B, and C concurrently. Ensure that the buffer handles and distributes them correctly to the corresponding compute engines.

    Concurrency:
        Create multiple threads that concurrently request computations and provide results. Check if your buffer can handle and synchronize these concurrent operations correctly.

    Buffer Capacity:
        Set the buffer capacity to a low value (e.g., 2) and perform computations until the buffer is full. Verify that additional requests block until space becomes available.

    Cancel Computations:
        Request computations and then cancel some of them. Ensure that canceled computations do not produce results and that the buffer handles the cancellation properly.

    Mixed Type Computations:
        Request computations of various types and provide results in a different order. Check if the buffer maintains the correct order and associates results with the corresponding computations.

    Stop and Resume:
        Stop the computation manager, try to perform operations, and then resume it. Ensure that the manager stops accepting new requests, releases waiting threads, and resumes operations after being restarted.

    Performance Test:
        Create a scenario where a large number of computations are requested and processed. Measure the time it takes for the computations to complete and check if the results are correct.

    Edge Cases:
        Test with extreme cases, such as having only one computation type, having only one compute engine, or having a very high buffer capacity.

    Exception Handling:
        Intentionally create scenarios that should throw exceptions (e.g., trying to get results after stopping). Ensure that the exceptions are caught and handled appropriately.

    Randomized Testing:
        Create a randomized test where computations of different types are requested and results are provided in a random order. This can help uncover potential race conditions or synchronization issues.


(Liste pour nous)
Tests fournis: 
- When the queues can contain 2 elements we should block with the third deposit
- When the queues can hold two items we should at least be able to deposit two in each
- Worker should go through with getWork correctly if there's a request in queue
- A Result Should Arrive (if precendant case is true)
- Worker should wait (getWork should block) if there's no request
- Id order of requests and results is correct when multiple requests
- WorkerShouldNotContinueWorkingOnAbortedTask
- WorkerShouldNotGetAbortedTask if requests is aborted before worker getWork
