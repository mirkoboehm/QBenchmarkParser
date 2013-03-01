#include <iostream>

#include <QtDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QFileInfo>

#include "QTestlibXmlParser.h"
#include "Exception.h"

QTestlibXmlParser::QTestlibXmlParser(QObject *parent)
    : QObject(parent)
{
}

QList<BenchmarkResult> QTestlibXmlParser::results() const
{
    return results_;
}

namespace {
const char* TestCaseElement = "TestCase";
const char* EnvironmentElement = "Environment";
const char* TestFunctionElement = "TestFunction";
const char* TestFunctionNameAttribute = "name";
const char* MessageElement = "Message";
const char* IncidentElement = "Incident";
const char* IncidentTypeAttribute = "type";
const char* DataTagElement = "DataTag";
const char* BenchmarkResultElement = "BenchmarkResult";
const char* BenchmarkMetricAttribute = "metric";
const char* BenchmarkTagAttribute = "tag";
const char* BenchmarkValueAttribute = "value";
const char* BenchmarkIterationsAttribute = "iterations";
const char* QtVersionElement = "QtVersion";
const char* QTestVersionElement = "QTestVersion";
}

void QTestlibXmlParser::parse(QSharedPointer<QSettings>, const QStringList& filenames)
{
    Q_FOREACH(const QString& filename, filenames) {
        QFile file(filename);
        if (!file.exists()) throw InputException(tr("File not found: %1").arg(filename));
        if (!file.open(QIODevice::ReadOnly)) throw InputException(tr("Unable to open file: %1").arg(filename));
        std::wcerr << tr("Parsing input file %1.").arg(filename).toStdWString() << std::endl;
        QXmlStreamReader reader(&file);
        BenchmarkResult result;
        QFileInfo fileInfo(file);
        result.filename_ = fileInfo.baseName();
        readTestCase(&reader, result);
    }
}

void QTestlibXmlParser::readTestCase(QXmlStreamReader *reader, const BenchmarkResult &resultSoFar)
{
    readNextStartElementExpecting(TestCaseElement, reader);
    readEnvironment(reader, resultSoFar);
    while (reader->readNextStartElement()) {
        if (reader->name() == TestFunctionElement) {
            readTestFunction(reader, resultSoFar);
        } else {
            throw InputFormatException(tr("Unexpected element %1").arg(reader->name().toString()), reader);
        }
    }
}

void QTestlibXmlParser::readEnvironment(QXmlStreamReader *reader, const BenchmarkResult&)
{
    readNextStartElementExpecting(EnvironmentElement, reader);
    readNextStartElementExpecting(QtVersionElement, reader);
    const QString qtVersion = reader->readElementText();
    readNextStartElementExpecting(QTestVersionElement, reader);
    const QString qtestVersion = reader->readElementText();
    Q_UNUSED(qtVersion); Q_UNUSED(qtestVersion);
    if (reader->readNextStartElement()) {
        throw InputFormatException(tr("Unexpected element %1").arg(reader->name().toString()), reader);
    }
}

void QTestlibXmlParser::readTestFunction(QXmlStreamReader *reader, const BenchmarkResult &resultSoFar)
{
    BenchmarkResult result(resultSoFar);
    result.testFunction_ = reader->attributes().value(TestFunctionNameAttribute).toString();
    while (reader->readNextStartElement()) {
        if (reader->name() == IncidentElement) {
            readIncident(reader, result);
        } else if (reader->name() == BenchmarkResultElement) {
            readBenchmarkResult(reader, result);
        } else if (reader->name() == MessageElement) {
            reader->skipCurrentElement();
        } else {
            throw InputFormatException(tr("Unexpected element %1").arg(reader->name().toString()), reader);
        }
        reader->readElementText();
    }
}

void QTestlibXmlParser::readIncident(QXmlStreamReader *reader, BenchmarkResult& resultSoFar)
{
    resultSoFar.passed_ = (reader->attributes().value(IncidentTypeAttribute).toString() == "pass");
    while (reader->readNextStartElement()) {
        if (reader->name() == DataTagElement) {
            reader->readElementText(); //ignore
        } else {
            throw InputFormatException(tr("Unexpected element %1").arg(reader->name().toString()), reader);
        }
        reader->readElementText();
    }
}

void QTestlibXmlParser::readBenchmarkResult(QXmlStreamReader *reader, const BenchmarkResult &resultSoFar)
{
    BenchmarkResult result(resultSoFar);
    result.metric_ = reader->attributes().value(BenchmarkMetricAttribute).toString();
    result.tag_ = reader->attributes().value(BenchmarkTagAttribute).toString();
    result.value_ = reader->attributes().value(BenchmarkValueAttribute).toString();
    result.iterations_ = reader->attributes().value(BenchmarkIterationsAttribute).toString();
    reader->readElementText();
    results_.append(result);
}

void QTestlibXmlParser::readNextStartElementExpecting(const char *elementName, QXmlStreamReader *reader)
{
    reader->readNextStartElement();
    if (reader->name() != elementName) {
        throw InputFormatException(tr("%1 element expected.").arg(elementName), reader);
    }
}
