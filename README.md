# ⭐ Process Scheduling Algorithms
This C++ program implements three different process scheduling algorithms: First Come First Serve (FCFS), Shortest Job First (SJF), and Round Robin. These algorithms are widely used in operating systems to manage the execution of processes efficiently.

## 🏷️ Features
**- FCFS Scheduling**
**- SJF Scheduling**
**- Round Robin Scheduling with customizable time quantum**

### FCFS Scheduling (First Come First Serve)
FCFS (First Come First Serve) is one of the simplest scheduling algorithms. In FCFS scheduling, processes are executed in the order they arrive in the ready queue. The CPU is assigned to the first process that arrives and remains allocated to that process until it completes its execution.

**- Characteristics:**

- Non-preemptive: Once a process starts executing, it continues until it completes its burst time.
- Simple and easy to implement.
- Can lead to poor turnaround time, especially if long processes arrive first (convoy effect).

### SJF Scheduling (Shortest Job First) - Preemptive
In the preemptive version of SJF (Shortest Job First) scheduling, the CPU can be taken away from a currently executing process if a new process arrives with a shorter burst time. The scheduler compares the remaining burst times of the processes in the ready queue and selects the one with the shortest remaining burst time to execute next.

**- Characteristics:**

- Preemptive: If a new process arrives with a shorter burst time than the currently executing process, the CPU is preempted from the current process and allocated to the new process.
- Provides better response time compared to non-preemptive SJF.
- Requires knowledge of burst times in advance, which may not always be available.
- Can lead to starvation for longer processes if a constant stream of short processes keeps arriving.

### Round Robin Scheduling with Customizable Time Quantum
Round Robin is a preemptive scheduling algorithm commonly used in time-sharing systems. Each process is assigned a fixed time slice (quantum), and the CPU scheduler switches between processes in a circular manner. If a process's quantum expires, it is preempted and added to the end of the ready queue.

**- Characteristics:**

- Preemptive: Each process gets a small unit of CPU time (quantum), and if it does not complete within this time, it is preempted.
- Fairness: Ensures fairness among processes by giving each process a share of CPU time.
- Higher context switch overhead: Due to frequent context switches between processes.
- Performance heavily depends on the choice of the time quantum; too small a quantum may lead to excessive context switching, while too large a quantum may decrease responsiveness.
