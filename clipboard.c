#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include <linux/if_packet.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <net/ethernet.h>

// #define DEF_IF "enp0s31f6"
#define DEF_IF "eth0"

#define HARDWARE_ETHERNET_LOCATION_PROTOCOL 0x9001

#define MODE_SEND 0x0
#define MODE_REPLY 0x1
#define MODE_BROADSEND 0x2
#define MODE_BROADREPLY 0x3

#define MODE_LEN 1 
#define PKG_ID_LEN 2
#define NETW_ID_LEN 8

/* function prototypes */
void process_argv(int argc, char *argv[], char *destination_mac, char *default_interface, unsigned char *count);
void print_mac(const unsigned char *mac_address, FILE *loc);
char char_to_hex(char input);
unsigned char *string_to_mac(const char *string);
unsigned char *create_package(const unsigned char *destination_mac, struct ifreq source_mac, const char *network_id, const char *package_id, uint_fast16_t *tx_len);
void send_package(int socket_fd, const unsigned char *package, const uint_fast16_t *tx_len, const unsigned char *destination_mac, struct ifreq interface_ID, unsigned char *counter);
void *wait_for_package(void *thread_argument);

/**************************************** Beginning of file *****************************************/
void print_mac(const unsigned char *mac_address, FILE *loc) {
  int i;
  for (i = 0; i < 6; i++) {
    fprintf(loc, "%2x", mac_address[i]);
    if (i < 5) {
      putc(':', loc);
    }
  } 
}



char char_to_hex(const char input) {
  switch (input) {
    case '0':
	  return '\x00';
    case '1':
      return '\x01';
    case '2':
      return '\x02';
    case '3':
      return '\x03';
    case '4':
      return '\x04';
    case '5':
      return '\x05';
    case '6':
      return '\x06';
    case '7':
      return '\x07';
    case '8':
      return '\x08';
    case '9':
      return '\x09';
    case 'a':
    case 'A':
      return '\x0A';
    case 'b':
    case 'B':
      return '\x0B';
    case 'c':
    case 'C':
      return '\x0C';
    case 'd':
    case 'D':
      return '\x0D';
    case 'e':
    case 'E':
      return '\x0E';
    case 'f':
    case 'F':
      return '\x0F';
    default:
	  fputs("\t[\033[31mERROR\033[0m] Unknown character in MAC address found\n", stderr);
	  exit(1);
  } 
}



unsigned char *string_to_mac(const char *string) {
  unsigned char *to_return = (unsigned char *) calloc(6, sizeof(*to_return));
  if (NULL == to_return) {
    fputs("\t[\033[31mERROR\033[0m] Failed to initialize mac address\n", stderr);
    exit(1);
  }
  
  int i, mac_bit = 0;
  for (i = 0; i < 17; i++) {
    if (!(i % 3)) {
      to_return[mac_bit] = char_to_hex(string[i])*16;
    } else if (1 == (i % 3)) {
      to_return[mac_bit++] += char_to_hex(string[i]);
    }
  }
  return to_return;
}



unsigned char *create_package(const unsigned char *destination_mac, struct ifreq source_mac, const char *network_id, const char *package_id, uint_fast16_t *tx_len) {
  unsigned char *sendbuffer = (unsigned char *) calloc(23, sizeof(*sendbuffer));
  if (NULL == sendbuffer) {
    fputs("\t[\033[31mERROR\033[0m] Failed to initialize send buffer\n", stderr);
    exit(1);
  }

  struct ether_header *eth = (struct ether_header *) sendbuffer;
  strncpy(eth->ether_shost, (uint8_t *) &source_mac.ifr_hwaddr.sa_data, 6);
  strncpy(eth->ether_dhost, destination_mac, 6);
  eth->ether_type = htons(HARDWARE_ETHERNET_LOCATION_PROTOCOL);
  
  tx_len[0] += sizeof(struct ether_header);
  int i;
  
  if (!strncmp((char *) destination_mac, "\xff\xff\xff\xff\xff\xff", 6)) {
    sendbuffer[tx_len[0]++] = MODE_BROADSEND;
  } else {
    sendbuffer[tx_len[0]++] = MODE_SEND;
  }

  for (i = 0; i < NETW_ID_LEN; i++) {
    sendbuffer[tx_len[0]++] = network_id[i];
  }

  for (i = 0; i < PKG_ID_LEN; i++) {
    sendbuffer[tx_len[0]++] = package_id[i];
  }

  return sendbuffer;
}



void send_package(const int socket_fd, const unsigned char *package, const uint_fast16_t *tx_len, const unsigned char *destination_mac, struct ifreq interface_ID, unsigned char *count) {
  struct sockaddr_ll socket_address;
  socket_address.sll_ifindex = interface_ID.ifr_ifindex;
  socket_address.sll_halen = ETH_ALEN;

  strncpy(socket_address.sll_addr, (char *) destination_mac, 6);

  unsigned char i;
  for (i = 0; i < count[0]; i++) {
	if (sendto(socket_fd, package, tx_len[0], 0, (const struct sockaddr *) &socket_address, sizeof(struct sockaddr_ll)) < 0) {
		puts("\t[\033[31;1mCRITICAL\033[0m] Failed to send package, please check your network connection");
	  } else {
		puts("\t[\033[32;1mSUCCESS\033[0m] Package sent successfully, waiting for reply");
	  }
  }
}



