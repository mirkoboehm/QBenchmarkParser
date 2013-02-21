#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>

#include <QString>

class Exception : public std::runtime_error {
public:
    explicit Exception(const QString& message);
    ~Exception() throw();
    QString message() const;

private:
    QString m_message;

};

class UsageException : public Exception {
public:
    explicit UsageException(const QString& message);

private:
    QString usageMessage(const QString& message) const;
};

#endif // EXCEPTION_H
