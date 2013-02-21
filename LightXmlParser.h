#ifndef LIGHTXMLPARSER_H
#define LIGHTXMLPARSER_H

#include <QObject>
#include <QSettings>

#include <BenchmarkResult.h>

class LightXmlParser : public QObject
{
    Q_OBJECT
public:
    explicit LightXmlParser(QObject *parent = 0);
    QList<BenchmarkResult> results() const;

    void parse(const QSettings& settings, const QStringList &filenames);

private:
    QList<BenchmarkResult> results_;
};

#endif // LIGHTXMLPARSER_H
