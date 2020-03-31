#include "KafkaConsumerClient.h"


KafkaConsumerClient::KafkaConsumerClient(const std::string& brokers, const std::string& topics, std::string groupid, int32_t nPartition /*= 0*/, int64_t offset /*= 0*/)
	:m_strBrokers(brokers),
	m_strTopics(topics),
	m_strGroupid(groupid),
	m_nPartition(nPartition),
	m_nCurrentOffset(offset)
{
	m_nLastOffset = 0;
	m_pKafkaConsumer = NULL;
	m_pTopic = NULL;
	m_nCurrentOffset = RdKafka::Topic::OFFSET_BEGINNING;
	m_nPartition = 0;
	m_bRun = false;
}

KafkaConsumerClient::~KafkaConsumerClient()
{
	Stop();
}

bool KafkaConsumerClient::Init() {
	std::string errstr;
	RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
	if (!conf) {
		std::cerr << "RdKafka create global conf failed" << endl;
		return false;
	}
	/*设置broker list*/
	if (conf->set("metadata.broker.list", m_strBrokers, errstr) != RdKafka::Conf::CONF_OK) {
		std::cerr << "RdKafka conf set brokerlist failed ::" << errstr.c_str() << endl;
	}
	/*设置consumer group*/
	if (conf->set("group.id", m_strGroupid, errstr) != RdKafka::Conf::CONF_OK) {
		std::cerr << "RdKafka conf set group.id failed :" << errstr.c_str() << endl;
	}
	std::string strfetch_num = "10240000";
	/*每次从单个分区中拉取消息的最大尺寸*/
	if (conf->set("max.partition.fetch.bytes", strfetch_num, errstr) != RdKafka::Conf::CONF_OK) {
		std::cerr << "RdKafka conf set max.partition failed :" << errstr.c_str() << endl;
	}
	/*创建kafka consumer实例*/ //Create consumer using accumulated global configuration.
	m_pKafkaConsumer = RdKafka::Consumer::create(conf, errstr);
	if (!m_pKafkaConsumer) {
		std::cerr << "failed to ceate consumer" << endl;
	}
	std::cout << "% Created consumer " << m_pKafkaConsumer->name() << std::endl;
	delete conf;
	/*创建kafka topic的配置*/
	RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
	if (!tconf) {
		std::cerr << "RdKafka create topic conf failed" << endl;
		return false;
	}
	if (tconf->set("auto.offset.reset", "smallest", errstr) != RdKafka::Conf::CONF_OK) {
		std::cerr << "RdKafka conf set auto.offset.reset failed:" << errstr.c_str() << endl;
	}
	/*
	 * Create topic handle.
	 */
	m_pTopic = RdKafka::Topic::create(m_pKafkaConsumer, m_strTopics, tconf, errstr);
	if (!m_pTopic) {
		std::cerr << "RdKafka create topic failed :" << errstr.c_str() << endl;
	}
	delete tconf;
	/*
	 * Start consumer for topic+partition at start offset
	 */
	RdKafka::ErrorCode resp = m_pKafkaConsumer->start(m_pTopic, m_nPartition, m_nCurrentOffset);
	if (resp != RdKafka::ERR_NO_ERROR) {
		std::cerr << "failed to start consumer : " << errstr.c_str() << endl;
	}
	return true;
}
void KafkaConsumerClient::Msg_consume(RdKafka::Message* message, void* opaque) {
	switch (message->err()) {
	case RdKafka::ERR__TIMED_OUT:
		break;

	case RdKafka::ERR_NO_ERROR:
		/* Real message */
		std::cout << "Read msg at offset " << message->offset() << std::endl;
		if (message->key()) {
			std::cout << "Key: " << *message->key() << std::endl;
		}
		printf("%.*s\n",
			static_cast<int>(message->len()),
			static_cast<const char *>(message->payload()));

		m_nLastOffset = message->offset();
		break;

	case RdKafka::ERR__PARTITION_EOF:
		/* Last message */
		cout << "Reached the end of the queue, offset: " << m_nLastOffset << endl;
		//Stop();
		break;
	case RdKafka::ERR__UNKNOWN_TOPIC:
	case RdKafka::ERR__UNKNOWN_PARTITION:
		std::cerr << "Consume failed: " << message->errstr() << std::endl;
		Stop();
		break;

	default:
		/* Errors */
		std::cerr << "Consume failed: " << message->errstr() << std::endl;
		Stop();
		break;
	}
}
void KafkaConsumerClient::Start(int timeout_ms) {
	RdKafka::Message *msg = NULL;
	m_bRun = true;
	while (m_bRun) {
		msg = m_pKafkaConsumer->consume(m_pTopic, m_nPartition, timeout_ms);
		Msg_consume(msg, NULL);
		delete msg;
		m_pKafkaConsumer->poll(0);
	}

	m_pKafkaConsumer->stop(m_pTopic, m_nPartition);
	m_pKafkaConsumer->poll(1000);

	if (m_pTopic) {
		delete m_pTopic;
		m_pTopic = NULL;
	}

	if (m_pKafkaConsumer) {
		delete m_pKafkaConsumer;
		m_pKafkaConsumer = NULL;
	}

	/*销毁kafka实例*/ //Wait for RdKafka to decommission.
	RdKafka::wait_destroyed(5000);
}

void KafkaConsumerClient::Stop()
{
	m_bRun = false;
}


