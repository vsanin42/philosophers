
# philosophers

A project for 42 School aimed at learning concurrent programming by solving the [Dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem).

The project is split into two parts (mandatory and bonus) based on the approach to solving the problem.

The mandatory part focuses on multithreaded environment and utilizes mutexes to prevent data races. Each philosopher is represented by a thread, each fork between them is represented by a mutex. The goal was to set up a balanced thread routine for each philosopher so that everyone has enough time to eat and sleep to prevent starvation, all while running independently without communicating. The main simulation management is done by a monitor thread which allows each thread to return in case of a death or reaching the "full" status.

In the bonus part the core logic remains the same, however each philosopher is no longer a thread but a process, and all forks are represented by a single semaphore. The mutexes are replaced by binary semaphores. Process termination is also handled using semaphores. Monitoring is done separately within each process by dedicated threads. It allows each process to terminate on its own, which avoids sending signals and causing memory leaks when the processes don't terminate correctly.
## Use
To run the mandatory part:
```bash
  ./philo [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]
```
To run the bonus part, replace ```./philo``` with ```./philo_bonus```.

Arguments:
- ```number_of_philosophers```: The number of philosophers and also the number of forks.
- ```time_to_die``` (in milliseconds): If a philosopher didn’t start eating ```time_to_die``` milliseconds since the beginning of their last meal or the beginning of the simulation, they die.
- ```time_to_eat``` (in milliseconds): The time it takes for a philosopher to eat. During that time, they will need to hold two forks.
- ```time_to_sleep``` (in milliseconds): The time a philosopher will spend sleeping.
- ```number_of_times_each_philosopher_must_eat``` (optional argument): If all philosophers have eaten at least ```number_of_times_each_philosopher_must_eat``` times, the simulation stops. If not specified, the simulation stops when a philosopher dies.

All performed actions are logged to the terminal output in the following format:

```timestamp_in_ms X action```, where ```X``` is the philosopher ID and ```action``` is taking a fork, eating, sleeping, thinking or dying.
