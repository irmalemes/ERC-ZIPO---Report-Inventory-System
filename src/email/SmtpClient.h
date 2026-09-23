#pragma once

#include "EmailManager.h"

#include <QSslSocket>

class SmtpClient {

    public: 
        SmtpClient();

    bool sendEmail(const EmailData &email, const EmailConfig &config); 
    bool waitForResponse(QSslSocket &socket);
    bool sendCommand(QSslSocket &socket, const QByteArray &command);


};