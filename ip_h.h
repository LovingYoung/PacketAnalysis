#ifndef IP_H
#define IP_H

typedef uint32_t addr_t;
typedef uint16_t port_t;

struct ip_header_t {
  uint8_t  ver_ihl;  // 4 bits version and 4 bits internet header length
  uint8_t  tos;
  uint16_t total_length;
  uint16_t id;
  uint16_t flags_fo; // 3 bits flags and 13 bits fragment-offset
  uint8_t  ttl;
  uint8_t  protocol;
  uint16_t checksum;
  addr_t   src_addr;
  addr_t   dst_addr;
} ;

#endif // IP_H


