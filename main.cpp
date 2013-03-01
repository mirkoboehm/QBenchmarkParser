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
#include <CommandLineParser.h>
#include <BenchmarkResult.h>
#include <Exception.h>
#include <CSVOutputFormatter.h>

int main(int argc, char *argv[])
{
    const QString QTestlibXmlFormatString("xml");
    const QString CSVOutputFormatString("CSV");
    using namespace std;

    QCoreApplication a(argc, argv);
    const QString copyrightNotice(QObject::tr("QBenchmarkParser - parse QTestLib benchmark output. "
                                              "(C) 2013 Mirko Boehm <mirko@kde.org>. Licensed under the GPLv3."));
    wcerr << copyrightNotice.toStdWString() << endl;
    try {
        QStringList arguments = a.arguments().mid(1);
        QSharedPointer<QSettings> settings(CommandLineParser::settings(arguments));
        const QStringList filenames = arguments;
        //generate parser, parse input XML files:
        //at the moment, only lightxml format is supported:
        QList<BenchmarkResult> results;
        //FIXME format could actually be autodetected per file, as long as it is XML
        const QString inputFileFormat(settings->value("Input/Format", QTestlibXmlFormatString).value<QString>());
        if (inputFileFormat==QTestlibXmlFormatString) {
            QTestlibXmlParser parser;
            parser.parse(settings, filenames);
            results = parser.results();
        } else {
            throw InputException(QObject::tr("Input file format %1 not supported.").arg(inputFileFormat));
        }
        //extend generated data, using SeriesRX and ConfigurationRX settings, and calculate labels
        const QString seriesRxSetting = settings->value("Input/SeriesRX").toString();
        QRegExp seriesRX(seriesRxSetting);
        const QString configurationRxSetting = settings->value("Input/ConfigurationRX").toString();
        QRegExp configurationRX(configurationRxSetting);
        const QString groupBySetting = settings->value("Output/GroupBy").toString();
        for(QList<BenchmarkResult>::iterator it = results.begin(); it != results.end(); ++it) {
            //extend results based on SeriesRX and ConfigurationRX specified in settings:
            BenchmarkResult& result = *it;
            if (seriesRX.indexIn(result.tag_) != -1) {
                result.series_ = seriesRX.cap(1);
            }
            if (configurationRX.indexIn(result.tag_) != -1) {
                result.configuration_ = configurationRX.cap(1);
            }
            //group results by setting label to the string specified in groupBySetting, sort them by label:
            if (!groupBySetting.isEmpty()) {
                result.label_ = groupBySetting;
                result.label_.replace("testfunction", result.testFunction_);
                result.label_.replace("tag", result.tag_);
                result.label_.replace("iterations", result.iterations_);
                result.label_.replace("filename", result.filename_);
                result.label_.replace("series", result.series_);
                result.label_.replace("configuration", result.configuration_);
                result.label_.replace("metric", result.metric_);
                result.label_.replace("value", result.value_);
                result.label_.replace("passed", result.passed_ ? "true" : "false");
            }
        }
        std::stable_sort(results.begin(), results.end(),
                         [](const BenchmarkResult& l, const BenchmarkResult& r) -> bool { return l.label_ < r.label_; });
        //create formatter, write output:
        const QString outFileFormat(settings->value("Output/Format", QTestlibXmlFormatString).value<QString>());
        if (outFileFormat==CSVOutputFormatString) {
            CSVOutputFormatter formatter(settings);
            formatter.write(results);
        } else {
            throw InputException(QObject::tr("Output file format %1 not supported.").arg(outFileFormat));
        }
    } catch(Exception& e) {
        wcerr << e.message().toStdWString() << endl;
        return 1;
    }
    return 0; //redundant, I know
}
