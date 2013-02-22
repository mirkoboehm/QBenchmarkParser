#include <algorithm>

#include <QSettings>
#include <QtDebug>
#include <QTextStream>
#include <QFile>

#include <Exception.h>
#include "CSVOutputFormatter.h"

CSVOutputFormatter::CSVOutputFormatter(QSettings *settings, QObject *parent)
    : QObject(parent)
    , settings_(settings)
{
}

void CSVOutputFormatter::write(const QList<BenchmarkResult> &results)
{
    //collect all existing configuration values for grouping
    QVector<QString> configurations;
    std::transform(results.begin(), results.end(), std::back_inserter(configurations),
                   [](const BenchmarkResult& b) -> const QString& { return b.configuration_; });
    std::sort(configurations.begin(), configurations.end());
    configurations.erase(std::unique(configurations.begin(), configurations.end()), configurations.end());
    //collect all existing data series
    QVector<QString> labels;
    std::transform(results.begin(), results.end(), std::back_inserter(labels),
                   [](const BenchmarkResult& b) -> const QString& { return b.label_; });
    std::sort(labels.begin(), labels.end());
    labels.erase(std::unique(labels.begin(), labels.end()), labels.end());
    //write output
    const QString separator = settings_->value("Output/Separator", QLatin1String(";")).value<QString>();
    QFile output;
    if (!output.open(stdout, QIODevice::WriteOnly)) {
        throw OutputException(tr("Unable to open standard output for writing."));
    }
    QTextStream stream(&output);
    const int columns = labels.count() + 1;
    const int rows = configurations.count() + 1;
    for (int row = 0; row < rows; ++ row) {
        for (int column = 0; column < columns; ++column) {
            if (row==0 && column==0) {
                stream << separator;
            } else if (row==0) {
                const QString value = labels[column-1];
                stream << value << separator;
            } else if (column==0) {
                const QString value = configurations[row-1];
                stream << value << separator;
            } else {
                stream << value(results, labels[column-1], configurations[row-1]) << separator;
            }
        }
        stream << endl;
    }
}

QString CSVOutputFormatter::value(const QList<BenchmarkResult> &results, const QString &label, const QString &configuration)
{
    Q_FOREACH(const BenchmarkResult& result, results) {
        if (result.label_==label && result.configuration_ == configuration) {
            return result.value_;
        }
    }
    return QString();
}
