#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>

#include <QString>

class QXmlStreamReader;

//FIXME Add label() virtual method ("Input error", "Format error", ...)
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

class InputException : public Exception {
public:
    explicit InputException(const QString& message) : Exception(message) {}
};

class InputFormatException : public Exception {
public:
    explicit InputFormatException(const QString& message) : Exception(message) {}
    InputFormatException(const QString& message, QXmlStreamReader* reader);
};

class OutputException : public Exception {
public:
    explicit OutputException(const QString& message) : Exception(message) {}
};

#endif // EXCEPTION_H
