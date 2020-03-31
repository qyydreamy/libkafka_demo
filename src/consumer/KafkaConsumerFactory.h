#pragma once

#include <vector>

class KafkaConsumerClient;

class KafkaConsumerFactory
{
public:
	KafkaConsumerFactory();
	virtual ~KafkaConsumerFactory();

	KafkaConsumerClient* CreateConsumer(const std::string& brokers, const std::string& topics, std::string groupid, int32_t nPartition /*= 0*/, int64_t offset /*= 0*/);
private:
	std::vector<KafkaConsumerClient*> m_consumers;
};

