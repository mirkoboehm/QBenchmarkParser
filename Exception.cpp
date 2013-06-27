#include <QtCore/QObject>
#include <QXmlStreamReader>

#include "Exception.h"

Exception::Exception( const QString& message )
    : std::runtime_error( message.toStdString() )
    , m_message( message )
{
}

Exception::~Exception() throw()
{
}

QString Exception::message() const
{
    return QObject::tr("%1: %2").arg(label()).arg(m_message);
}

QString Exception::label() const
{
    return QObject::tr("Error");
}


UsageException::UsageException(const QString &message)
    : Exception(usageMessage(message))
{
}

QString UsageException::label() const
{
    return QObject::tr("Usage error");
}

QString UsageException::usageMessage(const QString &message) const
{
    const QString usage = QObject::tr
            ("Usage: QBenchmarkParser -c config file name -o output file name <benchmark result files>\n"
             "Parses the input files, aggregates the contained benchmark data, and\n"
             "stores results in the specified output file.");
    return QObject::tr("%1\n\n%2").arg(message).arg(usage).trimmed();
}


InputFormatException::InputFormatException(const QString &message, QXmlStreamReader *reader)
    : Exception(QObject::tr("%1:%2: %3").arg(reader->lineNumber()).arg(reader->columnNumber()).arg(message))
{
}


QString InputException::label() const
{
    return QObject::tr("Input error");
}

QString InputFormatException::label() const
{
    return QObject::tr("Input format error");
}


QString OutputException::label() const
{
    return QObject::tr("Output error");
}
