#include "ui/mainwindow.h"
#include "ui/splashscreen.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

//#include "archive.h"
//#include "archive_entry.h"

//#include <fcntl.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>

//static void	errmsg(const char *);
//static void	extract(const char *filename, int do_extract, int flags);
//static void	fail(const char *, const char *, int);
//static int	copy_data(struct archive *, struct archive *);
//static void	msg(const char *);
//static void	usage(void);
//static void	warn(const char *, const char *);
//static int verbose = 0;

//#include <stdio.h>
//#include <string.h>
//#include <assert.h>
//#include "zlib.h"

//#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
//#  include <fcntl.h>
//#  include <io.h>
//#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
//#else
//#  define SET_BINARY_MODE(file)
//#endif

//#define CHUNK 16384

///* Decompress from file source to file dest until stream ends or EOF.
//   inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
//   allocated for processing, Z_DATA_ERROR if the deflate data is
//   invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
//   the version of the library linked do not match, or Z_ERRNO if there
//   is an error reading or writing the files. */
//int inf(FILE *source, FILE *dest)
//{
//    int ret;
//    unsigned have;
//    z_stream strm;
//    unsigned char in[CHUNK];
//    unsigned char out[CHUNK];

//    /* allocate inflate state */
//    strm.zalloc = Z_NULL;
//    strm.zfree = Z_NULL;
//    strm.opaque = Z_NULL;
//    strm.avail_in = 0;
//    strm.next_in = Z_NULL;
//    ret = inflateInit(&strm);
//    if (ret != Z_OK)
//        return ret;

//    /* decompress until deflate stream ends or end of file */
//    do {
//        strm.avail_in = fread(in, 1, CHUNK, source);
//        if (ferror(source)) {
//            (void)inflateEnd(&strm);
//            return Z_ERRNO;
//        }
//        if (strm.avail_in == 0)
//            break;
//        strm.next_in = in;

//        /* run inflate() on input until output buffer not full */
//        do {
//            strm.avail_out = CHUNK;
//            strm.next_out = out;
//            ret = inflate(&strm, Z_NO_FLUSH);
//            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
//            switch (ret) {
//            case Z_NEED_DICT:
//                ret = Z_DATA_ERROR;     /* and fall through */
//            case Z_DATA_ERROR:
//            case Z_MEM_ERROR:
//                (void)inflateEnd(&strm);
//                return ret;
//            }
//            have = CHUNK - strm.avail_out;
//            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
//                (void)inflateEnd(&strm);
//                return Z_ERRNO;
//            }
//        } while (strm.avail_out == 0);

//        /* done when inflate() says it's done */
//    } while (ret != Z_STREAM_END);

//    /* clean up and return */
//    (void)inflateEnd(&strm);
//    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
//}

///* report a zlib or i/o error */
//void zerr(int ret)
//{
//  fputs("zpipe: ", stderr);
//  switch (ret) {
//  case Z_ERRNO:
//    if (ferror(stdin))
//      fputs("error reading stdin\n", stderr);
//    if (ferror(stdout))
//      fputs("error writing stdout\n", stderr);
//    break;
//  case Z_STREAM_ERROR:
//    fputs("invalid compression level\n", stderr);
//    break;
//  case Z_DATA_ERROR:
//    fputs("invalid or incomplete deflate data\n", stderr);
//    break;
//  case Z_MEM_ERROR:
//    fputs("out of memory\n", stderr);
//    break;
//  case Z_VERSION_ERROR:
//    fputs("zlib version mismatch!\n", stderr);
//  }
//}
/* ===========================================================================
 * Test read/write of .gz files
 */
//void test_gzio(const char *fname, Byte *uncompr, uLong uncomprLen)
////    const char *fname; /* compressed file name */
////    Byte *uncompr;
////    uLong uncomprLen;
//{
//#ifdef NO_GZCOMPRESS
//  fprintf(stderr, "NO_GZCOMPRESS -- gz* functions cannot compress\n");
//#else
//  int err;
//  int len = (int)strlen(hello)+1;
//  gzFile file;
//  z_off_t pos;

//  file = gzopen(fname, "wb");
//  if (file == NULL) {
//    fprintf(stderr, "gzopen error\n");
//    exit(1);
//  }
//  gzputc(file, 'h');
//  if (gzputs(file, "ello") != 4) {
//    fprintf(stderr, "gzputs err: %s\n", gzerror(file, &err));
//    exit(1);
//  }
//  if (gzprintf(file, ", %s!", "hello") != 8) {
//    fprintf(stderr, "gzprintf err: %s\n", gzerror(file, &err));
//    exit(1);
//  }
//  gzseek(file, 1L, SEEK_CUR); /* add one zero byte */
//  gzclose(file);

