import re
import matplotlib.pyplot as plt

# Lee los resultados desde el archivo
with open('caso_2:Q0.1.txt', 'r') as file:
    lines = file.readlines()

# Expresión regular para buscar valores de MFLOP100T
pattern = r'(\d+) MFLOP100T'
mflop_values = []

# Extraer los valores de MFLOP100T de las líneas del archivo
for line in lines:
    match = re.search(pattern, line)
    if match:
        mflop = int(match.group(1))
        mflop_values.append(mflop)

# Crear una lista de tiempo (puede ser simplemente el índice de la medición)
time_values = list(range(len(mflop_values)))

# Graficar los resultados
plt.plot(time_values, mflop_values, marker='o', linestyle='-', color='b', label='MFLOP100T')

# Etiquetas para máximo, mínimo y media
max_mflop = max(mflop_values)
min_mflop = min(mflop_values)
mean_mflop = sum(mflop_values) / len(mflop_values)

plt.xlabel('Tiempo')
plt.ylabel('Millones de Operaciones en Coma Flotante por Segundo (MFLOP100T)')
plt.title('Caso 2: Rendimiento 1 CPUBench solo(Quantum 10 veces menor)')
plt.grid(True)
plt.legend()

# Agregar etiquetas
plt.text(time_values[mflop_values.index(max_mflop)], max_mflop, f'Max: {max_mflop:.2f}', fontsize=11, ha='left', va='bottom')
plt.text(time_values[mflop_values.index(min_mflop)], min_mflop, f'Min: {min_mflop:.2f}', fontsize=11, ha='left', va='bottom')
plt.text(time_values[-1], mean_mflop, f'Media: {mean_mflop:.2f}', fontsize=11, ha='left', va='bottom')

plt.show()
