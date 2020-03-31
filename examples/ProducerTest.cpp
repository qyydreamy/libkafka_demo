#include <iostream>
#include "KafkaProducerClient.h"
#include "KafkaProducerFactory.h"

//int main()
//{
//	KafkaProducerClient* KafkaprClient_ = new KafkaProducerClient("127.0.0.1:9092", "test", 1);
//	KafkaprClient_->Init();
//
//	char str_msg[] = "Hello Kafka!";
//
//	while (fgets(str_msg, sizeof(str_msg), stdin))
//	{
//		size_t len = strlen(str_msg);
//		if (str_msg[len - 1] == '\n')
//		{
//			str_msg[--len] = '\0';
//		}
//
//		if (strcmp(str_msg, "end") == 0)
//		{
//			break;
//		}
//
//		KafkaprClient_->Send(str_msg);
//	}
//
//	if (KafkaprClient_)
//	{
//		delete KafkaprClient_;
//		KafkaprClient_ = NULL;
//	}
//
//	return 0;
//}

int main()
{
	//simple example
	KafkaProducerFactory factory;
	KafkaProducerClient* producer = factory.CreateProducer("127.0.0.1:9092", "player_login", 0);

	//demo: input message
	char str_msg[] = "Hello Kafka!";
	while (fgets(str_msg, sizeof(str_msg), stdin))
	{
		size_t len = strlen(str_msg);
		if (str_msg[len - 1] == '\n')
		{
			str_msg[--len] = '\0';
		}

		if (strcmp(str_msg, "end") == 0)
		{
			break;
		}

		producer->Send(str_msg);
	}

	return 0;
}

