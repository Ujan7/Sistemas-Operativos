import re
import matplotlib.pyplot as plt

# Lee los resultados desde el archivo resultados.txt
with open('caso_5:MLFQ.txt', 'r') as file:
    lines = file.readlines()

# Expresiones regulares para buscar MFLOP y OPW/OPR en cada línea
pattern_cpubench = r'(\d+): (\d+) MFLOP100T'
pattern_iobench = r'(\d+): (\d+) OPW(\d+)T, (\d+) OPR(\d+)T'

cpubench_values1 = []
cpubench_values2 = []
iobench_values = []

# Contadores para diferenciar entre las dos ejecuciones de cpubench
count_cpubench1 = 0
count_cpubench2 = 0

for line in lines:
    match_cpubench = re.search(pattern_cpubench, line)
    match_iobench = re.search(pattern_iobench, line)

    if match_cpubench:
        _, mflop = match_cpubench.groups()
        # Asigna los valores a cpubench_values1 o cpubench_values2 alternadamente
        if count_cpubench1 <= count_cpubench2:
            cpubench_values1.append(int(mflop))
            count_cpubench1 += 1
        else:
            cpubench_values2.append(int(mflop))
            count_cpubench2 += 1
    elif match_iobench:
        _, _, _, opr, _ = match_iobench.groups()
        iobench_values.append(int(opr))

# Crea una lista de tiempo (puede ser simplemente el índice de la medición)
time_values_cpubench1 = list(range(len(cpubench_values1)))
time_values_cpubench2 = list(range(len(cpubench_values2)))
time_values_iobench = list(range(len(iobench_values)))

# Calcula el máximo, mínimo y media para cpubench_values1 y cpubench_values2
max_cpubench1 = max(cpubench_values1)
min_cpubench1 = min(cpubench_values1)
avg_cpubench1 = sum(cpubench_values1) / len(cpubench_values1)

max_cpubench2 = max(cpubench_values2)
min_cpubench2 = min(cpubench_values2)
avg_cpubench2 = sum(cpubench_values2) / len(cpubench_values2)

# Calcula el máximo, mínimo y media para iobench_values
max_iobench = max(iobench_values)
min_iobench = min(iobench_values)
avg_iobench = sum(iobench_values) / len(iobench_values)

# Grafica los resultados de los dos cpubench y el resultado de iobench en la misma pestaña
plt.figure(figsize=(8, 8))

# Título general
plt.suptitle('Caso 5: rendimiento 1 CPUBench con 1 CPUBench con 1 IOBench', fontsize=16)

plt.subplot(2, 1, 1)
plt.plot(time_values_cpubench1, cpubench_values1, label='CPUBench 1', color='blue')
plt.plot(time_values_cpubench2, cpubench_values2, label='CPUBench 2', color='red')
plt.ylabel('Millones de Operaciones en Coma Flotante por Segundo')
plt.legend()
plt.title('Rendimiento de CPUBench (Ejecuciones 1 y 2)')
plt.grid(True)  # Agrega la grilla

# Etiquetas para CPUBench
plt.annotate(f'Max: {max_cpubench1} MFLOP', xy=(time_values_cpubench1[cpubench_values1.index(max_cpubench1)], max_cpubench1), xytext=(5, 10), textcoords='offset points', color='blue', fontsize=10, ha='right')
plt.annotate(f'Min: {min_cpubench1} MFLOP', xy=(time_values_cpubench1[cpubench_values1.index(min_cpubench1)], min_cpubench1), xytext=(5, 0), textcoords='offset points', color='blue', fontsize=10, ha='right')
plt.annotate(f'Media: {avg_cpubench1:.2f} MFLOP', xy=(0.5, 0.5), xycoords='axes fraction', xytext=(0, 20), textcoords='offset points', color='blue', fontsize=10, ha='center')

plt.annotate(f'Max: {max_cpubench2} MFLOP', xy=(time_values_cpubench2[cpubench_values2.index(max_cpubench2)], max_cpubench2), xytext=(5, 10), textcoords='offset points', color='red', fontsize=10, ha='right')
plt.annotate(f'Min: {min_cpubench2} MFLOP', xy=(time_values_cpubench2[cpubench_values2.index(min_cpubench2)], min_cpubench2), xytext=(5, -15), textcoords='offset points', color='red', fontsize=10, ha='right')
plt.annotate(f'Media: {avg_cpubench2:.2f} MFLOP', xy=(0.5, 0.5), xycoords='axes fraction', xytext=(0, -30), textcoords='offset points', color='red', fontsize=10, ha='center')

plt.subplot(2, 1, 2)
plt.plot(time_values_iobench, iobench_values, label='IOBench', color='green')
plt.xlabel('Tiempo')
plt.ylabel('Operaciones por unidad de tiempo')
plt.legend()
plt.title('Rendimiento de IOBench')
plt.grid(True)  # Agrega la grilla

# Etiquetas para IOBench
plt.annotate(f'Máximo IOBench: {max_iobench} OPR', xy=(time_values_iobench[iobench_values.index(max_iobench)], max_iobench), xytext=(5, 10), textcoords='offset points', color='green', fontsize=10, ha='right')
plt.annotate(f'Mínimo IOBench: {min_iobench} OPR', xy=(time_values_iobench[iobench_values.index(min_iobench)], min_iobench), xytext=(5, 0), textcoords='offset points', color='green', fontsize=10, ha='right')
plt.annotate(f'Media IOBench: {avg_iobench:.2f} OPR', xy=(0.5, 0.5), xycoords='axes fraction', xytext=(0, 20), textcoords='offset points', color='green', fontsize=10, ha='center')

plt.tight_layout()
plt.show()