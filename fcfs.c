#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int process_id;
    int arrival_time;
    int execution_time;
    int wait_time;
    int turnaround_time;
} ProcessInfo;

// Function to implement First Come First Serve Scheduling
void FCFS(ProcessInfo *processes[], int num_processes) {
    int current_time = 0;
    for (int i = 0; i < num_processes; i++) {
        processes[i]->wait_time = current_time - processes[i]->arrival_time;
        processes[i]->turnaround_time = processes[i]->wait_time + processes[i]->execution_time;
        current_time += processes[i]->execution_time;
    }
}

int main() {
    FILE *input_file;
    input_file = fopen("sample_in_schedule.txt", "r");
    if (input_file == NULL) {
        printf("File could not be opened.\n");
        return -1;
    }

    int process_count = 0;
    char *buffer = NULL;
    size_t buf_size = 0;
    while (getline(&buffer, &buf_size, input_file) != -1) {
        process_count++;
    }

    rewind(input_file);

    ProcessInfo *process_array[process_count];
    for (int i = 0; i < process_count; i++) {
        process_array[i] = (ProcessInfo *)malloc(sizeof(ProcessInfo));
    }

    int index = 0;
    while (fscanf(input_file, "%d,%d,%d", &process_array[index]->process_id, &process_array[index]->arrival_time, &process_array[index]->execution_time) != EOF) {
        process_array[index]->wait_time = 0;
        process_array[index]->turnaround_time = 0;
        index++;
    }

    fclose(input_file);

    FCFS(process_array, index);

    int total_turnaround = 0, total_wait = 0, completion_time = 0;

    printf("Proc ID\tArrival\tBurst Time\tCompletion\tTurnaround\tWait Time\n");
    for (int i = 0; i < index; i++) {
        total_wait += process_array[i]->wait_time;
        total_turnaround += process_array[i]->turnaround_time;
        completion_time = process_array[i]->turnaround_time + process_array[i]->arrival_time;

        printf("%d\t%d\t%d\t\t%d\t\t%d\t\t%d\n", process_array[i]->process_id, process_array[i]->arrival_time, process_array[i]->execution_time, completion_time, process_array[i]->turnaround_time, process_array[i]->wait_time);
    }

    printf("The average wait time: %.2f\nThe average turn-around time: %.2f\n", (float)total_wait / index, (float)total_turnaround / index);

    for (int i = 0; i < process_count; i++) {
        free(process_array[i]);
    }

    return 0;
}
