#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>

enum class SETTING {
    DOWNLOAD_PATH,
    USER_CERTIFICATE_PATH,
    USER_KEY_PATH
};

class SettingsManager
{
public:
    static SettingsManager& getInstance();
    void writeSettings(SETTING s, QString value);
    QString getSetting(SETTING s);

private:
    SettingsManager() {}
    static SettingsManager* instance_;

    const QMap<SETTING, QString>map = { { SETTING::DOWNLOAD_PATH,         "download_path" },
                                        { SETTING::USER_CERTIFICATE_PATH, "user_certificate_path" },
                                        { SETTING::USER_KEY_PATH,         "user_key_path" } };

    const QString settings_file_path = "./settings.ini";
};

#endif // SETTINGSMANAGER_H
