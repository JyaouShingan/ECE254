There are 5 tasks in our tests

Task1(prio 1): Original infinity loop.
Task2(prio 5): Periodically print out all task stats.
Task3(prio 3): Will try to alloc 15 memory blocks after task4 already alloc 10 (using semaphores)
Task4(prio 4): Alloc 10 memory blocks, delay few seconds and then free those blocks
Task5(prio 2): Trying to alloc memory blocks infinitely.

The user defined memory pool "user_mem" has 20 blocks of available memory.

TIMELINE:
The init task has the highest priority and will create all five tasks first.
Task 2 will start to print out all task info periodically
Task 4 starts to alloc 10 memory blocks from user_mem.
Task 4 signal the semaphore and Task 3 get the chance to run.
Task 3 alloc 10 memory blocks and then be blocked since all 20 blocks were allocated.
Task 5 will be blocked as well since no memory block is available.
Both task 3 and task 5 are in WAIT_MEM state.
Task 4 reaches delay and starts free 10 memory blocks
Task 3 get chance to alloc new memory since it has higher priority than task 5.
Task 3 allocates the rest 5 memory blocks and finished.
Task 5 then get chance to alloc new memory.
Task 5 get blocked after allocates 5 blocks since no more block is available.
