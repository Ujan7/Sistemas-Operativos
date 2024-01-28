import re
import matplotlib.pyplot as plt

# Lee los resultados desde el archivo resultados.txt
with open('caso_4:MLFQ.txt', 'r') as file:
    lines = file.readlines()

# Expresiones regulares para buscar MFLOP en cada línea
pattern_cpubench = r'(\d+): (\d+) MFLOP100T'

cpubench_values1 = []
cpubench_values2 = []

# Contadores para diferenciar entre las dos ejecuciones de cpubench
count_cpubench1 = 0
count_cpubench2 = 0

for line in lines:
    match_cpubench = re.search(pattern_cpubench, line)
    
    if match_cpubench:
        _, mflop = match_cpubench.groups()
        # Asigna los valores a cpubench_values1 o cpubench_values2 alternadamente
        if count_cpubench1 <= count_cpubench2:
            cpubench_values1.append(int(mflop))
            count_cpubench1 += 1
        else:
            cpubench_values2.append(int(mflop))
            count_cpubench2 += 1

# Crea una lista de tiempo (puede ser simplemente el índice de la medición)
time_values_cpubench1 = list(range(len(cpubench_values1)))
time_values_cpubench2 = list(range(len(cpubench_values2)))

# Grafica los resultados
plt.plot(time_values_cpubench1, cpubench_values1, label='CPUBench 1', color='blue')
plt.plot(time_values_cpubench2, cpubench_values2, label='CPUBench 2', color='red')

plt.xlabel('Tiempo')
plt.ylabel('Millones de Operaciones en Coma Flotante por Segundo')
plt.legend()
plt.title('Caso 4: rendimiento 1 CPUBench con 1 CPUBench')
plt.grid(True)

# Calcula valores estadísticos
max_cpubench1 = max(cpubench_values1)
min_cpubench1 = min(cpubench_values1)
avg_cpubench1 = sum(cpubench_values1) / len(cpubench_values1)

max_cpubench2 = max(cpubench_values2)
min_cpubench2 = min(cpubench_values2)
avg_cpubench2 = sum(cpubench_values2) / len(cpubench_values2)

# Agrega etiquetas al gráfico
plt.annotate(f'Máximo: {max_cpubench1} MFLOP', xy=(time_values_cpubench1[cpubench_values1.index(max_cpubench1)], max_cpubench1), color='blue', fontsize=10, ha='right')
plt.annotate(f'Mínimo: {min_cpubench1} MFLOP', xy=(time_values_cpubench1[cpubench_values1.index(min_cpubench1)], min_cpubench1), color='blue', fontsize=10, ha='right')
plt.annotate(f'Media: {avg_cpubench1:.2f} MFLOP', xy=(0.7, 0.5), xycoords='axes fraction', color='blue', fontsize=10, ha='center')

plt.annotate(f'Máximo: {max_cpubench2} MFLOP', xy=(time_values_cpubench2[cpubench_values2.index(max_cpubench2)], max_cpubench2), color='red', fontsize=10, ha='left')
plt.annotate(f'Mínimo: {min_cpubench2} MFLOP', xy=(time_values_cpubench2[cpubench_values2.index(min_cpubench2)], min_cpubench2), color='red', fontsize=10, ha='left')
plt.annotate(f'Media: {avg_cpubench2:.2f} MFLOP', xy=(0.5, 0.5), xycoords='axes fraction', color='red', fontsize=10, ha='right')

# Muestra el gráfico
plt.show()
