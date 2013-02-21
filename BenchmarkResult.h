#ifndef BENCHMARKRESULT_H
#define BENCHMARKRESULT_H

#include <QString>

class BenchmarkResult
{
public:
    BenchmarkResult();

private:
    //data about the benchmark:
    QString testFunction_;
    QString tag_;
    QString iterations_;
    //generated attributes:
    QString label_;
    QString revision_;
    //the benchmark result:
    QString value_;
};

#endif // BENCHMARKRESULT_H
