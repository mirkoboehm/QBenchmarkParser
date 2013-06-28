#include <algorithm>

#include <QSettings>
#include <QtDebug>
#include <QTextStream>
#include <QFile>

#include <Exception.h>
#include "CSVOutputFormatter.h"

CSVOutputFormatter::CSVOutputFormatter(QSharedPointer<QSettings> settings, QObject *parent)
    : QObject(parent)
    , settings_(settings)
{
}

void CSVOutputFormatter::write(const QList<BenchmarkResult> &results)
{
    using namespace std;

    //collect all existing configuration values for grouping
    QVector<QString> configurations;
    transform(results.begin(), results.end(), back_inserter(configurations),
              [](const BenchmarkResult& b) { return b.configuration_; });
    sort(configurations.begin(), configurations.end());
    configurations.erase(unique(configurations.begin(), configurations.end()), configurations.end());
    //collect all existing data series
    QVector<QString> labels;
    transform(results.begin(), results.end(), back_inserter(labels),
              [](const BenchmarkResult& b) { return b.label_; });
    sort(labels.begin(), labels.end());
    labels.erase(unique(labels.begin(), labels.end()), labels.end());
    //write output
    const QString separator = settings_->value("Output/Separator", QLatin1String(";")).value<QString>();
    QFile output;
    if (!output.open(stdout, QIODevice::WriteOnly)) {
        throw OutputException(tr("Unable to open standard output for writing."));
    }
    QTextStream stream(&output);
    const bool labelsInRows = settings_->value("Output/LabelsInRows", false).value<bool>();
    auto const separatorUnlessLastColumn = [&stream, &separator] (int column, int columns) {
        if (column + 1 < columns) stream << separator;
    };
    if (labelsInRows) {
        const int columns = configurations.count() + 1;
        const int rows = labels.count() + 1;
        for (int row = 0; row < rows; ++ row) {
            for (int column = 0; column < columns; ++column) {
                if (row==0 && column==0) {
                    stream << tr("Configurations");
                    separatorUnlessLastColumn(column, columns);
                } else if (row==0) {
                    const QString value = configurations[column-1];
                    stream << value;
                    separatorUnlessLastColumn(column, columns);
                } else if (column==0) {
                    const QString value = labels[row-1];
                    stream << value;
                    separatorUnlessLastColumn(column, columns);
                } else {
                    stream << value(results, labels[row-1], configurations[column-1]);
                    separatorUnlessLastColumn(column, columns);
                }
            }
            stream << endl;
        }
    } else { // labels in columns, configurations in rows
        const int columns = labels.count() + 1;
        const int rows = configurations.count() + 1;
        for (int row = 0; row < rows; ++ row) {
            for (int column = 0; column < columns; ++column) {
                if (row==0 && column==0) {
                    stream << tr("Configurations");
                    separatorUnlessLastColumn(column, columns);
                } else if (row==0) {
                    const QString value = labels[column-1];
                    stream << value;
                    separatorUnlessLastColumn(column, columns);
                } else if (column==0) {
                    const QString value = configurations[row-1];
                    stream << value;
                    separatorUnlessLastColumn(column, columns);
                } else {
                    stream << value(results, labels[column-1], configurations[row-1]);
                    separatorUnlessLastColumn(column, columns);
                }
            }
            stream << endl;
        }
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
