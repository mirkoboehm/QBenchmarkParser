#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QSharedPointer>

namespace CommandLineParser {

QSharedPointer<QSettings> settings(QStringList &arguments);

}

#endif // COMMANDLINEPARSER_H
