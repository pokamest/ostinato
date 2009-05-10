#ifndef _ABSTRACT_PROTOCOL_H
#define _ABSTRACT_PROTOCOL_H

#include <QString>
#include <QVariant>
#include <QByteArray>
#include <QWidget>
#include <QLinkedList>

//#include "../rpc/pbhelper.h"
#include "../common/protocol.pb.h"

#define BASE_BIN (2)
#define BASE_OCT (8)
#define BASE_DEC (10)
#define BASE_HEX (16)

#define uintToHexStr(num, bytes)	\
	QString("%1").arg(num, bytes*2, BASE_HEX, QChar('0'))

class OstProto::StreamCore;
class AbstractProtocol;

typedef QLinkedList<const AbstractProtocol*> ProtocolList;

class AbstractProtocol
{
private:
	mutable int		metaCount;
	mutable int		protoSize;
	mutable QString protoAbbr;

protected:
	OstProto::StreamCore	*stream;
	ProtocolList			&frameProtocols;

public:
	enum FieldAttrib {
		FieldName,			//! name
		FieldValue,			//! value in host byte order (user editable)
		FieldTextValue,		//! value as text
		FieldFrameValue,	//! frame encoded value in network byte order
		FieldBitSize,		//! size in bits
		FieldIsMeta			//! bool indicating if field is meta
	};

	enum ProtocolIdType {
		ProtocolIdLlc,
		ProtocolIdEth,
		ProtocolIdIp,
	};

	AbstractProtocol(ProtocolList &frameProtoList, 
		OstProto::StreamCore *parent = 0);
	virtual ~AbstractProtocol();

	static AbstractProtocol* createInstance(
		ProtocolList &frameProtoList,
		OstProto::StreamCore *streamCore = 0);

	virtual void protoDataCopyInto(OstProto::Stream &stream) = 0;
	virtual void protoDataCopyFrom(const OstProto::Stream &stream) = 0;

	virtual QString name() const;
	virtual QString shortName() const;

	virtual quint32 protocolId(ProtocolIdType type) const;
	quint32 payloadProtocolId(ProtocolIdType type) const;

	virtual int	fieldCount() const;
	virtual int	metaFieldCount() const;
	int	frameFieldCount() const;

	virtual QVariant fieldData(int index, FieldAttrib attrib,
		   	int streamIndex = 0) const;
	virtual bool setFieldData(int index, const QVariant &value, 
			FieldAttrib attrib = FieldValue);

	QByteArray protocolFrameValue(int streamIndex = 0) const;
	int protocolFrameSize() const;
	int protocolFrameOffset() const;
	int protocolFramePayloadSize() const;

	virtual QVariant protocolFrameCksum() const;
	QVariant protocolFramePayloadCksum() const;

	virtual QWidget* configWidget() = 0;
	virtual void loadConfigWidget() = 0;
	virtual void storeConfigWidget() = 0;
};

#endif