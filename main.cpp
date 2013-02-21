#include <iostream>

#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <QtDebug>
#include <QFile>
#include <QList>

#include <LightXmlParser.h>
#include <BenchmarkResult.h>
#include <Exception.h>

int main(int argc, char *argv[])
{
    const QString LightXmlFormatString("lightxml");
    const QString IniFileName("QBenchmarkParser.ini");
    using namespace std;

    QCoreApplication a(argc, argv);
    const QString copyrightNotice(QObject::tr("QBenchmarkParser - parse QTestLib benchmark output. "
                                              "(C) 2013 Mirko Boehm <mirko@kde.org>. Licensed under the GPLv3."));
    wcout << copyrightNotice.toStdWString() << endl;
    try {
        //load application init file from current directory:
        const QString filename(QDir::currentPath() + QDir::separator() + IniFileName);
        if (QFile(filename).exists()) {
            wcout << "Loading settings from " << filename.toStdWString() << "..." << endl;
        } else {
            wcout << "Settings file " << filename.toStdWString() << " not found." << endl;
        }
        if (argc <=1) {
            throw UsageException("No filenames given.");
        }
        const QStringList filenames = a.arguments().mid(1);
        QSettings settings(filename, QSettings::IniFormat);
        //generate parser, parse input XML files:
        //at the moment, only lightxml format is supported:
        QList<BenchmarkResult> results;
        if (settings.value("Input/Format", LightXmlFormatString).value<QString>() == LightXmlFormatString) {
            LightXmlParser parser;
            parser.parse(settings, filenames);
            results = parser.results();
        } else {
            //throw...
        }
        //generate formatte, write output file:
    } catch(Exception& e) {
        wcout << e.message().toStdWString() << endl;
        return 1;
    }
    return 0; //redundant, I know
}
