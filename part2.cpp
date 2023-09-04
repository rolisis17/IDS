#include <iostream>
#include <cstring>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

// Function to calculate checksum for TCP and IP headers
unsigned short calculateChecksum(unsigned short *buf, int nwords) {
    unsigned long sum;
    for (sum = 0; nwords > 0; nwords--) {
        sum += *buf++;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

int main() {
    int rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (rawSocket == -1) {
        std::cerr << "Error creating raw socket." << std::endl;
        return 1;
    }

    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons(80); // Change this to the port you want to scan
    target.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change this to the target IP address

    // IP header
    struct ip ipHeader;
    ipHeader.ip_hl = 5;
    ipHeader.ip_v = 4;
    ipHeader.ip_tos = 0;
    ipHeader.ip_len = sizeof(struct ip) + sizeof(struct tcphdr);
    ipHeader.ip_id = htons(54321);
    ipHeader.ip_off = 0;
    ipHeader.ip_ttl = 64;
    ipHeader.ip_p = IPPROTO_TCP;
    ipHeader.ip_sum = 0; // Will be filled in later
    ipHeader.ip_src.s_addr = inet_addr("192.168.1.100"); // Change this to your source IP address
    ipHeader.ip_dst = target.sin_addr;

    // TCP header
    struct tcphdr tcpHeader;
    tcpHeader.th_sport = htons(12345); // Change to a random source port
    tcpHeader.th_dport = target.sin_port;
    tcpHeader.th_seq = 0;
    tcpHeader.th_ack = 0;
    tcpHeader.th_x2 = 0;
    tcpHeader.th_off = 5;
    tcpHeader.th_flags = TH_SYN;
    tcpHeader.th_win = htons(5840);
    tcpHeader.th_sum = 0; // Will be filled in later
    tcpHeader.th_urp = 0;

    // Calculate TCP checksum
	char pseudoPacket[sizeof(struct ip) + sizeof(struct tcphdr)];
    std::memcpy(pseudoPacket, &ipHeader.ip_src.s_addr, 4);
    std::memcpy(pseudoPacket + 4, &ipHeader.ip_dst.s_addr, 4);
    pseudoPacket[8] = 0;
    pseudoPacket[9] = ipHeader.ip_p;
    pseudoPacket[10] = 0;
    pseudoPacket[11] = sizeof(struct tcphdr);
    std::memcpy(pseudoPacket + 12, &tcpHeader, sizeof(struct tcphdr));
    tcpHeader.th_sum = calculateChecksum((unsigned short *)pseudoPacket, 6);

    // Send the crafted packet
    if (sendto(rawSocket, &ipHeader, ipHeader.ip_len, 0, (struct sockaddr *)&target, sizeof(target)) == -1) {
        std::cerr << "Error sending packet." << std::endl;
        close(rawSocket);
        return 1;
    }

    close(rawSocket);
    return 0;
}