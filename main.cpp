#include <iostream>

#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <QtDebug>
#include <QFile>
#include <QList>
#include <QRegExp>

#include <QTestlibXmlParser.h>
#include <BenchmarkResult.h>
#include <Exception.h>

int main(int argc, char *argv[])
{
    const QString QTestlibXmlFormatString("xml");
    const QString IniFileName("QBenchmarkParser.ini");
    using namespace std;

    QCoreApplication a(argc, argv);
    const QString copyrightNotice(QObject::tr("QBenchmarkParser - parse QTestLib benchmark output. "
                                              "(C) 2013 Mirko Boehm <mirko@kde.org>. Licensed under the GPLv3."));
    wcout << copyrightNotice.toStdWString() << endl;
    try {
        //load application init file from current directory:
        const QString filename(QDir::currentPath() + QDir::separator() + IniFileName);
        if (QFile(filename).exists()) {
            wcout << QObject::tr("Loading settings from %1.").arg(filename).toStdWString() << endl;
        } else {
            wcout << QObject::tr("Settings file %1 not found!").arg(filename).toStdWString() << endl;
        }
        if (argc <=1) {
            throw UsageException(QObject::tr("No filenames specified."));
        }
        const QStringList filenames = a.arguments().mid(1);
        QSettings settings(filename, QSettings::IniFormat);
        //generate parser, parse input XML files:
        //at the moment, only lightxml format is supported:
        QList<BenchmarkResult> results;
        //FIXME format could actually be autodetected per file, as long as it is XML
        const QString inputFileFormat(settings.value("Input/Format", QTestlibXmlFormatString).value<QString>());
        if (inputFileFormat==QTestlibXmlFormatString) {
            QTestlibXmlParser parser;
            parser.parse(settings, filenames);
            results = parser.results();
        } else {
            throw InputException(QObject::tr("Input file format %1 not supported.").arg(inputFileFormat));
        }
        //generate formatte, write output file:
        const QString seriesRXsetting = settings.value("Input/SeriesRX").toString();
        QRegExp seriesRX(seriesRXsetting);
        const QString configurationRXsetting = settings.value("Input/ConfigurationRX").toString();
        QRegExp configurationRX(configurationRXsetting);
        for(QList<BenchmarkResult>::iterator it = results.begin(); it != results.end(); ++it) {
            //group results based on SeriesRX and ConfigurationRX specified in settings:
            BenchmarkResult& result = *it;
            if (seriesRX.indexIn(result.tag_) != -1) {
                result.series_ = seriesRX.cap(1);
            }
            if (configurationRX.indexIn(result.tag_) != -1) {
                result.configuration_ = configurationRX.cap(1);
            }
        }
        Q_FOREACH(const BenchmarkResult& result, results)  {
            qDebug() << result.testFunction_ << result.series_ << result.configuration_ << result.passed_ << result.tag_ << result.metric_
                     << result.value_;
        }
    } catch(Exception& e) {
        wcout << e.message().toStdWString() << endl;
        return 1;
    }
    return 0; //redundant, I know
}
