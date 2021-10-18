def rellenarBits(numero:str,bitsDisponibles:int)->str:  # Medicion analitica: 4n + 1
    while len(numero)!=bitsDisponibles: # 1 + 1 = 2  -> Resultado del ciclo 4n
        numero = "0" + numero # 1 + 1 = 2
    return numero # 1

def decimalABinario(numeroDecimal:int,bitsDisponibles:int)->str: # Medicion analitica: 10n + 9
    numeroBinario = 0 # 1
    multiplicador = 1 # 1

    while numeroDecimal != 0: # 1 + 1 = 2 -> Resultado del ciclo 10n
        numeroBinario = numeroBinario + numeroDecimal % 2 * multiplicador # 1 + 1 + 1 + 1 = 4 
        numeroDecimal //= 2 # 1 + 1 = 2 
        multiplicador *= 10 # 1 + 1 = 2
    numeroBinario = str(numeroBinario) # 1 + 1 = 2
    
    if len(numeroBinario) < bitsDisponibles: # 1 + 1 = 2
        return rellenarBits(numeroBinario,bitsDisponibles) # 1 + 1 = 2

    return numeroBinario # 1
