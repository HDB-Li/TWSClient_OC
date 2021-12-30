/* Copyright (C) 2019 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef TWS_API_SAMPLES_TESTCPPCLIENT_TESTCPPCLIENT_H
#define TWS_API_SAMPLES_TESTCPPCLIENT_TESTCPPCLIENT_H

#include "EWrapper.h"
#include "EReaderOSSignal.h"
#include "EReader.h"
#include "OrderState.h"
#include "Order.h"

#include <memory>
#include <vector>

class EClientSocket;

struct LogData {
    std::string log = "";
};

struct UpdatePortfolioData {
    Contract contract = Contract();
    double position = 0;
    double marketPrice = 0;
    double marketValue = 0;
    double averageCost = 0;
    double unrealizedPNL = 0;
    double realizedPNL = 0;
    std::string accountName = "";
};

struct TickPriceData {
    TickerId tickerId = -1;
    int field = 0;
    double price = -1;
};

struct TickStringData {
    TickerId tickerId = -1;
    int tickType = -1;
    std::string value = "";
};

struct UpdateAccountValueData {
    std::string key = "";
    std::string value = "";
};

struct Message {
    enum class Type {
        Unknown,
        Log,
        Disconnect,
        UpdatePortfolio,
        AccountDownloadFinish,
        TickPrice,
        TickString,
        UpdateAccountValue,
    };
    
    Message() : Message(Type::Unknown) {}
    Message(Type type_) : type(type_) {}
    
    Type type = Type::Unknown;
    
    LogData logData;
    UpdatePortfolioData updatePortfolioData;
    TickPriceData tickPriceData;
    TickStringData tickStringData;
    UpdateAccountValueData updateAccountValueData;
};

//! [ewrapperimpl]
class TWSClient : public EWrapper
{
//! [ewrapperimpl]
public:

	TWSClient();
	~TWSClient();

	void setConnectOptions(const std::string&);
    typedef std::function<void(const Message&)> Observer;
public:

	bool connect(const char * host, int port, int clientId = 0);
	void disconnect() const;
	bool isConnected() const;

    void setCallback(Observer observer_);
    void executeOrder(const Contract &contract, const Order &order);
    void reqAccountUpdates(bool subscribe, const std::string &acctCode);
    void processMsgs();
    TickerId reqMktData(const Contract &contract);
    void reqMarketDataType(int type);
    std::string getVersion();
    
public:
	// events
	#include "EWrapper_prototypes.h"


private:
	void printContractMsg(const Contract& contract);
	void printContractDetailsMsg(const ContractDetails& contractDetails);
	void printContractDetailsSecIdList(const TagValueListSPtr &secIdList);
	void printBondContractDetailsMsg(const ContractDetails& contractDetails);

private:
	//! [socket_declare]
	EReaderOSSignal m_osSignal;
	EClientSocket * const m_pClient;
	//! [socket_declare]

	OrderId m_orderId;
    TickerId m_tickerId;
	EReader *m_pReader;
    bool m_extraAuth;
	std::string m_bboExchange;
    Observer m_observer;
};

#endif