//  file = gzopen(fname, "rb");
//  if (file == NULL) {
//    fprintf(stderr, "gzopen error\n");
//    exit(1);
//  }
//  strcpy((char*)uncompr, "garbage");

//  if (gzread(file, uncompr, (unsigned)uncomprLen) != len) {
//    fprintf(stderr, "gzread err: %s\n", gzerror(file, &err));
//    exit(1);
//  }
//  if (strcmp((char*)uncompr, hello)) {
//    fprintf(stderr, "bad gzread: %s\n", (char*)uncompr);
//    exit(1);
//  } else {
//    printf("gzread(): %s\n", (char*)uncompr);
//  }

//  pos = gzseek(file, -8L, SEEK_CUR);
//  if (pos != 6 || gztell(file) != pos) {
//    fprintf(stderr, "gzseek error, pos=%ld, gztell=%ld\n",
//            (long)pos, (long)gztell(file));
//    exit(1);
//  }

//  if (gzgetc(file) != ' ') {
//    fprintf(stderr, "gzgetc error\n");
//    exit(1);
//  }

//  if (gzungetc(' ', file) != ' ') {
//    fprintf(stderr, "gzungetc error\n");
//    exit(1);
//  }

//  gzgets(file, (char*)uncompr, (int)uncomprLen);
//  if (strlen((char*)uncompr) != 7) { /* " hello!" */
//    fprintf(stderr, "gzgets err after gzseek: %s\n", gzerror(file, &err));
//    exit(1);
//  }
//  if (strcmp((char*)uncompr, hello + 6)) {
//    fprintf(stderr, "bad gzgets after gzseek\n");
//    exit(1);
//  } else {
//    printf("gzgets() after gzseek: %s\n", (char*)uncompr);
//  }

//  gzclose(file);
//#endif
//}

/* Compress from file source to file dest until EOF on source.
   def() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_STREAM_ERROR if an invalid compression
   level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
   version of the library linked do not match, or Z_ERRNO if there is
   an error reading or writing the files. */
//int def(FILE *source, FILE *dest, int level)
//{
//    int ret, flush;
//    unsigned have;
//    z_stream strm;
//    unsigned char in[CHUNK];
//    unsigned char out[CHUNK];

//    /* allocate deflate state */
//    strm.zalloc = Z_NULL;
//    strm.zfree = Z_NULL;
//    strm.opaque = Z_NULL;
//    ret = deflateInit(&strm, level);
//    if (ret != Z_OK)
//        return ret;

//    /* compress until end of file */
//    do {
//        strm.avail_in = fread(in, 1, CHUNK, source);
//        if (ferror(source)) {
//            (void)deflateEnd(&strm);
//            return Z_ERRNO;
//        }
//        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
//        strm.next_in = in;

//        /* run deflate() on input until output buffer not full, finish
//           compression if all of source has been read in */
//        do {
//            strm.avail_out = CHUNK;
//            strm.next_out = out;
//            ret = deflate(&strm, flush);    /* no bad return value */
//            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
//            have = CHUNK - strm.avail_out;
//            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
//                (void)deflateEnd(&strm);
//                return Z_ERRNO;
//            }
//        } while (strm.avail_out == 0);
//        assert(strm.avail_in == 0);     /* all input will be used */

//        /* done when last data in file processed */
//    } while (flush != Z_FINISH);
//    assert(ret == Z_STREAM_END);        /* stream will be complete */

//    /* clean up and return */
//    (void)deflateEnd(&strm);
//    return Z_OK;
//}
//#include <QFile>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

//  FILE *a, *b, *c;
//   int ret;

//   //ZIP
//   a = fopen("test.txt", "rb");
//   b = fopen("b_compressedData.zip", "wb");
//   if(a != NULL && b != NULL){
//       ret = def(a, b, Z_DEFAULT_COMPRESSION);
//       printf("%d\n", ret);

//       if (ret != Z_OK) zerr(ret);

//       fclose(a);
//       fclose(b);
//   }

