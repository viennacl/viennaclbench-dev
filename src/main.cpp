#include "ui/mainwindow.h"
#include "ui/splashscreen.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

#include "archive.h"
#include "archive_entry.h"

static void	errmsg(const char *);
static void	extract(const char *filename, int do_extract, int flags);
static void	fail(const char *, const char *, int);
static int	copy_data(struct archive *, struct archive *);
static void	msg(const char *);
static void	usage(void);
static void	warn(const char *, const char *);
static int verbose = 0;

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);


  int verbose = 1;


  struct archive *a;
  struct archive *ext;
  struct archive_entry *entry;
  int r;
  /* The "flags" argument selects optional behavior, 'OR' the flags you want. */

  /* Default: Do not try to set owner/group. */
#define	ARCHIVE_EXTRACT_OWNER			(0x0001)
  /* Default: Do obey umask, do not restore SUID/SGID/SVTX bits. */
#define	ARCHIVE_EXTRACT_PERM			(0x0002)
  /* Default: Do not restore mtime/atime. */
#define	ARCHIVE_EXTRACT_TIME			(0x0004)
  /* Default: Replace existing files. */
#define	ARCHIVE_EXTRACT_NO_OVERWRITE 		(0x0008)
  /* Default: Try create first, unlink only if create fails with EEXIST. */
#define	ARCHIVE_EXTRACT_UNLINK			(0x0010)
  /* Default: Do not restore ACLs. */
#define	ARCHIVE_EXTRACT_ACL			(0x0020)
  /* Default: Do not restore fflags. */
#define	ARCHIVE_EXTRACT_FFLAGS			(0x0040)
  /* Default: Do not restore xattrs. */
#define	ARCHIVE_EXTRACT_XATTR 			(0x0080)
  /* Default: Do not try to guard against extracts redirected by symlinks. */
  /* Note: With ARCHIVE_EXTRACT_UNLINK, will remove any intermediate symlink. */
#define	ARCHIVE_EXTRACT_SECURE_SYMLINKS		(0x0100)
  /* Default: Do not reject entries with '..' as path elements. */
#define	ARCHIVE_EXTRACT_SECURE_NODOTDOT		(0x0200)
  /* Default: Create parent directories as needed. */
#define	ARCHIVE_EXTRACT_NO_AUTODIR		(0x0400)
  /* Default: Overwrite files, even if one on disk is newer. */
#define	ARCHIVE_EXTRACT_NO_OVERWRITE_NEWER	(0x0800)
  /* Detect blocks of 0 and write holes instead. */
#define	ARCHIVE_EXTRACT_SPARSE			(0x1000)
  /* Default: Do not restore Mac extended metadata. */
  /* This has no effect except on Mac OS. */
#define	ARCHIVE_EXTRACT_MAC_METADATA		(0x2000)
  int flags;
  //      flags |= ARCHIVE_EXTRACT_TIME;
  //      flags |= ARCHIVE_EXTRACT_NO_AUTODIR;
  //      flags |= ARCHIVE_EXTRACT_NO_OVERWRITE_NEWER;
  const char *filename = "C:/Users/Namik/Documents/GitHub/build-ViennaCL_Benchmark-Qt_4_8_5-Release/MatrixMarket/Andrews.tar.gz";
  int do_extract = 1;

  a = archive_read_new();
  ext = archive_write_disk_new();
  archive_write_disk_set_options(ext, flags);
  //      archive_read_support_compression_all(a);//same as filter_all...
  archive_read_support_filter_gzip(a);
  archive_read_support_format_gnutar(a);
  if((r = archive_read_open_filename(a, filename/*"test.tar.gz"*/, 10240)) ){
    errmsg(archive_error_string(a));
  }
  for(;;){
    r = archive_read_next_header(a, &entry);
    if(r == ARCHIVE_EOF) break;
    if(r != ARCHIVE_OK) {
      errmsg(archive_error_string(a));
    }
    if (verbose && do_extract) msg("x ");
    if (verbose || !do_extract) msg(archive_entry_pathname(entry));
    if (do_extract) {
      std::cout << "extracting" << std::endl;
      r = archive_write_header(ext, entry);
      if (r != ARCHIVE_OK) errmsg(archive_error_string(a));
      else copy_data(a, ext);
    }
    if (verbose || !do_extract) msg("\n");
  }
  archive_read_close(a);
  archive_read_free(a);
  std::cout << "Done extracting" <<std::endl;

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

static void
extract(const char *filename, int do_extract, int flags)
{
  struct archive *a;
  struct archive *ext;
  struct archive_entry *entry;
  int r;

  a = archive_read_new();
  ext = archive_write_disk_new();
  archive_write_disk_set_options(ext, flags);
  /*
   * Note: archive_write_disk_set_standard_lookup() is useful
   * here, but it requires library routines that can add 500k or
   * more to a static executable.
   */
  archive_read_support_format_tar(a);
  /*
   * On my system, enabling other archive formats adds 20k-30k
   * each.  Enabling gzip decompression adds about 20k.
   * Enabling bzip2 is more expensive because the libbz2 library
   * isn't very well factored.
   */
  if (filename != NULL && strcmp(filename, "-") == 0)
    filename = NULL;
  if ((r = archive_read_open_filename(a, filename, 10240)))
    fail("archive_read_open_filename()",
         archive_error_string(a), r);
  for (;;) {
    r = archive_read_next_header(a, &entry);
    if (r == ARCHIVE_EOF)
      break;
    if (r != ARCHIVE_OK)
      fail("archive_read_next_header()",
           archive_error_string(a), 1);
    if (verbose && do_extract)
      msg("x ");
    if (verbose || !do_extract)
      msg(archive_entry_pathname(entry));
    if (do_extract) {
      r = archive_write_header(ext, entry);
      if (r != ARCHIVE_OK)
        warn("archive_write_header()",
             archive_error_string(ext));
      else {
        copy_data(a, ext);
        r = archive_write_finish_entry(ext);
        if (r != ARCHIVE_OK)
          fail("archive_write_finish_entry()",
               archive_error_string(ext), 1);
      }

    }
    if (verbose || !do_extract)
      msg("\n");
  }
  archive_read_close(a);
  archive_read_free(a);
  //exit(0);
}

static int
copy_data(struct archive *ar, struct archive *aw)
{
  int r;
  const void *buff;
  size_t size;
#if ARCHIVE_VERSION_NUMBER >= 3000000
  int64_t offset;
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
 * These reporting functions use low-level I/O; on some systems, this
 * is a significant code reduction.  Of course, on many server and
 * desktop operating systems, malloc() and even crt rely on printf(),
 * which in turn pulls in most of the rest of stdio, so this is not an
 * optimization at all there.  (If you're going to pay 100k or more
 * for printf() anyway, you may as well use it!)
 */
static void
msg(const char *m)
{
  std::cout << m;
  //  write(1, m, strlen(m));
}

static void
errmsg(const char *m)
{
  std::cout << m;
  //  write(2, m, strlen(m));
}

static void
warn(const char *f, const char *m)
{
  errmsg(f);
  errmsg(" failed: ");
  errmsg(m);
  errmsg("\n");
}

static void
fail(const char *f, const char *m, int r)
{
  warn(f, m);
  //exit(r);
}

static void
usage(void)
{
  const char *m = "Usage: untar [-tvx] [-f file] [file]\n";
  errmsg(m);
  //exit(1);
}
