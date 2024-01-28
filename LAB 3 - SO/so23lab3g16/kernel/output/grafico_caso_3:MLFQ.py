import re
import matplotlib.pyplot as plt

# Lee los resultados desde el archivo resultados.txt
with open('caso_3:MLFQ.txt', 'r') as file:
    lines = file.readlines()

# Expresiones regulares para buscar OPW y OPR en cada línea
pattern_iobench = r'(\d+): (\d+) OPW(\d+)T, (\d+) OPR(\d+)T'
pattern_cpubench = r'(\d+): (\d+) MFLOP100T'

opw_values = []
opr_values = []
cpubench_values = []

for line in lines:
    match_iobench = re.search(pattern_iobench, line)
    match_cpubench = re.search(pattern_cpubench, line)
    
    if match_iobench:
        _, opw, _, opr, _ = match_iobench.groups()
        opw_values.append(int(opw))
        opr_values.append(int(opr))
    elif match_cpubench:
        _, mflop = match_cpubench.groups()
        cpubench_values.append(int(mflop))

# Crea una lista de tiempo (puede ser simplemente el índice de la medición)
time_values_iobench = list(range(len(opw_values)))
time_values_cpubench = list(range(len(cpubench_values)))

# Calcula la media de OPR
mean_opr = sum(opr_values) / len(opr_values)

# Grafica los resultados
plt.figure(figsize=(10, 6))

# Título general
plt.suptitle('Caso 3: rendimiento 1 IOBench con 1 CPUBench', fontsize=16)

plt.subplot(2, 1, 1)
plt.plot(time_values_iobench, opw_values, label='OPW')
plt.plot(time_values_iobench, opr_values, label='OPR')
plt.xlabel('Tiempo')
plt.ylabel('Operaciones W/R por segundo')
plt.legend()
plt.title('Rendimiento IOBench')

# Agrega etiquetas para el valor mínimo y máximo de OPR
min_opr = min(opr_values)
max_opr = max(opr_values)
plt.text(time_values_iobench[opr_values.index(min_opr)], min_opr, f'Min OPR: {min_opr}', fontsize=11, ha='left', va='bottom')
plt.text(time_values_iobench[opr_values.index(max_opr)], max_opr, f'Max OPR: {max_opr}', fontsize=11, ha='left', va='bottom')

# Agrega la media de OPR al gráfico
plt.text(time_values_iobench[-1], mean_opr, f'Media OPR: {mean_opr:.2f}', fontsize=11, ha='right', va='bottom')

plt.grid(True)

plt.subplot(2, 1, 2)
plt.plot(time_values_cpubench, cpubench_values, label='MFLOP')
plt.xlabel('Tiempo')
plt.ylabel('Millones de Operaciones en Coma Flotante por Segundo')
plt.legend()
plt.title('Rendimiento CPUBench')
plt.grid(True)

# Agrega etiquetas para el valor mínimo, máximo y media en cpubench
min_cpubench = min(cpubench_values)
max_cpubench = max(cpubench_values)
mean_cpubench = sum(cpubench_values) / len(cpubench_values)

plt.text(time_values_cpubench[cpubench_values.index(min_cpubench)], min_cpubench, f'Min MFLOP: {min_cpubench}', fontsize=11, ha='left', va='bottom')
plt.text(time_values_cpubench[cpubench_values.index(max_cpubench)], max_cpubench, f'Max MFLOP: {max_cpubench}', fontsize=11, ha='left', va='bottom')
plt.text(time_values_cpubench[-1], mean_cpubench, f'Media MFLOP: {mean_cpubench:.2f}', fontsize=11, ha='right', va='bottom')
plt.grid(True)

plt.tight_layout()

plt.show()
