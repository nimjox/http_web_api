/*** Errors & logging info ***/
#define SD_EMERG   "<0>"  /* system is unusable */
#define SD_ALERT   "<1>"  /* action must be taken immediately */
#define SD_CRIT    "<2>"  /* critical conditions */
#define SD_ERR     "<3>"  /* error conditions */
#define SD_WARNING "<4>"  /* warning conditions */
#define SD_NOTICE  "<5>"  /* normal but significant condition */
#define SD_INFO    "<6>"  /* informational */
#define SD_DEBUG   "<7>"  /* debug-level messages */

#define EXIT_SUCCESS          0   /* No Error */
#define EXIT_EPERM            1   /* Operation not permitted */
#define EXIT_ENOENT           2   /* No such file or directory */
#define EXIT_ESRCH            3   /* No such process */
#define EXIT_EINTR            4   /* Interrupted system call */
#define EXIT_EIO              5   /* I/O error */
#define EXIT_ENXIO            6   /* No such device or address */
#define EXIT_E2BIG            7   /* Argument list too long */
#define EXIT_ENOEXEC          8   /* Exec format error */
#define EXIT_EBADF            9   /* Bad file number */
#define EXIT_ECHILD           10  /* No child processes */
#define EXIT_EAGAIN           11  /* Try again */
#define EXIT_ENOMEM           12  /* Out of memory */
#define EXIT_EACCES           13  /* Permission denied */
#define EXIT_EFAULT           14  /* Bad address */
#define EXIT_ENOTBLK          15  /* Block device required */
#define EXIT_EBUSY            16  /* Device or resource busy */
#define EXIT_EEXIST           17  /* File exists */
#define EXIT_EXDEV            18  /* Cross-device link */
#define EXIT_ENODEV           19  /* No such device */
#define EXIT_ENOTDIR          20  /* Not a directory */
#define EXIT_EISDIR           21  /* Is a directory */
#define EXIT_EINVAL           22  /* Invalid argument */
#define EXIT_ENFILE           23  /* File table overflow */
#define EXIT_EMFILE           24  /* Too many open files */
#define EXIT_ENOTTY           25  /* Not a typewriter */
#define EXIT_ETXTBSY          26  /* Text file busy */
#define EXIT_EFBIG            27  /* File too large */
#define EXIT_ENOSPC           28  /* No space left on device */
#define EXIT_ESPIPE           29  /* Illegal seek */
#define EXIT_EROFS            30  /* Read-only file system */
#define EXIT_EMLINK           31  /* Too many links */
#define EXIT_EPIPE            32  /* Broken pipe */
#define EXIT_EDOM             33  /* Math argument out of domain of func */
#define EXIT_ERANGE           34  /* Math result not representable */
#define EXIT_EDEADLK          35  /* Resource deadlock would occur */
#define EXIT_ENAMETOOLONG     36  /* File name too long */
#define EXIT_ENOLCK           37  /* No record locks available */
#define EXIT_ENOSYS           38  /* Function not implemented */
#define EXIT_ENOTEMPTY        39  /* Directory not empty */
#define EXIT_ELOOP            40  /* Too many symbolic links encountered */
#define EXIT_ENOMSG           42  /* No message of desired type */
#define EXIT_EIDRM            43  /* Identifier removed */
#define EXIT_ECHRNG           44  /* Channel number out of range */
#define EXIT_EL2NSYNC         45  /* Level 2 not synchronized */
#define EXIT_EL3HLT           46  /* Level 3 halted */
#define EXIT_EL3RST           47  /* Level 3 reset */
#define EXIT_ELNRNG           48  /* Link number out of range */
#define EXIT_EUNATCH          49  /* Protocol driver not attached */
#define EXIT_ENOCSI           50  /* No CSI structure available */
#define EXIT_EL2HLT           51  /* Level 2 halted */
#define EXIT_EBADE            52  /* Invalid exchange */
#define EXIT_EBADR            53  /* Invalid request descriptor */
#define EXIT_EXFULL           54  /* Exchange full */
#define EXIT_ENOANO           55  /* No anode */
#define EXIT_EBADRQC          56  /* Invalid request code */
#define EXIT_EBADSLT          57  /* Invalid slot */
#define EXIT_EBFONT           59  /* Bad font file format */
#define EXIT_ENOSTR           60  /* Device not a stream */
#define EXIT_ENODATA          61  /* No data available */
#define EXIT_ETIME            62  /* Timer expired */
#define EXIT_ENOSR            63  /* Out of streams resources */
#define EXIT_ENONET           64  /* Machine is not on the network */
#define EXIT_ENOPKG           65  /* Package not installed */
#define EXIT_EREMOTE          66  /* Object is remote */
#define EXIT_ENOLINK          67  /* Link has been severed */
#define EXIT_EADV             68  /* Advertise error */
#define EXIT_ESRMNT           69  /* Srmount error */
#define EXIT_ECOMM            70  /* Communication error on send */
#define EXIT_EPROTO           71  /* Protocol error */
#define EXIT_EMULTIHOP        72  /* Multihop attempted */
#define EXIT_EDOTDOT          73  /* RFS specific error */
#define EXIT_EBADMSG          74  /* Not a data message */
#define EXIT_EOVERFLOW        75  /* Value too large for defined data type */
#define EXIT_ENOTUNIQ         76  /* Name not unique on network */
#define EXIT_EBADFD           77  /* File descriptor in bad state */
#define EXIT_EREMCHG          78  /* Remote address changed */
#define EXIT_ELIBACC          79  /* Can not access a needed shared library */
#define EXIT_ELIBBAD          80  /* Accessing a corrupted shared library */
#define EXIT_ELIBSCN          81  /* .lib section in a.out corrupted */
#define EXIT_ELIBMAX          82  /* Attempting to link in too many shared libraries */
#define EXIT_ELIBEXEC         83  /* Cannot exec a shared library directly */
#define EXIT_EILSEQ           84  /* Illegal byte sequence */
#define EXIT_ERESTART         85  /* Interrupted system call should be restarted */
#define EXIT_ESTRPIPE         86  /* Streams pipe error */
#define EXIT_EUSERS           87  /* Too many users */
#define EXIT_ENOTSOCK         88  /* Socket operation on non-socket */
#define EXIT_EDESTADDRREQ     89  /* Destination address required */
#define EXIT_EMSGSIZE         90  /* Message too long */
#define EXIT_EPROTOTYPE       91  /* Protocol wrong type for socket */
#define EXIT_ENOPROTOOPT      92  /* Protocol not available */
#define EXIT_EPROTONOSUPPORT  93  /* Protocol not supported */
#define EXIT_ESOCKTNOSUPPORT  94  /* Socket type not supported */
#define EXIT_EOPNOTSUPP       95  /* Operation not supported on transport endpoint */
#define EXIT_EPFNOSUPPORT     96  /* Protocol family not supported */
#define EXIT_EAFNOSUPPORT     97  /* Address family not supported by protocol */
#define EXIT_EADDRINUSE       98  /* Address already in use */
#define EXIT_EADDRNOTAVAIL    99  /* Cannot assign requested address */
#define EXIT_ENETDOWN         100 /* Network is down */
#define EXIT_ENETUNREACH      101 /* Network is unreachable */
#define EXIT_ENETRESET        102 /* Network dropped connection because of reset */
#define EXIT_ECONNABORTED     103 /* Software caused connection abort */
#define EXIT_ECONNRESET       104 /* Connection reset by peer */
#define EXIT_ENOBUFS          105 /* No buffer space available */
#define EXIT_EISCONN          106 /* Transport endpoint is already connected */
#define EXIT_ENOTCONN         107 /* Transport endpoint is not connected */
#define EXIT_ESHUTDOWN        108 /* Cannot send after transport endpoint shutdown */
#define EXIT_ETOOMANYREFS     109 /* Too many references: cannot splice */
#define EXIT_ETIMEDOUT        110 /* Connection timed out */
#define EXIT_ECONNREFUSED     111 /* Connection refused */
#define EXIT_EHOSTDOWN        112 /* Host is down */
#define EXIT_EHOSTUNREACH     113 /* No route to host */
#define EXIT_EALREADY         114 /* Operation already in progress */
#define EXIT_EINPROGRESS      115 /* Operation now in progress */
#define EXIT_ESTALE           116 /* Stale NFS file handle */
#define EXIT_EUCLEAN          117 /* Structure needs cleaning */
#define EXIT_ENOTNAM          118 /* Not a XENIX named type file */
#define EXIT_ENAVAIL          119 /* No XENIX semaphores available */
#define EXIT_EISNAM           120 /* Is a named type file */
#define EXIT_EREMOTEIO        121 /* Remote I/O error */
#define EXIT_EDQUOT           122 /* Quota exceeded */
#define EXIT_ENOMEDIUM        123 /* No medium found */
#define EXIT_EMEDIUMTYPE      124 /* Wrong medium type */
#define EXIT_ECANCELED        125 /* Operation Canceled */
#define EXIT_ENOKEY           126 /* Required key not available */
#define EXIT_EKEYEXPIRED      127 /* Key has expired */
#define EXIT_EKEYREVOKED      128 /* Key has been revoked */
#define EXIT_EKEYREJECTED     129 /* Key was rejected by service */
#define EXIT_EOWNERDEAD       130 /* Owner died */
#define EXIT_ENOTRECOVERABLE  131 /* State not recoverable */
/*** End Errors & logging info ***/
