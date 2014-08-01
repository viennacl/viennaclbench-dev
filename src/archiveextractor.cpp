#include "archiveextractor.h"
#include <QDebug>

/*
 * ArchiveExtractor class provides easy decompression of .tar.gz files
 * use the function extractFile(QString filePath) to perform the extraction
 * can be easily modified to provide support for additional formats
 * */

ArchiveExtractor::ArchiveExtractor(QObject *parent) :
  QObject(parent)
{
}

/*
 * Returns the user home folder in which matrix market files are to be stored
 * If the folder does not exist or cannot be created, returns a null string
 * */
QString ArchiveExtractor::getMatrixMarketUserFolder(){
  if(checkUserHomeFolder()){
    return QDir::home().absolutePath() + "/ViennaCL Benchmark/MatrixMarket/";
  }
  else{
    return "";
  }
}

/*
 * Checks if user's home folder contains the appropriate folder for matrix file storage
 * Attempts to create a matrix market folder in case one does not already exist
 * Returns true if a /ViennaCL Benchmark/MatrixMarket/ exists in the user home folder
 * Returns false if the folder does not exist or cannot be created
 * */
bool ArchiveExtractor::checkUserHomeFolder(){
  QDir userHomeFolder = QDir::home();
//  qDebug()<<"User's home folder: " << userHomeFolder.absolutePath();
  QDir benchmarkFolder(userHomeFolder.absolutePath() + "/ViennaCL Benchmark");
//  qDebug()<<"User's benchmarkFolder folder: " << benchmarkFolder.absolutePath();
  QDir matrixMarketFolder(benchmarkFolder.absolutePath() + "/MatrixMarket");
//  qDebug()<<"User's matrixMarketFolder folder: " << matrixMarketFolder.absolutePath();
  //check for ViennaCL Benchmark folder
  if(!benchmarkFolder.exists()){//does not exist
    //create it
    if(userHomeFolder.mkdir("ViennaCL Benchmark")){
      qDebug()<<"ViennaCL Benchmark folder created";
    }
    else{
      qDebug()<<"Error creating ViennaCL Benchmark folder";
      return false;
    }
  }
  //check for MatrixMarket folder
  if(!matrixMarketFolder.exists()){//does not exists
    //create it
    if(benchmarkFolder.mkdir("MatrixMarket")){
      qDebug()<<"MatrixMarket folder created";
    }
    else{
      qDebug()<<"Error creating MatrixMarket folder";
      return false;
    }
  }
  return true;
}


/*
 * Extracts the selected .tar.gz archive to the selected folder
 * will try to create the target folder if it does not exist
 * */
void ArchiveExtractor::extractFileToTargetFolder(const char *filePath, const char * targetFolderPath){
  ArchiveExtractor::extractFileToTargetFolder(QString(filePath), QString(targetFolderPath));
}

/*
 * Extracts the selected .tar.gz archive to the selected folder
 * will try to create the target folder if it does not exist
 * */
