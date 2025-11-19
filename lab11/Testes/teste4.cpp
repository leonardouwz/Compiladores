// Prueba avanzada de conversiones
int main()
{
    int contador;
    int limite;
    bool activo;
    
    contador = 5;
    limite = 0;
    activo = true;
    
    // Mezcla de int y bool
    if (contador && activo)     // int && bool
    {
        contador = contador - 1;
    }
    
    while (!limite && contador) // !int && int
    {
        contador = contador - 1;
    }
    
    // Expresiones complejas con conversiones
    if (!limite || contador && activo)
    {
        limite = 1;
    }
    
    // Bucle do-while con int
    do
    {
        contador = contador + 1;
    }
    while (contador < 10 && limite);
}