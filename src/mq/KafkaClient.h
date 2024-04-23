#ifndef _KAFKA_CLIENT_H_
#define _KAFKA_CLIENT_H_

#include <functional>

namespace hsx
{
class KafkaClient
{
private:
    RdKafka::KafkaConsumer *consumer;
    void (*consumeMessageCallback)(int msg_len, const char *msg) = NULL;

public:
    KafkaClient(std::string brokers, const std::vector<std::string> &topics, const std::string &group_id);
    void startListen();
    void stopListen();
    void setConsumeMessageCallback(void (*consumeMessageCb)(int msg_len, const char *msg));
    const RdKafka::TopicMetadata::PartitionMetadataVector *getTopicPartition(const std::string &topic_str, int timeout_ms);
    int assignOffsetToHighWatermark(const std::string &topic_str, int timeout_ms);
    ~KafkaClient();
    void subscribe(const std::vector<std::string> &topics);
};
}
#endif
