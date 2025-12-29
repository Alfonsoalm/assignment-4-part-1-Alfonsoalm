
#!/bin/sh
# finder.sh - Cuenta archivos y coincidencias de una cadena en un directorio
# Compatible con BusyBox /bin/sh

filesdir=$1
echo "Debug: filesdir is '$filesdir'" >&2
searchstr=$2
echo "Debug: searchstr is '$searchstr'" >&2

# Explicación: Si no se proporcionan los dos argumentos, muestra un mensaje de uso y sale con código 1.
if [ -z "$filesdir" ] || [ -z "$searchstr" ]; then
    echo "Usage: $0 <files_directory> <search_string>"
    exit 1
fi

# Explicación: Si el primer argumento no es un directorio válido, muestra un mensaje de error y sale con código 1.
if [ ! -d "$filesdir" ]; then
    echo "Error: $filesdir is not a valid directory."
    exit 1
fi

# Imprime el mensaje "El número de ficheros es X y el número de líneas coincidentes es Y"
#  donde X es el número de ficheros en el directorio y todos los subdirectorios e 
#  Y es el número de líneas coincidentes encontradas en los respectivos ficheros, 
#  donde una línea coincidente se refiere a una línea que contiene searchstr (y también 
#  puede contener contenido adicional).
num_files=$(find "$filesdir" -type f 2>/dev/null | wc -l)
# imprimir los files encontrados
echo $(find "$filesdir" -type f 2>/dev/null) >&2
num_matches=$(grep -r --text --line-number -- "$searchstr" "$fil:qesdir" 2>/dev/null | wc -l)

echo "The number of files are $num_files and the number of matching lines are $num_matches"
exit 0
fi