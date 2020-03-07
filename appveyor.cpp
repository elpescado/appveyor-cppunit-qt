#include "appveyor.h"

#include <QDebug>
#include <QEventLoop>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcessEnvironment>

namespace AppVeyor {

QString toString(TestOutcome outcome)
{
    switch (outcome) {
        case TestOutcome::None: return "None";
        case TestOutcome::Running: return "Running";
        case TestOutcome::Passed: return "Passed";
        case TestOutcome::Failed: return "Failed";
        case TestOutcome::Ignored: return "Ignored";
        case TestOutcome::Skipped: return "Skipped";
        case TestOutcome::Inconclusive: return "Inconclusive";
        case TestOutcome::NotFound: return "NotFound";
        case TestOutcome::Cancelled: return "Cancelled";
        case TestOutcome::NotRunnable: return "NotRunnable";
    }
    return "";
}

void Test::toJson(QJsonObject &json) const
{
    json["testName"] = name;
    json["testFramework"] = framework;
    json["fileName"] = fileName;
    json["outcome"] = toString(outcome);
    json["durationMilliseconds"] = duration;
    json["ErrorMessage"] = errorMessage;
    json["ErrorStackTrace"] = errorStackTrace;
    json["StdOut"] = stdOut;
    json["stdErr"] = stdErr;
}


BuildWorkerApiClient::BuildWorkerApiClient()
{
    QString apiRoot = QProcessEnvironment::systemEnvironment().value("APPVEYOR_API_URL", "");
    m_ApiRoot = QUrl(apiRoot);
    qDebug() << "Using API Root: " << apiRoot;
}


void BuildWorkerApiClient::addTest(const Test &testData)
{
    QJsonObject json;
    testData.toJson(json);
    QJsonDocument jsonDoc(json);
    doSyncRequest("POST", "api/tests", jsonDoc);
}


void BuildWorkerApiClient::updateTest(const Test &testData)
{
    QJsonObject json;
    testData.toJson(json);
    QJsonDocument jsonDoc(json);
    doSyncRequest("PUT", "api/tests", jsonDoc);
}


void BuildWorkerApiClient::doSyncRequest(QByteArray method, QString endpoint, const QJsonDocument &payload)
{
    QUrl endpointUrl(endpoint);

    QByteArray jsonPayload = payload.toJson(QJsonDocument::Compact);

    QNetworkAccessManager qnam;
    QNetworkRequest request{m_ApiRoot.resolved(endpointUrl)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    auto reply = qnam.sendCustomRequest(request, method, jsonPayload);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    qDebug() << reply->errorString();
    QByteArray content = reply->readAll();
    qDebug() << QString{content};
}

};
