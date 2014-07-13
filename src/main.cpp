#include "ui/mainwindow.h"
#include "ui/splashscreen.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

#include "archive.h"
#include "archive_entry.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//static void	errmsg(const char *);
//static void	extract(const char *filename, int do_extract, int flags);
//static void	fail(const char *, const char *, int);
//static int	copy_data(struct archive *, struct archive *);
//static void	msg(const char *);
//static void	usage(void);
//static void	warn(const char *, const char *);
//static int verbose = 0;

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

    struct archive *a;
    struct archive_entry *entry;
    int r;

    a = archive_read_new();
//    archive_read_support_compression_all(a);//same as filter_all...
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    r = archive_read_open_filename(a, "test.tar", 10240);
    if (r != ARCHIVE_OK){
      std::cout << "archive is not ok" << std::endl;
    }
    else{
      while(archive_read_next_header(a, &entry) == ARCHIVE_OK){
        printf( "%s\\n", archive_entry_pathname(entry) );
        archive_read_data_skip(a);
      }
      r = archive_read_free(a);
      if(r != ARCHIVE_OK){
        std::cout << "archive is not ok 2" << std::endl;
      }
    }
    exit(1);

//  const char *filename = "mplate.tar.gz";
//  const char *filename2 = "C:/Users/Namik/Documents/GitHub/build-ViennaCL_Benchmark-Qt_4_8_5-Release/MatrixMarket/mplate.tar.gz";
//  int compress, flags, mode, opt;
//  flags = ARCHIVE_EXTRACT_TIME;
//  extract(filename, 1, flags);
//  extract(filename2, 1, flags);

//  struct archive *a;
//     struct archive_entry *entry;
//     int r;
//     int64_t entry_size;
//     a = archive_read_new();
//     archive_read_support_compression_none(a);
//     archive_read_support_format_tar(a);
//     r = archive_read_open_filename(a, "test.zip", 1024);
//     if (r != ARCHIVE_OK)
//     {
//         printf("archive not found");
//     }
//     else
//     {
//         while (archive_read_next_header(a, &entry) == ARCHIVE_OK)
//         {
//             const char *currentFile = archive_entry_pathname(entry);
//             char *fileContents;
//             entry_size = archive_entry_size(entry); //get the size of the file
//             fileContents = malloc(entry_size); //alloc enough for string - from my testing I see that this is how many bytes tar and ls report from command line
//             archive_read_data(a, fileContents, entry_size); //read data into fileContents string for the HTML file size
//             if(strcmp(currentFile, "vendar-definition.html") == 0)
//             {
//                 printf("file name = %s, size = %lld\n", currentFile, entry_size);
//                 printf("%s\n\n", fileContents); //this output over-reads chars from another file in this tar file
//             }
//             free(fileContents); //free the C string because I malloc'd
//         }
//     }
//     printf("exit");






  SplashScreen *splashScreen = new SplashScreen();
  splashScreen->setPixmap(QPixmap(":/resources/images/splash-screen.jpg"));
  splashScreen->show();
  splashScreen->showMessage("Loading GUI");
  qApp->processEvents();
  MainWindow w;
  QTimer::singleShot(2000, &w, SLOT(show()) );
  QTimer::singleShot(2000, splashScreen, SLOT(deleteLater()) );
  return app.exec();
}

//static void
//extract(const char *filename, int do_extract, int flags)
//{
//  struct archive *a;
//  struct archive *ext;
//  struct archive_entry *entry;
//  int r;

//  a = archive_read_new();
//  ext = archive_write_disk_new();
//  archive_write_disk_set_options(ext, flags);
//  /*
//   * Note: archive_write_disk_set_standard_lookup() is useful
//   * here, but it requires library routines that can add 500k or
//   * more to a static executable.
//   */
//  archive_read_support_format_tar(a);
//  /*
//   * On my system, enabling other archive formats adds 20k-30k
//   * each.  Enabling gzip decompression adds about 20k.
//   * Enabling bzip2 is more expensive because the libbz2 library
//   * isn't very well factored.
//   */
//  if (filename != NULL && strcmp(filename, "-") == 0)
//    filename = NULL;
//  if ((r = archive_read_open_filename(a, filename, 10240)))
//    fail("archive_read_open_filename()",
//        archive_error_string(a), r);
//  for (;;) {
//    r = archive_read_next_header(a, &entry);
//    if (r == ARCHIVE_EOF)
//      break;
//    if (r != ARCHIVE_OK)
//      fail("archive_read_next_header()",
//          archive_error_string(a), 1);
//    if (verbose && do_extract)
//      msg("x ");
//    if (verbose || !do_extract)
//      msg(archive_entry_pathname(entry));
//    if (do_extract) {
//      r = archive_write_header(ext, entry);
//      if (r != ARCHIVE_OK)
//        warn("archive_write_header()",
//            archive_error_string(ext));
//      else {
//        copy_data(a, ext);
//        r = archive_write_finish_entry(ext);
//        if (r != ARCHIVE_OK)
//          fail("archive_write_finish_entry()",
//              archive_error_string(ext), 1);
//      }

//    }
//    if (verbose || !do_extract)
//      msg("\n");
//  }
//  archive_read_close(a);
//  archive_read_free(a);
//  //exit(0);
//}

//static int
//copy_data(struct archive *ar, struct archive *aw)
//{
//  int r;
//  const void *buff;
//  size_t size;
//#if ARCHIVE_VERSION_NUMBER >= 3000000
//  int64_t offset;
//#else
//  off_t offset;
//#endif

//  for (;;) {
//    r = archive_read_data_block(ar, &buff, &size, &offset);
//    if (r == ARCHIVE_EOF)
//      return (ARCHIVE_OK);
//    if (r != ARCHIVE_OK)
//      return (r);
//    r = archive_write_data_block(aw, buff, size, offset);
//    if (r != ARCHIVE_OK) {
//      warn("archive_write_data_block()",
//          archive_error_string(aw));
//      return (r);
//    }
//  }
//}

///*
// * These reporting functions use low-level I/O; on some systems, this
// * is a significant code reduction.  Of course, on many server and
// * desktop operating systems, malloc() and even crt rely on printf(),
// * which in turn pulls in most of the rest of stdio, so this is not an
// * optimization at all there.  (If you're going to pay 100k or more
// * for printf() anyway, you may as well use it!)
// */
//static void
//msg(const char *m)
//{
//  write(1, m, strlen(m));
//}

//static void
//errmsg(const char *m)
//{
//  write(2, m, strlen(m));
//}

//static void
//warn(const char *f, const char *m)
//{
//  errmsg(f);
//  errmsg(" failed: ");
//  errmsg(m);
//  errmsg("\n");
//}

//static void
//fail(const char *f, const char *m, int r)
//{
//  warn(f, m);
//  //exit(r);
//}

//static void
//usage(void)
//{
//  const char *m = "Usage: untar [-tvx] [-f file] [file]\n";
//  errmsg(m);
//  //exit(1);
//}
