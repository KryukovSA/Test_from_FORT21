//
// Created by sergeyk on 03.07.25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct {
    unsigned long user, nice, system, idle, iowait, irq, softirq, steal;
} CPUStats;

static int sockfd = -1;
static struct sockaddr_in servaddr;

void InitUdpSocket() {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(1234);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
}

void CleanupUdpSocket() {
    if (sockfd != -1) {
        close(sockfd);
        sockfd = -1;
    }
}

void ReadCpuStats(CPUStats *stats) {
    FILE *file = fopen("/proc/stat", "r");
    if (!file) {
        perror("Failed to open /proc/stat");
        exit(EXIT_FAILURE);
    }

    char line[256];
    fgets(line, sizeof(line), file);
    sscanf(line + 5, "%lu %lu %lu %lu %lu %lu %lu %lu",
           &stats->user, &stats->nice, &stats->system, &stats->idle,
           &stats->iowait, &stats->irq, &stats->softirq, &stats->steal);
    fclose(file);
}

float CalculateCpuUsage(const CPUStats *prev, const CPUStats *curr) {
    unsigned long prev_total = prev->user + prev->nice + prev->system + prev->idle +
                             prev->iowait + prev->irq + prev->softirq + prev->steal;
    unsigned long curr_total = curr->user + curr->nice + curr->system + curr->idle +
                             curr->iowait + curr->irq + curr->softirq + curr->steal;

    unsigned long total_diff = curr_total - prev_total;
    unsigned long idle_diff = curr->idle - prev->idle;

    if (total_diff == 0) return 0.0f;
    return 100.0f * (total_diff - idle_diff) / total_diff;
}

void SendUdpMessage(float usage) {
    if (sockfd == -1) {
        fprintf(stderr, "Socket not initialized\n");
        return;
    }

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%.2f", usage);
    sendto(sockfd, buffer, strlen(buffer), 0,
          (struct sockaddr *)&servaddr, sizeof(servaddr));
}

int main() {
    InitUdpSocket();
    atexit(CleanupUdpSocket);
    CPUStats prev, curr;
    ReadCpuStats(&prev);

    while (1) {
        sleep(1);
        ReadCpuStats(&curr);
        float usage = CalculateCpuUsage(&prev, &curr);
        prev = curr;

        SendUdpMessage(usage);
        printf("CPU Usage: %.2f%%\n", usage);
    }

    return 0;
}
