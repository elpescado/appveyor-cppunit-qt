#ifndef APPVEYOR_H
#define APPVEYOR_H

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QUrl>

namespace AppVeyor {

enum class TestOutcome {
    None,
    Running,
    Passed,
    Failed,
    Ignored,
    Skipped,
    Inconclusive,
    NotFound,
    Cancelled,
    NotRunnable
};


struct Test {
    QString name;
    QString framework;
    QString fileName;
    TestOutcome outcome;
    int duration = 0;
    QString errorMessage;
    QString errorStackTrace;
    QString stdOut;
    QString stdErr;

    void toJson(QJsonObject &json) const;
};


class BuildWorkerApiClient {
    public:
    BuildWorkerApiClient();
    void addTest(const Test &testData);
    void updateTest(const Test &testData);

    private:
    QUrl m_ApiRoot;

    void doSyncRequest(QByteArray method, QString endpoint, const QJsonDocument &payload);
};

};

#endif
