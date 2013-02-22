#include <iostream>
#include <algorithm>

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

bool compareLabel(const BenchmarkResult& left, const BenchmarkResult& right) {
    return left.label_ < right.label_;
}

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
        const QString seriesRxSetting = settings.value("Input/SeriesRX").toString();
        QRegExp seriesRX(seriesRxSetting);
        const QString configurationRxSetting = settings.value("Input/ConfigurationRX").toString();
        QRegExp configurationRX(configurationRxSetting);
        const QString groupBySetting = settings.value("Output/GroupBy").toString();
        for(QList<BenchmarkResult>::iterator it = results.begin(); it != results.end(); ++it) {
            //extend results based on SeriesRX and ConfigurationRX specified in settings:
            BenchmarkResult& result = *it;
            if (seriesRX.indexIn(result.tag_) != -1) {
                result.series_ = seriesRX.cap(1);
            }
            if (configurationRX.indexIn(result.tag_) != -1) {
                result.configuration_ = configurationRX.cap(1);
            }
            //group results by setting label to the string specified in groupBySetting:
            if (!groupBySetting.isEmpty()) {
                result.label_ = groupBySetting;
                result.label_.replace("testfunction", result.testFunction_);
                result.label_.replace("tag", result.tag_);
                result.label_.replace("iterations", result.iterations_);
                result.label_.replace("revision", result.revision_);
                result.label_.replace("series", result.series_);
                result.label_.replace("configuration", result.configuration_);
                result.label_.replace("metric", result.metric_);
                result.label_.replace("value", result.value_);
                result.label_.replace("passed", result.passed_ ? "true" : "false");
            }
        }
        //sort results by label:
        std::stable_sort(results.begin(), results.end(), compareLabel);
        Q_FOREACH(const BenchmarkResult& result, results)  {
            qDebug() << result.label_ << result.testFunction_ << result.series_ << result.configuration_ << result.passed_ << result.tag_ << result.metric_
                     << result.value_;
        }
    } catch(Exception& e) {
        wcout << e.message().toStdWString() << endl;
        return 1;
    }
    return 0; //redundant, I know
}
