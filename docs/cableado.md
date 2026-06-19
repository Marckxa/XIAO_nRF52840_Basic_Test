# Cableado minimo

Para la primera prueba no hace falta ningun componente externo.

## Prueba 1: placa sola

```text
Mac/PC USB-C  ->  XIAO nRF52840 USB-C
```

Resultado esperado:

- El firmware carga sin errores.
- El LED integrado parpadea.
- El monitor serie muestra mensajes `OK`.

## Prueba 2: dispositivo I2C externo

```text
XIAO 3V3  ->  VCC del modulo I2C
XIAO GND  ->  GND del modulo I2C
XIAO SDA  ->  SDA del modulo I2C
XIAO SCL  ->  SCL del modulo I2C
```

En el monitor serie escribe:

```text
i
```

Si el cableado es correcto, deberia aparecer al menos una direccion I2C.
