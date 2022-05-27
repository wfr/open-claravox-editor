#include "Util.h"
#include <deque>
#include <chrono>
#include <QJsonObject>

class ExecutionsPerSecond {
public:
    double measure() {
        auto now = std::chrono::high_resolution_clock::now();
        history.push_back(now);
        while (history.size() > 5) {
            history.pop_front();
        }
        if (history.size() >= 2) {
            std::chrono::duration<double> diff = history.back() - history.front();
            return 1.0 / (diff.count() / history.size());
        }
        return 0.0;
    }
protected:
    std::deque<std::chrono::time_point<std::chrono::high_resolution_clock>> history;
};

bool require(const QJsonObject& obj, const QString &key) {
    if (obj.value(key).isUndefined()) {
        qWarning() << "Expected JSON key not found:" << key;
        return false;
    }
    return true;
}

bool getJsonValue(const QJsonObject& obj, const QString &key, double &out) {
    require(obj, key);
    QJsonValue val = obj.value(key);
    if (val.isUndefined()) {
        return false;
    }
    if (!val.isDouble()) {
        return false;
    }
    out = val.toDouble();
    return true;
}

bool getJsonValue(const QJsonObject& obj, const QString &key, bool &out) {
    require(obj, key);
    QJsonValue val = obj.value(key);
    if (val.isUndefined()) {
        return false;
    }
    if (!val.isBool()) {
        return false;
    }
    out = val.toBool();
    return true;
}

bool getJsonValue(const QJsonObject& obj, const QString &key, QString &out) {
    require(obj, key);
    QJsonValue val = obj.value(key);
    if (val.isUndefined()) {
        return false;
    }
    if (!val.isString()) {
        return false;
    }
    out = val.toString();
    return true;
}
