#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>
#include <list>
#include <rdkafkacpp.h>
#include <vector>
#include <fstream>

using namespace std;

class KafkaConsumerClient {
public:
	KafkaConsumerClient(const std::string& brokers, const std::string& topics, std::string groupid, int32_t nPartition = 0, int64_t offset = 0);
	virtual ~KafkaConsumerClient();
	//初始化
	bool Init();
	//开始获取消息
	void Start(int timeout_ms);
	//停止
	void Stop();

private:
	void Msg_consume(RdKafka::Message* message, void* opaque);

private:
	std::string m_strBrokers;
	std::string m_strTopics;
	std::string m_strGroupid;
	int64_t m_nLastOffset;
	RdKafka::Consumer *m_pKafkaConsumer;
	RdKafka::Topic    *m_pTopic;
	int64_t           m_nCurrentOffset;
	int32_t           m_nPartition;
	bool m_bRun;
};

