#include "hexlineedit.h"
#include "qdebug.h"

QString & uintToHexStr(quint64 num, QString &hexStr, quint8 octets);

HexLineEdit::HexLineEdit( QWidget * parent) 
	: QLineEdit(parent)
{
	//QLineEdit::QLineEdit(parent);
}

void HexLineEdit::focusOutEvent( QFocusEvent *e )
{
#if 0
    const QValidator *v = validator();
    if ( v )
    {
        int curpos = cursorPosition();
        QString str = text();
        if ( v->validate( str, curpos ) == QValidator::Acceptable )
        {
            if ( curpos != cursorPosition() )
                setCursorPosition( curpos );
            if ( str != text() )
                setText( str );
        }
        else
        {
            if ( curpos != cursorPosition() )
                setCursorPosition( curpos );
            str = text();
            v->fixup( str );
            if ( str != text() )
            {
                setText( str );
            }
        }
    }
    QLineEdit::focusOutEvent( e );
    emit focusOut();
#else
	bool isOk;
	ulong	num;
	QString str;

	qDebug("before = %s\n", text().toAscii().data());
	num = text().remove(QChar(' ')).toULong(&isOk, 16);
	setText(uintToHexStr(num, str, 4));
	qDebug("after = %s\n", text().toAscii().data());
	qDebug("after2 = %s\n", str.toAscii().data());
#endif
}

#if 0
void HexLineEdit::focusInEvent( QFocusEvent *e )
{
    QLineEdit::focusInEvent( e );
    emit focusIn();
}

void HexLineEdit::keyPressEvent( QKeyEvent *e )
{
    QLineEdit::keyPressEvent( e );
    if ( e->key() == Key_Enter || e->key() == Key_Return )
    {
        setSelection( 0, text().length() );
    }
}
#endif
