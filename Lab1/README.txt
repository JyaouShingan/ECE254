In our project, HelloWorld, the source code helloworld.c contains three tasks.
The first task will print out the increasing number around every 1 seconds, and it will kill itself after 10 counts.
The second task will print out "HelloWorld!" around every 3 seconds.
The third task will check the number of exising active task every 5 seconds and print out the number.

When the program starts running, it the third task will print the number of current active tasks. It will be 4 if the idle task is active.
And when the first task kills itself, this number should decreased by 1.