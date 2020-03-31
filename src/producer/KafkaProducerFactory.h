#pragma once

#include <vector>
#include "KafkaProducerClient.h"

class KafkaProducerClient;

class KafkaProducerFactory
{
public:
	KafkaProducerFactory();
	virtual ~KafkaProducerFactory();

	KafkaProducerClient* CreateProducer(const string &brokers, const string &topics, int nPpartition /*= 1*/);
private:
	std::vector<KafkaProducerClient*> m_producers;
};

