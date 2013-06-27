#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>

#include <QString>

class QXmlStreamReader;

class Exception : public std::runtime_error {
public:
    explicit Exception(const QString& message);
    ~Exception() throw();
    QString message() const;

protected:
    virtual QString label() const;

private:
    QString m_message;

};

class UsageException : public Exception {
public:
    explicit UsageException(const QString& message);
protected:
    QString label() const override;
private:
    QString usageMessage(const QString& message) const;
};

class InputException : public Exception {
public:
    explicit InputException(const QString& message) : Exception(message) {}
protected:
    QString label() const override;
};

class InputFormatException : public Exception {
public:
    explicit InputFormatException(const QString& message) : Exception(message) {}
    InputFormatException(const QString& message, QXmlStreamReader* reader);
protected:
    QString label() const override;
};

class OutputException : public Exception {
public:
    explicit OutputException(const QString& message) : Exception(message) {}
protected:
    QString label() const override;
};

#endif // EXCEPTION_H
