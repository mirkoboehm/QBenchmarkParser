#ifndef CSVOUTPUTFORMATTER_H
#define CSVOUTPUTFORMATTER_H

#include <QObject>
#include <QList>
#include <QSettings>
#include <QSharedPointer>

#include <BenchmarkResult.h>

class QSettings;

class CSVOutputFormatter : public QObject
{
    Q_OBJECT
public:
    explicit CSVOutputFormatter(QSharedPointer<QSettings> settings, QObject *parent = 0);
    
    void write(const QList<BenchmarkResult>& results);

private:
    QString value(const QList<BenchmarkResult>& results, const QString& label, const QString& configuration);
    QSharedPointer<QSettings> settings_;
};

#endif // CSVOUTPUTFORMATTER_H
