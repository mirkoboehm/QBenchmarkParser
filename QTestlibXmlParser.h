#ifndef LIGHTXMLPARSER_H
#define LIGHTXMLPARSER_H

#include <QObject>
#include <QSettings>
#include <QStringList>

#include <BenchmarkResult.h>

class QXmlStreamReader;

class QTestlibXmlParser : public QObject
{
    Q_OBJECT
public:
    explicit QTestlibXmlParser(QObject *parent = 0);
    QList<BenchmarkResult> results() const;

    void parse(const QSettings& settings, const QStringList &filenames);

private:
    void readTestCase(QXmlStreamReader* reader, const BenchmarkResult& resultSoFar);
    void readEnvironment(QXmlStreamReader* reader, const BenchmarkResult &resultSoFar);
    void readTestFunction(QXmlStreamReader* reader, const BenchmarkResult &resultSoFar);
    void readIncident(QXmlStreamReader* reader, BenchmarkResult &resultSoFar);
    void readBenchmarkResult(QXmlStreamReader* reader, const BenchmarkResult& resultSoFar);

    void readNextStartElementExpecting(const char* elementName, QXmlStreamReader* reader);
    QList<BenchmarkResult> results_;
};

#endif // LIGHTXMLPARSER_H
