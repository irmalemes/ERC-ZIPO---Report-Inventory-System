#include <cassert>
#include <iostream>

#include "../src/email/EmailManager.h"

int main() {

    EmailManager emailManager;

    std::cout<<"Pokretanje EmailManager testova...\n";

    EmailData email = emailManager.createEmailData("test@example.com", "23.09.2026");

    assert(email.recipient == "test@example.com");
    assert(email.date == "23.09.2026");
    assert(email.subject == "CBBH najava - 23.09.2026");
    assert(email.body.find("23.09.2026") != std::string::npos);
    std::cout<<"Kreiranje email podataka: OK\n";

    assert(emailManager.isValidEmail("test@example.com"));
    assert(emailManager.isValidEmail("korisnik@gmail.com"));
    assert(!emailManager.isValidEmail("pogresanemail"));
    assert(!emailManager.isValidEmail("@gmail.com"));
    assert(!emailManager.isValidEmail("test@gmail"));
    assert(!emailManager.isValidEmail("test@.com"));
    std::cout<<"Validacija email adresa: OK\n";

    std::string preview = emailManager.createPreview(email);
    assert(preview.find("test@example.com") != std::string::npos);
    assert(preview.find("CBBH najava - 23.09.2026") != std::string::npos);
    assert(preview.find("23.09.2026") != std::string::npos);
    std::cout<<"Email preview: OK\n";

    EmailConfig validConfig;
    validConfig.smtpHost = "smtp.gmail.com";
    validConfig.smtpPort = 465;
    validConfig.username = "test@example.com";
    validConfig.password = "test-password";
    validConfig.sender = "test@example.com";

    assert(emailManager.isConfigValid(validConfig));
    EmailConfig invalidConfig;

    invalidConfig.smtpHost = "";
    invalidConfig.smtpPort = 0;
    invalidConfig.username = "";
    invalidConfig.password = "";
    invalidConfig.sender = "";

    assert(!emailManager.isConfigValid(invalidConfig));
    std::cout<<"Validacija SMTP konfiguracije: OK\n";

    return 0;
}