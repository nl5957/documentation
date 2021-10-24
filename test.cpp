#include "rdkafkacpp.h"
#include "pcap.h"

// create struct
struct pktStruct {
   struct pcap_pkthdr pkt_header;
   const u_char * pkt_data;
};

struct pcap_file_header *global_header

vector<pktStruct> packets;

// read kafka message
RdKafka::Message *kafka_message = consumer->consume(1000);
long int64_t kafka_message_length = kafka_message->len();
long int64_t kafka_message_position = 0;

// read pcap in kafka message
while ( kafka_message_position < kafka_message_length) {

   // check for empty or invalid files
   if (kafka_message_length <= sizeof(pcap_file_header)) {
      kafka_message_position = kafka_message_length;
      continue;
   }

   // point first 24 bytes to global_header, do not copy
   global_header = static_cast<pcap_file_header*> (kafka_message->payload());

   // check magic and swapped magic
   if (global_header.magic != 0xd4c3b2a1 && global_header.magic != 0xa1b23c4d) {
      kafka_message_position = kafka_message_length;
      continue;
   }

   // check linktype for ethernet header
   if (global_header.linktype != DLT_EN10MB) {
      kafka_message_position = kafka_message_length;
      continue
   }

   // recast data so we can use it. Do not copy
   const u_char * kafka_message_data = static_cast<const u_char*> (kafka_message->payload());
   kafka_message_position=sizeof(pcap_file_header)
   
   // Read all packets
   while ( kafka_message_position < kafka_message_length ) {

      // create packet struct
      pktStruct packet_struct;

      if( kafka_message_length - kafka_message_position < sizeof(pcap_pkthdr) ) {
          kafka_message_position = kafka_message_length;
          continue;
      }

      // copy packet header
      memcpy(packet_struct.pkt_header, (kafka_message_data+kafka_message_position),  sizeof(pcap_pkthdr));
      kafka_message_position += sizeof(pcap_pkthdr)

      // check packet header length for sanity
      if( packet_struct.pkt_header.caplen < 0 || packet_struct.pkt_header.caplen > 65535) {
         kafka_message_position = kafka_message_length;
         continue;
      } 

      // copy packet data
      data_copy = (u_char *)malloc(packet_struct.pkt_header.caplen);
      memcpy(data_copy, kafka_message_data + kafka_message_position, packet_struct.pkt_header.caplen);
      packet_struct.pkt_data = data_copy;
      kafka_message_position += packet_struct.pkt_header.caplen

      packets.push_back(packet_struct)
   }
}

delete kafka_message;
