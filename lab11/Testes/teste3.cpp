// Prueba de conversión int a bool
int main()
{
    int x;
    int y;
    int z;
    bool resultado;
    
    x = 10;
    y = 0;
    z = 5;
    
    // Conversión implícita en operadores lógicos
    resultado = x && y;      // 10 && 0 = true && false = false
    resultado = x || y;      // 10 || 0 = true || false = true
    resultado = !z;          // !5 = !true = false
    
    // Conversión en condiciones
    if (x)                   // if (true)
    {
        y = 1;
    }
    
    while (z)                // while (true)
    {
        z = z - 1;
    }
    
    do
    {
        x = x - 1;
    }
    while (x);               // while (true hasta que x=0)
}