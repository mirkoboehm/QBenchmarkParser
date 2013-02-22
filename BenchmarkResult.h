#ifndef BENCHMARKRESULT_H
#define BENCHMARKRESULT_H

#include <QString>

struct BenchmarkResult
{
    BenchmarkResult();

    //data about the benchmark:
    QString testFunction_;
    QString tag_;
    QString iterations_;
    //generated attributes:
    QString label_;
    QString filename_;
    QString series_;
    QString configuration_;
    //the benchmark result:
    QString metric_;
    QString value_;
    bool passed_;
};

#endif // BENCHMARKRESULT_H