//   switch(ret){
//   case Z_OK:
//     std::cout << "Return code:" <<ret<< " Z_OK" <<std::endl;
//     break;
//   case Z_MEM_ERROR:
//     std::cout << "Return code:" <<ret<< " Z_MEM_ERROR" <<std::endl;
//     break;
//   case Z_DATA_ERROR:
//     std::cout << "Return code:" <<ret<< " Z_DATA_ERROR" <<std::endl;
//     break;
//   case Z_ERRNO:
//     std::cout << "Return code:" <<ret<< " Z_ERRNO" <<std::endl;
//     break;
//   case Z_VERSION_ERROR:
//     std::cout << "Return code:" <<ret<< " Z_VERSION_ERROR" <<std::endl;
//     break;
//   case  Z_BUF_ERROR:
//     std::cout << "Return code:" <<ret<< " Z_BUF_ERROR" <<std::endl;
//     break;
//   default:
//     std::cout << "Return code:" <<ret<<std::endl;
//   }


//   //UNZIP
//   b = fopen("test.tar", "r");
//   c = fopen("c_uncompressedData.txt", "w");
//   if(c != NULL && b != NULL){
//       ret = inf(b, c);
//       printf("%d\n", ret);
//       if (ret != Z_OK) zerr(ret);

//       fclose(b);
//       fclose(c);
//   }

//       switch(ret){
//       case Z_OK:
//         std::cout << "Return code:" <<ret<< " Z_OK" <<std::endl;
//         break;
//       case Z_MEM_ERROR:
//         std::cout << "Return code:" <<ret<< " Z_MEM_ERROR" <<std::endl;
//         break;
//       case Z_DATA_ERROR:
//         std::cout << "Return code:" <<ret<< " Z_DATA_ERROR" <<std::endl;
//         break;
//       case Z_ERRNO:
//         std::cout << "Return code:" <<ret<< " Z_ERRNO" <<std::endl;
//         break;
//       case Z_VERSION_ERROR:
//         std::cout << "Return code:" <<ret<< " Z_VERSION_ERROR" <<std::endl;
//         break;
//       case  Z_BUF_ERROR:
//         std::cout << "Return code:" <<ret<< " Z_BUF_ERROR" <<std::endl;
//         break;
//       default:
//         std::cout << "Return code:" <<ret<<std::endl;
//       }



//  gzFile file;
//  const char *fname = "testfsdf.zip";
//  file = gzopen(fname, "wb");
//  if (file == NULL) {
//    fprintf(stderr, "gzopen error\n");
//    std::cout << "failed to open gz file";
//    exit(1);
//  }
//  else{
//    std::cout << "gz file open";
//  }

//#if !defined(__MACTYPES__)
//typedef unsigned char  Byte;  /* 8 bits */
//#endif
//typedef unsigned int   uInt;  /* 16 bits or more */
//typedef unsigned long  uLong; /* 32 bits or more */

//  typedef Byte  FAR Bytef;
//  typedef uLong FAR uLongf;
//  typedef unsigned long  uLong; /* 32 bits or more */

//  Bytef dest;
//  uLongf destLen;
//  const Bytef *source;
//  uLong sourceLen;

//  ZEXTERN int ZEXPORT uncompress OF((Bytef *dest,   uLongf *destLen,
//                                     const Bytef *source, uLong sourceLen));
  /*
       Decompresses the source buffer into the destination buffer.  sourceLen is
     the byte length of the source buffer. Upon entry, destLen is the total
     size of the destination buffer, which must be large enough to hold the
     entire uncompressed data. (The size of the uncompressed data must have
     been saved previously by the compressor and transmitted to the decompressor
     by some mechanism outside the scope of this compression library.)
     Upon exit, destLen is the actual size of the compressed buffer.
       This function can be used to decompress a whole file at once if the
     input file is mmap'ed.

       uncompress returns Z_OK if success, Z_MEM_ERROR if there was not
     enough memory, Z_BUF_ERROR if there was not enough room in the output
     buffer, or Z_DATA_ERROR if the input data was corrupted or incomplete.
  */
//  QFile file("test.zip");
//  QByteArray fileDataArray;
//  if(file.open(QIODevice::ReadOnly)){
//    std::cout << "reading file" <<std::endl;
//    fileDataArray = file.readAll();
//  }
//  else{
//    std::cout << "failed to read file";
//  }
//  unsigned char *kurac = (unsigned char *)fileDataArray.data();
//source = kurac;
//sourceLen = 2*fileDataArray.size();

