#ifndef AUCTIONTRANSFERAPI_H
#define AUCTIONTRANSFERAPI_H

#pragma warning (disable : 4786)

#include "AuctionTransferEnum.h"
#include <Base/Archive.h>
#include "Character.h"

//////////////////////////////////////////////////////////////////////////////////////
namespace AuctionTransfer
{

class AuctionTransferAPICore;

//////////////////////////////////////////////////////////////////////////////////////
class AuctionTransferAPI
{
public:
	// constructor & destructor
	AuctionTransferAPI(const char *hostName[], const short port[], int count, const char *identifier[], unsigned identifierCount, unsigned reqTimeout = 60, unsigned maxRecvMessageSizeInKB = 16000);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// the identifier list does not have one entry for each connection, it is a simple list of world names that
	// are handled by this API connection, no matter how many servers are connected to.  In the case of EQ2 this
	// identifier list will just contain 1 name, the name of the world making the connection.

	// the following constructor does the same as the one above, except that the host array is in the format of:
	//	"hostname1:port1 hostname2:port2" 
	// and the identifier array would be a semi-colon separated list of world names such as:
	//		"Guk Befallen" or "GUK"
	AuctionTransferAPI(const char *hostNames, const char *identifiers);
	virtual ~AuctionTransferAPI();

	// connect and disconnect call backs
	virtual void onConnect(const char* hostname, unsigned short port, const short current, const short max) = 0;
	virtual void onDisconnect(const char *host, const short port, const short current, const short max) = 0;


	//processing
	void process();

	// Requests Generated by API
	unsigned sendPrepareTransaction (const char *serverIdentifier, long long transactionID, unsigned stationID, unsigned characterID, long long assetID, const char *xmlAsset, void *user, bool compress = false);
	unsigned sendPrepareTransactionCompressed (const char *serverIdentifier, long long transactionID, unsigned stationID, unsigned characterID, long long assetID, const unsigned char *zippedXmlAsset, unsigned length, void *user);
	unsigned sendCommitTransaction (long long transactionID, void *user);
	unsigned sendAbortTransaction (long long transactionID, void *user);
	unsigned sendAuditAssetTransfer(const char *gameCode, const char *serverCode, long long inGameAssetID, unsigned stationID, const char *event, const char *message, void *user);
	unsigned getNewTransactionID(void *user);

	// requests as responses to a call initiated by the auction system
	unsigned replyReceivePrepareTransaction (unsigned trackingNumber,unsigned responseCode,void *user);
	unsigned replyReceiveCommitTransaction (unsigned trackingNumber, unsigned responseCode, void *user);
	unsigned replyReceiveAbortTransaction (unsigned trackingNumber, unsigned responseCode, void *user);
	unsigned replyReceiveGetCharacterList(unsigned trackingNumber, unsigned responseCode, const Character characters[], unsigned numCharacters, void *user);

	// house keeping routines
	unsigned identifyHost( const char *serverID[], unsigned idCount, void *user);

	// Callbacks that are repsonses
	virtual void onSendPrepareTransaction (unsigned trackingNumber, unsigned responseCode, void *user) = 0;
	virtual void onSendPrepareTransactionCompressed (unsigned trackingNumber, unsigned responseCode, void *user) = 0;
	virtual void onSendCommitTransaction (unsigned trackingNumber, unsigned responseCode, void *user) = 0;
	virtual void onSendAbortTransaction (unsigned trackingNumber, unsigned responseCode, void *user) = 0;
	virtual void onSendAuditAssetTransfer(unsigned trackingNumber, unsigned responseCode, void *user) = 0;
	virtual void onGetNewTransactionID( unsigned trackingNumber, unsigned responseCode, long long transactionID, void *user) = 0;
	
	// responses to reply requests
	virtual void onReplyReceivePrepareTransaction (unsigned trackingNumber, unsigned responseCode, void *user) = 0;
	virtual void onReplyReceiveCommitTransaction (unsigned trackingNumber, unsigned responseCode, void *user) = 0;
	virtual void onReplyReceiveAbortTransaction (unsigned trackingNumber, unsigned responseCode, void *user) = 0;
	virtual void onReplyReceiveGetCharacterList(unsigned trackingNumber, unsigned responseCode, void *user) = 0;
	
	// house keeping callbacks
	virtual void onIdentifyHost(unsigned trackingNumber, unsigned responseCode, void *user) = 0;


	// Callbacks initiated by the Auction System
	virtual void onReceivePrepareTransaction (unsigned trackingNumber, long long transactionID, unsigned stationID, unsigned characterID, long long assetID, const char *newName) = 0;
	virtual void onReceiveCommitTransaction (unsigned trackingNumber, long long transactionID) = 0;
	virtual void onReceiveAbortTransaction (unsigned trackingNumber, long long transactionID) = 0;
	virtual void onReceiveGetCharacterList(unsigned trackingNumber, unsigned stationID, const char *serverID) = 0;

private:
	AuctionTransferAPICore *m_apiCore;


};


}; // namespace
//////////////////////////////////////////////////////////////////////////////////////
#endif