/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"

#include <uxr/client/client.h>
#include <ucdr/microcdr.h>

#define STREAM_HISTORY  2
#define MAX_TRANSPORT_MTU UXR_CONFIG_UDP_TRANSPORT_MTU
#define BUFFER_SIZE    MAX_TRANSPORT_MTU * STREAM_HISTORY

uxrSession session;
uxrUDPTransport transport;

typedef struct Point32 {
    float x;
    float y;
    float z;
} Point32;

static bool Point32_serialize_topic(ucdrBuffer* writer, const Point32* topic);
static bool Point32_deserialize_topic(ucdrBuffer * reader, Point32* topic);
static uint32_t Point32_size_of_topic(const Point32* topic, uint32_t size);

static int count = 0;

void on_topic(uxrSession* session, uxrObjectId object_id, uint16_t request_id, uxrStreamId stream_id, struct ucdrBuffer* ub, void* args)
{
    (void) session; (void) object_id; (void) request_id; (void) stream_id;

    Point32 topic;
    Point32_deserialize_topic(ub, &topic);

    printf("Received %d topic: %f %f %f\n", ++count, (double) topic.x, (double) topic.y, (double) topic.z);
}

void appMain(){

    // Micro-XRCE-DDS init transport and session
    if(!uxr_init_udp_transport(&transport, UXR_IPv4, UXRCEDDS_AGENT_IP, UXRCEDDS_AGENT_PORT)){
        printf("Error: Init serial transport fail\n");
        vTaskSuspend( NULL );
    }

    uxr_init_session(&session, &transport.comm, 0xBA5EBA11);
    uxr_set_topic_callback(&session, on_topic, NULL);

    if(!uxr_create_session(&session))
    {
        printf("Error: Create session fail\n");
        vTaskSuspend( NULL );
    }

    // Create Micro-XRCE-DDS buffers
    uint8_t out_stream_buff[BUFFER_SIZE];
    uxrStreamId reliable_out = uxr_create_output_reliable_stream(&session,
                                                                    out_stream_buff,
                                                                    BUFFER_SIZE,
                                                                    STREAM_HISTORY);

    uint8_t in_stream_buff[BUFFER_SIZE];
    uxrStreamId reliable_in = uxr_create_input_reliable_stream(&session, 
                                                                in_stream_buff, 
                                                                BUFFER_SIZE,
                                                                STREAM_HISTORY);

    // Create Micro-XRCE-DDS participant
    uxrObjectId participant_id = uxr_object_id(0x01, UXR_PARTICIPANT_ID);
    const char* participant_xml = "<dds>"
                                        "<participant>"
                                            "<rtps>"
                                                "<name>olimex_node</name>"
                                            "</rtps>"
                                        "</participant>"
                                    "</dds>";
                                    
    uint16_t participant_req = uxr_buffer_create_participant_xml(&session,
                                                                    reliable_out,
                                                                    participant_id,
                                                                    0,
                                                                    participant_xml,
                                                                    UXR_REPLACE);

    // Create Micro-XRCE-DDS topic
    uxrObjectId topic_id = uxr_object_id(0x01, UXR_TOPIC_ID);
    const char* topic_xml =     "<dds>"
                                    "<topic>"
                                        "<dataType>geometry_msgs::msg::dds_::Point32_</dataType>"
                                    "</topic>"
                                "</dds>";
    uint16_t topic_req = uxr_buffer_create_topic_xml(&session, reliable_out,
                                                        topic_id, participant_id,
                                                        topic_xml, UXR_REPLACE);

    // Create Micro-XRCE-DDS publisher
    uxrObjectId publisher_id = uxr_object_id(0x01, UXR_PUBLISHER_ID);
    const char* publisher_xml = "";
    uint16_t publisher_req = uxr_buffer_create_publisher_xml(&session,
                                                                reliable_out,
                                                                publisher_id,
                                                                participant_id,
                                                                publisher_xml,
                                                                UXR_REPLACE);

    // Create Micro-XRCE-DDS subscriber
    uxrObjectId subscriber_id = uxr_object_id(0x01, UXR_SUBSCRIBER_ID);
    const char* subscriber_xml = "";
    uint16_t subscriber_req = uxr_buffer_create_subscriber_xml(&session,
                                                                reliable_out,
                                                                subscriber_id,
                                                                participant_id,
                                                                subscriber_xml,
                                                                UXR_REPLACE);

    // Create Micro-XRCE-DDS datareader
    uxrObjectId datareader_id = uxr_object_id(0x01, UXR_DATAREADER_ID);
    const char* datareader_xml = "<dds>"
                                     "<data_reader>"
                                         "<topic>"
                                             "<kind>NO_KEY</kind>"
                                             "<name>rt/olimex/subscriber</name>"
                                             "<dataType>geometry_msgs::msg::dds_::Point32_</dataType>"
                                         "</topic>"
                                     "</data_reader>"
                                 "</dds>";
    uint16_t datareader_req = uxr_buffer_create_datareader_xml(&session, 
                                                               reliable_out, 
                                                               datareader_id, 
                                                               subscriber_id, 
                                                               datareader_xml, 
                                                               UXR_REPLACE);


    // Create Micro-XRCE-DDS datawriter 1
    uxrObjectId datawriter_1_id = uxr_object_id(0x01, UXR_DATAWRITER_ID);
    const char* datawriter_1_xml = "<dds>"
                                        "<data_writer>"
                                            "<topic>"
                                                "<kind>NO_KEY</kind>"
                                                "<name>rt/olimex/publisher</name>"
                                                "<dataType>geometry_msgs::msg::dds_::Point32_</dataType>"
                                            "</topic>"
                                        "</data_writer>"
                                    "</dds>";
    uint16_t datawriter_1_req = uxr_buffer_create_datawriter_xml(&session,
                                                                reliable_out,
                                                                datawriter_1_id,
                                                                publisher_id,
                                                                datawriter_1_xml,
                                                                UXR_REPLACE);


    // Run Micro-XRCE-DDS session until all status
    uint8_t status[6];
    uint16_t requests[6] = {    participant_req, 
                                topic_req, 
                                publisher_req, 
                                subscriber_req,
                                datareader_req,
                                datawriter_1_req
                            };

    if(!uxr_run_session_until_all_status(&session, 1000, requests, status, 6))
    {
        vTaskSuspend( NULL );
    }

    // Main loop
    Point32 message = {0.0, 0.0, 0.0};
    ucdrBuffer ub;

    while(1){

        // Update messages
        message.x++;
        message.y--;

        // Publish topics
        uint32_t topic_size = Point32_size_of_topic(&message, 0);
        uxr_prepare_output_stream(&session, reliable_out, datawriter_1_id, &ub, topic_size);
        Point32_serialize_topic(&ub, &message);

        // Request topic subscription
        uxr_buffer_request_data(&session, reliable_out, datareader_id, reliable_in, NULL);

        // Run session
        uxr_run_session_until_timeout(&session, 500);

        vTaskDelay(500/portTICK_RATE_MS);
    }

  uxr_delete_session(&session);
  vTaskSuspend( NULL );
}


static bool Point32_serialize_topic(ucdrBuffer* writer, const Point32* topic)
{
    (void) ucdr_serialize_float(writer, topic->x);
    (void) ucdr_serialize_float(writer, topic->y);
    (void) ucdr_serialize_float(writer, topic->z);

    return !writer->error;
}

static bool Point32_deserialize_topic(ucdrBuffer * reader, Point32* topic)
{
    bool rv = false;
    rv = ucdr_deserialize_float(reader, &topic->x);
    rv = ucdr_deserialize_float(reader, &topic->y);
    rv = ucdr_deserialize_float(reader, &topic->z);

    return rv;
}

static uint32_t Point32_size_of_topic(const Point32* topic, uint32_t size)
{
    uint32_t previousSize = size;
    size += ucdr_alignment(size, 4) + 4;
    size += ucdr_alignment(size, 4) + 4;
    size += ucdr_alignment(size, 4) + 4;

    return size - previousSize;
}
