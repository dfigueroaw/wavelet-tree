import matplotlib.pyplot as plt

x = [1e6, 1e7, 1e8]
str_1 = [9.59, 96.21, 963.03]
str_2 = [11.11, 113.15, 1128.18]
str_3 = [10.92, 109.74, 1111.57]

plt.plot(x, str_1, marker='o', label='str_1')
plt.plot(x, str_2, marker='o', label='str_2')
plt.plot(x, str_3, marker='o', label='str_3')

plt.xscale('log')
plt.xlabel('# de consultas')
plt.ylabel('Tiempo (ms)')
plt.title('Benchmarks para la operación wavelet_leq')
plt.legend()
plt.grid(True)

plt.show()

