#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>

#ifndef _WIN32
#include <sys/time.h>
#else
#include <windows.h> /* for GetLocalTime */
#endif

#ifdef _MSC_VER
//    #include "../win32/wingetopt.h"
#elif _AIX
#include <unistd.h>
#else
#include <getopt.h>
#include <unistd.h>
#endif
#include <rdkafkacpp.h>
//#pragma comment(lib,"librdkafkacpp.lib")
#include "KafkaClient.h"
using namespace hsx;
static volatile sig_atomic_t run = 1;
static bool exit_eof = false;
static int eof_cnt = 0;
static int partition_cnt = 0;
static int verbosity = 1;
static long msg_cnt = 0;
static int64_t msg_bytes = 0;

/**
 * @brief format a string timestamp from the current time
 */
static void print_time()
{
#ifndef _WIN32
    struct timeval tv;
    char buf[64];
    gettimeofday(&tv, NULL);
    strftime(buf, sizeof(buf) - 1, "%Y-%m-%d %H:%M:%S", localtime(&tv.tv_sec));
    fprintf(stderr, "%s.%03d: ", buf, (int)(tv.tv_usec / 1000));
#else
    SYSTEMTIME lt = {0};
    GetLocalTime(&lt);
    // %Y-%m-%d %H:%M:%S.xxx:
    fprintf(stderr, "%04d-%02d-%02d %02d:%02d:%02d.%03d: ", lt.wYear, lt.wMonth,
            lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
#endif
}

void print_ount_line(std::string outstr)
{
    std::cout << outstr << std::endl;
}
void print_cerr_line(std::string errstr)
{
    std::cerr << errstr << std::endl;
}
void set_conf_string(RdKafka::Conf *conf, const std::string &name,
                     const std::string &value,
                     std::string &errstr)
{
    if(conf->set(name, value, errstr) != RdKafka::Conf::CONF_OK)
    {
        print_cerr_line(errstr);
    }
}

class ExampleEventCb : public RdKafka::EventCb
{
public:
    void event_cb(RdKafka::Event &event)
    {
        print_time();
        switch(event.type())
        {
        case RdKafka::Event::EVENT_ERROR:
            if(event.fatal())
            {
                std::cerr << "FATAL ";
                run = 0;
            }
            std::cerr << "ERROR (" << RdKafka::err2str(event.err())
                      << "): " << event.str() << std::endl;
            break;
        case RdKafka::Event::EVENT_STATS:
            std::cerr << "\"STATS\": " << event.str() << std::endl;
            break;
        case RdKafka::Event::EVENT_LOG:
            fprintf(stderr, "LOG-%i-%s: %s\n", event.severity(), event.fac().c_str(),
                    event.str().c_str());
            break;
        case RdKafka::Event::EVENT_THROTTLE:
            std::cerr << "THROTTLED: " << event.throttle_time() << "ms by "
                      << event.broker_name() << " id " << (int)event.broker_id()
                      << std::endl;
            break;
        default:
            std::cerr << "EVENT " << event.type() << " ("
                      << RdKafka::err2str(event.err()) << "): " << event.str()
                      << std::endl;
            break;
        }
    }
};

void msg_consume(RdKafka::Message *message, void *opaque, void (*consumeMessageCallback)(int msg_len, const char *msg))
{
    switch(message->err())
    {
    case RdKafka::ERR__TIMED_OUT:
        break;
    case RdKafka::ERR_NO_ERROR:
        /* Real message */
        msg_cnt++;
        msg_bytes += message->len();
        if(verbosity >= 3)
        {
            std::cerr << "Read msg at offset " << message->offset() << std::endl;
        }
        RdKafka::MessageTimestamp ts;
        ts = message->timestamp();
        if(verbosity >= 2 &&
                ts.type != RdKafka::MessageTimestamp::MSG_TIMESTAMP_NOT_AVAILABLE)
        {
            std::string tsname = "?";
            if(ts.type == RdKafka::MessageTimestamp::MSG_TIMESTAMP_CREATE_TIME)
            {
                tsname = "create time";
            }
            else if(ts.type ==
                    RdKafka::MessageTimestamp::MSG_TIMESTAMP_LOG_APPEND_TIME)
            {
                tsname = "log append time";
            }
            std::cout << "Timestamp: " << tsname << " " << ts.timestamp << std::endl;
        }
        if(verbosity >= 2 && message->key())
        {
            std::cout << "Key: " << *message->key() << std::endl;
        }
        if(verbosity >= 1 && consumeMessageCallback)
        {
            (*consumeMessageCallback)(static_cast<int>(message->len()), static_cast<const char *>(message->payload()));
        }
        break;
    case RdKafka::ERR__PARTITION_EOF:
        /* Last message */
        if(exit_eof && ++eof_cnt == partition_cnt)
        {
            std::cerr << "%% EOF reached for all " << partition_cnt << " partition(s)"
                      << std::endl;
            run = 0;
        }
        break;
    case RdKafka::ERR__UNKNOWN_TOPIC:
    case RdKafka::ERR__UNKNOWN_PARTITION:
        std::cerr << "Consume failed: " << message->errstr() << std::endl;
        run = 0;
        break;
    default:
        /* Errors */
        std::cerr << "Consume failed: " << message->errstr() << std::endl;
        run = 0;
    }
}
void msg_consume(RdKafka::Message *message, void *opaque)
{
    msg_consume(message, opaque, NULL);
}
class ExampleConsumeCb : public RdKafka::ConsumeCb
{
public:
    void (*consumeMessageCallback)(int msg_len, const char *msg);

    void consume_cb(RdKafka::Message &msg, void *opaque)
    {
        msg_consume(&msg, opaque);
    }
};

KafkaClient::KafkaClient(std::string brokers, const std::vector<std::string> &topics, const std::string &group_id)
{
    std::string errstr;
    /*
     * Create configuration objects
     */
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    set_conf_string(conf, "group.id", group_id, errstr);
    set_conf_string(conf, "bootstrap.servers", brokers, errstr);
    ExampleEventCb ex_event_cb;
    if(conf->set("event_cb", &ex_event_cb, errstr) != RdKafka::Conf::CONF_OK)
    {
        print_cerr_line(errstr);
        return;
    }
    consumer = RdKafka::KafkaConsumer::create(conf, errstr);
    if(!consumer)
    {
        print_cerr_line("Failed to create consumer: " + errstr);
        return;
    }
    print_ount_line("% Create consumer " + consumer->name());
    RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
    if(conf->set("default_topic_conf", tconf, errstr) != RdKafka::Conf::CONF_OK)
    {
        print_cerr_line(errstr);
        return;
    }
    RdKafka::ErrorCode err = consumer->subscribe(topics);
    if(err)
    {
        print_cerr_line("Failed to subscribe to " + std::to_string(topics.size()) + " topics: " + RdKafka::err2str(err));
        return;
    }
    delete conf;
    delete tconf;
}

void KafkaClient::subscribe(const std::vector<std::string> &topics)
{
    RdKafka::ErrorCode err = consumer->subscribe(topics);
    if(err)
    {
        print_cerr_line("Failed to subscribe to " + std::to_string(topics.size()) + " topics: " + RdKafka::err2str(err));
        return;
    }
}
void KafkaClient::startListen()
{
    while(run)
    {
        RdKafka::Message *msg = consumer->consume(5000);
        msg_consume(msg, NULL, consumeMessageCallback);
        delete msg;
    }
}
void KafkaClient::setConsumeMessageCallback(void (*consumeMessageCb)(int msg_len, const char *msg))
{
    consumeMessageCallback = consumeMessageCb;
}


void KafkaClient::stopListen()
{
    run = 0;
}
const RdKafka::TopicMetadata::PartitionMetadataVector *KafkaClient::getTopicPartition(const std::string &topic_str, int timeout_ms)
{
    RdKafka::Topic *topic = NULL;
    class RdKafka::Metadata *metadata;
    /* Fetch metadata */
    RdKafka::ErrorCode err =
        consumer->metadata(!topic, topic, &metadata, timeout_ms);
    if(err != RdKafka::ERR_NO_ERROR)
    {
        print_cerr_line("%% Failed to acquire metadata: " + RdKafka::err2str(err));
        return NULL;
    }
    RdKafka::Metadata::TopicMetadataIterator it;
    for(it = metadata->topics()->begin(); it != metadata->topics()->end();
            ++it)
    {
        if((*it)->err() != RdKafka::ERR_NO_ERROR)
        {
            std::cout << " " << err2str((*it)->err());
            if((*it)->err() == RdKafka::ERR_LEADER_NOT_AVAILABLE)
            {
                std::cout << " (try again)";
            }
        }
        if((*it)->topic() == topic_str)
        {
            const RdKafka::TopicMetadata::PartitionMetadataVector *ret = (*it)->partitions();
            return ret;
        }
    }
    return NULL;
}

int KafkaClient::assignOffsetToHighWatermark(const std::string &topic_str, int timeout_ms)
{
    int64_t low = 0, high = 0;
    const RdKafka::TopicMetadata::PartitionMetadataVector *pmv = getTopicPartition(topic_str, timeout_ms);
    if(!pmv)
    {
        return 0;
    }
    RdKafka::TopicMetadata::PartitionMetadataIterator ip;
    std::vector<RdKafka::TopicPartition *> partitions;
    for(ip = pmv->begin(); ip != pmv->end(); ++ip)
    {
        RdKafka::ErrorCode err = consumer->query_watermark_offsets(topic_str, (*ip)->id(), &low, &high, timeout_ms);
        if(err != RdKafka::ERR_NO_ERROR)
        {
            print_cerr_line("%% Failed to query watermark offsets: " + RdKafka::err2str(err));
            continue;
        }
        RdKafka::TopicPartition *tp = RdKafka::TopicPartition::create(topic_str, (*ip)->id(), high);
        partitions.push_back(tp);
    }
    RdKafka::ErrorCode err = consumer->assign(partitions);
    if(err != RdKafka::ERR_NO_ERROR)
    {
        print_cerr_line("%% Failed to assign hight watermark offsets : " + RdKafka::err2str(err));
        return 0;
    }
    return 1;
}
KafkaClient::~KafkaClient()
{
    run = 0;
    if(consumer)
    {
        consumer->poll(1000);
        delete consumer;
        RdKafka::wait_destroyed(5000);
    }
}
