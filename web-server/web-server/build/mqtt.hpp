#include <MQTTClient.h>
#include <MQTTAsync.h>
#include <iostream>
#include <syslog.h>

volatile int finished = 0;
volatile int connected = 0;
std::string address, port, topic_name, client_id, qos, mqtt_timeout,
        times_execution_onstartup, sleepTime, mqtt_on_value;

void connlost(void *context, char *cause)
{
    int retry = 5;
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    int rc;

    syslog(LOG_INFO,"\nConnection lost\n");
    syslog(LOG_INFO,"     cause: %s\n", cause);

    syslog(LOG_INFO,"Reconnecting\n");
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    int counter = 1;
    while (rc != MQTTASYNC_SUCCESS && counter <= retry)
    {
        rc = MQTTAsync_connect(client, &conn_opts);
        syslog(LOG_INFO,"Trying to reconnect... %d/%d return code %d\n",
               counter,
               retry, rc);

        counter++;
    }

    if(rc!= MQTTASYNC_SUCCESS){
        finished = 1;
    }else{
        syslog(LOG_INFO,"Successufly reconnected!");
    }
}

void onDisconnectFailure(void* context, MQTTAsync_failureData* response)
{
    syslog(LOG_INFO,"Disconnect failed\n");
    finished = 1;
}

void onDisconnect(void* context, MQTTAsync_successData* response)
{
    syslog(LOG_INFO,"Successful disconnection\n");
    finished = 1;
}

void onSendFailure(void* context, MQTTAsync_failureData* response)
{
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
    int rc;

    syslog(LOG_INFO,"Message send failed token %d error code %d\n", response->token, response->code);
    opts.onSuccess = onDisconnect;
    opts.onFailure = onDisconnectFailure;
    opts.context = client;
    if ((rc = MQTTAsync_disconnect(client, &opts)) != MQTTASYNC_SUCCESS)
    {
        syslog(LOG_INFO,"Failed to start disconnect, return code %d\n", rc);
    }
}

void onSend(void* context, MQTTAsync_successData* response)
{
    // This gets called when a message is acknowledged successfully.
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
    int rc;

    syslog(LOG_INFO,"Message with token value %d delivery confirmed\n", response->token);
    opts.onSuccess = onDisconnect;
    opts.onFailure = onDisconnectFailure;
    opts.context = client;
    if ((rc = MQTTAsync_disconnect(client, &opts)) != MQTTASYNC_SUCCESS)
    {
        syslog(LOG_INFO,"Failed to start disconnect, return code %d\n", rc);
    }
}


void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
    syslog(LOG_INFO,"Connect failed, rc %d\n", response ? response->code : 0);
    finished = 1;
}


void onConnect(void* context, MQTTAsync_successData* response)
{
    syslog(LOG_INFO,"Successful connection\n");
    connected = 1;
}

int messageArrived(void* context, char* topicName, int topicLen, MQTTAsync_message* m)
{
    /* not expecting any messages */
    return 1;
}

void sendMessageMQTT(std::string message){

    finished = 0;
    connected = 0;

    syslog(LOG_INFO,"MQTT Address: %s",address.c_str());
    syslog(LOG_INFO,"MQTT Port: %s",port.c_str());
    syslog(LOG_INFO,"MQTT Topic Name: %s",topic_name.c_str());
    syslog(LOG_INFO,"MQTT Client ID: %s",client_id.c_str());
    syslog(LOG_INFO,"MQTT QoS: %s",qos.c_str());
    syslog(LOG_INFO,"MQTT Timeout: %s",mqtt_timeout.c_str());

    MQTTAsync client;
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;

    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
    MQTTAsync_responseOptions pub_opts = MQTTAsync_responseOptions_initializer;

    int rc;
    std::string fullAddress = address + ":" + port;

    if ((rc = MQTTAsync_create(&client, fullAddress.c_str(), client_id.c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS)
    {
        syslog(LOG_INFO,"Failed to create client object, return code %d\n", rc);
    }

    if ((rc = MQTTAsync_setCallbacks(client, NULL, connlost, messageArrived, NULL)) != MQTTASYNC_SUCCESS)
    {
        syslog(LOG_INFO,"Failed to set callback, return code %d\n", rc);
    }

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.onSuccess = onConnect;
    conn_opts.onFailure = onConnectFailure;
    conn_opts.context = client;
    if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
    {
        syslog(LOG_INFO,"Failed to start connect, return code %d\n", rc);
    }

    while (!connected && !finished) {
        usleep(100000L);
    }

    pub_opts.onSuccess = onSend;
    pub_opts.onFailure = onSendFailure;
    pub_opts.context = client;

    pubmsg.payload = (void*)message.c_str();
    pubmsg.payloadlen = message.size();
    pubmsg.qos = stoi(qos);
    pubmsg.retained = 0;

    if ((rc = MQTTAsync_sendMessage(client, topic_name.c_str(), &pubmsg, &pub_opts)) != MQTTASYNC_SUCCESS)
    {
        syslog(LOG_INFO,"Failed to start sendMessage, return code %d\n", rc);
    }
}
