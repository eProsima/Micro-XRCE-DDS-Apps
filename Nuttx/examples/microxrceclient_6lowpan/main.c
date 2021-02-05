/****************************************************************************
 * examples/microxrceclient_6lowpan/hello_main.c
 *
 *   Copyright (C) 2020 Eprosima. All rights reserved.
 *   Author: Eprosima <juanflores@eprosima.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include<stdio.h>
#include<time.h>
#include <string.h> //strcmp
#include <stdlib.h> //atoi

#include <uxr/client/client.h>
#include <ucdr/microcdr.h>

#include "HelloWorld.h"

/************Not change! This is the proper configuration for 6lowpan!***********/
#define STREAM_HISTORY  32
#define BUFFER_SIZE     32 * STREAM_HISTORY

/****************************************************************************
 * Private Functions
 ****************************************************************************/

void on_topic(uxrSession* session, uxrObjectId object_id, uint16_t request_id, uxrStreamId stream_id, struct ucdrBuffer* ub, void* args)
{
    (void) session; (void) object_id; (void) request_id; (void) stream_id;

    HelloWorld topic;
    HelloWorld_deserialize_topic(ub, &topic);

    printf("Received topic: %s, id: %i\n", topic.message, topic.index);

    uint32_t* count_ptr = (uint32_t*) args;
    (*count_ptr)++;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/


#if defined(BUILD_MODULE)
int main(int argc, FAR char *argv[])
#else
int microxrceclient_6lowpan_main(int argc, char *argv[])
#endif
{
    char buffer[256]; // Buffer to save configuration commands.
    char aux_buffer[2]; // Buffer to save the configuration ID.

    if(3 > argc || 0 == atoi(argv[2]))
    {
        printf("usage: program [-h | --help] | ip port sub/pub [<max_topics>]\n");
        return 0;
    }

    //6lowpan configuration menu
    printf("Do you want to configure the 6lowpan network? (Y/N)\r\n");
    memset(buffer,0,sizeof(buffer));
    scanf("%s", buffer);

    if(!strcmp(buffer,"y")){
        system("ifdown wpan0"); // Is necessary to bring down the network to configure.
        system("i8sak wpan0 startpan cd:ab"); //Set the radio as an endpoint.
        system("i8sak set chan 26"); //Set the radio channel.
        system("i8sak set panid cd:ab"); //Set network PAN ID.

        // Set the ID
        printf("Introduce your 6LowPan ID (It must between 00 and FF (Hex))\r\n");
        scanf("%s", aux_buffer);
        if (strlen(aux_buffer) < 2) {
            printf("Error ID must be between 00 and FF\n");
            return 0;
        }
        sprintf(buffer,"i8sak set saddr 42:%c%c",aux_buffer[0],aux_buffer[1]); // Set the short address of the radio
        system(buffer);
        sprintf(buffer, "i8sak set eaddr 00:fa:de:00:de:ad:be:%c%c", aux_buffer[0],aux_buffer[1]); // TODO: This won't work on the lastest version of NuttX
        system(buffer);
        system("i8sak acceptassoc");
        system("ifup wpan0"); // Bring up the network.
        system("mount -t procfs /proc");// Mount the proc file system to check the connection data.

        printf("Connection data\r\n");
        system("cat proc/net/wpan0");
        //6lowpan finished
    }

    //Wait until the agent is ready to receive the data.
    printf("Push any key once the agent is ready to receive the data \r\n");
    scanf("%s", buffer); // We don't care what yo push.

    char* ip = argv[1]; // Save the IP
    char* port= argv[2]; // Save the port number
    
    uint32_t max_topics = (argc == 4) ? (uint32_t)atoi(argv[3]) : UINT32_MAX;

    // Transport
    uxrUDPTransport transport;
    if(!uxr_init_udp_transport(&transport, UXR_IPv6, ip, port))
    {
        printf("Error at create transport.\n");
        return 1;
    }


    if(!strcmp(argv[3],"pub")){
        printf("Init micro-XRCE-DDS Publisher\r\n");
        //Publisher code
        uxrSession session;
        uxr_init_session(&session, &transport.comm, 0xAAAADDDD);
        if(!uxr_create_session(&session))
        {
            printf("Error at create session.\n");
            return 1;
        }
        printf("UXR Init Session\r\n");
        // Streams
        uint8_t output_reliable_stream_buffer[BUFFER_SIZE];
        uxrStreamId reliable_out = uxr_create_output_reliable_stream(&session, output_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);

        uint8_t input_reliable_stream_buffer[BUFFER_SIZE];
        uxr_create_input_reliable_stream(&session, input_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);

        // Create entities
        uxrObjectId participant_id = uxr_object_id(0x01, UXR_PARTICIPANT_ID);
        const char* participant_xml = "<dds>"
                                        "<participant>"
                                            "<rtps>"
                                                "<name>default_xrce_participant</name>"
                                            "</rtps>"
                                        "</participant>"
                                    "</dds>";
        uint16_t participant_req = uxr_buffer_create_participant_xml(&session, reliable_out, participant_id, 0, participant_xml, UXR_REPLACE);
        uxrObjectId topic_id = uxr_object_id(0x01, UXR_TOPIC_ID);
        const char* topic_xml = "<dds>"
                                    "<topic>"
                                        "<name>HelloWorldTopic</name>"
                                        "<dataType>HelloWorld</dataType>"
                                    "</topic>"
                                "</dds>";
        uint16_t topic_req = uxr_buffer_create_topic_xml(&session, reliable_out, topic_id, participant_id, topic_xml, UXR_REPLACE);
        uxrObjectId publisher_id = uxr_object_id(0x01, UXR_PUBLISHER_ID);
        const char* publisher_xml = "";
        uint16_t publisher_req = uxr_buffer_create_publisher_xml(&session, reliable_out, publisher_id, participant_id, publisher_xml, UXR_REPLACE);
        uxrObjectId datawriter_id = uxr_object_id(0x01, UXR_DATAWRITER_ID);
        const char* datawriter_xml = "<dds>"
                                        "<data_writer>"
                                            "<topic>"
                                                "<kind>NO_KEY</kind>"
                                                "<name>HelloWorldTopic</name>"
                                                "<dataType>HelloWorld</dataType>"
                                            "</topic>"
                                        "</data_writer>"
                                    "</dds>";
        uint16_t datawriter_req = uxr_buffer_create_datawriter_xml(&session, reliable_out, datawriter_id, publisher_id, datawriter_xml, UXR_REPLACE);

        // Send create entities message and wait its status
        uint8_t status[4];
        uint16_t requests[4] = {participant_req, topic_req, publisher_req, datawriter_req};
        

        if(!uxr_run_session_until_all_status(&session, 1000, requests, status, 4))
        {
            printf("Error at create entities: participant: %i topic: %i publisher: %i darawriter: %i\n", status[0], status[1], status[2], status[3]);
            return 1;
        }

        // Write topics
        bool connected = true;
        uint32_t count = 0;
        printf("write topics \r\n");
        while(connected)
        {
            HelloWorld topic = {++count, "Hello DDS world!"};

            ucdrBuffer ub;
            uint32_t topic_size = HelloWorld_size_of_topic(&topic, 0);
            uxr_prepare_output_stream(&session, reliable_out, datawriter_id, &ub, topic_size);
            HelloWorld_serialize_topic(&ub, &topic);

            printf("Send topic: %s, id: %i\n", topic.message, topic.index);
            connected = uxr_run_session_time(&session, 1000);
        }
        // Delete resources
        uxr_delete_session(&session);
        uxr_close_udp_transport(&transport);
    }
    else if(!strcmp(argv[3],"sub")){
        printf("Init micro-XRCE-DDS Subscriber\r\n");
        //Subscriber code
        uxrSession session;
        uxr_init_session(&session, &transport.comm, 0xCCCCDDDD);
        if(!uxr_create_session(&session))
        {
            printf("Error at create session.\n");
            return 1;
        }
        printf("UXR Init Session\r\n");

        // State
         uint32_t count = 0;

        uxr_set_topic_callback(&session, on_topic, &count);
        // Streams
        uint8_t output_reliable_stream_buffer[BUFFER_SIZE];
        uxrStreamId reliable_out = uxr_create_output_reliable_stream(&session, output_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);

        uint8_t input_reliable_stream_buffer[BUFFER_SIZE];
        uxrStreamId reliable_in = uxr_create_input_reliable_stream(&session, input_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);

        // Create entities
        uxrObjectId participant_id = uxr_object_id(0x01, UXR_PARTICIPANT_ID);
        const char* participant_xml = "<dds>"
                                        "<participant>"
                                            "<rtps>"
                                                "<name>default_xrce_participant</name>"
                                            "</rtps>"
                                        "</participant>"
                                    "</dds>";
        uint16_t participant_req = uxr_buffer_create_participant_xml(&session, reliable_out, participant_id, 0, participant_xml, UXR_REPLACE);

        uxrObjectId topic_id = uxr_object_id(0x01, UXR_TOPIC_ID);
        const char* topic_xml = "<dds>"
                                    "<topic>"
                                        "<name>HelloWorldTopic</name>"
                                        "<dataType>HelloWorld</dataType>"
                                    "</topic>"
                                "</dds>";
        uint16_t topic_req = uxr_buffer_create_topic_xml(&session, reliable_out, topic_id, participant_id, topic_xml, UXR_REPLACE);

        uxrObjectId subscriber_id = uxr_object_id(0x01, UXR_SUBSCRIBER_ID);
        const char* subscriber_xml = "";
        uint16_t subscriber_req = uxr_buffer_create_subscriber_xml(&session, reliable_out, subscriber_id, participant_id, subscriber_xml, UXR_REPLACE);

        uxrObjectId datareader_id = uxr_object_id(0x01, UXR_DATAREADER_ID);
        const char* datareader_xml = "<dds>"
                                        "<data_reader>"
                                            "<topic>"
                                                "<kind>NO_KEY</kind>"
                                                "<name>HelloWorldTopic</name>"
                                                "<dataType>HelloWorld</dataType>"
                                            "</topic>"
                                        "</data_reader>"
                                    "</dds>";
        uint16_t datareader_req = uxr_buffer_create_datareader_xml(&session, reliable_out, datareader_id, subscriber_id, datareader_xml, UXR_REPLACE);

        // Send create entities message and wait its status
        uint8_t status[4];
        uint16_t requests[4] = {participant_req, topic_req, subscriber_req, datareader_req};
        if(!uxr_run_session_until_all_status(&session, 1000, requests, status, 4))
        {
            printf("Error at create entities: participant: %i topic: %i subscriber: %i datareader: %i\n", status[0], status[1], status[2], status[3]);
            return 1;
        }

        // Request topics
        uxrDeliveryControl delivery_control = {0};
        delivery_control.max_samples = UXR_MAX_SAMPLES_UNLIMITED;
        uint16_t read_data_req = uxr_buffer_request_data(&session, reliable_out, datareader_id, reliable_in, &delivery_control);

        // Read topics
        bool connected = true;
        printf("Waiting for topics\r\n");
        while(connected)
        {
            uint8_t read_data_status;
            connected = uxr_run_session_until_all_status(&session, UXR_TIMEOUT_INF, &read_data_req, &read_data_status, 1);
            
        }
        // Delete resources
        uxr_delete_session(&session);
        uxr_close_udp_transport(&transport);
    }
    else{
        printf("Error: It must be publisher (pub) or subscriber(sub)\r\n");
    }

    return 0;
}
