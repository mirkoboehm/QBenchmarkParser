#include <iostream>

#include <QSettings>
#include <QRegExp>
#include <QStringList>
#include <QFile>
#include <QDir>

#include "CommandLineParser.h"
#include "Exception.h"

namespace CommandLineParser {

/** @brief Parses settings, removes identified command line parameters. */
QSharedPointer<QSettings> settings(QStringList& arguments)
{
    using namespace std;

    const QString DefaultIniFileName("QBenchmarkParser.ini");
    QString iniFileName = QDir::currentPath() + QDir::separator() + DefaultIniFileName;
    QRegExp rx("--configuration=(.+)");
    forever {
        const int index = arguments.indexOf(rx, 0);
        if (index == -1) {
            break;
        } else {
            iniFileName = rx.cap(1);
            arguments.removeAt(index);
        }
    }

    if (QFile(iniFileName).exists()) {
        wcerr << QObject::tr("Loading settings from %1.").arg(iniFileName).toStdWString() << endl;
    } else {
        throw UsageException(QObject::tr("Settings file %1 not found!").arg(iniFileName));
    }
    if (arguments.isEmpty()) {
        throw UsageException(QObject::tr("No filenames specified."));
    }
    QSharedPointer<QSettings> settings(new QSettings(iniFileName, QSettings::IniFormat));
    return settings;
}

}
