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
    TickType field = NOT_SET;
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
    std::string accountName = "";
};

struct OrderStatusData {
    OrderId orderId;
    std::string status;
    double filled;
    double remaining;
    double avgFillPrice;
    int permId;
    int parentId;
    double lastFillPrice;
    int clientId;
    std::string whyHeld;
    double mktCapPrice;
};

struct OpenOrderData {
    OrderId orderId;
    Contract contract;
    Order order;
    OrderState orderState;
};

struct Message {
    enum class Type {
        Unknown,
        Log,
        UpdatePortfolio,
        AccountDownloadFinish,
        TickPrice,
        TickString,
        UpdateAccountValue,
        OrderStatus,
        OpenOrder,
    };
    
    Message() : Message(Type::Unknown) {}
    Message(Type type_) : type(type_) {}
    
    Type type = Type::Unknown;
    
    // normal
    std::string log = "";
    
    UpdatePortfolioData updatePortfolioData;
    TickPriceData tickPriceData;
    TickStringData tickStringData;
    UpdateAccountValueData updateAccountValueData;
    OrderStatusData orderStatusData;
    OpenOrderData openOrderData;
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
    OrderId executeOrder(const Contract &contract, const Order &order);
    void reqAccountUpdates(bool subscribe, const std::string &acctCode);
    void processMsgs();
    TickerId reqMktData(const Contract &contract);
    void cancelMktData(long tickerId);
    void reqMarketDataType(int type);
    void reqGlobalCancel();
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

