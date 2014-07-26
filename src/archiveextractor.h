#ifndef ARCHIVEEXTRACTOR_H
#define ARCHIVEEXTRACTOR_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QFile>
#include "archive.h"
#include "archive_entry.h"
#include <iostream>

class ArchiveExtractor : public QObject
{
  Q_OBJECT
public:
  explicit ArchiveExtractor(QObject *parent = 0);

  static void extractFileToWorkFolder(QString filePath);
  static void extractFileToUserHomeFolder(QString filePath);
  static bool checkUserHomeFolder();
  static void extractFileToTargetFolder(QString filePath, QString targetFolderPath);
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
