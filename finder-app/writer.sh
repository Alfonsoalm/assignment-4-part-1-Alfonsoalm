writefile=$1
writestr=$2
# Explicación: Si no se proporcionan los dos argumentos, muestra un mensaje de uso y sale con código 1.
if [ -z "$writefile" ] || [ -z "$writestr" ]; then
    echo "Usage: $0 <write_file> <write_string>"
    exit 1
fi
# Explicación: Crea el directorio si no existe
write_dir=$(dirname "$writefile")
mkdir -p "$write_dir"
# Explicación: Escribe writestr en writefile, sobrescribiendo cualquier fichero existente
echo "$writestr" > "$writefile"
# Explicación: Verifica si la escritura fue exitosa
if [ $? -ne 0 ]; then
    echo "Error: Could not write to file $writefile."
    exit 1
fi
exit 0