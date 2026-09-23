#include "SmtpClient.h"
#include <QSslSocket>



SmtpClient::SmtpClient() {

}

bool SmtpClient::waitForResponse(QSslSocket &socket) {

    if(!socket.waitForReadyRead(5000)) {
        return false;
    }

    QByteArray response = socket.readAll();

    if(response.startsWith("2") || response.startsWith("3")) {
        return true;
    }

    return false;
}

bool SmtpClient::sendCommand(QSslSocket &socket, const QByteArray &command) {
    
    socket.write(command);

    if(!socket.waitForBytesWritten(5000)) {
        return false;
    }

    return waitForResponse(socket);
}


bool SmtpClient::sendEmail(const EmailData &email, const EmailConfig &config) {

    QSslSocket socket;
    socket.connectToHostEncrypted(QString::fromStdString(config.smtpHost), config.smtpPort);

    if(!socket.waitForEncrypted(10000)) {
        return false;
    }

    if(!waitForResponse(socket)) {
        return false;
    }

    if(!sendCommand(socket, "EHLO localhost\r\n")) {
        return false;
    }

    if(!sendCommand(socket, "AUTH LOGIN\r\n")) {
        return false;
    }

    QByteArray username = QByteArray::fromStdString(config.username).toBase64();
    username.append("\r\n");

    if(!sendCommand(socket, username)) {
        return false;
    }

    QByteArray password = QByteArray::fromStdString(config.password).toBase64();
    password.append("\r\n");

    if(!sendCommand(socket, password)) {
        return false;
    }

    QByteArray mailFrom = "MAIL FROM:<" + QByteArray::fromStdString(config.sender) + ">\r\n";

    if(!sendCommand(socket, mailFrom)) {
        return false;
    }

    QByteArray rcptTo = "RCPT TO:<" + QByteArray::fromStdString(email.recipient) + ">\r\n";

    if(!sendCommand(socket, rcptTo)) {
        return false;
    }

    if(!sendCommand(socket, "DATA\r\n")) {
        return false;
    }

    QByteArray message;
    message += "From: <" + QByteArray::fromStdString(config.sender) + ">\r\n";
    message += "To: <" + QByteArray::fromStdString(email.recipient) + ">\r\n";
    message += "Subject: " + QByteArray::fromStdString(email.subject) + "\r\n";
    message += "\r\n";
    message += QByteArray::fromStdString(email.body);
    message += "\r\n"; 
    message += ".\r\n";

    socket.write(message);

    if(!socket.waitForBytesWritten(5000)) {
        return false;
    }

    if(!waitForResponse(socket)) {
        return false;
    }

    socket.write("QUIT\r\n");
    socket.waitForBytesWritten(5000);

    socket.disconnectFromHost();

    return true;
}
