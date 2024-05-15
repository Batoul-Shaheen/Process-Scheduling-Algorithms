#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <climits>

using namespace std;

const int MAX_PROCESSES = 100;

struct Process {
    int pid, arrival, burst;
};

struct ProcessInfo {
    int pid, at, bt, ct, wt, tat, start_time, bt_remaining;
};

void pline(int x) {
    for (int i = 0; i < x; i++) {
        cout << "-";
    }
    cout << endl;
}

bool comparatorAT(const ProcessInfo& a, const ProcessInfo& b) {
    return a.at < b.at;
}

void fcfs(int n, vector<ProcessInfo>& ps) {
    float sum_tat = 0, sum_wt = 0, cpu_utilization = 0;
    int total_idle_time = 0, length_cycle = 0;

    sort(ps.begin(), ps.end(), comparatorAT);

    for (int i = 0; i < n; i++) {
        ps[i].start_time = (i == 0) ? ps[i].at : max(ps[i].at, ps[i - 1].ct);
        ps[i].ct = ps[i].start_time + ps[i].bt;
        ps[i].tat = ps[i].ct - ps[i].at;
        ps[i].wt = ps[i].tat - ps[i].bt;

        sum_tat += ps[i].tat;
        sum_wt += ps[i].wt;
        total_idle_time += (i == 0) ? 0 : (ps[i].start_time - ps[i - 1].ct);
    }
    length_cycle = ps[n - 1].ct - ps[0].start_time;

    pline(44);
    cout << "\nProcess No.\tAT\tBurst Time\tFinish Time\tTAT\tWT\n";
    for (int i = 0; i < n; i++)
        cout << i << "\t\t" << ps[i].at << "\t   " << ps[i].bt << "\t\t" << ps[i].ct << "\t\t" << ps[i].tat << "\t" << ps[i].wt << endl;
    cout << endl;

    cpu_utilization = (float)(length_cycle - total_idle_time) / length_cycle;

    cout << "\nAverage Turn Around time= " << sum_tat / n;
    cout << "\nAverage Waiting Time= " << sum_wt / n;
    cout << "\nThroughput= " << n / (float)length_cycle;
    cout << "\nCPU Utilization(Percentage)= " << cpu_utilization * 100;
}

void sjf(int n, vector<ProcessInfo>& ps) {
    bool is_completed[MAX_PROCESSES] = { false }, is_first_process = true;
    int current_time = 0;
    int completed = 0;
    int sum_tat = 0, sum_wt = 0, total_idle_time = 0, prev = 0, length_cycle;
    float cpu_utilization;
    int max_completion_time, min_arrival_time;

    while (completed != n) {
        int min_index = -1;
        int minimum = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (ps[i].at <= current_time && !is_completed[i]) {
                if (ps[i].bt < minimum) {
                    minimum = ps[i].bt;
                    min_index = i;
                }
                if (ps[i].bt == minimum) {
                    if (ps[i].at < ps[min_index].at) {
                        minimum = ps[i].bt;
                        min_index = i;
                    }
                }
            }
        }

        if (min_index == -1) {
            current_time++;
        }
        else {
            ps[min_index].start_time = current_time;
            ps[min_index].ct = ps[min_index].start_time + ps[min_index].bt;
            ps[min_index].tat = ps[min_index].ct - ps[min_index].at;
            ps[min_index].wt = ps[min_index].tat - ps[min_index].bt;

            sum_tat += ps[min_index].tat;
            sum_wt += ps[min_index].wt;
            total_idle_time += (is_first_process ? 0 : (ps[min_index].start_time - prev));

            completed++;
            is_completed[min_index] = true;
            current_time = ps[min_index].ct;
            prev = current_time;
            is_first_process = false;
        }
    }

    max_completion_time = INT_MIN;
    min_arrival_time = INT_MAX;
    for (int i = 0; i < n; i++) {
        max_completion_time = max(max_completion_time, ps[i].ct);
        min_arrival_time = min(min_arrival_time, ps[i].at);
    }
    length_cycle = max_completion_time - min_arrival_time;

    cpu_utilization = (float)(length_cycle - total_idle_time) / length_cycle;

    pline(44);
    cout << "\nProcess No.\tAT\tCPU Burst Time\tFinish Time\tTAT\tWT\n";
    for (int i = 0; i < n; i++)
        cout << i << "\t\t" << ps[i].at << "\t   " << ps[i].bt << "\t\t" << ps[i].ct << "\t\t" << ps[i].tat << "\t" << ps[i].wt << endl;
    cout << endl;

    cout << "\nAverage Turn Around time= " << (float)sum_tat / n;
    cout << "\nAverage Waiting Time= " << (float)sum_wt / n;
    cout << "\nThroughput= " << n / (float)length_cycle;
    cout << "\nCPU Utilization(Percentage)= " << cpu_utilization * 100;
}

