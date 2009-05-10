#include <qendian.h>
#include <QHostAddress>

#include "udp.h"

UdpConfigForm *UdpProtocol::configForm = NULL;

UdpConfigForm::UdpConfigForm(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
}

UdpProtocol::UdpProtocol(
	ProtocolList &frameProtoList,
	OstProto::StreamCore *parent)
	: AbstractProtocol(frameProtoList, parent)
{
	if (configForm == NULL)
		configForm = new UdpConfigForm;
}

UdpProtocol::~UdpProtocol()
{
}

AbstractProtocol* UdpProtocol::createInstance(
	ProtocolList &frameProtoList,
	OstProto::StreamCore *streamCore)
{
	return new UdpProtocol(frameProtoList, streamCore);
}

void UdpProtocol::protoDataCopyInto(OstProto::Stream &stream)
{
	// FIXME: multiple headers
	stream.MutableExtension(OstProto::udp)->CopyFrom(data);
}

void UdpProtocol::protoDataCopyFrom(const OstProto::Stream &stream)
{
	// FIXME: multiple headers
	if (stream.HasExtension(OstProto::udp))
		data.MergeFrom(stream.GetExtension(OstProto::udp));
}

QString UdpProtocol::name() const
{
	return QString("User Datagram Protocol");
}

QString UdpProtocol::shortName() const
{
	return QString("UDP");
}

quint32 UdpProtocol::protocolId(ProtocolIdType type) const
{
	switch(type)
	{
		case ProtocolIdIp: return 0x11;
		default: break;
	}

	return AbstractProtocol::protocolId(type);
}

int	UdpProtocol::fieldCount() const
{
	return udp_fieldCount;
}

QVariant UdpProtocol::fieldData(int index, FieldAttrib attrib,
		int streamIndex) const
{
	switch (index)
	{
		case udp_srcPort:
			switch(attrib)
			{
				case FieldName:			
					return QString("Source Port");
				case FieldValue:
					return data.src_port();
				case FieldTextValue:
					return QString("%1").arg(data.src_port());
				case FieldFrameValue:
				{
					QByteArray fv;
					fv.resize(2);
					qToBigEndian((quint16) data.src_port(), (uchar*) fv.data());
					return fv;
				}
				default:
					break;
			}
			break;

		case udp_dstPort:
			switch(attrib)
			{
				case FieldName:			
					return QString("Destination Port");
				case FieldValue:
					return data.dst_port();
				case FieldTextValue:
					return QString("%1").arg(data.dst_port());
				case FieldFrameValue:
				{
					QByteArray fv;
					fv.resize(2);
					qToBigEndian((quint16) data.dst_port(), (uchar*) fv.data());
					return fv;
				}
				default:
					break;
			}
			break;

		case udp_totLen:
		{

			switch(attrib)
			{
				case FieldName:			
					return QString("Datagram Length");
				case FieldValue:
				{
					int totlen;

					totlen = data.is_override_totlen() ? 
						data.totlen() : 
						(protocolFramePayloadSize() + 8);
					return totlen;
				}
				case FieldFrameValue:
				{
					QByteArray fv;
					int totlen;
					totlen = data.is_override_totlen() ? 
						data.totlen() : 
						(protocolFramePayloadSize() + 8);
					fv.resize(2);
					qToBigEndian((quint16) totlen, (uchar*) fv.data());
					return fv;
				}
				case FieldTextValue:
				{
					int totlen;
					totlen = data.is_override_totlen() ? 
						data.totlen() : 
						(protocolFramePayloadSize() + 8);
					return QString("%1").arg(totlen);
				}
				case FieldBitSize:
					return 16;
				default:
					break;
			}
			break;
		}
		case udp_cksum:
			switch(attrib)
			{
				case FieldName:			
					return QString("Checksum");
				case FieldValue:
					return data.cksum();
				case FieldTextValue:
					return QString("%1").arg(data.cksum());
				case FieldFrameValue:
				{
					QByteArray fv;
					fv.resize(2);
					qToBigEndian((quint16) data.cksum(), (uchar*) fv.data());
					return fv;
				}
				default:
					break;
			}
			break;

		// Meta fields
		case udp_isOverrideTotLen:
		case udp_isOverrideCksum:
			switch(attrib)
			{
				case FieldIsMeta:
					return true;
				default:
					break;
			}
			break;

		default:
			break;
	}

	return AbstractProtocol::fieldData(index, attrib, streamIndex);
}

bool UdpProtocol::setFieldData(int index, const QVariant &value, 
		FieldAttrib attrib)
{
	// FIXME
	return false;
}


QWidget* UdpProtocol::configWidget()
{
	return configForm;
}

void UdpProtocol::loadConfigWidget()
{
	configForm->leUdpSrcPort->setText(QString().setNum(data.src_port()));
	configForm->leUdpDstPort->setText(QString().setNum(data.dst_port()));

	configForm->leUdpLength->setText(QString().setNum(data.totlen()));
	configForm->cbUdpLengthOverride->setChecked(data.is_override_totlen());

	configForm->leUdpCksum->setText(QString().setNum(data.cksum()));
	configForm->cbUdpCksumOverride->setChecked(data.is_override_cksum());
}

void UdpProtocol::storeConfigWidget()
{
	bool isOk;

	data.set_src_port(configForm->leUdpSrcPort->text().toULong(&isOk));
	data.set_dst_port(configForm->leUdpDstPort->text().toULong(&isOk));

	data.set_totlen(configForm->leUdpLength->text().toULong(&isOk));
	data.set_is_override_totlen(configForm->cbUdpLengthOverride->isChecked());

	data.set_cksum(configForm->leUdpCksum->text().remove(QChar(' ')).toULong(&isOk));
	data.set_is_override_cksum(configForm->cbUdpCksumOverride->isChecked());
}
