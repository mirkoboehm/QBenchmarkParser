#include <QtCore/QObject>

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
    return m_message;
}


UsageException::UsageException(const QString &message)
    : Exception(usageMessage(message))
{
}

QString UsageException::usageMessage(const QString &message) const
{
    const QString usage = QObject::tr
            ("Usage: QBenchmarkParser -c config file name -o output file name <benchmark result files>\n"
             "Parses the input files, aggregates the contained benchmark data, and\n"
             "stores results in the specified output file.");
    return QObject::tr("%1\n\n%2").arg(message).arg(usage).trimmed();
}
