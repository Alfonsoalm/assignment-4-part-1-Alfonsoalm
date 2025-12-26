/**
 * writer.c - Utilidad para escribir una cadena en un fichero usando File I/O POSIX
 *
 * Requisitos:
 *  - No crear directorios (asumimos que ya existen).
 *  - Usar syslog con LOG_USER.
 *  - LOG_DEBUG: "Escribiendo <cadena> en <fichero>"
 *  - LOG_ERR: registrar cualquier error inesperado.
 *
 * Uso:
 *   writer <fichero> <cadena>
 */

#define _POSIX_C_SOURCE 200809L
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    /* Habilitar hasta LOG_DEBUG y usar facility LOG_USER */
    setlogmask(LOG_UPTO(LOG_DEBUG));
    openlog(argv[0] ? argv[0] : "writer", LOG_PID, LOG_USER);

    if (argc != 3)
    {
        syslog(LOG_ERR, "Uso: %s <fichero> <cadena>", argv[0] ? argv[0] : "writer");
        closelog();
        return EXIT_FAILURE;
    }

    const char *filepath = argv[1];
    const char *writestr = argv[2];

    /* Mensaje de depuración requerido */
    syslog(LOG_DEBUG, "Escribiendo %s en %s", writestr, filepath);

    /* Abrir/crear el fichero, truncando si existe. Permisos 0644 (respetando umask). */
    int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        syslog(LOG_ERR, "Error al abrir/crear '%s': %s", filepath, strerror(errno));
        closelog();
        return EXIT_FAILURE;
    }

    /* Escribir la cadena completa, gestionando escrituras parciales y EINTR */
    size_t len = strlen(writestr);
    size_t total = 0;
    while (total < len)
    {
        ssize_t n = write(fd, writestr + total, len - total);
        if (n < 0)
        {
            if (errno == EINTR)
                continue;
            syslog(LOG_ERR, "Error al escribir en '%s': %s", filepath, strerror(errno));
            close(fd); /* Intento de cerrar, incluso en error */
            closelog();
            return EXIT_FAILURE;
        }
        total += (size_t)n;
    }

    /* (Opcional) Forzar a disco; si falla, lo reflejamos como error */
    if (fsync(fd) < 0)
    {
        syslog(LOG_ERR, "Error al sincronizar '%s': %s", filepath, strerror(errno));
        close(fd);
        closelog();
        return EXIT_FAILURE;
    }

    if (close(fd) < 0)
    {
        syslog(LOG_ERR, "Error al cerrar '%s': %s", filepath, strerror(errno));
        closelog();
        return EXIT_FAILURE;
    }

    closelog();
    return EXIT_SUCCESS;
}
