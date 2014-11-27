#ifndef ARCHIVEEXTRACTOR_H
#define ARCHIVEEXTRACTOR_H

/* =========================================================================
   Copyright (c) 2014-2015, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaCLBench - A Free Open-Source Benchmark
                            -----------------

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

#define LIBARCHIVE_STATIC

#include <QObject>
#include <QString>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include "external/libarchive/libarchive/archive.h"
#include "external/libarchive/libarchive/archive_entry.h"
#include <iostream>


/*! \class ArchiveExtractor
 * \brief Provides easy decompression functionality of .tar.gz files.
 * Use the extractFile functions to perform the extraction. All functions are static.
 * */

class ArchiveExtractor : public QObject
{
  Q_OBJECT
public:
  explicit ArchiveExtractor(QObject *parent = 0);

  static QString extractFileToWorkFolder(QString filePath);
  static QString extractFileToWorkFolder(const char * filePath);
  static QString extractFileToUserHomeFolder(QString filePath);
  static QString extractFileToUserHomeFolder(const char * filePath);
  static QString extractFileToTargetFolder(QString filePath, QString targetFolderPath);
  static QString extractFileToTargetFolder(const char *filePath, const char *targetFolderPath);
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
