#include "totomat.h"

Totomat::Totomat(QObject *parent) : QObject(parent) {
    // Create the network access objects
    this->nam = new QNetworkAccessManager(this);
    this->decoder = new Json(this);

    // Create the game lists
//    this->nlaData = new GamedayData(this);

    // Create a timer and set the update interval
    this->timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(30000);

    // Send an initial query
    this->query();
}

// Set the data store
void Totomat::setData(GamedayData *data){
    this->nlaData = data;
}

// Return the data store
GamedayData* Totomat::getData() {
    return this->nlaData;
}

// Send a query to the National League Server
void Totomat::query(void) {
    // Request URL / Headers
    QString url = "http://www.swiss-icehockey.ch/liveticker/totomat.php";
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader, 0);
    request.setRawHeader("Connection", "Close");

    // Request parameters: None
    QByteArray parameters;

    // Send the request and connect the finished() signal of the reply to parser
    this->reply = this->nam->post(request, parameters);
    connect(reply, SIGNAL(finished()), this, SLOT(parseResponse()));
}

// Parse the response from the HTTP Request
void Totomat::parseResponse() {
    QMap<QString, QVariant> data = this->decoder->decode(this->reply->readAll().data());
    QVariantMap content = data["content"].toMap();
    QVariantMap games = content["games"].toMap();
    QVariantMap dates = content["nav"].toMap();
    QVariantList nla = games["nla"].toList();
    this->nlaData->updateGames(dates["nla"].toString(), nla);
}

// Update the totomat when the timer times out
void Totomat::update() {
#if 0
    qDebug() << "Timer fired, updating totomat.";
#endif

    // Query the website and update
    this->query();
}
