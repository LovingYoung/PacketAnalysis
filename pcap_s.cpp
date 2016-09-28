#include <pcap_s.h>
#include <string>
#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <ip_h.h>
#include <tcp_udp_h.h>
#include <pthread.h>
#include <mediaclass.h>
#include "homogenization.h"
#include "my_queue.h"

std::vector<std::string> Pcap::m_alldevs;
std::vector<std::string> Pcap::m_alldevsdescription;
char Pcap::errbuf[PCAP_ERRBUF_SIZE];
std::queue<std::string> Pcap::m_queue;

std::vector<std::string> Pcap::getAllDevs(){
    return m_alldevs;
}
std::queue<Homo *> myQueue::homoqueue;
int myQueue::mutex;

std::vector<std::string> Pcap::getAllDevsDescription(){
    return m_alldevsdescription;
}

void Pcap::refreshAllDevs(){
    Pcap::m_alldevs.clear();
    Pcap::m_alldevsdescription.clear();
    pcap_if_t * alldevs, *t;
    if(pcap_findalldevs(&alldevs, Pcap::errbuf) == -1){
        fprintf(stderr, "Fail to find all devs: %s\n", errbuf);
        exit(2);
    }
    for(t = alldevs; t != NULL; t = t->next){
        m_alldevs.push_back(t->name);
        if(t->description)
            m_alldevsdescription.push_back(t->description);
        else
            m_alldevsdescription.push_back(std::string());
    }
}

u_int16_t handle_ethernet(u_char * args, const pcap_pkthdr * pkthdr, const u_char * packet);

void my_callback(u_char *useless, const pcap_pkthdr *pkthdr, const u_char *packet){
    u_int16_t type = handle_ethernet(useless, pkthdr, packet);
    if(type == ETHERTYPE_IP){
        ip_header_t * ipHeader = (ip_header_t *) (packet + 14);
        tcp_header_t * tcpHeader;
        udp_header_t * udpHeader;
        switch(ipHeader->protocol){
        case 1: //ICMP
            break;
        case 2: //IGMP
            break;
        case 6:{ //TCP
            if((ipHeader->ver_ihl & 0x0F) == 5)
                tcpHeader = (tcp_header_t *)((char*)ipHeader + 20);
            else
                tcpHeader = (tcp_header_t *)((char*)ipHeader + 24);
            Homo *homo = new Homo();
            in_addr ip_addr;
            ip_addr.s_addr = ipHeader->dst_addr;
            homo->dst_ip = inet_ntoa(ip_addr);
            ip_addr.s_addr = ipHeader->src_addr;
            homo->src_ip = inet_ntoa(ip_addr);
            ether_header * eptr = (ether_header *) packet;
            homo->src_mac = ether_ntoa((const ether_addr *) & eptr->ether_shost);
            homo->dst_mac = ether_ntoa((ether_addr *) & eptr->ether_dhost);
            homo->protocol = "TCP";
            homo->packet_length = pkthdr->len;
            homo->tcp_header = tcpHeader;
            homo->udp_header = NULL;
            myQueue::homoqueue.push(homo);
            printf("%d", myQueue::homoqueue.size());
            fflush(stdout);
            break;
        }
        case 17:{ //UDP
            if((ipHeader->ver_ihl & 0x0F) == 5)
                udpHeader = (udp_header_t *)((char*)ipHeader + 20);
            else
                udpHeader = (udp_header_t *)((char*)ipHeader + 24);
            Homo *homo = new Homo();
            in_addr ip_addr;
            ip_addr.s_addr = ipHeader->dst_addr;
            homo->dst_ip = inet_ntoa(ip_addr);
            ip_addr.s_addr = ipHeader->src_addr;
            homo->src_ip = inet_ntoa(ip_addr);
            ether_header * eptr = (ether_header *) packet;
            homo->src_mac = ether_ntoa((const ether_addr *) & eptr->ether_shost);
            homo->dst_mac = ether_ntoa((ether_addr *) & eptr->ether_dhost);
            homo->protocol = "UDP";
            homo->packet_length = pkthdr->len;
            homo->udp_header = udpHeader;
            homo->tcp_header = NULL;
            myQueue::homoqueue.push(homo);
            printf("%d", myQueue::homoqueue.size());
            fflush(stdout);
            break;
        }
        }
    }
}

u_int16_t handle_ethernet(u_char *args, const pcap_pkthdr *pkthdr, const u_char *packet){
    ether_header * eptr;
    eptr = (ether_header *) packet;
    fprintf(stdout, "ethernet header source: %s", ether_ntoa((const ether_addr *) & eptr->ether_shost));
    fprintf(stdout, ", destination: %s\n", ether_ntoa((ether_addr *) & eptr->ether_dhost));
    if (ntohs(eptr->ether_type) == ETHERTYPE_IP){
        fprintf(stdout, "(IP)");
    } else if (ntohs(eptr->ether_type) == ETHERTYPE_ARP){
        fprintf(stdout, "(ARP)");
    } else if (ntohs(eptr->ether_type) == ETHERTYPE_REVARP){
        fprintf(stdout, "(RARP)");
    }
    return ntohs(eptr->ether_type);
}


void Pcap::startSniff(std::string devname, int catchcount, std::string filter_string){
    pcap_t * descr;
    const u_char * packet;
    struct pcap_pkthdr hdr;
    struct ether_header * eptr;
    struct bpf_program fp;
    bpf_u_int32 maskp;
    bpf_u_int32 netp;

    pcap_lookupnet(devname.c_str(), &netp, &maskp, errbuf);
    descr = pcap_open_live(devname.c_str(), BUFSIZ, 0, -1, errbuf);
    if(descr == NULL){
        fprintf(stderr, "pcap_open_live(): %s\n", errbuf);
        exit(1);
    }
    if(pcap_compile(descr, &fp, filter_string.c_str(), 0, netp) == -1){
        fprintf(stderr, "Error calling pcap_compile\n");
        exit(1);
    }
    if(pcap_setfilter(descr, &fp) == -1){
        fprintf(stderr, "Error setting filter\n");
        exit(1);
    }
    pcap_loop(descr, catchcount, my_callback, NULL);
}

void * Pcap::startSniffThread(void *Opt){
    Option * option = (Option *) Opt;
    Pcap::startSniff(option->devname, option->catchcount, option->filterstring);
}
