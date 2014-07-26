#ifndef ARCHIVEEXTRACTOR_H
#define ARCHIVEEXTRACTOR_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include "archive.h"
#include "archive_entry.h"
#include <iostream>

class ArchiveExtractor : public QObject
{
  Q_OBJECT
public:
  explicit ArchiveExtractor(QObject *parent = 0);

  static void extractFileToWorkFolder(QString filePath);
  static void extractFileToWorkFolder(const char * filePath);
  static void extractFileToUserHomeFolder(QString filePath);
  static void extractFileToUserHomeFolder(const char * filePath);
  static void extractFileToTargetFolder(QString filePath, QString targetFolderPath);
  static void extractFileToTargetFolder(const char *filePath, const char *targetFolderPath);
  static bool checkUserHomeFolder();
  static QString getMatrixMarketUserFolder();
private:
  static const int verbose = 1;

  static void	errmsg(const char *);
  static int	copy_data(struct archive *, struct archive *);
  static void	msg(const char *);
  static void	warn(const char *, const char *);
signals:

public slots:

};

#endif // ARCHIVEEXTRACTOR_H
