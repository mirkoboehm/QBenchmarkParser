#include <QString>
#include <QtTest>

class ParserTests : public QObject
{
    Q_OBJECT
    
public:
    ParserTests();
    
private Q_SLOTS:
    void testXmlParser();
};

ParserTests::ParserTests()
{
}

void ParserTests::testXmlParser()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(ParserTests)

#include "tst_ParserTests.moc"
