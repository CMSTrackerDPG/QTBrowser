#include "settingsmanager.h"
#include <QDebug>

SettingsManager* SettingsManager::instance_ = nullptr;

SettingsManager&SettingsManager::getInstance() {
    if(instance_ == nullptr) {
        instance_ = new SettingsManager();
    }
    return *instance_;
}

void SettingsManager::writeSettings(SETTING s, QString value) {
    QSettings settings(settings_file_path, QSettings::IniFormat);
    QString key = map.value(s);
    settings.setValue(key, value);
}

QString SettingsManager::getSetting(SETTING s) {
    QSettings settings(settings_file_path, QSettings::IniFormat);
    QString key = map.value(s);
    return settings.value(key).toString();
}

void SettingsManager::printAllSettings()
{
    for(auto& setting : map) {
        qDebug() << setting << ": " << getSetting(map.key(setting));
    }
}
