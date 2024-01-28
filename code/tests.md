En plus des tests fournis, nous avons effectué les tests suivants: 

| Test | Status |
| ---- | ----- |

A faire:
- Performance Test:
        Create a scenario where a large number of computations are requested and processed. Measure the time it takes for the computations to complete and check if the results are correct.
- Edge Cases:
        Test with extreme cases, such as having only one computation type, having only one compute engine, or having a very high buffer capacity.

-  When any requests are issued while its corresponding worker is busy, the requests wait. Each result is
returned in order
- When the buffer for a given type is full, and an unfinished computation of the same type is aborted,
another computation request of the same type may be added.
- When a result for a computation is available but is waiting on a computation of another type to finish
before being displayed, cancellation of the computation will not display the result    
- A cancellation request for an already cancelled workload is logged, but ignored.
- A cancellation request for an already finished and displayed workload is logged, but ignored.
- When no request are present, the program stops immediately and the threads are joined.
- When the program is stopped and a request for a new computation is issued, the request is ignored.
- When the program is stopped and a cancellation request is issued, the cancellation is ignored.

Tests fournis: (Liste pour nous)
- When the queues can contain 2 elements we should block with the third deposit
- When the queues can hold two items we should at least be able to deposit two in each
- Worker should go through with getWork correctly if there's a request in queue
- A Result Should Arrive (if precendant case is true)
- Worker should wait (getWork should block) if there's no request
- Id order of requests and results is correct when multiple requests
- Worker Should Not Continue Working On Aborted Task
- Worker Should Not Get Aborted Task if requests is aborted before worker getWork
- Worker should not get abortet task if there's no tasks
- Aborted result should not come back
- Abort should release client waiting on full queue
- Buffer should throw exception if getNextResult/getWork is called after stop()
- Client should be released if stop is called and there are clients waiting on conditions
- The results should arrive in the correct order and hold the correct value 
