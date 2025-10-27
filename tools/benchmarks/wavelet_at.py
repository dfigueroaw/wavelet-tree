import matplotlib.pyplot as plt

x = [1e6, 1e7, 1e8]
str_1 = [8.73, 47.76, 429.66]
str_2 = [9.20, 45.24, 471.70]
str_3 = [9.25, 45.19, 460.87]

plt.plot(x, str_1, marker='o', label='str_1')
plt.plot(x, str_2, marker='o', label='str_2')
plt.plot(x, str_3, marker='o', label='str_3')

plt.xscale('log')
plt.xlabel('# de consultas')
plt.ylabel('Tiempo (ms)')
plt.title('Benchmarks para la operación wavelet_at')
plt.legend()
plt.grid(True)

plt.show()
