#include <iostream>
#include "KafkaConsumerFactory.h"
#include "KafkaConsumerClient.h"
//
//int main()
//{
//	KafkaConsumerClient *KafkaConsumerClient_ = new KafkaConsumerClient("127.0.0.1:9092", "test", "0", 0, RdKafka::Topic::OFFSET_BEGINNING);//OFFSET_BEGINNING,OFFSET_END
//
//	if (!KafkaConsumerClient_->Init())
//	{
//		fprintf(stderr, "kafka server initialize error\n");
//		return -1;
//	}
//
//	KafkaConsumerClient_->Start(1000);
//
//	if (KafkaConsumerClient_)
//	{
//		delete KafkaConsumerClient_;
//		KafkaConsumerClient_ = NULL;
//	}
//
//	return 0;
//}

int main()
{
	//simple example
	KafkaConsumerFactory factory;
	KafkaConsumerClient* consumer = factory.CreateConsumer("127.0.0.1:9092", "player_login", "0", 0, RdKafka::Topic::OFFSET_BEGINNING);
	consumer->Start(1000);

	return 0;
}