//  destLen = sourceLen;
//  int status;
//  status = uncompress(&dest, &destLen, source, sourceLen );
//    switch(status){
//    case Z_OK:
//      std::cout << "Return code:" <<status<< " Z_OK" <<std::endl;
//      break;
//    case Z_MEM_ERROR:
//      std::cout << "Return code:" <<status<< " Z_MEM_ERROR" <<std::endl;
//      break;
//    case Z_DATA_ERROR:
//      std::cout << "Return code:" <<status<< " Z_DATA_ERROR" <<std::endl;
//      break;
//    case Z_ERRNO:
//      std::cout << "Return code:" <<status<< " Z_ERRNO" <<std::endl;
//      break;
//    case Z_VERSION_ERROR:
//      std::cout << "Return code:" <<status<< " Z_VERSION_ERROR" <<std::endl;
//      break;
//    case  Z_BUF_ERROR:
//      std::cout << "Return code:" <<status<< " Z_BUF_ERROR" <<std::endl;
//      break;
//    default:
//      std::cout << "Return code:" <<status<<std::endl;
//    }



  //  int ret;

  //  FILE *compressedFile;
  //  FILE *decompressionTargetFile;
  //  char inputFilename[] = "test.zip";
  //  char outputFilename[] = "untest.zip";
  //  char *writeMode = "w";
  //  char *readMode = "r";

  //  QFile inputFile(inputFilename);
  //  QByteArray inputFileData;
  //  if(inputFile.open(QIODevice::ReadOnly )){
  //    QByteArray inputFileData = inputFile.readAll();
  //    std::cout << "file open" <<std::endl;
  //  }
  //  else{
  //    std::cout << "failed to op[en file"<< std::endl;
  //  }
  ////uncompress(inputFileData.constData() )


  //  /* compress or decompress from stdin to stdout */

  //  /* avoid end-of-line conversions */

  //  compressedFile = fopen(inputFilename, readMode);
  //  decompressionTargetFile = fopen(outputFilename, writeMode);

  //  if (compressedFile == NULL) {
  //    fprintf(stderr, "Can't open input file %s!\n", inputFilename);
  //    //    exit(1);
  //  }

  //  if (decompressionTargetFile == NULL) {
  //    fprintf(stderr, "Can't open output file %s!\n", outputFilename);
  //    //    exit(1);
  //  }
  //  /* do decompression if -d specified */
  //  ret = inf(compressedFile , decompressionTargetFile );
  //  if (ret != Z_OK)
  //    zerr(ret);
  //  /* Decompress from file source to file dest until stream ends or EOF.
  //   inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
  //   allocated for processing, Z_DATA_ERROR if the deflate data is
  //   invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
  //   the version of the library linked do not match, or Z_ERRNO if there
  //   is an error reading or writing the files. */
  //  switch(ret){
  //  case Z_OK:
  //    std::cout << "Return code:" <<ret<< " Z_OK" <<std::endl;
  //    break;
  //  case Z_MEM_ERROR:
  //    std::cout << "Return code:" <<ret<< " Z_MEM_ERROR" <<std::endl;
  //    break;
  //  case Z_DATA_ERROR:
  //    std::cout << "Return code:" <<ret<< " Z_DATA_ERROR" <<std::endl;
  //    break;
  //  case Z_ERRNO:
  //    std::cout << "Return code:" <<ret<< " Z_ERRNO" <<std::endl;
  //    break;
  //  case Z_VERSION_ERROR:
  //    std::cout << "Return code:" <<ret<< " Z_VERSION_ERROR" <<std::endl;
  //    break;
  //  default:
  //    std::cout << "Return code:" <<ret<<std::endl;
  //  }







//      struct archive *a;
//      struct archive_entry *entry;
//      int r;

//      a = archive_read_new();
////      archive_read_support_compression_all(a);//same as filter_all...
//      archive_read_support_filter_all(a);
//      archive_read_support_format_all(a);
//      r = archive_read_open_filename(a, "test.gz", 10240);
//      if (r != ARCHIVE_OK){
//        std::cout << "archive is not ok" << std::endl;
//      }
//      else{
//        while(archive_read_next_header(a, &entry) == ARCHIVE_OK){
//          printf( "%s\\n", archive_entry_pathname(entry) );
//          archive_read_data_skip(a);
//        }
//        r = archive_read_free(a);
//        if(r != ARCHIVE_OK){
//          std::cout << "archive is not ok 2" << std::endl;
//        }
//      }
//      exit(1);

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