void roundRobin(int n, vector<ProcessInfo>& ps, int quantum) {
    bool is_completed[MAX_PROCESSES] = { false };
    int current_time = 0;
    int completed = 0;
    int sum_tat = 0, sum_wt = 0, total_idle_time = 0, prev = 0, length_cycle;
    float cpu_utilization;
    int max_completion_time, min_arrival_time;

    while (completed != n) {
        for (int i = 0; i < n; i++) {
            if (ps[i].bt_remaining > 0) {
                if (ps[i].bt_remaining <= quantum) {
                    current_time += ps[i].bt_remaining;
                    ps[i].bt_remaining = 0;
                    completed++;

                    ps[i].ct = current_time;
                    ps[i].tat = ps[i].ct - ps[i].at;
                    ps[i].wt = ps[i].tat - ps[i].bt;

                    sum_tat += ps[i].tat;
                    sum_wt += ps[i].wt;
                    total_idle_time += (prev == 0) ? 0 : (ps[i].start_time - prev);

                    prev = current_time;
                }
                else {
                    current_time += quantum;
                    ps[i].bt_remaining -= quantum;
                }
            }
        }
    }

    max_completion_time = INT_MIN;
    min_arrival_time = INT_MAX;
    for (int i = 0; i < n; i++) {
        max_completion_time = max(max_completion_time, ps[i].ct);
        min_arrival_time = min(min_arrival_time, ps[i].at);
    }
    length_cycle = max_completion_time - min_arrival_time;

    cpu_utilization = (float)(length_cycle - total_idle_time) / length_cycle;

    pline(44);
    cout << "\nProcess No.\tAT\tCPU Burst Time\tFinish Time\tTAT\tWT\n";
    for (int i = 0; i < n; i++)
        cout << i << "\t\t" << ps[i].at << "\t   " << ps[i].bt << "\t\t" << ps[i].ct << "\t\t" << ps[i].tat << "\t" << ps[i].wt << endl;
    cout << endl;

    cout << "\nAverage Turn Around time= " << (float)sum_tat / n;
    cout << "\nAverage Waiting Time= " << (float)sum_wt / n;
    cout << "\nThroughput= " << n / (float)length_cycle;
    cout << "\nCPU Utilization(Percentage)= " << cpu_utilization * 100;
}


int main() {
    string filename;
    int x, q;

    cout << "Enter the filename containing process data: ";
    cin >> filename;

    ifstream infile(filename);
    if (!infile) {
        cerr << "Error: Could not open file." << endl;
        return 1;
    }

    vector<Process> processes;
    int pid, arrival, burst;
    while (infile >> pid >> arrival >> burst) {
        processes.push_back({ pid, arrival, burst });
    }
    infile.close();

    cout << "Enter the Context Switch Time (x): ";
    cin >> x;
    cout << "Enter the Quantum (Q): ";
    cin >> q;

    const int num_processes = processes.size();

    cout << "\nFCFS Scheduling:\n";
    vector<ProcessInfo> ps_fcfs(num_processes);
    for (int i = 0; i < num_processes; i++) {
        ps_fcfs[i].pid = processes[i].pid;
        ps_fcfs[i].at = processes[i].arrival;
        ps_fcfs[i].bt = processes[i].burst;
    }
    fcfs(num_processes, ps_fcfs);

    cout << "\n\nSJF Scheduling:\n";
    vector<ProcessInfo> ps_sjf(num_processes);
    for (int i = 0; i < num_processes; i++) {
        ps_sjf[i].pid = processes[i].pid;
        ps_sjf[i].at = processes[i].arrival;
        ps_sjf[i].bt = processes[i].burst;
        ps_sjf[i].bt_remaining = ps_sjf[i].bt;
    }
    sjf(num_processes, ps_sjf);

    cout << "\n\nRound Robin Scheduling with time quantum " << q << ":\n";
    vector<ProcessInfo> ps_rr(num_processes);
    for (int i = 0; i < num_processes; i++) {
        ps_rr[i].pid = processes[i].pid;
        ps_rr[i].at = processes[i].arrival;
        ps_rr[i].bt = processes[i].burst;
        ps_rr[i].bt_remaining = ps_rr[i].bt;
    }
    roundRobin(num_processes, ps_rr, q);

    return 0;
}