void *wait_for_package(void *thread_argument) {
  /* this function waits three seconds, prints out that the host is unavailable and quits the program */
  unsigned char *mac_address = (unsigned char *) calloc(6, sizeof(*mac_address));
  if (NULL == mac_address) {
    fputs("\t[\033[31mERROR\033[0m] Failed to initialize mac address display buffer\n", stderr);
    exit(1);
  }
  strncpy((char *) mac_address, (char *) thread_argument, 6);
  sleep(2);
  fputs("\t[\033[31mERROR\033[0m] Failed to reach host (", stderr);print_mac(mac_address, stderr);putc(')', stderr);fputs(", please check your ethernet configuration", stderr);putc('\n', stderr);
  exit(0);
}



int main(int argc, char *argv[]) {
  if (argc < 2) {
    puts("Usage: ping2 [\033[32;1mMAC_ADDRESS\033[0m]\n\t  -i [\033[32;1mINTERFACE\033[0m]\tDefault Interface:\t\"eth0\"\n\t  -n [\033[32;1mCOUNT\033[0m]\t\tDefault count:\t\t1");
    exit(1);
  }

  pthread_t pkg_timeout;
  unsigned char *destination_mac = string_to_mac(argv[1]);

  char *interface = (char *) calloc(64, sizeof(*interface));
  if (NULL == interface) {
    fputs("\t[\033[31mERROR\033[0m] Failed to initialize interface string\n", stderr);
	exit(1);
  }
  interface[0] = '\x01';

  unsigned char *counter = (unsigned char *) calloc(1, sizeof(*counter));
  if (NULL == counter) {
	  fputs("\t[\033[31mERROR\033[0m] Failed to initialize counter object\n", stderr);
	  exit(1);
  }
  counter[0] = 0;

/**************************************** SOCKET CREATION *****************************************/
  int socket_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (socket_fd < 0) {
    fputs("\t[\033[31mERROR\033[0m] Failed to initialize socket object\n", stderr);
    exit(1);
  }
  
  uint_fast16_t *transmission_length = (uint_fast16_t *) calloc(1, sizeof(*transmission_length));
  if (NULL == transmission_length) {
    fputs("\t[\033[31mERROR\033[0m] Failed to initialize transmission length register\n", stderr);
    exit(1);
  }
  transmission_length[0] = 0;

  char *package_id = (char *) calloc(2, sizeof(*package_id));
  if (NULL == package_id) {
    fputs("\t[\033[31mERROR\033[0m] Failed to initialize package ID register\n", stderr);
    exit(1);
  }

  char *network_id = (char *) calloc(8, sizeof(*network_id));
  if (NULL == network_id) {
    fputs("\t[\033[31mERROR\033[0m] Failed to initialize network ID register\n", stderr);
    exit(1);
  } /* here, the program would, in the future, read the ID from a config */
  
  struct ifreq interface_ID, interface_MAC;

  char interface_name[IFNAMSIZ];
  strncpy(interface_name, DEF_IF, IFNAMSIZ);

  memset(&interface_ID, 0, sizeof(struct ifreq));
  memset(&interface_MAC, 0, sizeof(struct ifreq));

  strncpy(interface_ID.ifr_name, interface_name, IFNAMSIZ-1);
  strncpy(interface_MAC.ifr_name, interface_name, IFNAMSIZ-1);
  if (ioctl(socket_fd, SIOCGIFINDEX, &interface_ID) < 0) {
    fputs("\t[\033[31mERROR\033[0m] Failed to bind network interface to socket\n", stderr);
    exit(1);
  }
  if (ioctl(socket_fd, SIOCGIFHWADDR, &interface_MAC) < 0) {
    fputs("\t[\033[31mERROR\033[0m] Failed to bind network hardware address to socket\n", stderr);
    exit(1);
  }

  unsigned char *read_buffer = (unsigned char *) calloc(64, sizeof(*read_buffer));
  if (NULL == read_buffer) {
    fputs("\t[\033[31mERROR\033[0m] Failed to initialize read buffer\n", stderr);
    exit(1);
  }

  unsigned char *mac_string_buffer = (unsigned char *) calloc(6, sizeof(*mac_string_buffer));
  strncpy((char *) mac_string_buffer, (char *) destination_mac, 6);

  unsigned char *transmission_package = create_package(destination_mac, interface_MAC, network_id, package_id, transmission_length);
  send_package(socket_fd, transmission_package, transmission_length, destination_mac, interface_ID, counter);
  pthread_create(&pkg_timeout, NULL, wait_for_package, destination_mac);

  /* read the interface */
  while (1) {
    recvfrom(socket_fd, read_buffer, 64, 0, NULL, NULL);
    read_buffer += 12;

    if (!(0x90 == read_buffer[0] && 0x01 == read_buffer[1])) {
      continue;
    }

    read_buffer += 2;
    if (/* mode == "uni" && */ MODE_REPLY == read_buffer[0]) {
	  read_buffer++;

	  if (read_buffer[0] == package_id[0] && read_buffer[1] == package_id[1]) {
	    printf("\t[\033[32;1mSUCCESS\033[0m] Successfully located host (");print_mac(destination_mac, stdout);putchar(')');putchar('\n');
		return 0;
	  }
	}
  }
}
