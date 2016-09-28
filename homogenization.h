#ifndef HOMOGENIZATION
#define HOMOGENIZATION

#include <string>
#include "tcp_udp_h.h"

struct Homo{
    std::string src_mac;
    std::string dst_mac;
    std::string src_ip;
    std::string dst_ip;
    std::string protocol;
    int packet_length;
    tcp_header_t * tcp_header;
    udp_header_t * udp_header;
};

#endif
