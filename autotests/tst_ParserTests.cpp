#include <QString>
#include <QtTest>
#include <QTestlibXmlParser.h>

#include <CommandLineParser.h>
#include <Exception.h>

class ParserTests : public QObject
{
    Q_OBJECT
    
public:
    ParserTests();
    
private Q_SLOTS:
    void testCommandLineParser();
    void testXmlParser();

private:
    QStringList sampleThreadWeaverArguments();
    QList<BenchmarkResult> filterResults(const QList<BenchmarkResult>& results, const QString& function, const QString& tag);
};

namespace {
const QString configFile(":/sample_threadweaver/QBenchmarkParser.ini");
const QString configOption(QObject::tr("--configuration=%1").arg(configFile));
const QString inputFile(":/sample_threadweaver/896c847.xml");
}

ParserTests::ParserTests()
{
}

void ParserTests::testCommandLineParser()
{
    try {
        QStringList arguments(sampleThreadWeaverArguments());
        QSharedPointer<QSettings> settings = CommandLineParser::settings(arguments);
        Q_UNUSED(settings);
        QVERIFY(!arguments.contains(configOption));
        QCOMPARE(arguments.count(), 1);
        QCOMPARE(settings->fileName(), configFile);
    } catch(Exception& e) {
        QFAIL(e.what());
    }
}

void ParserTests::testXmlParser()
{
    QStringList arguments(sampleThreadWeaverArguments());
    QSharedPointer<QSettings> settings = CommandLineParser::settings(arguments);
    QTestlibXmlParser parser;
    parser.parse(settings, arguments);
    QList<BenchmarkResult> results = parser.results();
    QList<BenchmarkResult> filtered = filterResults(results, "IndividualJobsBenchmark", "1 threads, 1 values");
    QCOMPARE(filtered.count(), 1);
    QCOMPARE(filtered[0].value_, QLatin1String("4.93994e+08"));
}

QStringList ParserTests::sampleThreadWeaverArguments()
{
    const QString configFile(":/sample_threadweaver/QBenchmarkParser.ini");
    const QString configOption(tr("--configuration=%1").arg(configFile));
    const QString inputFile(":/sample_threadweaver/896c847.xml");
    return QStringList() << configOption << inputFile;
}

QList<BenchmarkResult> ParserTests::filterResults(const QList<BenchmarkResult> &results,
                                                  const QString &function, const QString &tag)
{
    QList<BenchmarkResult> filtered;
    Q_FOREACH(const BenchmarkResult& result, results) {
        if (result.testFunction_ == function && result.tag_ == tag) {
            filtered << result;
        }
    }
    return filtered;
}

QTEST_APPLESS_MAIN(ParserTests)

#include "tst_ParserTests.moc"
