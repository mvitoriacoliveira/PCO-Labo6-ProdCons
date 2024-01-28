En plus des tests fournis, nous avons effectué les tests suivants: 

| Test | Status |
| ---- | ----- |

A faire:
- Performance Test:
        Create a scenario where a large number of computations are requested and processed. Measure the time it takes for the computations to complete and check if the results are correct.
- Edge Cases:
        Test with extreme cases, such as having only one computation type, having only one compute engine, or having a very high buffer capacity.
    


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