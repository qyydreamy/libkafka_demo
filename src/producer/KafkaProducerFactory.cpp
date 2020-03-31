#include "KafkaProducerFactory.h"

#include "KafkaProducerClient.h"

KafkaProducerFactory::KafkaProducerFactory()
{

}

KafkaProducerFactory::~KafkaProducerFactory()
{
	for (std::vector<KafkaProducerClient*>::iterator it = m_producers.begin();
		it < m_producers.end(); ++it)
	{
		delete *it;
		it = m_producers.erase(it);
	}
	m_producers.clear();
}

KafkaProducerClient* KafkaProducerFactory::CreateProducer(const string &brokers, const string &topics, int nPpartition /*= 1*/)
{
	KafkaProducerClient *producer = new KafkaProducerClient(brokers, topics, nPpartition);
	if (!producer->Init())
	{
		std::cerr << "Producer init failed:" << topics << std::endl;
	}
	m_producers.push_back(producer);
	return producer;
}


