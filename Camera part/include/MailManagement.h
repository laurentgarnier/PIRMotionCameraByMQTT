#include "ESP_Mail_Client.h"
#ifndef CONFIGURATION_H
#include "Configuration.h"
#endif
#include "TraceDebug.h"

SMTPSession smtp;

//Callback function to get the Email sending status
void smtpCallback(SMTP_Status info);

void sendMail(String picturesDirectory, String pictureName, int nbFile)
{
  smtp.debug(1);
  smtp.callback(smtpCallback);

  ESP_Mail_Session session;
  session.server.host_name = smtpServer.c_str();
  session.server.port = smtpServerPort;
  session.login.email = emailSenderAccount.c_str();
  session.login.password = emailSenderPassword.c_str();

  SMTP_Message message;
  message.enable.chunking = true;
  message.sender.name = emailSender.c_str();
  message.sender.email = emailSenderAccount.c_str();

  message.subject = emailSubject.c_str();

  int indexVirgule = 0;
  int lastIndexVirgule = 0;
  do
  {
    indexVirgule = emailRecipient.indexOf(",", lastIndexVirgule);
    String recipient;

    if (indexVirgule > 0)
    {
      recipient = emailRecipient.substring(lastIndexVirgule, indexVirgule);
      // La recherche suivante commence après la virgule
      lastIndexVirgule = indexVirgule + 1;
    }
    else
    {
      recipient = emailRecipient.substring(lastIndexVirgule, emailRecipient.length());
    }
    recipient.trim();

    char *destinataire = (char *)malloc(recipient.length());
    strcpy(destinataire, recipient.c_str());

    message.addRecipient(destinataire, destinataire);

  } while (indexVirgule > 0);

  message.text.content = emailMessage.c_str();
  message.text.charSet = "utf-8";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_base64;
  message.priority = esp_mail_smtp_priority_normal;

  SMTP_Attachment att;

  int indexPicture = 0;
  while (indexPicture < nbFile)
  {
    String nomImage = picturesDirectory + String("/") + pictureName + String(indexPicture) + ".jpg";

    char *fileName = (char *)malloc(nomImage.length());
    strcpy(fileName, nomImage.c_str());

    att.descr.filename = fileName;
    att.descr.mime = "image/jpeg";
    att.file.path = fileName;
    att.file.storage_type = esp_mail_file_storage_type_sd;
    att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;

    message.addParallelAttachment(att);

    message.resetAttachItem(att);
    indexPicture++;
  }

  if (!smtp.connect(&session))
    return;

  if (!MailClient.sendMail(&smtp, &message, true))
    TraceDebug("Error sending Email, " + smtp.errorReason());
}

//Callback function to get the Email sending status
void smtpCallback(SMTP_Status status)
{
  /* Print the current status */
  TraceDebug(status.info());

  /* Print the sending result */
  if (status.success())
  {
    TraceDebug("----------------");
    TraceDebug("Message sent success: " + String(status.completedCount()));
    TraceDebug("Message sent failled: " + String(status.failedCount()));
    TraceDebug("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++)
    {
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      localtime_r(&result.timesstamp, &dt);

      TraceDebug("Message No: " + String(i + 1));
      TraceDebug("Status: " + result.completed ? "success" : "failed");
      TraceDebug("Date/Time: " + String(dt.tm_year + 1900) + " " + String(dt.tm_mon + 1)  + " " +  String(dt.tm_mday) + " " +  String(dt.tm_hour) + " " + String(dt.tm_min)  + " " + String(dt.tm_sec));
      TraceDebug("Recipient: " + String(result.recipients));
      TraceDebug("Subject: " + String(result.subject));
    }
    TraceDebug("----------------");
  }
}