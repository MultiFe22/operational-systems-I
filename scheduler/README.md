# First scheduler

To compile the scheduler, run the following command:

```gcc scheduler.c -o scheduler queue.c -Wall```

Then run the following command:

```./scheduler```

The scheduler works with round robin scheduling. If you want to change the scheduling algorithm, you can change the scheduling function in the scheduler.c file. To change the parameters, like the quantum, moddify the scheduler.h file.