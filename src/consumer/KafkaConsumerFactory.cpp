#include "KafkaConsumerFactory.h"

#include "KafkaConsumerClient.h"

KafkaConsumerFactory::KafkaConsumerFactory()
{

}

KafkaConsumerFactory::~KafkaConsumerFactory()
{
	for (std::vector<KafkaConsumerClient*>::iterator it = m_consumers.begin(); 
		it < m_consumers.end(); ++it)
	{
		delete *it;
		it = m_consumers.erase(it);
	}
	m_consumers.clear();
}

KafkaConsumerClient* KafkaConsumerFactory::CreateConsumer(const std::string& brokers, const std::string& topics, std::string groupid, int32_t nPartition /*= 0*/, int64_t offset /*= 0*/)
{
	KafkaConsumerClient *consumer = new KafkaConsumerClient(brokers, topics, groupid, 0, RdKafka::Topic::OFFSET_BEGINNING);//OFFSET_BEGINNING,OFFSET_END
	if (!consumer->Init())
	{
		std::cerr << "Consumer init failed:" << topics << std::endl;
	}
	m_consumers.push_back(consumer);
	return consumer;
}


