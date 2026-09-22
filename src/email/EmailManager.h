#pragma once

#include <string>

struct EmailData {

    std::string recipient;
    std::string subject;
    std::string body;
    std::string date;

};

struct EmailConfig {

    std::string smtpHost;
    int smtpPort;
    std::string username;
    std::string password;
    std::string sender;

};

class EmailManager {

    public:
        EmailManager();
        EmailData createEmailData(const std::string &recipient, const std::string &date) const;
        bool isValidEmail(const std::string &email) const;
        std::string createPreview(const EmailData &email) const;
        EmailConfig loadConfig() const;
        bool isConfigValid(const EmailConfig &config) const; 

};
