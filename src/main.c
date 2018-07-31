#define _XOPEN_SOURCE 600

#include <ftw.h>
#include "../inc/word_counter.h"

#include <stdarg.h>

#define DEFAULT_PATH "/usr/share/man/man1"

static pthread_cond_t thread_died = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t thread_mutext = PTHREAD_MUTEX_INITIALIZER;

static int tot_threads = 0;
static int num_live = 0;
static int num_unjoined = 0;

pthread_t t[20000];
int i=0;
/*
enum tstate{
    TS_ALIVE,
    TS_TERMINATED,
    TS_JOINED
};

static struct {
    pthread_t tid;
    enum tstate state;
    int sleep_time
} *thread;
*/
t_tree words;

static char *ename[] = {
    /*   0 */ "", 
    /*   1 */ "EPERM", "ENOENT", "ESRCH", "EINTR", "EIO", "ENXIO", 
    /*   7 */ "E2BIG", "ENOEXEC", "EBADF", "ECHILD", 
    /*  11 */ "EAGAIN/EWOULDBLOCK", "ENOMEM", "EACCES", "EFAULT", 
    /*  15 */ "ENOTBLK", "EBUSY", "EEXIST", "EXDEV", "ENODEV", 
    /*  20 */ "ENOTDIR", "EISDIR", "EINVAL", "ENFILE", "EMFILE", 
    /*  25 */ "ENOTTY", "ETXTBSY", "EFBIG", "ENOSPC", "ESPIPE", 
    /*  30 */ "EROFS", "EMLINK", "EPIPE", "EDOM", "ERANGE", 
    /*  35 */ "EDEADLK/EDEADLOCK", "ENAMETOOLONG", "ENOLCK", "ENOSYS", 
    /*  39 */ "ENOTEMPTY", "ELOOP", "", "ENOMSG", "EIDRM", "ECHRNG", 
    /*  45 */ "EL2NSYNC", "EL3HLT", "EL3RST", "ELNRNG", "EUNATCH", 
    /*  50 */ "ENOCSI", "EL2HLT", "EBADE", "EBADR", "EXFULL", "ENOANO", 
    /*  56 */ "EBADRQC", "EBADSLT", "", "EBFONT", "ENOSTR", "ENODATA", 
    /*  62 */ "ETIME", "ENOSR", "ENONET", "ENOPKG", "EREMOTE", 
    /*  67 */ "ENOLINK", "EADV", "ESRMNT", "ECOMM", "EPROTO", 
    /*  72 */ "EMULTIHOP", "EDOTDOT", "EBADMSG", "EOVERFLOW", 
    /*  76 */ "ENOTUNIQ", "EBADFD", "EREMCHG", "ELIBACC", "ELIBBAD", 
    /*  81 */ "ELIBSCN", "ELIBMAX", "ELIBEXEC", "EILSEQ", "ERESTART", 
    /*  86 */ "ESTRPIPE", "EUSERS", "ENOTSOCK", "EDESTADDRREQ", 
    /*  90 */ "EMSGSIZE", "EPROTOTYPE", "ENOPROTOOPT", 
    /*  93 */ "EPROTONOSUPPORT", "ESOCKTNOSUPPORT", 
    /*  95 */ "EOPNOTSUPP/ENOTSUP", "EPFNOSUPPORT", "EAFNOSUPPORT", 
    /*  98 */ "EADDRINUSE", "EADDRNOTAVAIL", "ENETDOWN", "ENETUNREACH", 
    /* 102 */ "ENETRESET", "ECONNABORTED", "ECONNRESET", "ENOBUFS", 
    /* 106 */ "EISCONN", "ENOTCONN", "ESHUTDOWN", "ETOOMANYREFS", 
    /* 110 */ "ETIMEDOUT", "ECONNREFUSED", "EHOSTDOWN", "EHOSTUNREACH", 
    /* 114 */ "EALREADY", "EINPROGRESS", "ESTALE", "EUCLEAN", 
    /* 118 */ "ENOTNAM", "ENAVAIL", "EISNAM", "EREMOTEIO", "EDQUOT", 
    /* 123 */ "ENOMEDIUM", "EMEDIUMTYPE", "ECANCELED", "ENOKEY", 
    /* 127 */ "EKEYEXPIRED", "EKEYREVOKED", "EKEYREJECTED", 
    /* 130 */ "EOWNERDEAD", "ENOTRECOVERABLE", "ERFKILL", "EHWPOISON"
};
#define MAX_ENAME 133

static void terminate(Boolean use_exit_3)
{
    char *s;
    
    s = getenv("EF_DUMPCORE");
    if (s != NULL && *s != '\0')
        abort();
    else if(use_exit_3)
        exit(EXIT_FAILURE);
    else
        _exit(EXIT_FAILURE);
}

static void output_error(Boolean use_err, int err, Boolean flush_std_out, const char *format, va_list ap)
{
    #define BUF_SIZE 500
    char buf[BUF_SIZE], user_msg[BUF_SIZE], err_text[BUF_SIZE];
    vsnprintf(user_msg, BUF_SIZE, format, ap);

    if (use_err)
        snprintf(err_text, BUF_SIZE, " [%s %s]", (err > 0 && err <= MAX_ENAME) ? ename[err] : "?UNKNOW?", strerror(err));
    else
        snprintf(err_text, BUF_SIZE, ":");
    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", err_text, user_msg);
    if(flush_std_out)
        fflush(stdout);
    fputs(buf, stderr);
    fflush(stdout);
}

void err_exit_en(int errnum, const char *format, ...)
{
    va_list arg_list;
    va_start(arg_list, format);
    output_error(TRUE, errno, FALSE, format, arg_list);
    va_end(arg_list);
    terminate(FALSE);
}

static void usage_error(const char *prog_name, char *msg)
{
  if (msg != NULL)
    fprintf(stderr, "%s\n", msg);
  fprintf(stderr, "Usage: %s [directory-path]\n", prog_name);
  exit(EXIT_FAILURE);
}

static int dir_tree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
  if ((sbuf->st_mode & S_IFMT) == S_IFREG)
    open_file(&pathname[ftwb->base], &(words));
  return 0;
}

int main(int argc, char **argv)
{
  int s, idx;

  if (argc > 2)
    usage_error(argv[0], NULL);
  tr_initialize(&(words));
  if (nftw(argc == 1 ? DEFAULT_PATH : argv[1], dir_tree, 20, FTW_CHDIR) == -1) {
    perror("nftw");
    usage_error(argv[0], NULL);
  }



 //show_words(&(words));
  return 0;
}