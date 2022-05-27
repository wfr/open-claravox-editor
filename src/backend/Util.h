#pragma once

class QJsonObject;
class QString;

bool getJsonValue(const QJsonObject& obj, const QString &key, double &out);
bool getJsonValue(const QJsonObject& obj, const QString &key, bool &out);
bool getJsonValue(const QJsonObject& obj, const QString &key, QString& out);

template<class T>
bool in_range(T x, T a, T b) {
    return x >= a && x <= b;
}