void ArchiveExtractor::extractFileToTargetFolder(QString filePath, QString targetFolderPath){
  //check if the selected archive file exist
  QFile *selectedFile = new QFile(filePath);
  if(!selectedFile->exists()){
    qDebug()<<"ERROR: File marked for decompression does not exist!";
    delete selectedFile;
    return;
  }
  delete selectedFile;
  struct archive *a;
  struct archive *ext;
  struct archive_entry *entry;
  int r;
  //  /* The "flags" argument selects optional behavior, 'OR' the flags you want. */
  //  /* Default: Do not try to set owner/group. */
  //#define	ARCHIVE_EXTRACT_OWNER			(0x0001)
  //  /* Default: Do obey umask, do not restore SUID/SGID/SVTX bits. */
  //#define	ARCHIVE_EXTRACT_PERM			(0x0002)
  //  /* Default: Do not restore mtime/atime. */
  //#define	ARCHIVE_EXTRACT_TIME			(0x0004)
  //  /* Default: Replace existing files. */
  //#define	ARCHIVE_EXTRACT_NO_OVERWRITE 		(0x0008)
  //  /* Default: Try create first, unlink only if create fails with EEXIST. */
  //#define	ARCHIVE_EXTRACT_UNLINK			(0x0010)
  //  /* Default: Do not restore ACLs. */
  //#define	ARCHIVE_EXTRACT_ACL			(0x0020)
  //  /* Default: Do not restore fflags. */
  //#define	ARCHIVE_EXTRACT_FFLAGS			(0x0040)
  //  /* Default: Do not restore xattrs. */
  //#define	ARCHIVE_EXTRACT_XATTR 			(0x0080)
  //  /* Default: Do not try to guard against extracts redirected by symlinks. */
  //  /* Note: With ARCHIVE_EXTRACT_UNLINK, will remove any intermediate symlink. */
  //#define	ARCHIVE_EXTRACT_SECURE_SYMLINKS		(0x0100)
  //  /* Default: Do not reject entries with '..' as path elements. */
  //#define	ARCHIVE_EXTRACT_SECURE_NODOTDOT		(0x0200)
  //  /* Default: Create parent directories as needed. */
  //#define	ARCHIVE_EXTRACT_NO_AUTODIR		(0x0400)
  //  /* Default: Overwrite files, even if one on disk is newer. */
  //#define	ARCHIVE_EXTRACT_NO_OVERWRITE_NEWER	(0x0800)
  //  /* Detect blocks of 0 and write holes instead. */
  //#define	ARCHIVE_EXTRACT_SPARSE			(0x1000)
  //  /* Default: Do not restore Mac extended metadata. */
  //  /* This has no effect except on Mac OS. */
  //#define	ARCHIVE_EXTRACT_MAC_METADATA		(0x2000)
  int flags = 0;
  //      flags |= ARCHIVE_EXTRACT_TIME;
  //      flags |= ARCHIVE_EXTRACT_NO_AUTODIR;
  //      flags |= ARCHIVE_EXTRACT_NO_OVERWRITE_NEWER;

  QFileInfo *fileInfo = new QFileInfo(filePath);
//  qDebug()<<"resolved filename: "<<fileInfo->fileName();
  delete fileInfo;

  //MEMORY LEAK!!! (Pointers be dangerous, man. :)
  //  const char *filename = fileInfo->fileName().toUtf8().constData();
  //AVOID IT BY CONVERTING TO A QBYTEARRAY FIRST!
  QByteArray byteArray = filePath.toUtf8();
  const char *filename = byteArray.constData();
  //That's better :D

  //toggle extraction
  bool do_extract = true;

  a = archive_read_new();
  ext = archive_write_disk_new();
  archive_write_disk_set_options(ext, flags);

  //tuned for .tar.gz
  archive_read_support_filter_gzip(a);
  archive_read_support_format_gnutar(a);

  if((r = archive_read_open_filename(a, filename, 10240)) ){
    errmsg(archive_error_string(a));
  }
  for(;;){
    r = archive_read_next_header(a, &entry);
    if(r == ARCHIVE_EOF) break;
    if(r != ARCHIVE_OK){
      errmsg(archive_error_string(a));
    }
    if (verbose || !do_extract){
      std::cout << "Detected files in archive: ";
      msg(archive_entry_pathname(entry));
      std::cout << std::endl;
    }
    QString currentPath(archive_entry_pathname( entry ));
    std::cout << currentPath.toStdString() << std::endl;

    QDir targetFolder(targetFolderPath);
    if(!targetFolder.exists()){//target folder does not exist
      //attempt to create it
      if(!targetFolder.mkpath(targetFolderPath)){//failed to create target folder
        //break procedure
        std::cout << "ERROR: Target folder does not exist and cannot be created" << std::endl;
        return;
      }
    }

    QString newPath = targetFolderPath + currentPath;
    std::cout << "newPath: " << newPath.toStdString() << std::endl;

    archive_entry_set_pathname( entry, newPath.toUtf8().constData() );
    if (verbose && do_extract){
//      msg("About to start extracting\n");
    }
    if (do_extract){
      std::cout << "Extracting..." << std::endl;
      r = archive_write_header(ext, entry);
      if (r != ARCHIVE_OK) errmsg(archive_error_string(a));
      else copy_data(a, ext);
      std::cout << "File extracted: ";
      msg(archive_entry_pathname(entry));
      std::cout << std::endl;
    }
  }
  archive_read_close(a);
  archive_read_free(a);
  archive_write_close(ext);
  archive_write_free(ext);
}

/*
 * Extract the selected .tar.gz archive to the current user's home folder
 * */
void ArchiveExtractor::extractFileToUserHomeFolder(const char *filePath)
{
  ArchiveExtractor::extractFileToUserHomeFolder(QString(filePath));
}

/*
 * Extract the selected .tar.gz archive to the current user's home folder
 * */
void ArchiveExtractor::extractFileToUserHomeFolder(QString filePath){
  QString userHomeFolder = ArchiveExtractor::getMatrixMarketUserFolder();/* QDir::home().absolutePath() + "/ViennaCL Benchmark/MatrixMarket/";*/
  if(!userHomeFolder.isNull()){
    extractFileToTargetFolder(filePath, userHomeFolder);
  }
}

/*
 * Extracts the selected .tar.gz archive to the program's current working directory
 * */
void ArchiveExtractor::extractFileToWorkFolder(const char *filePath)
{
  ArchiveExtractor::extractFileToWorkFolder(QString(filePath));
}

/*
 * Extracts the selected .tar.gz archive to the program's current working directory
 * */
void ArchiveExtractor::extractFileToWorkFolder(QString filePath){
  extractFileToTargetFolder(filePath, QDir::currentPath()+"/");
}


int ArchiveExtractor::copy_data(struct archive *ar, struct archive *aw){
  int r;
  const void *buff;
  size_t size;
#if ARCHIVE_VERSION_NUMBER >= 3000000
  __LA_INT64_T offset;
#else
  off_t offset;
#endif

  for (;;) {
    r = archive_read_data_block(ar, &buff, &size, &offset);
    if (r == ARCHIVE_EOF)
      return (ARCHIVE_OK);
    if (r != ARCHIVE_OK)
      return (r);
    r = archive_write_data_block(aw, buff, size, offset);
    if (r != ARCHIVE_OK) {
      warn("archive_write_data_block()",
           archive_error_string(aw));
      return (r);
    }
  }
}

/*
 * Reporting functions
 * */

void ArchiveExtractor::msg(const char *m){
  std::cout << m;
}

void ArchiveExtractor::errmsg(const char *m){
  std::cout << "Error: " << m;
}

void ArchiveExtractor::warn(const char *f, const char *m){
  errmsg(f);
  errmsg(" failed: ");
  errmsg(m);
  errmsg("\n");
}
