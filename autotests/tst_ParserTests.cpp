#include <QString>
#include <QtTest>

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
};

ParserTests::ParserTests()
{
}

void ParserTests::testCommandLineParser()
{
    try {
        const QString configFile(":/sample_threadweaver/QBenchmarkParser.ini");
        const QString configOption(tr("--configuration=%1").arg(configFile));
        const QString inputFile(":/sample_threadweaver/896c847.xml");
        QStringList arguments;
        arguments << configOption << inputFile;
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
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(ParserTests)

#include "tst_ParserTests.moc"
