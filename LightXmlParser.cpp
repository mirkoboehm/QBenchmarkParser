#include "LightXmlParser.h"

LightXmlParser::LightXmlParser(QObject *parent)
    : QObject(parent)
{
}

QList<BenchmarkResult> LightXmlParser::results() const
{
    return results_;
}

void LightXmlParser::parse(const QSettings &settings, const QStringList& filenames)
{

}
