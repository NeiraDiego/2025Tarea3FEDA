
import pandas as pd
import matplotlib.pyplot as plt

# Cargar archivo Excel
df_excel = pd.read_excel("tiempos.csv.xlsx")

# Definir cantidades objetivo
cantidades_objetivo = [1000, 5000, 10000, 15000, 20000, 25000, 30000]

# Columnas específicas a analizar
columnas_deseadas = ['t_HC_ID', 't_HC_SN', 't_HA_ID', 't_HA_SN', 't_BST_ID', 't_BTS_SN']

# Preparar figura
plt.figure(figsize=(12, 7))

# Graficar media y varianza para cada columna con respecto a 'Cantidad'
for col in columnas_deseadas:
    medias_col = []
    varianzas_col = []
    for c in cantidades_objetivo:
        grupo = df_excel[df_excel['Cantidad'] == c][col]
        medias_col.append(grupo.mean())
        varianzas_col.append(grupo.var())

    # Graficar media con barra de error de varianza
    plt.errorbar(
        cantidades_objetivo,
        medias_col,
        yerr=varianzas_col,
        label=col,
        capsize=4,
        marker='o'
    )

plt.title('Media y Varianza por columna t_* según Cantidad')
plt.xlabel('Cantidad')
plt.ylabel('Media (con varianza)')
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
