#include "EmailManager.h"
#include <cstdlib>

EmailManager::EmailManager() {

}

EmailData EmailManager::createEmailData(const std::string &recipient, const std::string &date) const {

    EmailData email;

    email.recipient = recipient;
    email.date = date;
    email.subject = "CBBH najava - " + date;

    email.body = 
        "Postovani,\n\n"
        "ovim putem dostavljamo najavu za Centralnu banku Bosne i Hercegovine.\n\n"
        "Datum: " + date + "\n\n"
        "Lijep pozdrav.";

    return email;
}

bool EmailManager::isValidEmail(const std::string &email) const {

    std::size_t atPosition = email.find('@');
    std::size_t dotPosition = email.find('.', atPosition);

    if(atPosition == std::string::npos) return false;
    if(dotPosition == std::string::npos) return false;
    if(atPosition == 0 || dotPosition <= atPosition + 1) return false;
    if(dotPosition == email.length() - 1) return false;

    return true;
}

std::string EmailManager::createPreview(const EmailData &email) const {

    std::string preview;
    preview += "Primalac: " + email.recipient + "\n";
    preview += "Nalov: " + email.subject + "\n\n";
    preview += email.body;

    return preview;
}

EmailConfig EmailManager::loadConfig() const {

    EmailConfig config;

    const char *host = std::getenv("RIS_SMTP_HOST");
    const char *port = std::getenv("RIS_SMTP_PORT");
    const char *username = std::getenv("RIS_SMTP_USERNAME");
    const char *password = std::getenv("RIS_SMTP_PASSWORD");
    const char *sender = std::getenv("RIS_SMTP_FROM");

    config.smtpHost = host ? host : "";
    config.smtpPort = 0;
    config.username = username ? username : "";
    config.password = password ? password : "";
    config.sender = sender ? sender : "";

    if(port) {
        try {
            config.smtpPort = std::stoi(port);
        } catch(...) {
            config.smtpPort = 0;
        }
    }

    return config;
}


bool EmailManager::isConfigValid(const EmailConfig &config) const {

    if(config.smtpHost.empty()) return false;
    if(config.smtpPort <= 0) return false;
    if(config.username.empty()) return false;
    if(config.password.empty()) return false;
    if(config.sender.empty()) return false;

    return true;
}



